/* This is the firmware for the RFMUSB device (USB handling functions).
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   Copyright (C) 2013 Soeren Heisrath (forename at surename dot org)
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#pragma once

#include "rfm12.h"
#include "../common/rfm12usb_config.h"
#include "../common/requests.h"
#include "oddebug.h"
#include "usbdrv.h"
#include "usbdebug.h"
#include "ook.h"

/* modes for the internal state machine
 */
#define MODE_IDLE     0
#define MODE_RX       1
#define MODE_TX       2
#define MODE_OOK_TX   3

/* states of the usb rx/tx buffer */
#define BUFSTATE_IDLE 0  /* buffer may be used */
#define BUFSTATE_RX   1  /* buffer contains received data from the radio interface */
#define BUFSTATE_TX   2  /* buffer contains data that is to be sent through the radio interface */

void usbstuff_init ();
void handle_rx ();
