#ifndef FAT12_DRIVER_C
#define FAT12_DRIVER_C

#include "FAT12Driver.h"
#include "MemoryUtils.h"
#include "String.h"

asm(".code16gcc\n");

/* A global structure that contains the BPB data associated with the
   floppy disk being read.  This value is loaded by the
   initializeFAT12() function and never changed.  It has been made
   global so that it can be readily used in various functions.
 */
static struct BPB cachedBPB;

int initializeFAT12() {
  byte bootSector[512];
  unsigned short stackSegment;
  byte errorFlag = 0;
  
  // First reset the floppy disk with ah=0 and al=0
  asm volatile("clc\n\t"               /* clear carry flag */
	       "xorw %%ax, %%ax\n\t"   /* Set ah=al=0 */
	       "xorw %%dx, %%dx\n\t"
	       "int $0x13\n\t"         /* call int 13h */
	       "jnc  endInit\n\t"
	       "movb $1, %[error]\n\t" /* Set error flag = 1 if CF =1 */
	       "endInit:\n\t"
	       : [error] "=m" (errorFlag)
	       : /* No inputs */
	       : "eax", "ebx", "edx"  /* Clobbered registers */
	       );

  if (errorFlag != 0) {
    return IO_ERROR;
  }

  GET_SS(stackSegment);
  
  // Now read boot sector into memory using readSector
  if (readPhysicalSector(0, 0, 1, 1, stackSegment,
			 OFFSET(bootSector)) != SUCCESS) {
    return IO_ERROR;
  }

  // Copy necessary bytes into the cachedBPB.
  farmemcpy(0, OFFSET(&cachedBPB), stackSegment, OFFSET(bootSector + 3),
	    sizeof(struct BPB));
  
  // Everything went well.  Initialization was successful!
  return SUCCESS;
}

int finalizeFAT12() {
  // Nothing to be done in finalize.
  return SUCCESS;
}

// Function to copy global bpb into caller's version of the BPB
int getBPB(const int segment, const int offset) {
  // Must use far mem copy to copy data across segements if necessary.
  farmemcpy(segment, offset, 0, OFFSET(&cachedBPB),
	    sizeof(struct BPB));
  
  return SUCCESS;
}

// Function to read a root directory entry.
int getFileEntry(const int index, const int segment, const int offset) {
  byte sectorData[512];
  int  errorCode;
  unsigned short stackSegment;
  struct BPB localBPB;
  // Copy BPB from kernel space to process space.
  GET_SS(stackSegment);
  getBPB(stackSegment, OFFSET(&localBPB));
  
  // Ensure index is within bounds.
  if ((index < 0) || (index >= localBPB.rootEntries)) {
    return INVALID_ROOT_DIRECTORY_INDEX;
  }
  // Now compute starting sector of root directory:
  int rdSector = (localBPB.fatCount * localBPB.sectorsPerFAT)
    + localBPB.reservedSectors;
  // Now compute sector in the root directory where the given index
  // maps to.
  rdSector += (index * sizeof(struct FileEntry) / localBPB.bytesPerSector);
  // Read the sector into our internal buffer.
  if ((errorCode = readSector(rdSector, stackSegment,
			      OFFSET(sectorData))) != SUCCESS) {
    return errorCode;
  }
  // Now compute offset within the sector where the given index maps to.
  int entryOffset = (index * sizeof(struct FileEntry)
		     % localBPB.bytesPerSector);
  // Check if entry is valid.
  if (sectorData[entryOffset] == 0) {
    // Invalid entry
    return INVALID_ROOT_DIRECTORY_INDEX;
  }
  // Copy data into user's data structure.
  farmemcpy(segment, offset, stackSegment, OFFSET(sectorData) + entryOffset,
	    sizeof(struct FileEntry));
  
  return SUCCESS;
}

