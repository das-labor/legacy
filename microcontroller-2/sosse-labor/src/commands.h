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
	\brief Command declarations.

	The documentation of the commands is on the main page of the doxygen
	documentation.

	$Id: commands.h,v 1.15 2002/12/22 15:42:55 m Exp $
*/

#ifndef SOSSE_COMMANDS_H
#define SOSSE_COMMANDS_H

#define CLA_PROP			0x80	//!< CLA byte: Proprietary

#define INS_WRITE			0x02	//!< INS byte: Write EEPROM
#define INS_READ			0x04	//!< INS byte: Read EEPROM
#define INS_LED				0x06	//!< INS byte: LED Effects

#define INS_CHANGE_PIN		0x24	//!< INS byte: Change PIN
#define INS_CREATE			0xE0	//!< INS byte: Create File
#define INS_DELETE			0xE4	//!< INS byte: Delete File
#define INS_EXTERNAL_AUTH	0x82	//!< INS byte: External Authentication
#define INS_GET_CHALLENGE	0x84	//!< INS byte: Get Challenge
#define INS_GET_RESPONSE	0xC0	//!< INS byte: Get Response
#define INS_INTERNAL_AUTH	0x88	//!< INS byte: Internal Authentication
#define INS_READ_BINARY		0xB0	//!< INS byte: Read Binary
#define INS_SELECT			0xA4	//!< INS byte: Select File
#define INS_UNBLOCK_PIN		0x2C	//!< INS byte: Unblock PIN
#define INS_UPDATE_BINARY	0xD6	//!< INS byte: Update Binary
#define INS_VERIFY_KEY		0x2A	//!< INS byte: Verify Key
#define INS_VERIFY_PIN		0x20	//!< INS byte: Verify PIN

/*! \brief Valid data in response array. Invalid if zero. */
extern uint8_t resplen;

/*! \brief Write EEPROM. (Test command)
*/
void cmd_write( void );
/*! \brief Read EEPROM. (Test command)
*/
void cmd_read( void );

/*! \brief LED Effects.
*/
void cmd_led( void );

/*! \brief Change or Unblock PIN.
*/
void cmd_changeUnblockPIN( void );
/*! \brief Create File.
*/
void cmd_create( void );
/*! \brief Delete File.
*/
void cmd_delete( void );
/*! \brief External Authentication.
*/
void cmd_extAuth( void );
/*! \brief Get Challenge
*/
void cmd_getChallenge( void );
/*! \brief Get Response.
*/
void cmd_getResponse( void );
/*! \brief Internal Authentication.
*/
void cmd_intAuth( void );
/*! \brief Read Binary.
*/
void cmd_readBinary( void );
/*! \brief Select File.
*/
void cmd_select( void );
/*! \brief Update Binary.
*/
void cmd_updateBinary( void );
/*! \brief Verify Key/Verify PIN.

	This function is a combined implementation of Verify Key and Verify
	PIN. This is done to save code size.
*/
void cmd_verifyKeyPIN( void );

#endif /* SOSSE_COMMANDS_H */
