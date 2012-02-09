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

#include "muxd_client.h"
int mmmux_client_connect (mmmux_sctx_t *in_c)
{
	struct sockaddr_un r;
	int len, e, rv;

	if (in_c->listenfd > 0)
	{
		err ("listener socket already set? (%i)", in_c->listenfd);
		return - __LINE__;
	}

	in_c->listenfd = socket (AF_UNIX, SOCK_STREAM, 0);
	e = errno;

	if (in_c->listenfd < 0)
	{
		err ("can't create socket: %s", strerror(e));
		return - __LINE__;
	}
	r.sun_family = AF_UNIX;
	strcpy (r.sun_path, in_c->sockname);
	len = strlen (r.sun_path) + sizeof(r.sun_family);

	rv = connect (in_c->listenfd, (struct sockaddr *)&r, len);
	e = errno;
	if (rv < 0)
	{
		err ("can't connect to socket '%s', reason: %s", in_c->sockname, strerror(e));
		in_c->listenfd = 0;
		return - __LINE__;
	}
	
	dbg ("socket connected");
	return 0;
}

int mmmux_send (mmmux_sctx_t *in_c, void* in_buf, size_t in_len)
{
	size_t sent = 0;
	int rv, e;

	while (sent < in_len)
	{
		rv = send (in_c->listenfd, (void*) ((size_t) in_buf + sent), in_len - sent, 0);
		e = errno;

		if (rv < 0)
		{
			err ("error sending: %s", strerror(e));
			return rv;
		}

		sent -= rv;
	}
	return sent;
}

int mmmux_receive (mmmux_sctx_t *in_c, void* out_buf, size_t in_maxlen)
{
	return recv (in_c->listenfd, out_buf, in_maxlen, 0);
}
