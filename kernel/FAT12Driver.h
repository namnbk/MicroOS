#ifndef FAT12_DRIVER_H
#define FAT12_DRIVER_H

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */


/** \file FAT12Driver.h

   \brief A simple FAT12 (floppy) file system driver (read only).

   This header file provides a set of forward declarations for the
   different data structures and API methods that the FAT12 driver
   supports for reading data from a FAT12 file system.

   \note These functions are internally used by the OS.  An
   application program should never call these functions directly.
*/

#include "FAT12.h"
#include "ErrorCodes.h"

/**
   Initialize the driver and load the BPB information from the disk.

   This method must be the first method to be invoked to use this
   driver.  This function performs the following tasks using INT 0x13:

   <OL>

   <LI> It first initializes the floppy driver (int 0x13, ah =
   0). </LI>

   <LI>It then reads sector 0 (boot sector) into a temporary buffer
   (using int 0x13, ah=2). </LI>

   <LI>It copies the necessary BPB bytes into an internal cachedBPB
   data structure.</LI>
   
   </OL>

   \return This method returns one of the error codes from the list
   defined in ErrorCodes.h on errors.  If the initialization was
   successful it returns SUCCESS.
 */
int initializeFAT12();

/**
   Wrap up the working of the FAT12 driver.

   This method must be invoked once all the processing has been
   completed by the FAT12 driver to wind-up its working.  This
   function relinquishes file handles.  Once this method is invoked
   further calls to other methods in this driver will fail, unless
   initializeFAT12() function is invoked again.
*/
int finalizeFAT12();

/**
   This method may be used to read data from a given sector from the
   floppy disk into the given buffer.  The sectors are identified
   using a logical sector number with a 0 based index.  If the sector
   is invalid then this method returns -1 indicating an error.

   \note The initializeFAT12() function should have been invoked
   before this method is called.  Otherwise this method returns -2
   indicating an error.
      
   \param sectorNumber The logical sector number (zero index based)
   from where the data is to be read into the buffer.

   \param segment The segment address value where the data must be
   stored.

   \param offset The Address of the first byte in the buffer into
   which the data read from the sector is to be placed.  This method
   reads 512 bytes from the disk and places it in the buffer.

   \return This method returns one of the error codes from the list
   defined in ErrorCodes.
*/
int readSector(const int sector, const int segment, const int offset);

/**
   This method may be used to read data from a given cluster from the
   floppy disk into the given buffer.  Clusters are identified using a
   logical sector number with a 2 based index.  If the cluster is
   invalid then this method returns -1 indicating an error.  Note that
   a cluster may consist of multiple sectors.

   \note The initializeFAT12() function should have been invoked
   before this method is called.
      
   \param cluster The logical cluster number (two index based) from
   where the data is to be read into the buffer.

   \param segment The segment address value where the data must be
   stored.

   \param[out] bytesRead This value is updated with the number of
   bytes read.  This value is multiples of sector size because a
   cluster may consists of multiple sectors.
   
   \return This method returns one of the error codes from the list
   defined in ErrorCodes.h.
*/
int readCluster(const int sector, const int segment, const int offset,
		int *bytesRead);

/**

   This method may be used to read data from a given sector using int
   0x13h function 2.  The physical information (head, track, sector)
   from where the data is to be read must be passed in as the
   parameters.  in addition, the segment and offset of the buffer into
   which the sector must be read must be passed in as well.  

   \note Unlike other functions in this file, the initializeFAT12()
   function need <b>not</b> have been invoked before this method is called.

   \param[in] head The physical head number from where the data is to
   be read.  Typically the head value for floppy disk is 0 or 1.

   \param[in] track The track number from where the data is to be
   read.  Typically this value for a floppy disk is in the range 0 to
   80.

   \param[in] sector The sector on the given head/track that is to be
   read into memory.  Typically this value for a floppy disk is in the
   range 1 to 17.

   \param[in] segment The memory segment where the data read from the
   floppy disk is to be placed.

   \param[in] offset The offset in memory where the data read from the
   floppy disk is to be placed.
   
   \note On errors this function resets the floppy disk controller and
   retries the operation 3 times as per the BIOS API documentation.

   \return If the data is read successfully, then this method returns
   with SUCCESS.  Otherwise it returns a suitable error code.
*/
int readPhysicalSector(const byte head, const byte track, byte sector,
		       const byte sectorsToRead, const int segment,
		       const int offset);

