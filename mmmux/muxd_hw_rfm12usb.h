/*   MMMUX - a device access multiplexing library
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
 *   Copyright (C) 2012 Soeren Heisrath (forename at surename dot org)
 */

#include "muxd_config.h"

#if MMMUX_USE_RFM12USB == 1
#include "muxd.h"
#include "muxd_hw.h"
#include <usb.h>
#pragma once

#define RFMUSB_RQ_ECHO             0x00
#define RFMUSB_RQ_RFM12_PUT        0x01
#define RFMUSB_RQ_RFM12_GET        0x02
#define RFMUSB_RQ_GETNOTIFICATION  0x03
#define DEFAULT_USB_TIMEOUT        1000


/* additional userdata struct for the hw description. this
 * struct holds additional information that is stored in the
 * udata field.
 */
typedef struct
{
	int vid;
	int pid;
	size_t rxlen;
	size_t txlen;
	usb_dev_handle *uhandle;
} rfm12usb_t;

int rfm12usb_find (mmmux_sctx_t *in_c);
#endif
