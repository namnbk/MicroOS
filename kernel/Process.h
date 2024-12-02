#ifndef PROCESS_H
#define PROCESS_H

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

#include "DataTypes.h"

/** \file Process.h

    \brief Segment memory and manage process(s).

    This file provides a set of of prototypes for functions that
    perform the following tasks:

    <UL>

    <LI> The \c initializeProcesses() function breaks the memory into
    logical blocks of 32KB.  It then initializes the ProcessTable with
    default entries.  The first 2 32KB blocks are typically used use
    by the kernel. </LI>

    <LI> Each logical block is associated with a Process and
    information regarding each process is stored in the ProcessInfo
    datastructure defined in this file.  An array of ProcessInfo
    datastructures constitute the ProcessTable data structure. </LI>

    <LI> The \c allocateMemory() function must be used to allocate a
    free chunk of memory if one is available.  </LI>

    <LI> The \c deallocateMemory() function must be used to deallocate
    a previously allocated chunk of memory. </LI>

    </UL>

    Refer to the details of each one of the functions and data
    structures for further information.
*/

/** \enum ProcessStatus

    \brief Different state or status a process can exist in.

    This enumeration defines the various states through which a
    process can transition throughout its lifetime.  The enumeration
    constants are interpreted in the following manner:

    <UL>

    <LI> \c UNUSED_PROCESS : This status incdicates that the process
    entry in the memory is not really used and is free to be used to
    load another process into memory.  Initialy the MemoryManager
    creates a set of process entries and set's their status to this
    value.</LI>

    <LI> \c PROCESS_STARTING: This status indicates that the kernel is
    in the process of loading this process into memory.  While the
    kernel is loading this process into memory it cannot be run nor
    can the memory be used for other purposes. </LI>

    <LI> \c PROCESS_BLOCKED: This status indicates that the process
    cannot be run right now as it is blocked waiting for some kind of
    I/O to be completed.  </LI>

    <LI> \c PROCESS_READY: This status indicates that the process is
    ready for using some CPU time but is currently not running because
    some other process (or the kernel) is using the CPU. </LI>
    
    <LI> \c PROCESS_RUNNING: This status indicates the process is
    actively running using the CPU. </LI>

    <LI> \c PROCESS_STOPPING: This status indicates that the kernel is
    in the process of shutting down this process.  </LI>
    
    </UL>

*/
enum ProcessStatus{UNUSED_PROCESS, PROCESS_STARTING, PROCESS_KEYBOARD_BLOCK,
		   PROCESS_WAITING_FOR_CHILD, 
		   PROCESS_READY, PROCESS_RUNNING, PROCESS_STOPPING,
		   KERNEL_PROCESS};

/** \def MAX_PROCESSES

    The following define places an upper limit on the maximum number
    of processes that can be supported by this kernel.  This value
    does includes the 1 process slot reserved for kernel use.  The
    maximum value is 19 while the minumum is 2.  The default is setup
    for 1 slot for kernel and 8 other processes.
*/
#define MAX_PROCESSES 9

/** \def MAX_PROCESS_SIZE

    The following define specifies the upper limit on the size of a
    process.
*/
#define MAX_PROCESS_SIZE 0x7fff

/** \def SEGMENT_SHIFT

    The following constant specifies the amount of shift that is to be
    added to the segment registers to allocate some empty space before
    the process.  Currently, this empty space is not used.  Later on
    it will be used to buffer the output data from this process to
    display multiple windows.
*/
#define SEGMENT_SHIFT 0x100

/** \def KEY_BUFFER_SIZE

    The maximum number of keystrokes that must be buffered for a
    process while it is is busy doing some other task.  Default value
    is 128.  This value must be atleast 2.  Don't push it above 128
    unless you really want to handle long input lines.
*/
#define KEY_BUFFER_SIZE 128

