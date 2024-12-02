#ifndef VIDEO_DRIVER_C
#define VIDEO_DRIVER_C

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

/* The following asm directive informs the assembler to generate */
/* 16-bit code even though gcc spits out 32-bit code.            */
asm(".code16gcc\n");

#include "Video.h"

void
displayChar(const char text, const byte color) {
    byte data = (byte) text;
    asm("movb $0x00, %%bh\n\t"
        "movw $0x1,  %%cx\n\t"
        "movb %0,    %%al\n\t"
        "movb %1,    %%bl\n"
    : /* No output registers. */
    :"a" (data), "d" (color)
    :"bl", "bh", "cx"
    );
    
    if (data >= 0x20) {
        /* This is a standard character to be displayed. */
        asm("movb $0x9,  %%ah\n\t" /* Display character with color */
            "int  $0x10\n"         /* Display character.           */
        :/* No output registers */
        :/* No inputs expected  */
        :"ah"
        );
    }

    asm("movb $0xe, %%ah\n\t"      /* Display character & interpret */
        "int $0x10\n"              /* special escape codes          */
    :/* No output registers */
    :/* No inputs expected  */
    :"ah"
    );

    if (text == 0xc) {
      // New page
      clearScreen(color);
      moveCursor(0, 0);
    }
}

void
moveCursor(const byte row, const byte column) {
    
    asm("movb %0,   %%dh\n\t"       /* Set the row number in DH    */
        "movb %1,   %%dl\n\t"       /* Set the col number in DL    */
        "movb $0x2, %%ah\n\t"       /* Move cursor function number */
        "movb $0x0, %%bh\n\t"       /* Set page number to 0        */
        "int  $0x10\n"              /* Call int 10h to move cursor */
    : /* No outputs */
    : "a" (row), "b" (column)
    : "dh", "dl"
    );
}

void
getCursor(byte *row, byte *column) {
    asm volatile ("movb $3,   %%ah\n\t"       /* Set function to read cursor */
        "movb $0,   %%bh\n\t"       /* Set video page to 0         */
        "int  $0x10\n\t"            /* Call int 10h to read cursor */
        "movl %[row], %%eax\n\t"    /* eax = address of row        */
	"movb %%dh, (%%eax)\n\t"    /* Store row value in param1   */
        "movl %[col], %%eax\n\t"    /* eax = address of row        */	  
        "movb %%dl, (%%eax)\n\t"     /* Store col value in param2   */
    : [row] "=m" (row), [col] "=m" (column)
    : 
    : "eax", "bx", "cx", "dx"
    );
}

void
clearScreen(const byte color) {
    /* First move the cursor to top of the screen */
    moveCursor(0, 0);
    
    /* Now write 2000 spaces on the screen thereby clearing it out*/
    asm("movw $0x0920, %%ax\n\t"  /* Select Function 09 and space character */
        "movb $0x0,    %%bh\n\t"  /* Set the page number to 0.              */
        "movw $2000,   %%cx\n\t"  /* Display 2000 spaces in given color.    */
        "int  $0x10\n"            /* Display spaces thereby clear screen!   */
    : /* No outputs */
    : "bl" (color)
    : "ax", "cx"
    );
}

void
displayString(const char *data, const byte color) {
    register short int counter;
    
    for (counter = 0; *(data + counter); counter++) {
        displayChar(*(data + counter), color);
    }
}

void
drawBox(const byte row, const byte column, const int width, const int height,
        const byte color) {
    if ((row > 24) || (column > 79)) {
        /* Invalid starting positions */
        return;
    }
    
    if ((column + width) > 79) {
        /* Cannot draw boxes of this size! */
        return;
    }
    if ((row + height) > 24) {
        /* Cannot draw boxes of this size! */
        return;        
    }
    
    char buffer[80];
    unsigned short int counter;

    /* First display the 2 top and bottom horizontal lines for the box. */
    for(counter = 0; (counter < width); counter++) {
        buffer[counter] = 205; /* See ASCII Table */
    }
    buffer[width] = '\0'; /* Terminate the string properly. */
    
    /* Setup the top-left and top-right corners of the buffer; */
    buffer[0] = 201; /* ASCII for top-left corner */
    buffer[width - 1] = 187; /* ASCII for top-right corner */
    moveCursor(row, column);
    displayString(buffer, color);

    /* Setup the top-left and top-right corners of the buffer; */
    buffer[0] = 200; /* ASCII for top-left corner */
    buffer[width - 1] = 188; /* ASCII for top-right corner */
    moveCursor(row + height, column);
    displayString(buffer, color);
    
    /* Now draw the vertical lines of the box after generating each
       line in the buffer */
    for(counter = 0; (counter < width); counter++) {
        buffer[counter] = ' ';
    }
    buffer[0] = 186;
    buffer[width - 1] = 186;

    for(counter = 1; (counter < height); counter++) {
        moveCursor(row + counter, column);
        displayString(buffer, color);
    }
}

#endif
