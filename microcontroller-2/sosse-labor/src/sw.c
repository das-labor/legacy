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
	\brief Errno functions.

	$Id: sw.c,v 1.1 2002/12/22 15:42:55 m Exp $
*/

#include <config.h>
#include <sw.h>
#include <types.h>

#if CONF_WITH_RETCODEMGR==1
uint8_t sw;

void sw_set( uint8_t value )
{
	sw = value;
}

uint16_t sw_get( void )
{
	switch( sw ) {
    case SW_ACCESS_DENIED:
		return 0x6982;
    case SW_AUTH_BLOCKED:
		return 0x6983;
    case SW_AVAILABLE:
		return 0x6100;
    case SW_COUNTER:
		return 0x63C0|(sw&0xF);
    case SW_FILE_EXISTS:
		return 0x6A89;
    case SW_FILE_NOT_FOUND:
		return 0x6A82;
    case SW_FILE_TO_SHORT:
		return 0x6A84;
    case SW_INCOMPATIBLE_FILE:
		return 0x6981;
    case SW_NOT_ALLOWED:
		return 0x6986;
    case SW_OK:
		return 0x9000;
    case SW_REF_DATA_INVALID:
		return 0x6984;
    case SW_REF_DATA_NOT_FOUND:
		return 0x6A88;
    case SW_WRONG_CLA:
		return 0x6E00;
    case SW_WRONG_CONDITION:
		return 0x6985;
    case SW_WRONG_DATA:
		return 0x6A80;
    case SW_WRONG_INS:
		return 0x6D00;
    case SW_WRONG_LE:
		return 0x6C00;
    case SW_WRONG_LEN:
		return 0x6700;
    case SW_WRONG_P1P2:
		return 0x6A86;
    case SW_WRONG_REFERENCE:
		return 0x6B00;
	default:
		return 0x6F00;
	}
}
#else /* CONF_WITH_RETCODEMGR!=1 */
uint16_t sw;

void sw_set( uint16_t value )
{
	sw = value;
}
#endif /* CONF_WITH_RETCODEMGR!=1 */

