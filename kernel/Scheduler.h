#ifndef SCHEDULER_H
#define SCHEDULER_H

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

/* This assembler directive is necessary to inform the assembler to
   generate 16-bit code even though gcc generates 32-bit code.
*/
asm(".code16gcc\n");

/** \file Scheduler.h

    \brief Functions and datastructures for the scheduler to use.

    This file contains various functions and data structures that are
    used by the scheduler.  Refer to the documentation with each
    function for further details.
*/

/**
   The following global variable is used to hold the pIndex of the
   process that currently owns the keyboard focus.  In other words,
   when keys are typed out on the keyboard, the keystrokes are put
   into this process's keyBuffer.  This value is initially set to -1.
*/
extern int FocusOwnerPIndex;

/**
   The following global variable is used to hold the pIndex of the
   process that is currently running on the CPU.  This value is
   initially set to -1.
*/
extern int RunningPIndex;

/**
    This function must be invoked in order to initialize the
    scheduler.  Currently this function does not perform any
    signficiant task other than set the values of FocusOwnerPIndex and
    the RunningPIndex global variables to their default initial values.
*/
void initializeScheduler();

/**
    This function is invoked from the CreateProcess function to
    establish the initial running stack for a newly created process.
    It switches the current stack segement for this operation.
    Consequently, this operation is placed within a critical section
    to ensure that it is never interrupted.

    \param[in] pIndex The process index in the ProcessTable whose
    stack is to be initialized.
*/
void initializeProcessStack(const int pIndex);

/**
   This is the core function of the Scheduler that performs context
   switching.  This function assumes that the process stacks have been
   properly initialized via a suitable call to the
   initializeProcessStack() function.  It performs the following
   tasks:

   <OL>

   <LI>It uses the \c schedule() function to determine the process to
   switch context to.  If the target process is the same as current
   process then it returns immediately. </LI>

   <LI>If the status of the current process is PROCESS_RUNNING then it
   changes the status of the current process to PROCESS_READY.  It
   changes the status of the target process to PROCESS_RUNNING. </LI>

   <LI>It then pushes flags, \c CS, and \c IP of the current process
   onto the stack.  It also pushes \c DS, \c ES, and all general
   purpose registers of the current process on to the current
   process's stack space. </LI>

   <LI>It then switches the stack to the target process's stack
   pointer and stack space.  It pops off all general purpose
   registers, \c ES, and \c DS registers for the target process from
   its process space.  It then uses the \c iret instruction to
   transfer control to the target process, thereby completing the
   context switch. </LI>

   </OL>
*/
void contextSwitch();

/**
    This is a utility function that determines the next process to be
    run on a context switch.  This function can be modified to
    implement various scheduling policies in the kernel.

    \param currPindex The process table index of the currently running
    process.
*/
int scheduler(const int currPindex);

/** \def CRITICAL_SECTION

    \brief Wrap a piece of C code in a critial section.

    This macro is used to wrap sections of C programs in a critical
    section.  A critical section is a piece of code that is guaranteed
    not to be interrupted (unless fatal errors occur) during normal
    operation.  Note that this macro preserves the values of the
    flags.  Consequently, if interrupts were previously disabled, they
    continue to remain in disabled state.

    This macro achieves a critical section in the following manner:

    <OL>

    <LI> First it saves the status of the interrupt flag on the stack
    using the \c pushfw instruction. </LI>

    <LI> It disables interrupt processing using the \c cli
    instruction </LI>

    <LI> It then embeds the statements passed in as the parameter.
    These statements must run very fast and not consume much CPU time.
    Furthermore this code must not block for any reason.  </LI>

    <LI> Once the code has run, it restores the state of the interrupt
    flag by simply popping the value of the flags off the stack. </LI>

    </OL>
*/
#define CRITICAL_SECTION(x)  asm volatile("pushfw\n\t"	\
                                          "cli\n\t"	\
					  );		\
                                          x     	\
                                          asm volatile("popfw\n\t")
    
#endif