/**
   This function may be used to obtain the BPB information from the
   boot sector.  The structure into which the BPB is to be copied in
   must be specified as a parameter.  If the parameter is NULL, this
   method returns without performed any operations.

   \param segment The segment address value where the data must be
   stored.

   \param offset The Address of the first byte in the buffer into
   which the data read from the sector is to be placed.  This method
   reads 512 bytes from the disk and places it in the buffer.
   
   \return This function copies the BPB information to the specified
   segment:offset value.
*/
int getBPB(const int segment, const int offset);

/**
   This function may be used to obtain the file entry given the
   logical index of a file in the root directory.  Given a file index,
   this function should perform the following steps:

   <OL>

   <LI> Check if the driver has been initialized by ensuring the
   floppy variable is not NULL.  If it is NULL return with error
   DRIVER_NOT_INITIALIZED. </LI>
   
   <LI> Check if the index is in the range 0 to cachedBPB.rootEntries.
   If not return INVALID_ROOT_DIRECTORY_INDEX. </LI>

   <LI> Compute the starting sector number from where the root
   directory is stored using the information in the BPB. </LI>

   <LI> Compute the sector in the root directory area where the file
   entry for the given index will be present using the sizeof(struct
   FileEntry) and cachedBpb.bytesPerSector.  Add this value to the
   starting sector number of the root directory (computed in previous
   step). </LI>

   <LI> Read the specified sector into an internal array using the
   readSector() method.  <i> Don't forget to check for errors</i>.
   </LI>

   <LI> Figure out the offset of the root directory entry in the given
   sector.  Call this value entryOffset </LI>

   <LI> Check if the byte at entryOffset is '\0'.  That indicates end
   of root directory entries and this function should return with
   INVALID_ROOT_DIRECTORY_INDEX. </LI>
   
   <LI> Otherwise, copy 32 bytes from entryOffset into the fe using
   memcpy function (man memcpy for details). Return SUCCESS. </LI>

   </OL>
   
   \param index The logical index entry in the root directory that
   must be read.

   \param segment The segment address value where the file entry must
   be stored.

   \param offset The address of the first byte in the buffer into
   which the file entry is to be copied.  This method writes
   sizeof(FileEntry) bytes from the disk and places in memory at the
   specified address.
   
   \return This method returns one of the error codes from the list
   defined in ErrorCodes.h.   It returns SUCCESS on success.  
*/
int getFileEntry(const int index, const int segment, const int offset);

/**
   This function must extract the FAT entry corresponding to the given
   cluster and store the information in the location pointed by
   fatEntry parameter.  This method should operate in the following
   manner:

   <OL>

   <LI> Check if the driver has been initialized by ensuring the
   floppy variable is not NULL.  If it is NULL return with error
   DRIVER_NOT_INITIALIZED. </LI>
   
   <LI> Next check to ensure that the cluster number is greater than
   0. </LI>

   <LI> Now, compute the logical offset in the FAT where the entry for
   the given cluster beings.  This can be determined via: cluster * 3
   / 2. </LI>

   <LI> Determine the sector in which the offset maps to (by dividing
   by 512). Read the sector and extract the appropriate byte (offset %
   512) from it and store it into a temproary unsigned short variable,
   say tempEntry. </LI>

   <LI> Increment offset and determine the sector in which the offset
   maps to (by dividing by 512).  Read the sector (if necessary) and
   extract the appropriate byte (offset % 512) from it, shift it to
   left by 8 bits, and bitwise OR it into tempEntry. </LI>

   <LI> Now tempEntry has necessary FAT entry information from which
   the actual value needs to be extracted and stored into the location
   pointed by fatEntry parameter, using the following information:

   <UL>

   <LI> If the cluster is even then the lower 12 bits of tempEntry
   have the desired FAT12 entry.</LI>

   <LI> On the other hand, if the cluster is odd then the high 12 bits
   of tempEntry have the desired FAT12 entry value. </LI>

   </UL>

   <LI> If all of the aforementioned steps proceed without errors
   return SUCCESS.  If you encounter errors (including I/O errors)
   along the way return immediately with appropriate error code. </LI>

   </OL>

   \param cluster The cluster number whose FAT entry is to be
   extracted.

   \param fatEntry A pointer to the location where the FAT entry
   corresponding to cluster is to be stored.

   \return This method returns SUCCESS if the FAT entry was
   successfully extracted and stored in the fatEntry parameter.
   Otherwise it returns a suitable error code.
 */
