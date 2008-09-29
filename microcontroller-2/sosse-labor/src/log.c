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
	\brief Logging functions.

	$Id: log.c,v 1.6 2002/12/22 15:42:55 m Exp $
*/

#include <config.h>
#include <hal.h>
#include <log.h>
#include <types.h>

#if CONF_WITH_LOGGING==1
uint16_t dst=LOG_START+2;
bool log_enable;

void log_init( void )
{
	dst=LOG_START+2;
	hal_eeprom_write( LOG_START, (uint8_t *) &dst, 2 );
	log_enable=FALSE;
}

void log_add( uint8_t tag, uint8_t *data, uint8_t len )
{
	bool save=log_enable;

	if( !log_enable || dst<LOG_START ) return;

	log_enable=FALSE;
	hal_eeprom_write( dst++, &tag, 1 );
	hal_eeprom_write( dst++, &len, 1 );
	hal_eeprom_write( dst, data, len );
	dst+=len;
	hal_eeprom_write( LOG_START, (uint8_t *) &dst, 2 );
	log_enable=save;
}
#endif /* CONF_WITH_LGGING==1 */

