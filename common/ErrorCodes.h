#ifndef ERROR_CODES_H
#define ERROR_CODES_H

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

/** \file ErrorCodes.h

    \brief List of error codes used in muos.

    This file defines the list of error codes used and returned by
    muos.
*/

#define SUCCESS                      0
#define DRIVER_NOT_INITIALIZED       1
#define INVALID_FILE_NAME            2
#define ACCESS_DENIED                3
#define IO_ERROR                     4
#define INVALID_ROOT_DIRECTORY_INDEX 5
#define INVALID_CLUSTER              6
#define FILE_TOO_LARGE               7
#define FILE_NOT_FOUND               8
#define TOO_MANY_PROCESSES           9
#define INVALID_SERVICE_ID           10

#define NOT_IMPLEMENTED              99
#define NOT_SUPPORTED                100


#endif
