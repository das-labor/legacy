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
	\brief T=0 functions.

	$Id: t0.c,v 1.13 2002/12/22 15:42:55 m Exp $
*/

#include <config.h>
#include <sw.h>
#include <hal.h>
#include <t0.h>
#include <types.h>

uint8_t header[5];

/* Saves 4 bytes per ACK and costs itself 8 bytes. */
void t0_sendAck( void )
{
	hal_io_sendByteT0( header[1] );
}

/* Saves 4 bytes per ~ACK and costs itself 10 bytes. */
void t0_sendCAck( void )
{
	hal_io_sendByteT0( ~header[1] );
}

void t0_sendSw( void )
{
#if CONF_WITH_RETCODEMGR==1
	uint16_t sw2;

	sw2 = sw_get();

	hal_io_sendByteT0( (sw2>>8)&0xFF );
	hal_io_sendByteT0( sw2&0xFF );
#else
	hal_io_sendByteT0( (sw>>8)&0xFF );
	hal_io_sendByteT0( sw&0xFF );
#endif

	return;
}

void t0_sendWord( uint16_t w )
{
	hal_io_sendByteT0( (w>>8)&0xFF );
	hal_io_sendByteT0( w&0xFF );

	return;
}

void t0_recBlock( uint8_t *dst, uint8_t len )
{
	uint8_t i, b;

	/* Receive */
	for( i=0; i<len; i++ ) {
		/* Data */
		b = hal_io_recByteT0();

		*dst++ = b;
	}
}

bool t0_testP1P2( uint16_t p1p2 )
{
	if( (((uint16_t)header[2]<<8)|header[3])==p1p2 )
		return TRUE;

	sw_set( SW_WRONG_P1P2 );
	return FALSE;
}

bool t0_testP3( uint8_t p3 )
{
	if( header[4]==p3 )
		return TRUE;

	sw_set( SW_WRONG_LEN );
	return FALSE;
}

