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
	\brief Tool functions.

	$Id: tools.c,v 1.7 2002/12/22 15:42:55 m Exp $
*/

#include <config.h>
#include <tools.h>
#include <types.h>

#ifdef ENDIAN_LITTLE
void hton_us( uint16_t *us, uint8_t num )
{
	uint16_t local;

	while( num-- ) {
		local=*us;
		*us++=swap_us(local);
	}
}

#if defined(__i386__)
void hton_ul( uint32_t *ul, uint8_t num )
{
	uint32_t local;

	while( num-- ) {
		local=*ul;
		*ul++=swap_ul(local);
	}
}
#endif
#endif /* ENDIAN_LITTLE */

