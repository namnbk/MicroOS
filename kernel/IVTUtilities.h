#ifndef IVT_UTILITIES_H
#define IVT_UTILITIES_H

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

asm(".code16gcc\n");

#include "DataTypes.h"

/**\file IVTUtilities.h

    This file provides prototypes for various functions that can be
    used to manipulate Interrupt Vector Table (IVT) entries.  These
    functions are typically used to develop Interrupt Service Routine
    (ISR) for the operating system.
*/

/**
   Function set a given IVT entry to point to a given interrupt
   service routine.  This method changes the IVT entry corresponding
   to the given interruptNumber to point to cs:isrAddress.

   \param[in] interruptNumber The interrupt number whose IVT entry
   must be modified.

   \param[in] isrAddress The address (offset only, the code segement
   is auotmatically used) where the ISR for the given interrupt is
   located.
 */
void setIVT(const byte interruptNumber, const short isrAddress);

/**
   Function to obtain the IVT entry corresponding to a given interrupt
   number.  This function does <b>not</b> modify the IVT entry.

   \param[in] interruptNumber The interrupt number whose IVT entry
   is to be read and stored in address.

   \param[out] address Pointer to a long where the cs:offset
   associated with the ISR for the given interruptNumber is to be
   copied.  
 */
void getIVT(const byte interruptNumber, unsigned long *address);

/**
   This is a convenience function to invoke an ISR given the full
   address (cs:ip) of the ISR in memory.

   \param[in] addrss The address where the ISR is stored.  This value
   is typically obtained via a call to getIVT() function.

   \note This function does not save any registers.
*/
void callISR(const unsigned long address);

/** \def ISR_CALLER_PROLOGUE

    The following #define macro provides a standard set of assembly
    instructions that serve as a default entry prologue to an ISR.
    The instructions perform the following tasks:

    <OL>

    <LI> First all the general purpose registers are saved (using
    \c pusha instruction)</LI>

    <LI> It pushes \c ds and \c es registers on to the stack </LI>

    <LI> Sets \c ds and \c es to 0 setting it to kernel space. </LI>

    </OL>

    A typical usage of this macro is shown below:

    \code
      void keyboardISRCaller() {
	ISR_CALLER_PROLOGUE();
	// Call the actual ISR.
	keyboardISR();
	ISR_CALLER_EPILOGUE();
      }
    \endcode
*/
#define ISR_CALLER_PROLOGUE() asm volatile ("pushaw\n\t"\
					    "pushw %ds\n\t" \
					    "pushw %es\n\t" \
                                            "xorw %ax, %ax\n\t" \
                                            "movw %ax, %ds\n\t" \
                                            "movw %ax, %es\n\t")

/** \def ISR_CALLER_EPILOGUE

     The following #define macro provides a standard set of assembly
     instructions that undo the operations of the ISR_CALLER_PROLOGUE.
     This macro performs the following tasks:

     <OL>

     <LI> Pops \c es and \ds registers off the stack </LI>

     <LI> Pops all general purpose registers off the stack (using \c
     popa instruction)</LI>

     <LI> Resets stack pointer and uses \c iret to exit the ISR</LI>

     </OL>

    A typical usage of this macro is shown below:

    \code
      void keyboardISRCaller() {
	ISR_CALLER_PROLOGUE();
	// Call the actual ISR.
	keyboardISR();
	ISR_CALLER_EPILOGUE();
      }
    \endcode     
*/
#define ISR_CALLER_EPILOGUE() asm volatile("popw %es\n\t" \
                                           "popw %ds\n\t"\
                                           "popaw\n\t"\
	                                   "leave\n\t" \
	                                   "iretw\n\t")

#endif
