#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H


/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

/* A simple keyboard driver for muos.

   This files provides a set of forward declarations for the different
   API methods supported by the simple keyboard driver.  These
   functions call BIOS interrupt 0x16 to achieve their functionality.

   @note This code is compiled into a 16-bit executable.
   Consequently, only 16-bit data types must be used throughout this
   code.
*/

#include "DataTypes.h"

/**
    This function returns a 1 or 0 value to indicate if a key stroke
    is available or not.  This method uses the BIOS 0x16 function to
    achieve this functionality.

    \return This method returns 0 if no key stroke is pending or
    readily available in the internal buffer.  If a keystroke is
    available then it returns 1.
*/
char haveKeyStroke();

/**
    This function returns a keystroke typed on the keyboard back to
    the caller.  If a keystroke is not readily available, then this
    function blocks until a keystroke is available.  So be careful
    when calling this method if you want to ensure non-blocking
    operations.

    \return This function returns the ASCII code (in least significant
    byte) and scan code for the key pressed (in the most significant
    byte).
*/
unsigned short getKeyStroke();

#endif
