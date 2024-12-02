#ifndef SYS_CALL_H
#define SYS_CALL_H

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

#include "FAT12.h"

/**
   This function is expected to call the OS service for exiting a
   process.  This function is expected to perform the following tasks:

   <OL>

   <LI> Set the \c AL register to 0. </LI>

   <LI> Set the \c AH register to the exitCode passed in as the
   parameter. </LI>

   <LI> Call interrupt 0x80 </LI>
   
   </OL>

   \param[in] exitCode The exit code to be passed onto the OS.
*/
void exitProcess(const byte exitCode);

/**
   This function is expected to invoke the OS service to write data to
   a given device.  This function must be implemented to perform the
   following tasks:

   <OL>

   <LI>Set the \c AL register to 1. </LI>

   <LI>Set the \c AH register to contain the value passed in for
   deviceID. </LI>

   <LI>Set the \c BL register to contain the dataType of the data to
   be displayed. </LI>

   <LI>Set the \c BH register to contain the color with which the
   characters must be displayed. </LI>

   <LI>Set the \c CX register to contain the address of the data to be
   displayed.</LI>

   <LI>Invoke interrupt 0x80</LI>

   <LI>Store the value returned in \c AX register into a local
   variable (say \c retVal). </LI>

   <LI> Return \c retVal. </LI>
   
   </OL>

   \param[in] deviceID The ID of the device to which data must be
   written.  Currently this value must be 1 indicating standard output.

   \param[in] dataType The data type of the value to be displayed.
   The data type values are interepreted as: 1=>string, 2=>short.

   \param[in] The color in which the character is to be displayed.
   Use the \c MAKE_COLOR() macro to define a color consisting of a
   foreground and background color.

   \param[in] data Pointer to the data to be displayed.
   
   \see MAKE_COLOR   
*/
int write(const byte deviceID, const byte dataType, const byte color,
	  const void* data);

/**
   This function is expected to invoke the OS service to write data to
   a given device.  This function must be implemented to perform the
   following tasks:

   <OL>

   <LI>Set the \c AL register to 2. </LI>

   <LI>Set the \c AH register to contain the value passed in for
   deviceID. </LI>

   <LI>Set the \c BL register to contain the dataType of the data to
   be read. </LI>

   <LI>Set the \c BH register to contain the color with which the
   characters must be echoed during input. </LI>

   <LI>Set the \c CX register to contain the address of the buffer
   into which the data read must be stored.</LI>

   <LI>Invoke interrupt 0x80</LI>

   <LI>Store the value returned in \c AX register into a local
   variable (say \c retVal). </LI>

   <LI> Return \c retVal. </LI>
   
   </OL>

   \param[in] deviceID The ID of the device to which data must be
   written.  Currently this value must be 0 indicating standard input.

   \param[in] dataType The data type of the value to be displayed.
   The data type values are interepreted as: 1=>string, 2=>short.

   \param[in] The color in which the character is to be displayed.
   Use the \c MAKE_COLOR() macro to define a color consisting of a
   foreground and background color.  Passing in 0 supresses echoing of
   characters and can be used to read private information like
   passwords.

   \param[in] data Pointer to the buffer into which data is to be
   stored.
   
   \see MAKE_COLOR   
*/
int read(const byte deviceID, const byte dataType, const byte color,
	 const void* data);

/**
   This function is expected to invoke the OS service to read a FAT12
   file entry in the following manner:

   <OL>

   <LI>Set \c AL register to 3.</LI>

   <LI>Set \c AH register to the value of entryIndex. </LI>

   <LI>Set \c BX register to value of parameter fe. </LI>

   <LI>Invoke interrupt 0x80</LI>

   <LI>Store the value returned in \c AX register into a local
   variable (say \c retVal). </LI>

   <LI> Return \c retVal. </LI>
   
   </OL>

   \param[in] entryIndex The entry in the root directory table of a
   FAT12 file system that must be read.

   \param[out] fe The FileEntry data structure into which the entry
   must be copied.
*/
int getFileEntry(const byte entryIndex, struct FileEntry *fe);

/**
   This function is expected to invoke the OS service to execute a
   process in the following manner:

   <OL>

   <LI>Set \c AL register to 4.</LI>

   <LI>Set \c AH register to the value of blockFlag. </LI>

   <LI>Set \c BX register to the value of fileName pointer. </LI>

   <LI>Invoke interrupt 0x80</LI>

   <LI>Store the value returned in \c AX register into a local
   variable (say \c retVal). </LI>

   <LI> Return \c retVal. </LI>
   
   </OL>

   \param[in] fileName The 8,3 ("TEST BIN") without "." name of the
   program that must be executed as a separate process.

   \param[in] blockFlag A flag that indicates if the calling process
   should be blocked or not.  Currently, this value must always be set
   to 1.
*/
int exec(const char *fileName, const byte blockFlag);

#endif
