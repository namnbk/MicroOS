#ifndef HELPERS_H
#define HELPERS_H

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

/**
    This is a helper function that uses the getFileEntry() sys call to
    list the files on the floppy.  This function is expected to use
    the getFileEntry() sys call to read one file at a time as long as
    getFileEntry() returns SUCCESS.  For each file entry successfully
    read, print its filename and size using the write() sys call with
    suitable parameters.
*/
void ls();

/**
    This is a helper function that uses the exec() sys call to run the
    specified program in buffer.  The file name in the buffer is
    considered invalid because of any one of the following reasons:

    <UL>
    
    <LI> The file name is longer than 11 characters or has 0
    charachters.</LI>
    
    <LI> The file name has a "." after the 8th character or as the 1st
    character</LI>
	
    <LI>3. The file name's extension is not BIN</LI>

    </UL>

    Otherwise it converts the filename to 8,3 (without period)
    representation and uses the exec() system call to try and run the
    program.  It prints an error message if errors are returned by the
    exec() system call.  Refer to ErrorCodes.h for a brief description
    of the error codes returned by exec().
    
    \param[inout] buffer The buffer that contains the name of the
    program to be executed.
*/
void run(char *buffer);

#endif
