#ifndef STARTUP_C
#define STARTUP_C

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

asm(".code16gcc\n");

#include "Video.h"
#include "Keyboard.h"
#include "Startup.h"
#include "Process.h"
#include "FAT12Driver.h"

void
showBanner() {
    clearScreen(MAKE_COLOR(0, 0));
    drawBox(1, 10, 60, 7, MAKE_COLOR(7, 4));
    moveCursor(2, 11);
    displayString("        ___  ___", MAKE_COLOR(15, 4));
    moveCursor(3, 11);
    displayString("  _  _ / _ \\/ __|", MAKE_COLOR(15, 4));
    moveCursor(4, 11);
    displayString(" | || | (_) \\__ \\", MAKE_COLOR(15, 4));
    moveCursor(5, 11);
    displayString(" | .__|\\___/|___/", MAKE_COLOR(15, 4));
    moveCursor(6, 11);
    displayString(" |_|", MAKE_COLOR(15, 4));

    moveCursor(3, 32);
    displayString("\346OS: Micro Operating System", MAKE_COLOR(15, 4));
    moveCursor(4, 32);    
    displayString("Copyright (C) 2007 raodm@muohio.edu", MAKE_COLOR(15, 4));
    moveCursor(5, 32);    
    displayString("Miami University", MAKE_COLOR(15, 4));
    moveCursor(6, 32);    
    displayString("Oxford OH.", MAKE_COLOR(15, 4));        

    moveCursor(10, 0);
}

void reboot() {
  displayString("Press any key to reboot...", MAKE_COLOR(12, 0));
  getKeyStroke();
  asm volatile("movw $0x40, %ax\n\t"
	       "movw %ax, %es\n\t"
	       "movw $0x72, %ax\n\t"
	       "movw $0x1234, %es:(%eax)\n\t"
	       "ljmp $0xffff, $0\n\t");
}

void loadShell() {
  if (createProcess("SHELL   BIN", 1) != SUCCESS) {
    displayString("Error loading shell.\r\n", MAKE_COLOR(12, 0));
  }
}

#endif
