/*
	Simple Operating System for Smartcard Education
	Copyright (C) 2002  Matthias Bruestle <m@mbsks.franken.de>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*! @file
	\brief Errno declarations.

	See also for the man page of sw.

	$Id: sw.h,v 1.1 2002/12/22 15:42:55 m Exp $
*/

#ifndef SOSSE_ERRNO_H
#define SOSSE_ERRNO_H

#include <types.h>

#if CONF_WITH_RETCODEMGR==1
/*! \brief Status byte values for return code manager.

	\note
	Implement this as defines, because doxygen apparently can't handle this?
*/
typedef enum {
	SW_COUNTER=0x00,
	SW_ACCESS_DENIED=0x10,
	SW_AUTH_BLOCKED,
	SW_AVAILABLE,
	SW_CHANGE_MEMORY_ERROR,	// Unused
	SW_EOD,	// Unused
	SW_FILE_EXISTS,
	SW_FILE_FILLED,	// Unused
	SW_FILE_NOT_FOUND,
	SW_FILE_TO_SHORT,
	SW_INCOMPATIBLE_FILE,
	SW_LC_INCONSISTEND_WITH_P1P2,	// Unused
	SW_NOT_ALLOWED,
	SW_NOT_SUPPORTED,	// Unused
	SW_OK,
	SW_OTHER,
	SW_REF_DATA_INVALID,
	SW_REF_DATA_NOT_FOUND,
	SW_VERIFCATION_FAILED,	// Unused
	SW_WRONG_CLA,
	SW_WRONG_CONDITION,
	SW_WRONG_DATA,
	SW_WRONG_INS,
	SW_WRONG_LE,
	SW_WRONG_LEN,
	SW_WRONG_P1P2,
	SW_WRONG_REFERENCE,
} SW;
#else /* CONF_WITH_RETCODEMGR!=1 */
/*! \brief Status word values.

	\note
	Implement this as defines, because doxygen apparently can't handle this?
*/
typedef enum {
	SW_ACCESS_DENIED=0x6982,
	SW_AUTH_BLOCKED=0x6983,
	SW_AVAILABLE=0x6100,
	SW_CHANGE_MEMORY_ERROR=0x6581,	// Unused
	SW_COUNTER=0x63C0,
	SW_EOD=0x6282,	// Unused
	SW_FILE_EXISTS=0x6A89,
	SW_FILE_FILLED=0x6381,	// Unused
	SW_FILE_NOT_FOUND=0x6A82,
	SW_FILE_TO_SHORT=0x6A84,
	SW_INCOMPATIBLE_FILE=0x6981,
	SW_LC_INCONSISTEND_WITH_P1P2=0x6A87,	// Unused
	SW_NOT_ALLOWED=0x6986,
	SW_NOT_SUPPORTED=0x6A81,	// Unused
	SW_OK=0x9000,
	SW_OTHER=0x6F00,
	SW_REF_DATA_INVALID=0x6984,
	SW_REF_DATA_NOT_FOUND=0x6A88,
	SW_VERIFCATION_FAILED=0x6300,	// Unused
	SW_WRONG_CLA=0x6E00,
	SW_WRONG_CONDITION=0x6985,
	SW_WRONG_DATA=0x6A80,
	SW_WRONG_INS=0x6D00,
	SW_WRONG_LE=0x6C00,
	SW_WRONG_LEN=0x6700,
	SW_WRONG_P1P2=0x6A86,
	SW_WRONG_REFERENCE=0x6B00,
} SW;
#endif /* CONF_WITH_RETCODEMGR!=1 */

/*! \brief Status word returned by functions.

	The state of sw is undefined, on return value TRUE.

	The content of this variable is returned at the end of the command
	interpreter loop. Functions falled from the command interpreter loop
	should set it to SW_OK if they return without error.
*/
#if CONF_WITH_RETCODEMGR==1
extern uint8_t sw;
#else /* CONF_WITH_RETCODEMGR!=1 */
extern uint16_t sw;
#endif /* CONF_WITH_RETCODEMGR!=1 */

/*! \brief Function for setting sw.

	This function reduces the code size on the AVR architecture.
*/
#if CONF_WITH_RETCODEMGR==1
void sw_set( uint8_t value );
#else /* CONF_WITH_RETCODEMGR!=1 */
void sw_set( uint16_t value );
#endif /* CONF_WITH_RETCODEMGR!=1 */

#if CONF_WITH_RETCODEMGR==1
/*! \brief Return Code Manager: Function for expanding the 1 byte sw
	to the 2 byte status word.

	To activate this functionality set CONF_WITH_RETCODEMGR in config.h

	This function reduces the code size on the AVR architecture.
*/
uint16_t sw_get( void );
#endif /* CONF_WITH_RETCODEMGR==1 */

#endif /* SOSSE_ERRNO_H */