int getFATEntry(const int cluster, unsigned short *fatEntry) {
  static byte buffer[512];
  static int currentSector = -1; // Force reading of first sector.
  unsigned short stackSegment;
  int errorCode;
  struct BPB localBPB;
  // Copy BPB from kernel space to process space.
  GET_SS(stackSegment);
  getBPB(stackSegment, OFFSET(&localBPB));

  // Is cluster basically valid?
  if (cluster < 0) {
    return INVALID_CLUSTER;
  }

  // Compute the logical offset in the complete FAT table from where 3
  // bytes need to be read to obtain the FAT entry for the given
  // cluster.  The offset is computed nothing the following facts:
  //    1. 2 FAT entries occupy 3 bytes.
  //    2. Even clusters start a 3 byte boundary.
  //    3. Odd clusters require reading from a 3 byte boundary.
  //  Note that FAT entries may be split across 2 physical sectors on the disk.
  int offset  = (cluster * 3 / 2);
  // Now compute the starting sector from where the FAT entry is to be
  // read.
  unsigned short tempEntry = 0; 
  int sectorToRead  = (offset  / 512) + localBPB.reservedSectors;
  // Read next sector only if necessary.
  if (sectorToRead != currentSector) {
    if ((errorCode = readSector(sectorToRead, stackSegment,
				OFFSET(buffer))) != SUCCESS) {
      return errorCode;
    }
    currentSector = sectorToRead;
  }
  // Get first byte of fat entry.
  tempEntry = buffer[offset % 512];
  offset++;
  sectorToRead  = (offset  / 512) + localBPB.reservedSectors;  
  if (sectorToRead != currentSector) {
    if ((errorCode = readSector(sectorToRead, stackSegment,
				OFFSET(buffer))) != SUCCESS) {
      return errorCode;
    }
    currentSector = sectorToRead;
  }
  tempEntry |= (buffer[offset % 512] << 8);
  
  // Now, tempEntry has the FAT entry we are looking for.  We need to
  // extract the appropriate bits depending on wether the cluster is
  // odd or even.
  if (cluster & 1) {
    // Odd cluster.  Get rid of lower 4 bits of next cluster
    *fatEntry = (tempEntry >> 4);
  }
  else {
    // Even cluster.  Just retain only the lower 12 bits.    
    *fatEntry = (tempEntry & 0xfff);
  }
  
  // The processing went on smoothly.
  return SUCCESS;
}

int readCluster(const int cluster, const int segment, const int offset,
		int *bytesRead) {
  struct BPB localBPB;
  unsigned short stackSegment;
  GET_SS(stackSegment);
  getBPB(stackSegment, OFFSET(&localBPB));
  
  const int DataSector   = localBPB.reservedSectors +
    (localBPB.fatCount * localBPB.sectorsPerFAT) +
    (localBPB.rootEntries * 32 / 512);

  int index, error;
  *bytesRead = 0;
  for(index = 0; (index < localBPB.sectorsPerCluster); index++) {
    int sector = ((cluster - 2) * localBPB.sectorsPerCluster) + DataSector;
    if ((error = readSector(sector, segment, offset +
			    (index * localBPB.bytesPerSector))) != SUCCESS) {
      return error;
    }
    *bytesRead += localBPB.bytesPerSector;
  }

  return SUCCESS;
}

int readSector(const int sector, const int segment, const int offset) {
  struct BPB localBPB;
  unsigned short stackSegment;  
  GET_SS(stackSegment);
  // Copy BPB from kernel space to process space.
  getBPB(stackSegment, OFFSET(&localBPB));
  
  if ((sector < 0) || (sector > localBPB.sectorsCount)) {
    // Invalid number of sectors have been specified.
    return -1;
  }

  // Convert logical sector to head, track, sector values
  byte phySector = (sector % localBPB.sectorsPerTrack) + 1;
  int  trackHead = sector / localBPB.sectorsPerTrack;
  byte head      = trackHead % localBPB.headCount;
  byte track     = trackHead / localBPB.headCount;

  return readPhysicalSector(head, track, phySector, 1, segment, offset);
}


int readPhysicalSector(const byte head, const byte track, byte sector,
		       const byte sectorsToRead, const int segment,
		       const int offset) {
  byte errorFlag = 0;
  
  asm volatile("pushw %%es\n\t"
	       "movb $0x2, %%ah\n\t"
	       "movb %[numSect], %%al\n\t"
	       "movb %[track], %%ch\n\t"
	       "movb %[sector], %%cl\n\t"
	       "movb %[head], %%dh\n\t"
	       "movb $0, %%dl\n\t"
	       "movw %[seg], %%es\n\t"
	       "movw %[off], %%bx\n\t"
	       "int $0x13\n\t"
	       "movw $1, %%bx\n\t"
	       "jnc  doneRead\n\t"
	       "movb $1, %[err]\n\t" /* Set error flag = 1 if CF =1 */
	       "doneRead:\n\t"
	       "popw %%es\n\t"	       
	       : [err] "=m" (errorFlag)
	       : [numSect] "m" (sectorsToRead), [track] "m" (track),
		 [head] "m" (head), [sector] "m" (sector),
		 [seg] "m" (segment), [off] "m" (offset)
	       : "eax", "ebx", "ecx", "edx" /* Clobbered registers */);

  return (errorFlag) ? IO_ERROR : SUCCESS;
}

const char* decodeFAT12FileAttributes(const byte attribute, char buffer[]) {
  const char *FLAGS = "RHSVDA";
  int bit;
  for(bit = 0; (FLAGS[bit]); bit++) {
    int bitSet = attribute & (1 << bit);
    buffer[bit] = (bitSet ? FLAGS[bit] : '-');
  }
  // Terminate string.
  buffer[6] = '\0';
  return buffer;
}

#endif
