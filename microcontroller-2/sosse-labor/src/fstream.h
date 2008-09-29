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
	\brief File stream declarations.

	These streams are used to pass more easily EEPROM areas around and
	to reduce the chances of overwriting unwanted EEPROM areas, e.g.
	by overflowing the data area.

	This is a very slim version of the streams implemented in cryptlib
	from Peter Gutmann.

	$Id: fstream.h,v 1.8 2002/12/22 15:42:55 m Exp $
*/

#ifndef SOSSE_FSTREAM_H
#define SOSSE_FSTREAM_H

#include <types.h>

//! Stream struct.
typedef struct s_fstream {
	uint16_t start;	//!< EEPROM start address of the stream.
	uint16_t size;	//!< Data size of the stream.
	uint16_t pos;		//!< Current position in the stream.
} S_FSTREAM;

/*! \brief Read data from a stream.

	Either this functions reads all data or nothing.

    \param fs Pointer to S_FSTREAM, on which it should be operated.
    \param dst Pointer to the data destination.
    \param len Length of the data

    \retval TRUE on success.
    \retval FALSE on failure. Error code given in sw.
*/
bool fstream_read( S_FSTREAM *fs, uint8_t *dst, uint8_t len );
/*! \brief Write data to a stream.

	This functions tries to write all or nothing.

    \param fs Pointer to S_FSTREAM, on which it should be operated.
    \param src Pointer to data source.
    \param len Length of the data

    \retval TRUE on success.
    \retval FALSE on failure. Error code given in sw.
*/
bool fstream_write( S_FSTREAM *fs, uint8_t *src, uint8_t len );
/*! \brief This function tests, if enough space is available to read or
		write <EM>len</EM> bytes.

    \param fs Pointer to S_FSTREAM, on which it should be operated.
    \param len Length of the data

    \retval TRUE on success.
    \retval FALSE on failure. Error code given in sw.
*/
bool fstream_test( S_FSTREAM *fs, uint16_t len );

#endif /* SOSSE_FSTREAM_H */

