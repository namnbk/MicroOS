#ifndef STARTUP_H
#define STARTUP_H

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

/**
    This is a simple utility function that displays the initial
    startup banner (in a Red box) with the copyright information.
    This function uses the drawBox() and displayString() function to
    achieve its functionality.  This function is typically invoked
    from the \c main method.
*/
void showBanner();

/**
    This function is invoked after the \c main method.  It uses the \c
    createProcess function in Process.h to try and startup a new shell
    process.  It assumes that the shell is named "SHELL.BIN" (all
    caps) and is stored on the floppy disk.
*/
void loadShell();

/**
    This is a utility function that is used to perform a cold reboot
    of the system.  It sets the word at 0x40:0x72 to 0x1234 and jumps
    to 0xffff:0x000 as per BIOS documentation. 
*/
void reboot();

#endif
