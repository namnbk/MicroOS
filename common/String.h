#ifndef STRING_H
#define STRING_H

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

#include "DataTypes.h"

/** \file String.h

    This file provides alternative implementation for commonly used
    string functions.  These functions replace the standard functions
    supported by glibc.  Note that when developing a custom operating
    system, we cannot rely on libraries from other operating systems.
    Consequently, we have to reimplement core functionality.
*/

/**
   Copy a given number of bytes from src to dest.  This function
   provides implementation for the standard memcpy function.

   \param[out] dest The destination address where the data is to be
   copied.

   \param[in] src The source address from where the data is to be
   copied.

   \param[in] numBytes The number of bytes to be copied from dest to
   src.  If this value is <= 0 then no bytes are copied.
*/
void* memcpy(void *dest, const void* src, const unsigned int numBytes);

/**
   This function replaces the standard string length function.  This
   function counts the number of charachters in \c str (until 0).  If
   the parameter is NULL, then this method returns 0.

   \param[in] str The string whose length is to be computed.
*/
unsigned int strlen(const char *str);

/**
   Copy a given number of bytes from src to dest.  This function
   provides implementation for the standard strcpy function.

   \param[out] dest The destination address where the string is to be
   copied.

   \param[in] src The source address from where the string is to be
   copied.
*/
char *strcpy(char *dest, const char* src);

/**
   Compare only n characters of 2 strings.  This function provides
   implementation for the standard strncmp function.

   \param[in] s1 The first string to be used for comparison.

   \param[in] s2 The second string to be used for comparison.

   \param[in] n The number characters to compare.
*/
int strncmp(const char *s1, const char *s2, unsigned int n);

/**
    This function reverses the string str.

    \param[inout] str The string to be reversed.
*/
void reverseString(char *str);

/**
   This is a utility method that can be used to convert a short to
   string, particularly for display purposes.

   \param[in] value A pointer to the short value to be converted to
   string.

   \param[out] buffer The buffer into wich the string is to be stored.
   The buffer must be capable of holding at least 8 characters.
*/
void shortToString(short value, char buffer[]);

#endif
