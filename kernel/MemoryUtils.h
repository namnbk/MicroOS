#ifndef MEMORY_UTILITIES_H
#define MEMORY_UTILITIES_H

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

/** \def GET_SS

    \brief Macro to obtain value of stack segment register.

    This macro provides a set of assembly instructions that can be
    used to obtain the current value of \c SS (stack segment) register
    and store it in a given unsigned short variable.

    \note Only ax register is destroyed.  All other registers are
    preserved.
*/
#define GET_SS(segment) asm volatile("mov %%ss, %%ax\n\t"	\
				     "mov %%ax, %[seg]\n\t"	\
				     : [seg] "=m" (segment)	\
				     : /* No inputs */		\
				     : "eax");

/** \def GET_DS

    \brief Macro to obtain value of data segment register.

    This macro provides a set of assembly instructions that can be
    used to obtain the current value of \c DS (data segment) register
    and store it in a given unsigned short variable.

    \note Only ax register is destroyed.  All other registers are
    preserved.    
*/
#define GET_DS(segment) asm volatile("mov %%ds, %%ax\n\t"	\
				     "mov %%ax, %[seg]\n\t"	\
				     : [seg] "=m" (segment)	\
				     : /* No inputs */		\
				     : "eax");

/** \def OFFSET

    This is a convenience macro to determine the offset of a given
    pointer x.
*/
#define OFFSET(x) ((unsigned short) (unsigned int) (x))

/**
    Utility method to copy data across segements.  This is a utility
    function written in pure assembly to copy data from one segment to
    another.

    \param[in] destSegment The destination segment value to which the
    data is to be copied.

    \param[in] destOffset The offset in the destination segement where
    the data is to be stored.

    \param[in] sourceSegment The source segment from where the data is
    to be copied.

    \param[in] sourceOffset The offset within the source segment from
    where the data is to be copied.

    \param[in] length The number of <b>bytes</b> to be copied from
    SourceSegment:SourceOffset to DestSegment:destOffset.
*/
void farmemcpy(const unsigned short destSegment,
	       const unsigned short destOffset,
	       const unsigned short sourceSegment,
	       const unsigned short sourceOffset,
	       const unsigned short length);


/** \def ENTER_KERNEL_DS

    \brief Macro to push ds and then change it to 0.

    This macro provides a set of assembly instructions that can be
    used to change the data segment register to point to kernel space.
    This macro pushes the current value of \c DS (data segment)
    register and then changes it to 0 (zero).  The \c AX register is
    destroyed.

    \note Only ax register is destroyed.  All other registers are
    preserved.    
*/
#define ENTER_KERNEL_DS() asm volatile("pushw %%ds\n\t"		\
				       "xorw %%ax, %%ax\n\t"	\
				       "movw %%ax, %%ds\n\t"	\
				       : : : "eax", "memory")

/** \def LEAVE_KERNEL_DS

    This macro simply pops the \c DS register off the stack.
*/
#define LEAVE_KERNEL_DS() asm volatile("popw %ds\n\t")

#endif
