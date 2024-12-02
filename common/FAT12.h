#ifndef FAT12_H
#define FAT12_H

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */


/** \file FAT12.h

   \brief FAT12 data structures shared by kernel & application programs.

   This header file provides a set of data structures that the kernel
   uses to communicate FAT12 information back to the application.
   Application programs may use these definition to conveniently
   process the data returned by the OS.
*/

#include "DataTypes.h"

/* The following pragmas (compiler directives) are necessary to inform
   the compiler that the following structures must not be aligned to
   word boundaries but rather on the byte boundaries.  In order to do
   this the pragmas are organized in the following manner:

   1.  pack(push) : Save current structure packaging value onto stack.
   2.  pack(0)    : Set packing value to 0, .ie. no alignment and packing
                    without empty slots between fields.
   3.  pack(pop)  : Restore original packing value from the stack.

*/
#pragma pack(push)
#pragma pack(1)

/* \struct BPB.

   \brief The structure defining the contents of the BIOS Parameter
   Block (BPB).

   This structure provides a convenient mechanism for encapsulating
   the BPB data that is present in the first sector of a floppy disk.
   This data is embedded in the middle of of the boot sector.
   Detailed layout of the BPB along with interpretation of the data is
   available off Blackboard (under Course Documents -> Lectures).

   \note This structure is placed between suitable pragmas to ensure
   that it is packed properly immaterial of other compiler flags.
*/
struct BPB {
  char     oemName[8];
  short    bytesPerSector;
  byte     sectorsPerCluster;
  short    reservedSectors;
  byte     fatCount;
  short    rootEntries;
  short    sectorsCount;
  byte     mediaDescriptor;
  short    sectorsPerFAT;
  short    sectorsPerTrack;
  short    headCount;
  long     hiddenSectors;
  long     largeSectorsCount;
  byte     driveNumber;
  byte     currentHead;
  byte     signature;
  unsigned long serialNumber;
  char     volumeLabel[11];
  char     fileSystemID[8];
};

/** \struct FAT12Date

    \brief A (2-byte) bit-field structure to ease processing of date
    stamps associated with file entries in a FAT12 file system.

    This structure uses a convenient feature of the C language to
    define elements of a field at the bit level.  The compiler takes
    care of performing the bit-wise operations on this struct when
    individual fields are accessed.  This is a very powerful and novel
    feature of the C language that other languages (other than C++)
    don't support.  Note that the total size of this structure is 2
    bytes.  Details on how this field is organized is available off
    Blackboard (or the FAT12 standards document).
*/
struct FAT12Date {
  byte day:   5; /* Day is 5 bits in size */
  byte month: 4; /* Month is 4 bits in size  */
  byte year:  7; /* Year is 7 bits in size (offset from 1980) */
};

/** \struct FAT12Time

    \brief A (2-byte) bit-field structure to ease processing of time
    stamps associated with file entries in a FAT12 file system.

    This structure uses a convenient feature of the C language to
    define elements of a field at the bit level.  The compiler takes
    care of performing the bit-wise operations on this struct when
    individual fields are accessed.  This is a very powerful and novel
    feature of the C language that other languages (other than C++)
    don't support.  Note that the total size of this structure is 2
    bytes.  Details on how this field is organized is available off
    Blackboard (or the FAT12 standards document).
*/
struct FAT12Time {
  byte seconds: 5; /* This value is actually seconds / 2 */
  byte minutes: 6; /* 6 bits for 0-60 minutes */
  byte hours:   5; /* 5 bits for 0-23 hours */
};

/** \struct FileEntry

   \brief A 32 byte structure defining the contents of a file entry in
   the Root Directory of a FAT12 file system.

   This strcutrue provides a convenient mechanism for encapsulating
   file entry information for a given file entry present in the root
   directory of a floppy disk.  This data extracted from one of the
   root directory entries in a floppy disk.  Detailed description of
   each root directory entry is available off Blackboard (under Course
   Documents -> Lectures).

   \note This structure is placed between suitable pragmas to ensure
   that it is packed properly immaterial of other compiler flags.
*/
struct FileEntry {
  char fileName[11];           /* The actual file name            */
  byte fileAttributes;         /* A bit-flag indicating file      */
                               /* attributes like: read only,     */
                               /* hidden, system, etc.            */
  byte reserved1;              /* This byte is not really used.   */
  
  byte createTimeMillis;       /* msecs when file was created.     */
  struct FAT12Time createTime; /* File creation time.              */
  struct FAT12Date createDate; /* Date when the file was created.  */
  
  struct FAT12Date accessDate; /* Date when file was last accessed */
  unsigned short reserved2;    /* 2-bytes not really used          */
  
  struct FAT12Time changeTime; /* Time when file was last modified */
  struct FAT12Date changeDate; /* Date when file was last modified */
  
  short firstCluster;          /* Starting cluster number for file */
                               /* and first entry in FAT.          */
  unsigned long fileSize;      /* Size of the file in bytes        */
};

#pragma pack(pop)

#endif