int getFATEntry(const int cluster, unsigned short *fatEntry);

/**
   This method is implemented to copy the complete contents of the
   specified fileName into the buffer (passed in as the parameter) if
   the filesize is less than the specified bufferSize.  The file size
   is filled in the fileSize parameter.  This method operates in the
   following manner:

   <OL>

   <LI> It iterate through the list of file entries (using the
   getFileEntry() function) until it finds the specified fileName. If
   a valid file name was not found returns with error code
   INVALID_FILE_NAME. </LI>

   <LI> It then checks to ensure that the file size is less than the
   bufferSize passed in.  If not it returns with error code
   FILE_TOO_LARGE.</LI>

   <LI> Starting with the first cluster, it reads (using readSector()
   function) cluster-by-cluster (note that a cluster consists of 1 or
   more sectors) into memory.  It then uses the getFatEntry() function
   to locate the next cluster in the file chain.  If an error occurs
   at any point during reading it exits the function immediately
   returning a suitable error code. </LI>

   <LI> Once the file has been successfully read, it updates the
   fileSize parameter value and return with SUCCESS.</LI>

   </OL>

   \param fileName The name (11 characters with a trailing '\0') of
   the file whose data is to be read into the buffer.

   \param bufferSegment The segement in memory that contains the
   buffer into which the file contents is to be read.

   \param bufferOffset The offset in memory that contains the buffer
   into which the file contents is to be read.
   
   \param bufferSize The maximum number of bytes that the buffer can
   contain.

   \param fileSize The file size as indicated by the root directory
   entry.

   \return If the data is successfully read this method returns
   SUCCESS.  Otherwise this method returns with a suitable error code.
 */
int readFile(const char* fileName, const unsigned short bufferSegment,
	     const unsigned short bufferOffset,
	     const int bufferSize, int *fileSize);

/**
   Utility function to convert file entry attributes to a human
   readable string representation.  This method needs to use bitwise
   operators to check each of the 5 bits in the attribute and
   appropriately concatenate a character to the buffer.  If none of
   the attribute values are set then this method should set buffer to
   an empty string.

   <TABLE>
   <TR><TD>Bit</TD><TD>Character</TD><TD>Description</TD></TR>

   <TR><TD>0</TD><TD>R</TD><TD>Read only</TD></TR>

   <TR><TD>1</TD><TD>H</TD><TD>Hidden file</TD></TR>   

   <TR><TD>2</TD><TD>S</TD><TD>System file</TD></TR>

   <TR><TD>3</TD><TD>V</TD><TD>Volume label</TD></TR>

   <TR><TD>4</TD><TD>D</TD><TD>Directory</TD></TR>

   <TR><TD>5</TD><TD>A</TD><TD>Archive</TD></TR>         
   
   </TABLE>
   
   \param attribute The attribute flag that must be decoded.

   \param buffer The string into which the attribute characters need
   to be stored.  Assume buffer can contain at least 7 characters.

   \return The buffer passed in to ease display of the decodedFAT12
   entries.
*/
const char* decodeFAT12FileAttributes(const byte attribute, char buffer[]);

#endif
