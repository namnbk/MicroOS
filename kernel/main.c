#ifndef MAIN_C
#define MAIN_C

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

/* This macro is necessary to inform the assembler to generate 16-bit
   code even though gcc generates 32-bit code.
*/
asm(".code16gcc\n");

#include "Startup.h"
#include "FAT12Driver.h"
#include "Process.h"
#include "Scheduler.h"
#include "Services.h"

void _start() {
  showBanner();
  initializeFAT12();
  initializeProcesses();
  initializeScheduler();
  initializeServices();
  
  loadShell();
  reboot();
}

#endif
