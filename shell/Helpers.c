#ifndef HELPERS_C
#define HELPERS_C

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

asm(".code16gcc\n");

#include "Helpers.h"
#include "String.h"
#include "SysCall.h"
#include "Color.h"
#include "ErrorCodes.h"

void ls() {
  struct FileEntry fe;
  // This function is expected to use the getFileEntry() sys call to
  // read one file at a time as long as getFileEntry() returns
  // SUCCESS.  For each file entry successfully read, print its
  // filename and size using the write() sys call with suitable
  // parameters.
}


void run(char *buffer) {
  // First check to ensure that buffer contains a valid file name. A
  // file name is not valid because of any one of the following
  // reasons:
  
  //    1. The file name is longer than 11 characters or has 0
  //       charachters.
  //    2. The file name has a "." after the 8th character or as the
  //       1st character
  //    3. The file name's extension is not BIN
  
  // If a filename is invalid display "Invalid program name or
  // command\r\n" in BRIGHT_RED and return.
  
  
  // Otherwise convert the buffer to 8, 3 (without period) file
  // format.  For example if buffer contains "CLEAR" you must convert
  // it to "CLEAR BIN".  On the other hand, if buffer contains
  // "HELLO.BIN" convert it to "HELLO BIN".

  // Run the program using the exec() sys call with suitable
  // parameters.  If exec returns an error print an error message in
  // BRIGHT_RED color in the format: "Error (code=<code>)
  // occured.\r\n" where "<code>" is the error code returned by the
  // exec() system call.
}

#endif
