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
	\brief Tool declarations.

	$Id: tools.h,v 1.11 2002/12/22 15:42:56 m Exp $
*/

#ifndef SOSSE_TOOLS_H
#define SOSSE_TOOLS_H

#include <config.h>
#include <types.h>

//! Byte swap single short.
#define swap_us(us) (((us&0x00FF)<<8)|((us&0xFF00)>>8))
//! Byte swap single long.
#define swap_ul(ul) (((ul&0x000000FF)<<24)|((ul&0x0000FF00)<<8)|((ul&0x00FF0000)>>8)|((ul&0xFF000000)>>24))

/*! \brief Byte swap multiple shorts.

	\param us Pointer to an array of shorts.
	\param num Number of shorts to process.
*/
void hton_us( uint16_t *us, uint8_t num );
/*! \brief Byte swap multiple longs.

	\param ul Pointer to an array of longs.
	\param num Number of longs to process.
*/
void hton_ul( uint32_t *ul, uint8_t num );

#ifndef ENDIAN_LITTLE
#define hton_us(us)
#define hton_ul(ul)
#endif

/*! Return minimum value */
#define min(a,b) ((a)<(b)?(a):(b))

/*! Return maximum value */
#define max(a,b) ((a)>(b)?(a):(b))

#endif /* SOSSE_TOOLS_H */
