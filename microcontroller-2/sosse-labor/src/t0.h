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
	\brief T=0 declarations.

	T=0 is not implemented fully as library, but must be partly done in
	the commands itself. This reduces the RAM requirements. E.g. when
	doing an Update Binary the data must not be received in total before
	writing, but can be received and written in single bytes.

	$Id: t0.h,v 1.12 2002/12/22 15:42:55 m Exp $
*/

#ifndef SOSSE_T0_H
#define SOSSE_T0_H

/*! \brief Header of the current command.

	This global variable contains the header (CLA INS P1 P2 P3) of the
	current command. It is fetched in the command interpreter loop in
	main().
*/
extern uint8_t header[5];

/*! \brief Send ACK byte.

	Sends the for the current command correct ACK byte to the terminal.
*/
void t0_sendAck( void );
/*! \brief Send complemented ACK byte.

	Sends the for the current command correct complemented ACK byte to the
	terminal.
*/
void t0_sendCAck( void );
/*! \brief Send word in sw.

	Sends the word in sw the the terminal. This is used to return the
	status word at the end of the command interpreter loop in main().
*/
void t0_sendSw( void );
/*! \brief Send word.

	\param w Word to send.
*/
void t0_sendWord( uint16_t w );
/*! \brief Receive mutiple bytes.

	Receive <EM>len</EM> bytes and write them to <EM>dst</EM>.

	\param dst Pointer to destination area in RAM.
	\param len Data bytes to receive.
*/
void t0_recBlock( uint8_t *dst, uint8_t len );
/*! \brief Test P1P2 for correct content.

	Compares the values of P1 and P2 in <EM>header</EM> to <EM>p1p2</EM>.
	If they differ, sw is set to SW_WRONG_P1P2 and FALSE is returned.

	\param p1p2 Target value of P1P2.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.
*/
bool t0_testP1P2( uint16_t p1p2 );
/*! \brief Test P3 for correct content.

	Compares the value of P3 in <EM>header</EM> to <EM>p3</EM>.
	If they differ, sw is set to SW_WRONG_P1P2 and FALSE is returned.

	\param p3 Target value of P3.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.
*/
bool t0_testP3( uint8_t p3 );

#endif /* SOSSE_T0_H */
