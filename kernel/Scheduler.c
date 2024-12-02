#ifndef SCHEDULER_C
#define SCHEDULER_C

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

#include "Process.h"
#include "MemoryUtils.h"
#include "String.h"
#include "Scheduler.h"

asm(".code16gcc\n");

/* The actual definition of FocusOwnerPIndex */
int FocusOwnerPIndex = -1;

/* The actual definition of RunningPIndex */
int RunningPIndex = 0;

void initializeScheduler() {
  FocusOwnerPIndex = -1;
  RunningPIndex = 0;  
}

void contextSwitch() {
  int currPindex = 0;
  ENTER_KERNEL_DS();
  currPindex = RunningPIndex;
  LEAVE_KERNEL_DS();
  
  int newPindex  = scheduler(currPindex); /* Index of new process */
  if ((newPindex == -1) || (newPindex == currPindex)) {
    // Huh! don't have anything to schedule!
    // should the kernel panic?
    return;
  }

  // Change status of current process if needed.
  if (getStatus(currPindex) == PROCESS_RUNNING) {
    setStatus(currPindex, PROCESS_READY);
  }
  // Change status of the new process.
  setStatus(newPindex, PROCESS_RUNNING);
  
  ENTER_KERNEL_DS();
  // Setup the current running process index
  RunningPIndex    = newPindex;
  FocusOwnerPIndex = newPindex;
  unsigned short *oldSP = &ProcessTable[currPindex].pSP;
  unsigned short newSP  = ProcessTable[newPindex].pSP;
  unsigned short newSS  = ProcessTable[newPindex].pSS;
  LEAVE_KERNEL_DS();
  
  asm volatile ("pushfw\n\t"            /* Save current flags */
		"pushw %%cs\n\t"        /* Save code sgement  */
		"pushw $endSwitch\n\t"  /* Set destination IP */
		"pushw %%ds\n\t"        /* Save data seg.     */
		"pushw %%es\n\t"        /* Save extra seg.    */
		"pushaw\n\t"            /* Save all gp. reg.  */

		"movw $0, %%ax\n\t"       /* Set as = 0 to set es=0 */
		"movw %%ax, %%es\n\t"     /* set es=0 */
		"movw %[oldSP], %%ax\n\t" /* Save current SP value */
		"movw %%sp, %%es:(%%eax)\n\t"  /* Save current SP value */
		"movw %[newSP], %%sp\n\t" /* Set up new SP value */
		"movw %[newSS], %%ss\n\t" /* Set new SS */
		"popaw\n\t"               /* Retore other process's gp. reg.*/
		"popw %%es\n\t"           /* Restore es of other process */
		"popw %%ds\n\t"           /* Restore ds of other process */
		"iretw\n\t"               /* Transfer to other process */
		"endSwitch:"
		: /* No output */
		: [oldSP] "m" (oldSP), [newSS] "m" (newSS), [newSP] "m" (newSP)
		: "eax", "memory");
}

int scheduler(int currPindex) {
  if (currPindex < 0) {
    currPindex = 0;
  }
  int nextPindex = currPindex;
  do {
    nextPindex++;
    if (nextPindex >= MAX_PROCESSES) {
      nextPindex = 0;
    }
    if (getStatus(nextPindex) == PROCESS_READY) {
      // This process is ready to run;
      return nextPindex;
    }
  } while (currPindex != nextPindex);
  // Don't have a process to schedule!
  return 0;
}

void initializeProcessStack(const int pIndex) {
  if ((pIndex < 0) || (pIndex >= MAX_PROCESSES)) {
    /* Invalid index.  Nothing further to do. */
    return;
  }

  ENTER_KERNEL_DS();
  unsigned short stackSegment = ProcessTable[pIndex].pSS;
  unsigned short stackPointer = ProcessTable[pIndex].pSP;

  
  /* Rest of this code mucks with the stack segment and must be in a
     critical  section to avoid unforeseen problems. */
  CRITICAL_SECTION({
      asm volatile("movw %%ss, %%ax\n\t"     /* Save SS in ax */
		   "movw %%sp, %%bx\n\t"     /* Save SP in bx */
		   "movw %[newSP], %%sp\n\t" /* Setup new stack pointer */
		   "movw %[newSS], %%ss\n\t" /* Setup new stack segment */
		 
		   "pushfw\n\t"              /* Setup flags             */
		   "pushw %%ss\n\t"          /* Setup ss=cs for iret    */
		   "pushw $0\n\t"            /* Setup initial ip=0      */
		   "pushw %%ss\n\t"          /* Setup ds = ss for prss  */
		   "pushw %%ss\n\t"          /* Setup es = ss for prss  */
		   "pushaw\n\t"              /* Setup gp. regs          */
		   
		   "movw %%sp, %%cx\n\t"     /* Save new SP in cx.      */
		   "movw %%bx, %%sp\n\t"     /* Restore the stack pointer */
		   "movw %%ax, %%ss\n\t"     /* Restore the stack segment */
		   "movw %%cx, %[newSP]\n\t" /* Update new stack pointer */
		   : [newSP] "=m" (stackPointer)
		   : [newSS] "m"  (stackSegment)
		   : "eax", "ebx" /* Clobbered registers */
		   );
      ProcessTable[pIndex].pSP = stackPointer;
    });
  LEAVE_KERNEL_DS();  
}

#endif
