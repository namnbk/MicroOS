#ifndef PROCESS_C
#define PROCESS_C

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

#include "Process.h"
#include "FAT12Driver.h"
#include "MemoryUtils.h"
#include "Scheduler.h"
#include "String.h"
#include "Video.h"

asm(".code16gcc\n");

/* The actual definition of the ProcessTable.  It is stored in the
   kernel's data segment.  Consequently, any time this table is
   accessed, the data segement must be set to the kernel's data
   segment.  */
struct ProcessInfo ProcessTable[MAX_PROCESSES];

void initializeProcesses() {
  int index;

  // Ensure we are in kernel's data segement.
  ENTER_KERNEL_DS();
  
  for(index = 0; (index < MAX_PROCESSES); index++) {
    /* initialize each process table entry */
    ProcessTable[index].pIndex      = index;
    ProcessTable[index].pID         = 0;
    ProcessTable[index].ppID        = 0;
    ProcessTable[index].pStatus     = UNUSED_PROCESS;
    ProcessTable[index].pSP         = 0x7fff - (SEGMENT_SHIFT << 4);
    ProcessTable[index].pSS         = 0x800 * (index + 1) + SEGMENT_SHIFT;
    ProcessTable[index].keyBufIndex = 0;
  }
  /* Note that first slot is reserved for kernel */
  ProcessTable[0].pSS         = 0x00;
  ProcessTable[0].pID         = 0;
  ProcessTable[0].ppID        = -1;
  ProcessTable[0].pStatus     = KERNEL_PROCESS;

  // Restore earlier ds
  LEAVE_KERNEL_DS();
}

int allocateProcess(const int ppIndex) {
  /* This static processID is stored in kernel's data segement */
  static int processID = 1;
  int index;
  CRITICAL_SECTION({
      // Ensure we are in kernel's data segement to correctly access
      // the ProcessTable.
      ENTER_KERNEL_DS();
      for(index = 0; (index < MAX_PROCESSES); index++) {
	if (ProcessTable[index].pStatus == UNUSED_PROCESS) {
	  /* Found an unused process slot! */
	  ProcessTable[index].pStatus = PROCESS_STARTING;
	  /* Reset basic critical process information */
	  ProcessTable[index].pSP = 0x7fff - (SEGMENT_SHIFT << 4);      
	  ProcessTable[index].pSS = 0x800 * (index + 1) + SEGMENT_SHIFT;
	  ProcessTable[index].ppID= ppIndex;
	  ProcessTable[index].pID = processID++;
	  ProcessTable[index].keyBufIndex = 0;
	  break;
	}
      }
      LEAVE_KERNEL_DS();
    });
  /* Return index on -1 depending on wether a valid index was found */
  return (index < MAX_PROCESSES) ? index : -1;
}

void deAllocateProcess(const int pIndex) {
  if ((pIndex >= 0) && (pIndex < MAX_PROCESSES)) {
    // Ensure we are in kernel's data segement to correctly access
    // the ProcessTable.
    ENTER_KERNEL_DS();
    ProcessTable[pIndex].pStatus = UNUSED_PROCESS;
    LEAVE_KERNEL_DS();
  }
}

int createProcess(const char *fileName, const byte block) {
  // First check if the file exists.
  int fileSize = 0;
  int cluster  = getStartingCluster(fileName, &fileSize);
  if (cluster < 2) {
    // File not found.
    return FILE_NOT_FOUND;
  }
  if ((fileSize < 1) || (fileSize > MAX_PROCESS_SIZE)) {
    return FILE_TOO_LARGE;
  }

  // Now that we have a valid file to operate with, let's try to
  // allocate process space.
  int ppIndex = getIndex(); /* Determine parent process index */
  int pIndex = allocateProcess(ppIndex);
  if (pIndex == -1) {
    // Oops. too many processes
    return TOO_MANY_PROCESSES;
  }

  int error, bytesRead;
  int offset = 0;
  unsigned short pSS = 0;
  ENTER_KERNEL_DS();
  pSS = ProcessTable[pIndex].pSS;
  LEAVE_KERNEL_DS();
  do {
    if ((error = readCluster(cluster, pSS, offset, &bytesRead)) != SUCCESS) {
      deAllocateProcess(pIndex);
      return error;
    }
    unsigned short nextCluster;
    if ((error = getFATEntry(cluster, &nextCluster)) != SUCCESS) {
      deAllocateProcess(pIndex);
      return error;
    }
    cluster = nextCluster;
    offset += bytesRead;
  } while (cluster < 0xff8);
  
  // Process has been created!  Initialize its runtime stack.
  initializeProcessStack(pIndex);
  
  // Finally decide if the new process should be run in a synchronous
  // or asynchronous manner.
  if (block) {
    // The new process should run in a blocking manner.  So the
    // current process should be blocked.
    if (getStatus(ppIndex) == PROCESS_RUNNING) {
      setStatus(ppIndex, PROCESS_WAITING_FOR_CHILD);
    }
    setStatus(pIndex, PROCESS_READY);
    CRITICAL_SECTION({contextSwitch();});
    // Ok the child process is done running.
    deAllocateProcess(pIndex);
  }
  else {
    // Simply set the new process to be ready to run and the scheduler
    // will run it when the time is right.
    setStatus(pIndex, PROCESS_READY);
  }
  
  return SUCCESS;
}

void stopProcess(int pIndex, const byte exitCode) {
  if (pIndex == -1) {
    pIndex = getIndex();
  }
  if ((pIndex < 1) || (pIndex >= MAX_PROCESSES)) {
    // Can't stop kernel or an invalid process.
    return;
  }
  // Change process status.
  ENTER_KERNEL_DS();
  ProcessTable[pIndex].pStatus = PROCESS_STOPPING;  
  int ppIndex = ProcessTable[pIndex].ppID;
  if (ProcessTable[ppIndex].pStatus == PROCESS_WAITING_FOR_CHILD) {
    // Unblock the parent that is waiting on this child.
    ProcessTable[ppIndex].pStatus = PROCESS_READY;
  }
  LEAVE_KERNEL_DS();
  CRITICAL_SECTION({contextSwitch();});
}

int getIndex() {
  unsigned short stackSegment;
  int index, retVal = -1;
  GET_SS(stackSegment);
  ENTER_KERNEL_DS();
  for(index = 0; (index < MAX_PROCESSES); index++) {
    if (ProcessTable[index].pSS == stackSegment) {
      retVal = index;
      break;
    }
  }
  LEAVE_KERNEL_DS();
  return retVal;
}

enum ProcessStatus getStatus(const int pIndex) {
  enum ProcessStatus retVal;
  ENTER_KERNEL_DS();
  retVal = ProcessTable[pIndex].pStatus;
  LEAVE_KERNEL_DS();
  return retVal;
}

void setStatus(const int pIndex, const enum ProcessStatus status) {
  ENTER_KERNEL_DS();
  ProcessTable[pIndex].pStatus = status;
  LEAVE_KERNEL_DS();  
}

int getStartingCluster(const char *fileName, int *fileSize) {
  struct FileEntry fe;
  unsigned short stackSegment;
  GET_SS(stackSegment);

  int index = 0;
  while (getFileEntry(index, stackSegment, OFFSET(&fe)) == SUCCESS) {
    if (!strncmp(fe.fileName, fileName, sizeof(fe.fileName))) {
      *fileSize = fe.fileSize;
      return fe.firstCluster;
    }
    index++;
  }

  return -1;
}

#endif
