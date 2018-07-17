/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file CISError.h
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Contains all error codes as constants
 */

#ifndef SRC_TOOLS_CISERROR_H_
#define SRC_TOOLS_CISERROR_H_

#define ERROR_SUCCESS           (0x00) //The operation completed successfully
#define ERROR_INVALID_ARGS      (0x01) //The arguments are incorrect
#define ERROR_OUT_OF_MEMORY     (0x02) //Not enough memory to complete the operation
#define ERROR_INVALID_DATA      (0x03) //Data error
#define ERROR_FILE_NOT_FOUND    (0x04) //The system cannot find the specified file
#define ERROR_WRITE_PROTECT     (0x05) //The media is write protected
#define ERROR_EOF               (0x06) //Reached the end of the file
#define ERROR_NOT_SUPPORTED     (0x07) //The request is not supported
#define ERROR_BUFFER_OVERFLOW   (0x08) //The buffer is too small
#define ERROR_INVALID_PASSWORD  (0x09) //Authentication failed
#define ERROR_WRITE             (0x0A) //Failed during write operation
#define ERROR_READ              (0x0B) //Failed during read operation
#define ERROR_NACK              (0x0C) //No acknowledgement
#define ERROR_OPEN_FAILED       (0x0D) //The system cannot open the device or file
#define ERROR_DISK_FULL         (0x0E) //There is not enough space in the disk
#define ERROR_BUSY              (0x0F) //The specified resource is in use
#define ERROR_ALREADY_EXISTS    (0x10) //Cannot create a file that already exists
#define ERROR_QUEUE_OVERFLOW    (0x11) //The queue is at maximum capactity
#define ERROR_WAIT_TIMEOUT      (0x12) //The operation timed out
#define ERROR_WATCHDOG          (0x13) //The watchdog was not pet properly
#define ERROR_INVALID_ADDRESS   (0x14) //Attempt to access invalid address
#define ERROR_POWER_LIMITED     (0x15) //Insuffient power to complete operation
#define ERROR_POWER_EMERGENCY   (0x16) //Emergency power level reached
#define ERROR_COMMS_INTERRUPTED (0x17) //Communication link lost
#define ERROR_UNKNOWN_COMMAND   (0x18) //The requested command is unknown

#endif /* SRC_TOOLS_CISERROR_H_ */
