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
	\brief Configuration settings.

	$Id: config.h,v 1.27 2003/03/30 12:42:21 m Exp $
*/

#ifndef SOSSE_CONFIG_H
#define SOSSE_CONFIG_H

/* Configuration defines */

/*! \brief En-/disables file system.

	\todo
	Make this define something happen.
*/
#define CONF_WITH_FILESYSTEM	1

/*! \brief En-/disables transactions.
*/
#define CONF_WITH_TRANSACTIONS	1

/*! \brief En-/disables test commands.

	If set to 1, commands READ EEPROM and WRITE EEPROM are included.
*/
#define CONF_WITH_TESTCMDS		1

/*! \brief En-/disables DELETE.
*/
#define CONF_WITH_DELETECMD		1

/*! \brief En-/disables CREATE.
*/
#define CONF_WITH_CREATECMD		1

/*! \brief En-/disables PIN authentication functions.

	If set to 1, functions to set and test PIN and PUK are included. This is a
	requirement for CONF_WITH_PINCMDS.
*/
#define CONF_WITH_PINAUTH		1

/*! \brief En-/disables PIN commands.

	If set to 1, commands CHANGE PIN and VERIFY PIN are included.
*/
#define CONF_WITH_PINCMDS		1

/*! \brief En-/disables key authentication functions.

	If set to 1, functions impelementing calculations with challenges are
	included. This is a requirement for CONF_WITH_KEY_CMDS.
*/
#define CONF_WITH_KEYAUTH		1

/*! \brief En-/disables key related commands.

	If set to 1, commands EXTERNAL KEYENTIFICATION, GET CHALLENGE,
	INTERNAL KEYENTIFICATION and VERIFY KEY are included.
*/
#define CONF_WITH_KEYCMDS		1

/*! \brief En-/disables support for the external I2C EEPROM.
*/
#define CONF_WITH_I2CEEPROM		1

/*! \brief En-/disables support for decryption.

	If set to 1, support for decryption side of the cryptographic algorithms
	are included.
*/
#define CONF_WITH_DECRYPT		1

/*! \brief En-/disables support for logging.

	If set to 1, support for logging is included.
*/
#define CONF_WITH_LOGGING		1

/*! \brief En-/disables return code manager.

	If set to 1, the return code manager is included. This increases
	code size on the AVR.
*/
#define CONF_WITH_RETCODEMGR	0

/*! \brief En-/disables funny commands.

	If set to 1, funny commands are included.
*/
#define CONF_WITH_FUNNY			0

/*! \brief En-/disables true random generator. <B>Highly experimental!</B>

	If set to 1, a true random number generator is included. This is
	currently only for test purposes and highly experimental!
*/
#define CONF_WITH_TRNG			0

/*! \brief Set crypto algorithm

	- 0: TEA
	- 1: Skipjack
	- 2: NEWDES-SK
*/
#define CONF_WITH_CRYPT_ALGO	0	/* TEA */
//#define CONF_WITH_CRYPT_ALGO	1	/* Skipjack */
//#define CONF_WITH_CRYPT_ALGO	2	/* NEWDES-SK */

/* Verify dependencies */

/* CONF_WITH_PINAUTH is required for CONF_WITH_PINCMDS */
#if (CONF_WITH_PINCMDS==1) && (CONF_WITH_PINAUTH!=1)
#undef CONF_WITH_PINAUTH
#define CONF_WITH_PINAUTH		1
#endif

/* CONF_WITH_PINAUTH is required for CONF_WITH_PINCMDS */
#if (CONF_WITH_PINCMDS==1) && (CONF_WITH_PINAUTH!=1)
#undef CONF_WITH_PINAUTH
#define CONF_WITH_PINAUTH		1
#endif

/* If we do not have a file system, we dom't need CREATE and DELETE */
#if (CONF_WITH_FILESYSTEM!=1) && (CONF_WITH_DELETECMD==1)
#undef CONF_WITH_DELETECMD
#define CONF_WITH_DELETECMD		0
#endif
#if (CONF_WITH_FILESYSTEM!=1) && (CONF_WITH_CREATECMD==1)
#undef CONF_WITH_CREATECMD
#define CONF_WITH_CREATECMD		0
#endif

/* CONF_WITH_KEYAUTH is required for CONF_WITH_KEYCMDS */
#if (CONF_WITH_KEYCMDS==1) && (CONF_WITH_KEYAUTH!=1)
#undef CONF_WITH_KEYAUTH
#define CONF_WITH_KEYAUTH		1
#endif

/* EEPROM addresses */

/*! \brief EEPROM address of the state of the card.

	This is currently unused.

	<EM>Warning: When using interrupts on the AVR, this byte may accidently
	be changed. See errata sheets of AVR.</EM>
*/
#define CARD_STATE_ADDR		0x0000

//! EEPROM address of the ATR length.
#define ATR_LEN_ADDR		(CARD_STATE_ADDR+1)
//! Max. length of the ATR
#define ATR_MAXLEN			28
//! EEPROM address of the ATR.
#define ATR_ADDR			(ATR_LEN_ADDR+1)

