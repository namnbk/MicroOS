#ifndef SERVICES_H
#define SERVICES_H

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */


/* Services provided by µos.

   This file provides the definitions for the various services offered
   by µos.  These services are invoked by application programs via
   interrupt 0x80.  The following table provides the list of services
   supported by the operating system (The \c AL register is assumed to
   contain the service code and the values are interpreted as
   described below):

   <UL>

   <LI> <b>Exit (AL = 0)</b>: If \c AL register is 0, then the
   invoking process is terminated.  The \c AH register is assumed to have
   the exit code. </LI>

   <LI> <b> Write (AL=1)</b>: If the \c AL register is 1, then the OS
   assumes that the calling program requires data to be written to the
   device specified in \c AH register.  Currently, \c AH must be set
   to 1 to indicate default output device.  The BL register must
   indicate the type of data to be displayed as per the table below:

   <TABLE>
   <TR><TD>Value in \c BL register</TD><TD>Interpretation</TD></TR>

   <TR><TD>1</TD><TD>\c CX register contains the starting address of
   the string to be displayed. </TR>

   <TD><TD>2</TD><TD>\c CX register contains the address of the short
   integer to be displayed.  </TD>

   <TD><TD>3</TD><TD>\c CX register contains the address of the long
   integer to be displayed. </TD>

   </TABLE>
   
   The \c BH register is assumed to contain the color that must be
   used to display the data. </LI>

   <LI> <b> Read (AL=2)</b>: If the \c AL register is 2, then the OS
   assumes that the calling program requires data to be read from the
   device specified in \c AH register.  Currently, \c AH must be set
   to 0 to indicate default input device (keyboard).  The BL register
   must indicate the type of data to be read as per the table below:

   <TABLE>
   <TR><TD>Value in \c BL register</TD><TD>Interpretation</TD></TR>

   <TR><TD>1</TD><TD>\c CX register contains the starting address of a
   string to be read.  Strings are arbitrarily cut off at 128
   characters. </TR>

   <TD><TD>2</TD><TD>\c CX register contains the address of the short
   integer into which a short number is to be stored.  </TD>

   <TD><TD>3</TD><TD>\c CX register contains the address of the long
   integer into which a integer is to be stored. </TD>

   </TABLE>

   The \c BH register is assumed to contain the color that must be
   used to display the data.  If BH is set to 0, then the characters
   typed at the keyboard are not echoed.  </LI>

   <LI> <b>Get file entry (AH=3)</b>: If the \c AL register is 2, then
   the OS assumes that the calling program requires a file entry index
   specified in \c AL register is to be read into the memory location
   indicated by the \c BX register.  The \c BX register must
   essentially be a pointer to a 32-byte location that corresponds to
   the FileEntry data structure defined in FAT12.h. </LI>

   <LI> <b>exec (AH=4)</b>: If the \c AL register is 3, then the OS
   assumes that the calling process want's to execute a new process.
   The file name of the program to be run is assumed to be in \c BX
   register.  The \c AH register is assumed to contain a flag, which
   if true, blocks the process requesting this service until the child
   process exits.  If the process was created successfully and run
   this service returns 0 in \c AX register.  Otherwise it returns a
   suitable error code (defined in ErrorCodes.h) in \c AX
   register.</LI>
   
   </UL>
   
   For example, if an application wishes to terminate execution then
   it should call the exit service in the following manner:

   \code

   movb $0x0, %al
   movb $0x0, %ah
   int $0x80

   \endcode

   @note This code is compiled into a 16-bit executable.
   Consequently, only 16-bit datatypes must be used throughout this
   code.
*/

/**

   This functions must be invoked before the kernel will respond to
   service requestes on interrupt 0x80.  This function essentially
   changes the IVT entry corresponding to interrupt 0x80 to point to
   the kernel's main \c service() function.  This function is
   typically invoked from the \c _start() function of µos.
*/
void initializeServices();

/**
   This is the main service() function of the OS.  This function is
   invoked via interrupt 0x80 and <FONT color="red"><b>should not be
   called directly.</b></FONT>.  This function uses the value in AL
   register to delegate the service task to appropriate helper
   functions within the kernel.  These helper functions are not
   exposed to user programs.
*/
void service();

#endif
