#ifndef SERVICES_C
#define SERVICES_C

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

asm(".code16gcc\n");

#include "Services.h"
#include "MemoryUtils.h"
#include "IVTUtilities.h"
#include "Process.h"
#include "Video.h"
#include "String.h"
#include "Keyboard.h"
#include "FAT12Driver.h"

void initializeServices() {
  setIVT(0x80, (unsigned short) service);
}

void exitProcess(const byte exitCode) {
  stopProcess(-1, exitCode);
}

int write(const byte deviceID, const byte dataType, const byte color,
	   const unsigned int dataOffset) {
  if (deviceID != 1) {
    // Device ID must be 1 for now.
    return 0;
  }
  char buffer[16];
  buffer[0] = '\0';
  const char *str = buffer;
  
  switch (dataType) {
  case 1: str = (const char *) dataOffset;
    break;

  case 2: shortToString(*((const short *) dataOffset), buffer);
    break;
  }

  displayString(str, color);
  return strlen(str);
}

int read(const byte deviceID, const byte dataType, const byte color,
	  const unsigned short dataOffset) {
  char buffer[129];

  int  index = 0;
  if ((deviceID != 0) || (dataType != 1)) {
    // Device ID must be 0 for now. and can't read numbers yet.
    return 0;
  }
  unsigned short key = -1;
  do {
    key = getKeyStroke();
    if ((byte) key == 0) {
      // Special characters such as cursor key's are not yet handled.
      continue;
    }
    key = (byte) key;
    if (key == 8) {
      // Backspace.
      if (index > 0) {
	index--;
	if (color) {
	  displayChar(8, color);
	  displayChar(' ', color);
	  displayChar(8, color);	
	}
      }
      continue;
    }

    if (index > 127) {
      // Line too long!
      continue;
    }

    if ((key < 32) || (key > 127)) {
      // Currently special keystrokes like cursor keys are not
      // handled.
      continue;
    }
    
    if (color > 0) {
      // Display the key typed.  If color is 0 no echo is done.
      displayChar((byte) key, color);
    }
    buffer[index] = (byte) key;
    index++;
  } while (key != 13);

  buffer[index] = '\0';
  // Display line feed for enter key.
  if (color > 0) {
    displayChar('\r', color);
    displayChar('\n', color);
  }
  
  // Copy the data back to the user provided buffer.
  unsigned short stackSegment;
  GET_SS(stackSegment);
  farmemcpy(stackSegment, dataOffset, stackSegment, OFFSET(buffer),
	    strlen(buffer) + 1);
  return strlen(buffer);
}

int getFATFileEntry(const int entry, const unsigned short dataOffset) {
  unsigned short stackSegment;
  GET_SS(stackSegment);
  return getFileEntry(entry, stackSegment, dataOffset);
}

int exec(const byte block, const unsigned int dataOffset) {
  unsigned int returnValue;

  returnValue = createProcess((const char *) dataOffset, block);
  return returnValue;
}

void service() {
  unsigned short ax, bx, cx, dx;
  /* The following assembly code copies values in ax, bx, cx, and dx
     registers to corresponding local variables.  This preserves the
     values in the registers while freeing them up for use by the
     kernel rouintes.
  */
  asm volatile("movw %%ax, %[xax]\n\t" /* ax = register ax */
	       "movw %%bx, %[xbx]\n\t" /* bx = register bx */
	       "movw %%cx, %[xcx]\n\t" /* cx = register cx */
	       "movw %%dx, %[xdx]\n\t" /* dx = register dx */
	       : [xax] "=m" (ax), [xbx] "=m" (bx), [xcx] "=m" (cx),
		 [xdx] "=m" (dx)
	       : /* No inputs */
	       : "memory");

  byte serviceCode = (byte) ax;
  int  returnValue = 0;

  if (serviceCode == 0) {
    exitProcess((byte) (ax >> 8));
  } else if (serviceCode == 1) {
    returnValue = write(ax >> 8, (byte) bx, (byte) (bx >> 8), cx);
  } else if (serviceCode == 2) {
    returnValue = read(ax >> 8, (byte) bx, (byte) (bx >> 8), cx);
  } else if (serviceCode == 3) {
    returnValue = getFATFileEntry(ax >> 8, bx);
  } else if (serviceCode == 4) {
    returnValue = exec(ax >> 8, bx);
  } else {
    returnValue = INVALID_SERVICE_ID;
  }

  asm volatile("movw %[ret], %%ax\n\t"
	       "leave\n\t"
	       "iretw\n\t"
	       : [ret] "=m" (returnValue)
	       :
	       /* No clobber */
	       );
}

#endif
