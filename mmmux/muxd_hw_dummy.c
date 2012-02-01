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

/* dummy hardware task for testing */

#include "muxd_hw_dummy.h"

#if MMMUX_USE_DUMMYHW == 1

ssize_t dummyhw_rx (void *in_ctx, size_t in_maxlen, void* out_data)
{
	static int c = 0;

	c++;
	if (c < 4)
		return 0;

	memset (out_data, 0x00, in_maxlen);
	snprintf (out_data, in_maxlen, "dummy hardware's counter is at %i\n", c);

	return strlen (out_data) + 1;
}
ssize_t dummyhw_tx (void *in_ctx, size_t in_len, void* in_data)
{
	return in_len;
}

int dummyhw_find (mmmux_sctx_t *in_c)
{
	mmmux_hw_t *hws;
	hws = malloc (sizeof(mmmux_hw_t));
	memset (hws, 0x00, sizeof(mmmux_hw_t));
	
	sprintf (hws->name, "DUMMY HARDWARE DEVICE");
	hws->sdelay = 1000000; /* once per second */
	hws->init = NULL;
	hws->close = NULL;
	hws->rx = dummyhw_rx;
	hws->tx = dummyhw_tx;
	mmmux_hw_add (in_c, hws);
}
#endif
