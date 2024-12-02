#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

/** \file Video.h A simple video dirver for µos.

   This files provides a set of prototype declerations for the different
   API methods supported by the simple video dirver.

   \note This code is compiled into a 16-bit executable.
   Consequently, only 16-bit datatypes must be used throughout this
   code.
*/
#include "DataTypes.h"
#include "Color.h"

/**
   \brief Display a single charachter with a given color.

   This function calls BIOS interrupt 0x10 to display the character
   passed in with the given color.

   \param[in] data The character to be displayed on screen.  Npte that
   the value is internally converted to unsigned byte.  Consequently
   passing in negative values results in changing the value to greater
   than 128 and displays special characters.

   \param[in] The color in which the character is to be displayed.
   Use the \c MAKE_COLOR() macro to define a color consisting of a
   foreground and background color.

   \see MAKE_COLOR
*/
void displayChar(const char data, const byte color);

/**

   This function is a convenience function that displays a string by
   calling \c displayChar on each character.

   \param[in] data The string to be displayed (must be terminted by
   '\0').

   \param[in] color The color in which the character is to be
   displayed.  Use the \c MAKE_COLOR() macro to define a color
   consisting of a foreground and background color.

   \see MAKE_COLOR
*/
void displayString(const char *data, const byte color);

/**
    This function can be used to to move the cursor to a given row and
    column on the screen.  This function uses int 0x10 for achieving
    this functionality.

    \param[in] row The row to which the cursor should be moved.  This
    value must be in the range 0 to 24.

    \param[in] col The column on the screen where the cursor should be
    moved.  This value must be in the range 0 to 79.
*/
void moveCursor(const byte row, const byte column);

/**
    This function can be used to determine the current location of the
    cursor on the screen.  It uses BIOS interrupt 0x10 to determine
    the location of the cursor and then copies the value into the
    parameters specified.

    \param[out] row The row where the cursor is currently located.
    This value is in the range 0 to 24 (inclusive).

    \param[out] column The column where the cursor is currently
    located.  This value is in the range 0 to 79 (inclusive).
*/
void getCursor(byte *row, byte *column);

/**
    This function can be used to clear the contents of the screen by
    filling in blank spaces with the given color.  This can be used to
    change the background of the screen as well.

   \param[in] color The color in which the character is to be
   displayed.  Use the \c MAKE_COLOR() macro to define a color
   consisting of a foreground and background color.

   \see MAKE_COLOR   
*/
void clearScreen(const byte color);

/**
   This function uses the displayString() function to display a series
   of special characters on the screen giving the impression of
   drawing a box.  The characters are printed such that the top-left
   corner of the box is at row and column and its size is width x
   height.

   \param[in] row The row corresponding to the the top-left corner of
   the box.

   \param[in] column The column corresponding to the top-left corner
   of the box.

   \param[in] width The widht of the box to be drawn on screen.

   \param[in] height The height of the box to be drawn on screen.

   \param[in] color The color in which the character is to be
   displayed.  Use the \c MAKE_COLOR() macro to define a color
   consisting of a foreground and background color.

   \see MAKE_COLOR
*/
void
drawBox(const byte row, const byte column, const int width, const int height,
	const byte color);

#endif
