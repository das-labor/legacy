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

/* mmmuxd compile-time configuration
 */

#pragma once

/* RFM12USB device
 */
#define MMMUX_USE_RFM12USB 1

/* l8er
 */
#define MMMUX_USE_CANUSB   0

/* dummy hardware task */
#define MMMUX_USE_DUMMYHW 1


/* maximum wait delay for client until server socket is established (usec) */
#define MMMUX_SOCK_WAIT_DELAY (1024 * 5000)
