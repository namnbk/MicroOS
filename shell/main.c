#ifndef SHELL_C
#define SHELL_C

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
/* DO NOT ADD ANY METHODS TO THIS FILE!   ADD METHODS TO Helper.c instead. */
/* ----------------------------------------------------------------------- */

#include "SysCall.h"
#include "Helpers.h"
#include "String.h"

void _start() {
  write(1, 1, 14, "\346OS (Micro Operating System) Shell\r\n");
  write(1, 1, 14, "Copyright (C) Miami University\r\n\r\n");
  char buffer[129];

  do {
    write(1, 1, 14, "\346> ");
    read(0, 1, 7, buffer);
    if (!strncmp(buffer, "ls", 3)) {
      ls();
      continue;
    }
    if (strncmp(buffer, "exit", 5)) {
      run(buffer);
    }
  } while (strncmp(buffer, "exit", 5));
  exitProcess(0);
}

#endif
