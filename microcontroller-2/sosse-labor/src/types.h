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
	\brief Type declarations.

	$Id: types.h,v 1.12 2003/03/30 19:36:48 m Exp $
*/

#ifndef SOSSE_TYPES_H
#define SOSSE_TYPES_H

#define FALSE	0
#define TRUE	!FALSE

#ifdef __AVR__
#include <avr/pgmspace.h>
//! Variable is in code space. Be carfull with pointers to this space.
#define	CODE	__ATTR_PROGMEM__
#else
#define PRG_RDB(x)	(*((iu8*)(x)))
#define	CODE
#endif

#include <stdint.h>

//! 1 byte unsigned data type.
typedef uint8_t iu8;
//! 2 byte unsigned data type.
typedef uint16_t iu16;
//! 4 byte unsigned data type.
typedef uint32_t iu32;
//! Boolean data type.
typedef uint8_t bool;

#endif /* SOSSE_TYPES_H */
