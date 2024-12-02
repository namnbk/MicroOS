#ifndef SYS_CALL_C
#define SYS_CALL_C

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

asm(".code16gcc\n");

#include "SysCall.h"

void exitProcess(const byte exitCode) {
  // Your implementation goes here.  Refer to instructions and inline
  // documentation to complete this function.
}

int write(const byte deviceID, const byte dataType, const byte color,
           const void* data) {
  unsigned short retVal = 0; 
  // Your implementation goes here.  Refer to instructions and inline
  // documentation to complete this function. 
  return retVal;
}

int read(const byte deviceID, const byte dataType, const byte color,
           const void* data) {
  unsigned short retVal = 0;
  // Your implementation goes here.  Refer to instructions and inline
  // documentation to complete this function.
  return retVal;
}

int getFileEntry(const byte entryIndex, struct FileEntry *fe) {
  unsigned short retVal = 0;
  // Your implementation goes here.  Refer to instructions and inline
  // documentation to complete this function.
  return retVal;
}

int exec(const char *fileName, const byte blockFlag) {
  unsigned short retVal = 0;
  // Your implementation goes here.  Refer to instructions and inline
  // documentation to complete this function. 
  return retVal;
}


#endif