/** \struct ProcessInfo

    \brief Structure containing information about a process.

    This structure holds all the information needed by the kernel to
    effectively handle operations of a process.  This structure
    contains the following fields:

    <OL>

    <LI>\c pIndex: The constant index of this process entry in the
    process table.  This value is actually used to compute the default
    initial segment register (cs, ds, es, and ss) values for the
    process during initialization and process creation.  This value is
    set when the ProcessTable is created and is never changed. </LI>
    
    <LI>\c pID: A logical process ID associated with this process.
    The process ID is set each time a new process is created by the
    createProcess() function in Process.h </LI>

    <LI>\c ppID: The process ID of the parent process that created this
    process.  This value is set each time a new process is created by
    the createProcess() function in Process.h </LI>

    <LI>\c pStatus: The current status of the process.  This value is one
    of the predefined constant values defined in this header file. </LI>

    <LI>\c pSP: The stack pointer value saved for this process so that
    it can be restored after operations or a context switch. </LI>

    <LI> \c pSS: The stack segement value saved for this process so that
    it can be restored after kernel operations or a context
    switch. </LI>

    <LI> \c keyBuffer: An array used to buffer keystrokes for this
    process when the user types on the keyboard and the process is not
    yet ready to read or process them. </LI>

    <LI> \c keyEntry: Location of the next free entry in the keyBuffer
    where the next charachter can be stored.  This value is
    initialized to 0 to indicate the buffer is empty.  It is
    incremented each time user types a key by the keyboard ISR.  It
    never exceeds KEY_BUFFER_SIZE.  It is decremented each time the
    application reads data from the buffer. </LI>

    <LI> \c name: The file name from where the process information was
    loaded into memory. </LI>
    
    </OL>
*/
struct ProcessInfo {
  unsigned short pIndex;  /**< Index (constant) in the ProcessTable */
  unsigned short pID;     /**< process id */
  unsigned short ppID;    /**< parent process id */
  enum ProcessStatus  pStatus; /**< Status of the process */
  unsigned short pSP;     /**< The stack pointer value for this process  */
  unsigned short pSS;     /**< The stack segement value for this process */
  unsigned short keyBuffer[KEY_BUFFER_SIZE]; /**< Buffer for keystrokes */
  byte  keyBufIndex;   /**< Next free entry in keyBuffer */
  char name[12];       /**< File name of the executable */
};

/**
   A prototype definition for the ProcessTable so that it can be
   readily accessed by other functions in the kernel.
*/
extern struct ProcessInfo ProcessTable[MAX_PROCESSES];

/**
   This function must be invoked before other functions in the memory
   manager can be used.  This function initializes the ProcessTable
   data structure to its default initial values.

   \note This function temporarily changes the \c DS register to
   kernel's data segment to access the ProcessTable.  \c DS is
   restored and left unchanged.
*/
void initializeProcesses();

/**
   Reserves an unused process slot.  This function searches the
   ProcessTable to locate an unused (pStatus is set to UNUSED_PROCESS)
   process slot.  If an unused slot is located, then it marks the slot
   as PROCESS_STARTING and returns the pIndex back to the caller.  If
   a free slot was not found, then this function returns -1.

   \param[in] ppIndex The index of the parent process that is
   allocating this process entry.  No real checks are made on this
   value.

   \note This function temporarily changes the \c DS register to
   kernel's data segment to access the ProcessTable.  \c DS is
   restored and left unchanged.
   
   \return This method returns -1 if a free slot was not found.
   Otherwise it returns the pIndex associated with the process slot.
*/
int allocateProcess(const int ppIndex);

/**
   Deallocate a given process entry.  This method simply sets the
   pStatus value of the process entry associated with pIndex to
   UNUSED_PROCESS indicating that the slot is free.

   \note This function temporarily changes the \c DS register to
   kernel's data segment to access the ProcessTable.  \c DS is
   restored and left unchanged.
   
   \param[in] pIndex Index in the process table whose entry is to be
   marked as being free.  This value must be in the range 0 <= pIndex
   < MAX_PROCESSES
 */
void deAllocateProcess(const int pIndex);

/**
   This is a helper function that is used to deterime the starting
   cluster of a file in the root directory.  This function uses the
   FAT12Driver::getFileEntry() function to read one entry at a time.
   If the fileName is found then it updates the fileSize parameter and
   returns its starting cluster.  If the filename is not found, it
   return -1.
   
   \param[in] fileName This is the 8, 3 (without dot) representation of
   the file whose first cluster is desired.

   \param[out] fileSize The size of the file as reported in the file
   entry.

   \return The starting cluster number if file was found.  If the file
   was not found then it returns -1.
*/
int getStartingCluster(const char *fileName, int *fileSize);

