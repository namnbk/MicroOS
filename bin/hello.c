#ifndef HELLO_C
#define HELLO_C

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

asm(".code16gcc\n");

/* ----------------------------------------------------------------------- */
/*                      DO NOT ADD ANY METHODS TO THIS FILE!               */
/* ----------------------------------------------------------------------- */

#include "SysCall.h"
#include "Color.h"

void _start() {
  write(1, 1, GRAY, "This is a simple example program that just prints\r\n");
  write(1, 1, GRAY, "this message on screen and exits by saying\r\n");
  write(1, 1, WHITE, "                 Hello World!\r\n");
  exitProcess(0);
}

#endif
