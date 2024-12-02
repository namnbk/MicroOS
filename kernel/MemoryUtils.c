#ifndef MEMORY_UTILITIES_C
#define MEMORY_UTILITIES_C

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

asm(".code16gcc\n");

#include "MemoryUtils.h"

void farmemcpy(const unsigned short destSegment,
	       const unsigned short destOffset,
	       const unsigned short sourceSegment,
	       const unsigned short sourceOffset,
	       const unsigned short length) {
  
  asm volatile("pushw %%ds\n\t"              /* Save ds */
	       "pushw %%es\n\t"              /* Save es */
	       "pushw %[destSeg]\n\t"
	       "popw  %%es\n\t"              /* es = destSegment */
	       "pushw %[srcSeg]\n\t"
	       "popw  %%ds\n\t"              /* ds = sourceSegment */
	       "movw  %[len], %%cx\n\t"      /* cx = length */
	       "movw  %[destOff], %%di\n\t"  /* di = destOffset */
	       "movw  %[srcOff], %%si\n\t"   /* si = sourceOffset */
	       "rep movsb\n\t"               /* copy bytes */
	       "popw  %%es\n\t"              /* Restore es */
	       "popw  %%ds\n\t"              /* Restore ds */
	       : /* No outputs */
	       : [destSeg] "g" (destSegment), [srcSeg] "g" (sourceSegment),
		 [destOff] "g" (destOffset),  [srcOff] "g" (sourceOffset),
		 [len] "g" (length)
	       : "ecx", "edi", "esi");
}

#endif