/**
    This function is a major functionality function with in the
    operating system.  It is responsible for loading a program from
    disk, creating a new process, and running the program.  It
    performs this task in the following manner:

    <OL>

    <LI> First it checks to ensure that a file with the given fileName
    exists on the floppy.  If the file is not present, then it returns
    with error code FILE_NOT_FOUND. </LI>

    <LI> If the file size is greater than 32 KB then it returns with
    error message FILE_TOO_LARGE. </LI>

    <LI> Next it attempts to allocate a process entry by calling the
    allocateProcess() function.  If a new process space cound not be
    allocated, then it returns with error TOO_MANY_PROCESSES.

    <LI> Once a process space has been allocated, it loads the program
    into the specified process space using the readCluster() and
    getFATEntry() functions in the FAT12Driver file.  If errors occur
    during the read process it deallocates the process (via
    deAllocateProcess() function) and returns immediately with a
    suitable error code. </LI>

    <LI>If the file is loaded successfully, it initializes the
    proces's stack (via intializeProcessStack() function). </LI>

    <LI>If the process is created in a blocking manner, then it sets
    the current process's status to PROCESS_WAITING_FOR_CHILD and
    immediately context switches to the newly created process by
    invoking the contextSwitch() function in the Scheduler</LI>

    <LI> It returns with code SUCCESS if the process was
    executed. </LI>
    
    </OL>

    \param[in] fileName The 8,3 (without period) filename of the
    program that should be started up as a new process.

    \param[in] block If this flag is set to a non-zero value, then the
    calling process is blocked until the newly created process
    completes running.

    \return This function returns SUCCESS if the process was started
    up successfully.  Otherwise it returns one of the errors defined
    in ErrorCodes.h
*/
int createProcess(const char *fileName, const byte block);

/**
    This function must be used to stop a process.  It is typically
    invoked from the exit() function in Service.h.  This function
    performs the following tasks:

    <OL>

    <LI> First it changes the processs's status to PROCESS_STOPPING.
    </LI>

    <li>If the parent process's status is PROCESS_WAITING_FOR_CHILD
    then it changes the parent process's status to
    PROCESS_READY. </li>

    </li> It then forces a context switch by calling the
    contextSwitch() function in the Scheduler. </LI>

    </OL>

    \param[in] pIndex The index of the process in the ProcessTable
    that must be terminated.  If this value is -1, then this function
    termines the currently running process (if it is not the kernel).

    \param[in] exitCode The exit code to be reported for this
    process's termination.

    \note This function temporarily changes the \c DS register to
    kernel's data segment to access the ProcessTable.  \c DS is
    restored and left unchanged.    
*/
void stopProcess(int pIndex, const byte exitCode);

/**
    This function is a helper function that uses the current stack
    segment value and searches the ProcessTable to locate the entry
    corresponding to the given stack segment.  If an entry is found it
    returns the corresponding index.  Otherwise it returns 0.

    \note This function temporarily changes the \c DS register to
    kernel's data segment to access the ProcessTable.  \c DS is
    restored and left unchanged.
    
    \return The index corresponding to the current process that is
    invoking functions in the kernel.
*/
int getIndex();

/**
    This is a utility function that is used to obtain the current
    status of a given process.

    \param[in] pIndex The index of the process whose status is
    required.  This value must be in the range -1 < pIndex <
    MAX_PROCESSES.

    \return The current status of process in the ProcessTable at the
    given pIndex.

    \note This function temporarily changes the \c DS register to
    kernel's data segment to access the ProcessTable.  \c DS is
    restored and left unchanged.
*/
enum ProcessStatus getStatus(int pIndex);

/**
    This is a utility function that is used to set the current status
    of a given process.

    \param[in] pIndex The index of the process whose status is
    required.  This value must be in the range -1 < pIndex <
    MAX_PROCESSES.

    \param[in] status The new status to be set for the process.

    \note This function temporarily changes the \c DS register to
    kernel's data segment to access the ProcessTable.  \c DS is
    restored and left unchanged.
*/
void setStatus(int pIndex, const enum ProcessStatus status);

#endif
