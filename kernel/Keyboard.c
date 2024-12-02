#ifndef KEYBOARD_DRIVER_C
#define KEYBOARD_DRIVER_C


/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

/* The following asm directive informs the assembler to generate */
/* 16-bit code even though gcc spits out 32-bit code.            */
asm(".code16gcc\n");

#include "Keyboard.h"

char
haveKeyStroke() {
  unsigned short haveKey = 0;
  asm volatile ("movb $0x1, %%ah\n\t"
		"int $0x16\n\t"
		"jnz haveCode\n\t"
		"xorw %%ax, %%ax\n\t"
		"haveCode: movw %%ax, %[flag]\n\t"
		: [flag] "=g" (haveKey) 
		: /* No input registers. */
		:"eax"
		);
  if (haveKey == 0) {
    return 0;
  }
  return 1;
}

unsigned short
getKeyStroke() {
  unsigned short keyStroke = -1;
  
  asm volatile ("movb $0x0, %%ah\n\t"
		"int $0x16\n\t"
		"movw %%ax, %[key]\n\t"
		: [key] "=m" (keyStroke) 
		: /* No input registers. */
		:"eax"
		);
  return keyStroke;
}

#endif
