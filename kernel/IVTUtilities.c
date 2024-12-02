#ifndef IVT_UTILITIES_C
#define IVT_UTILITIES_C

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*   ___  ___                    DJ Rao's Operating System              */
/*  |   \| _ \___ ___            Copyright (C) 2006 raodm@muohio.edu    */
/*  | |) |   / _ (_-<            Miami University                       */
/*  |___/|_|_\___/__/            Oxford, OH                             */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

asm(".code16gcc\n");

#include "IVTUtilities.h"

void setIVT(const byte entry, const short isr) {
  unsigned int offset = (entry << 2);
  
  asm volatile ("cli\n\t"
		"pushw %%es\n\t"
		"pushw $0\n\t"
		"popw %%es\n\t"
		"movl %[offset], %%eax\n\t"
		"movw %%cs, %%es:2(%%eax)\n\t"
		"movw %[isr], %%bx\n\t"
		"movw %%bx, %%es:0(%%eax)\n\t"
		"popw %%es\n\t"
		"sti\n\t"
		:  /* No outputs */
		: [isr] "g" (isr), [offset] "g" (offset)
		: "eax", "ebx" /* No other clobbered registers */
		);
}

void getIVT(const byte entry, unsigned long *address) {
  unsigned int offset = (entry << 2);
  
  asm volatile ("pushw %%es\n\t"
		"pushw $0\n\t"
		"popw %%es\n\t"
		"movl %[offset], %%eax\n\t"
		"movl %%es:(%%eax), %%ebx\n\t"
		"movl %[addr], %%eax\n\t"
		"movl %%ebx, (%%eax)\n\t"
		"popw %%es\n\t"
		: [addr] "=m" (address)
		: [offset] "g" (offset)
		: "eax", "ebx" /* No other clobbered registers */
		);
}

void callISR(const unsigned long address) {
  asm volatile ("pushfw\n\t"
		"lcallw *%[addr]\n\t"
		:
		: [addr] "m" (address)
		: "memory"
		);
}

#endif