//! EEPROM address of the serial number of the card.
#define SERNUM_ADDR			(ATR_ADDR+ATR_MAXLEN)
//! Length of the serial number.
#define SERNUM_LEN			8

//! EEPROM address of the state of the PRNG.
#define RAND_STATE_ADDR		(SERNUM_ADDR+SERNUM_LEN)
//! Length of the state of the PRNG. (Counter[8], State[8], Key[16])
#define RAND_STATE_LEN		8+8+16

//! EEPROM address of the transaction state.
#define TRANSAC_STATE_ADDR	(RAND_STATE_ADDR+RAND_STATE_LEN)
//! EEPROM address of the transaction data.
#define TRANSAC_DATA_ADDR	(TRANSAC_STATE_ADDR+1)
//! Length of the transaction data.
#define TRANSAC_DATA_LEN	19	/* Currently 2B addr, 1B len, 16B data */

//! Length of the External Authentication key.
#define ADM_KEY_LEN			16
//! Length of the Internal Authentication key.
#define INT_KEY_LEN			16
//! Length of the PIN.
#define PIN_LEN				8
//! Length of the PUK.
#define PUK_LEN				8

#if CONF_WITH_FILESYSTEM==1
//! Dummy address to handle the ifdefs here.
#define DATA_END_1			(TRANSAC_DATA_ADDR+TRANSAC_DATA_LEN)
#else /* CONF_WITH_FILESYSTEM!=1 */
//! EEPROM address of the External Authentication key.
#define ADM_KEY_ADDR		(TRANSAC_DATA_ADDR+TRANSAC_DATA_LEN)
//! EEPROM address of the retry counter for the External Authentication key.
#define ADM_RETCNTR_ADDR	(ADM_KEY_ADDR+ADM_KEY_LEN)

//! EEPROM address of the Internal Authentication key.
#define INT_KEY_ADDR		(ADM_RETCNTR_ADDR+1)
//! EEPROM address of the retry counter for the Internal Authentication key.
#define INT_RETCNTR_ADDR	(INT_KEY_ADDR+INT_KEY_LEN)

//! EEPROM address of the PIN.
#define PIN_ADDR			(INT_RETCNTR_ADDR+1)
//! EEPROM address of the retry counter for the PIN.
#define PIN_RETCNTR_ADDR	(PIN_ADDR+PIN_LEN)
//! EEPROM address of the PUK.
#define PUK_ADDR			(PIN_RETCNTR_ADDR+1)
//! EEPROM address of the retry counter for the PUK.
#define PUK_RETCNTR_ADDR	(PUK_ADDR+PUK_LEN)

//! Dummy address to handle the ifdefs here.
#define DATA_END_1			(PUK_RETCNTR_ADDR+1)
#endif /* CONF_WITH_FILESYSTEM!=1 */

//! EEPROM address of pointer to the file system start.
#define FS_START_PTR_ADDR	DATA_END_1

#if defined(__AVR_AT90S8515__)
//! This is a little endian architecture.
#define ENDIAN_LITTLE
//! Size of the internal EEPROM.
#define	EEPROM_SIZE			0x200
//! Size of the RAM.
#define RAM_SIZE			0x200
//! Chip ID.
#define CHIP				0x01
//! External EEPROM ID.
#define ESIZ				0x03
//! AVR architecture. Needed for assembler.
#define ARCH				avr2
#elif defined(__AVR_AT90S8535__)
//! This is a little endian architecture.
#define ENDIAN_LITTLE
//! Size of the internal EEPROM
#define	EEPROM_SIZE			0x200
//! Size of the RAM.
#define RAM_SIZE			0x200
//! Chip ID.
#define CHIP				0x01
//! External EEPROM ID.
#define ESIZ				0x03
//! AVR architecture. Needed for assembler.
#define ARCH				avr2
#elif defined(__AVR_AT90S2323__)
//! This is a little endian architecture.
#define ENDIAN_LITTLE
//! Size of the internal EEPROM
#define	EEPROM_SIZE			0x80
//! Size of the RAM.
#define RAM_SIZE			0x80
//! Chip ID.
#define CHIP				0x00
//! External EEPROM ID.
#define ESIZ				0x03
//! AVR architecture. Needed for assembler.
#define ARCH				avr2
#elif defined(__AVR_ATmega161__)
//! This is a little endian architecture.
#define ENDIAN_LITTLE
//! Size of the internal EEPROM
#define	EEPROM_SIZE			0x200
//! Size of the RAM.
#define RAM_SIZE			0x400
//! Chip ID.
#define CHIP				0x02
//! External EEPROM ID.
#define ESIZ				0x03
//! AVR architecture. Needed for assembler.
#define ARCH				avr5
#elif defined(__i386__)
//! This is a little endian architecture.
#define ENDIAN_LITTLE
//! Size of the internal EEPROM
#define	EEPROM_SIZE			0x200
//! Size of the RAM.
#define RAM_SIZE			0x200
//! Chip ID.
#define CHIP				0xFF
//! External EEPROM ID.
#define ESIZ				0xFF
#else
#error Unknown destination platform.
#endif

#endif /* SOSSE_CONFIG_H */

