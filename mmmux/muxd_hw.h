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

#include "muxd.h"

#pragma once
#if (MMMUX_USE_RFM12USB == 1)
//#include "muxd_hw_rfm12usb.h"
#endif

/* hardware init function */
typedef void (*mmmux_hw_init_f)  (void*);

/* hardware close/shutdown/deinit function */
typedef void (*mmmux_hw_close_f) (void*);

/* transmit & receive functions */
typedef ssize_t  (*mmmux_hw_tx_f)    (void*, size_t, void*);
typedef ssize_t  (*mmmux_hw_rx_f)    (void*, size_t, void*);

/* hardware description struct
 */
typedef struct
{
	char name[256];         /* human-readable name */
	mmmux_hw_init_f init;   /* init function */
	mmmux_hw_tx_f tx;       /* transmit function */
	mmmux_hw_rx_f rx;       /* receive function */
	mmmux_hw_close_f close; /* close function */
	void *udata;            /* extra data */
	void *next;
	size_t txcount;
	size_t rxcount;
	useconds_t sdelay;      /* sleep delay in usecs (for polled devices) */
} mmmux_hw_t;

int mmmux_hw_init (mmmux_sctx_t *);
int mmmux_hw_add (mmmux_sctx_t *in_c, mmmux_hw_t *in_h);
void mmmux_hw_remove (mmmux_hw_t *in_h);
