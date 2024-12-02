#ifndef STRING_C
#define STRING_C

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

asm(".code16gcc\n");

#include "String.h"

void* memcpy(void *dest, const void *src, const unsigned int numBytes) {
  if ((dest == NULL) || (src == NULL)) {
    return dest;
  }
  byte *target       = (byte *) dest;
  const byte *source = (const byte *) src;
  
  int i = 0;
  for(i = 0; (i < numBytes); i++) {
    target[i] = source[i];
  }

  return dest;
}

unsigned int strlen(const char *str) {
  int length;
  if (str == NULL) {
    return 0;
  }
  for(length = 0; str[length]; length++);
  return length;
}

char* strcpy(char *dest, const char* src) {
  if ((dest == NULL) || (src == NULL)) {
    return dest;
  }
  int i = 0;
  do {
    dest[i] = src[i];
    i++;
  } while (src[i]);

  return dest;
}

int strncmp(const char *s1, const char *s2, unsigned int n) {
  if (s1 == s2) {
    return 0;
  }
  int i;
  int diff;
  for(i = 0; (s1[i] && s2[i] && (i < n)); i++) {
    if ((diff = s1[i] - s2[i])) {
      return diff;
    }
  }

  return 0;
}

void reverseString(char *str) {
  int i = 0, j = strlen(str) - 1;
  while (i < j) {
    // swap str[i] and str[j]
    char temp = str[i];
    str[i]    = str[j];
    str[j]    = temp;
    i++;
    j--;
  }
}

void shortToString(short value, char buffer[]) {
  byte sign = 0;
  if (value < 0) {
    sign = 1;
    value *= -1;
  }
  int index = 0;
  do {
    buffer[index] = '0' + (value % 10);
    value        /= 10;
    index++;
  } while (value > 0);
  if (sign) {
    buffer[index] = '-';
    index++;
  }
  buffer[index] = '\0';
  // Reverse buffer.
  reverseString(buffer);
}

#endif
