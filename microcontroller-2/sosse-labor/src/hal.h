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
	\brief HAL (Hardware Abstraction Layer) declarations.

	$Id: hal.h,v 1.15 2002/12/22 15:42:55 m Exp $
*/

#ifndef SOSSE_HAL_H
#define SOSSE_HAL_H

#include <types.h>

/*! \brief Initializes the HAL.

	Must be called after each reset.
*/
void hal_init( void );

/*! \brief Read data from EEPROM.

	The internal EEPROM begins at address 0, the external EEPROM is
	located subsequently.

	\param dst Pointer to destination area.
	\param src EEPROM source address.
	\param len Length of data in bytes.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.
*/
bool hal_eeprom_read( uint8_t *dst, uint16_t src, uint8_t len );
/*! \brief Write data to EEPROM.

	The internal EEPROM begins at address 0, the external EEPROM is
	located subsequently.

	\param dst EEPROM destination address
	\param src Pointer to source area.
	\param len Length of data in bytes.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.
*/
bool hal_eeprom_write( uint16_t dst, uint8_t *src, uint8_t len );

/*! \brief Send a byte with T=0 error detection and recovery.

	Currently only 9600 bps at 3.58MHz with direct convention is supported.

	\param b Byte to send.
*/
void hal_io_sendByteT0( uint8_t b );
/*! \brief Receive a byte with T=0 error detection and recovery.

	Currently only 9600 bps at 3.58MHz with direct convention is supported.

	\return
	This function returns a received byte. There can't be an error,
	because T=0 tries endlessly to receive a characters. (And if there
	is really an error, what should SOSSE do with it anyway?)
*/
uint8_t hal_io_recByteT0( void );

/*! \brief Adds entropy to the random number generator state.

	<P><B>The idea:</B> Adding real randomness to the RAND_STATE via random
	events and a high speed timer.</P>

	<P><B>The implementation:</B> Timer 0 is started at the beginning
	and incremented every clock cycle, i.e. normally 3.58 million times
	per second. The timer is read at the beginning and at the end of
	the execution of every command. After every 8 timer values gathered
	in this way, RAND_STATE is read and encrypted with RAND_STATE and
	timer values as key. The result is written back to RAND_STATE.</P>

	<P><B>The analysis:</B> The analysis has been done by calculating
	the difference of each consecutive timer value pair. From these value
	two distributions are build. The first is the frequency for the
	timer value differences within the command and the second is the
	frequency for the timer value differences outside the command, i.e.
	while communicating with the terminal. The command used for this
	evaluation is the Read EEPROM command (80 04 18 00 10), which should
	have a relatively constant execution time and does not receive bytes
	from the terminal.</P>

	<P><B>Terminal randomness:</B> As expected the randomness of the
	distribution depends on the external system, i.e. card reader and
	host. Different distributions have been seen e.g. for Towitoko
	Chipdrive and Intertext IX2. This randomness can increas the
	randomness in the RAND_STATE buffer, when the card is in the hand
	of the owner of the card. By this, it changes RAND_STATE in a
	non-deterministic way for the institution writing the starting
	value of RAND_STATE</P>

	<P><B>Command randomness:</B> Because there is no obvious possibility
	for different runtimes, the expected result would be to have exactly
	the same delta values for all command runs, but this is not the case.
	The result is a 9 - 10 ticks wide curved distribution. The reason for
	this might be the I2C routine communicating with the external EEPROM,
	because Get Response and Read EE in the internal EEPROM area have the
	expected result with a single delta value. Independed of the source
	of the clock tick variations, there is probably not much non-determinism
	for a skilled attacker, because he can count the clocks a command has
	executed.</P>

	\note This function and the initialization in hal_init() is currently
	commented out.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.
*/
bool hal_rnd_addEntropy( void );

/*! \brief Returns 8 random bytes.

	<EM>The current AVR implementation does not produce real random
	numbers, but pseudo random numbers. These are produced by a X9.17
	generator modified to use a counter instead of a timer. The state
	of this PRNG is held at RAND_STATE_ADDR. The start value (at least
	the key) at this address should be unique for every card.</EM>

	\param dst Pointer to the destination, where the 8 random bytes
		should be written.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.

	\note There is still a simpler PRNG in the code, which uses
	16 bytes RAM and 126 bytes ROM less than the X9.17 generator.
*/
bool hal_rnd_getBlock( uint8_t *dst );

/*! \brief Sets the LEDs on Jupiter 2 cards.

	This can be used for example for debugging to show some state
	or to get a Nightrider feeling.

	\param set Bits 0-6 are currently used for specifying LEDs.

	\retval TRUE is currently returned

	\note This function is always included when compiling for the
		AT90S8535, which is AFAIK the only processor, which is available
		as PCB with LEDs.
*/
bool hal_led( char set );

#endif /* SOSSE_HAL_H */
