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
	\brief Logging declarations.

	<P>This logging facility can be used to assist debugging. It appends
	TLV objects (Tag, Length, Value) to logging area starting at LOG_START+2.
	At LOG_START the address of the end of data is maintained. This is in
	native byte order. (Native of architecture or compiler.)</P>

	<P>Tags can be used to specify what data has been logged. This is meant
	to assist testing, so be free to change the tags as needed.</P>

	$Id: log.h,v 1.5 2002/12/22 15:42:55 m Exp $
*/

#ifndef SOSSE_LOG_H
#define SOSSE_LOG_H

#include <types.h>

#define LOG_START			0x1800	//! Start of logging area.

#define LOG_TAG_JOKER		0xFE	//! Tag: Data types, where not tag exists.
#define LOG_TAG_EEWRITE_DST	0x01	//! Tag: EEPROM write destination address.
#define LOG_TAG_EEWRITE_SRC	0x02	//! Tag: EEPROM write source address.
#define LOG_TAG_EEWRITE_LEN	0x03	//! Tag: EEPROM write length.
#define LOG_TAG_EEREAD_DST	0x04	//! Tag: EEPROM read destination address.
#define LOG_TAG_EEREAD_SRC	0x05	//! Tag: EEPROM read source address.
#define LOG_TAG_EEREAD_LEN	0x06	//! Tag: EEPROM read length.
#define LOG_TAG_FSCREATE	0x07	//! Tag: File creation data.
#define LOG_TAG_RANDOM		0x08	//! Tag: Collected random data.

/*! \brief Enables/disables logging.

	This must be set the TRUE, before any log_add has an effect. It can
	be also changed in between to switch off and on logging to reduce
	unwanted data in the logging area.
*/
extern bool log_enable;

/*! \brief Initialize global logging variables.

	This function must be called before using log_add. It can be called
	even, when CONF_WITH_LOGGING is not 1, because it is then neutralized
	with a define.
*/
#if CONF_WITH_LOGGING==1
void log_init( void );
#else
#define log_init()
#endif

/*! \brief Log data.

	Tag, length and then the data itself is append to the log data.
	It can be called even, when CONF_WITH_LOGGING is not 1, because it is
	then neutralized with a define.

	\param tag Tag descriping the data.
	\param data Pointer to the data.
	\param len Length of the data in bytes.
*/
#if CONF_WITH_LOGGING==1
void log_add( uint8_t tag, uint8_t *data, uint8_t len );
#else
#define log_add(tag,data,len)
#endif

#endif /* SOSSE_LOG_H */
