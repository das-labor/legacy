/*   OOK Sending functions for the RFM12USB
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   Copyright (C) 2013 Soeren Heisrath (forename at surename dot org)
 */

#include "../common/rfm12usb_config.h"
#include <stdint.h>
#include <util/delay.h>
#include <string.h>
#include <avr/io.h>
#include "rfm12.h"
#include "../common/requests.h"

#ifndef RFMUSB_OOK_MAXLEN
	#define RFMUSB_OOK_MAXLEN 32
#endif

void ook_send (uint8_t in_bits, uint16_t in_delay, uint8_t *in_data, uint8_t in_type);
void ook_queue_msg (uint8_t *in_msg);
void ook_tick ();
