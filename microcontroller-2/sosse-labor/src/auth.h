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
	\brief Authentication declarations.

	Here are the declarations and definitions for the authentication functions.
	The authentication types supported are:

	<UL>
	<LI>PIN/PUK</LI>
	<LI>External Authentication</LI>
	<LI>Internal Authentication</LI>
	</UL>

	\todo
	Here are certainly some optimizations possible to reduce code size.

	$Id: auth.h,v 1.11 2002/12/22 15:42:55 m Exp $
*/

#ifndef SOSSE_AUTH_H
#define SOSSE_AUTH_H

#include <types.h>

//! Flag for authstate signalling a successful PIN verification.
#define AUTH_FLAG_PIN	0x01
//! Flag for authstate signalling a successful external authentication or key verification.
#define AUTH_FLAG_KEY	0x02

#define AUTH_AC_ALW	0x0	//!< Access Condition: Always.
#define AUTH_AC_PIN	0x1	//!< Access Condition: PIN.
#define AUTH_AC_ADM	0x2	//!< Access Condition: Key.
#define AUTH_AC_POA	0x3	//!< Access Condition: PIN or Key.
#define AUTH_AC_PAA	0x4	//!< Access Condition: PIN and Key.
#define AUTH_AC_NEV	0xF	//!< Access Condition: Never.

#define PIN_MAX_RETRIES	3	//!< Default max. retries for PIN.
#define PUK_MAX_RETRIES	10	//!< Default max. retries for PUK.

#define PIN_TYPE_PIN	0	//!< PIN type: PIN.
#define PIN_TYPE_PUK	1	//!< PIN type: PUK.
#define PIN_TYPE_EXT	2	//!< PIN type: Key for external authentication.
#define PIN_TYPE_INT	3	//!< PIN type: Key for internal authentication.

#define KEY_MAX_RETRIES 10	//!< Default max. retries for keys.

/*! \brief FID for file with external authentication key.

	It is only searched for the file in the MF and not in any DF.
*/
#define AUTH_FID_EXTAUTH	0xFF00
/*! \brief FID for file with internal authentication key.

	It is only searched for the file in the MF and not in any DF.
*/
#define AUTH_FID_INTAUTH	0xFF01
/*! \brief FID for file with PIN and PUK.

	It is only searched for the file in the MF and not in any DF.
*/
#define	AUTH_FID_PINPUK		0xFF02

/*! \brief Validity of challenge.

	The initial value if challvalidity is 2. This is decremented at the
	start of the command loop, so that it is 1 at the command following
	a Get Challenge.
*/
extern uint8_t challvalidity;

/*! \brief Initialize authentication variables.

	This function resets authstate and challvalidity. It must be called
	after reset.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.
*/
bool auth_init( void );

/*! \brief Check fullfillment of access condition.

	This function checks, if the current setting of authstate fullfills
	the condition specified with <EM>ac</EM>.

	\param ac A nibble specifying an access condition.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.
*/
bool auth_checkAc( uint8_t ac );

/*! \brief Verifies length of PIN and returns addresses of PIN in EEPROM.

	This functions does depending on <EM>pintype</EM>:

	<UL>
	<LI>Check the length of the PIN or key.</LI>
	<LI>Return the address of the PIN or key in EEPROM.</LI>
	<LI>Return the length of the PIN or key in EEPROM.</LI>
	<LI>Return the address of the retry counter in EEPROM.</LI>
	<LI>Return the default max. value for the retry counter.</LI>
	</UL>

	\param pintype PIN type. One of the PIN_TYPE defines.
	\param pinlen Length of supplied PIN or key.
	\param addr Returns the address of the PIN or key in EEPROM.
	\param len Returns the length ofthe PIN or key in EEPROM.
	\param rcaddr Returns the address of the retry counter for the PIN or key.
	\param maxret Returns the default max. value for the retry counter.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.
*/
bool auth_getCheckPinLen( uint8_t pintype, uint8_t pinlen, uint16_t *addr,
	uint8_t *len, uint16_t *rcaddr, uint8_t *maxret );

/*! \brief Write PIN, PUK, External Authentication key or Internal
	Authentication key.

	The process is secured by transactions.

	\param pintype Type of the PIN/Key. May be one of the PIN_TYPE_* defines.
	\param pin Pointer to byte array containing PIN/Key.
	\param pinlen Length of PIN/Key.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.
*/
bool auth_setPin( uint8_t pintype, uint8_t *pin, uint8_t pinlen );
/*! \brief Verifies PIN, PUK or External Authentication key and sets flags
	in authstate.

	The update of the retry counter is secured by transactions.

	<EM>Warning: Currently the retry counter is not handled in
	a secure way. For more information see e.g. Rankl/Effing.</EM>

	\param pintype Type of the PIN/Key. May be one of the PIN_TYPE_* defines.
	\param pin Pointer to byte array containing PIN/Key.
	\param pinlen Length of PIN/Key.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.
*/
bool auth_verifyPin( uint8_t pintype, uint8_t *pin, uint8_t pinlen );

/*! \brief Returns 8 byte challenge for External Authentication.

	The challenge is also written to the challenge array and challvalidity
	is set to 2.

	\param dst Destination byte array.
	\param rndlen Length of requested data. This must be 8.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.
*/
bool auth_getChallenge( uint8_t *dst, uint8_t rndlen );
/*! \brief Encrypt or decrypt challenge or verify cryptogram.

	To include the decryption process CONF_WITH_DECRYPT must be set to 1.

	\param cry Pointer to byte array, which supplies challenge and which
		is used to return the encrypted or decrypted challenge or a
		cryptogram.
	\param crylen Length of challenge or cryptogram.
	\param create If set to TRUE, a challenge is processed, else a
		cryptogram is verified.
	\param enc If CONF_WITH_DECRYPT is not set to 1, this has no effect.
		Otherwise, if TRUE encrypt the challenge, else decrypt it.
		If <I>create</I> is FALSE, this parameter is ignored.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.
*/
bool auth_createVerifyCryptogram( uint8_t *cry, uint8_t crylen, bool create,
	bool enc );

#endif /* SOSSE_AUTH_H */

