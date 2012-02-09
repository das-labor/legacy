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

int v = 0;

/* @description check if the given file is actually a socket and working.
 * 	returns 1 on success, 0 if the file doesn't exist and/or the socket
 *	appears to be dead.
 *	return values < 0 indicate an error - the file should not be used as a
 *	socket and should not be unlinked, since it may be a real file.
 */
int socket_ping (char* in_sockname)
{
	struct stat st;
	int e, rv, s, len;
	struct sockaddr_un r;
	
	rv = stat (in_sockname, &st);
	e = errno;

	if (rv != 0)
	{
		dbg ("stat() on '%s' failed: %s", in_sockname, strerror(e));
		return 0;
	}

	if (st.st_size != 0 || !S_ISSOCK(st.st_mode))
	{
		err ("size of file '%s' is non-zero and/or file is not a socket; we really should not touch this one.",
			in_sockname);
		return - __LINE__;
	}

	s = socket (AF_UNIX, SOCK_STREAM, 0);
	e = errno;
	if (s < 0)
	{
		err ("can't create socket (TEH FUGG?!) %s", strerror(e));
		return - __LINE__;
	}

	r.sun_family = AF_UNIX;
	strcpy (r.sun_path, in_sockname);
	len = strlen (r.sun_path) + sizeof(r.sun_family);

	rv = connect (s, (struct sockaddr *)&r, len);
	e = errno;
	if (rv < 0)
	{
		/* file is a socket, but nobody's listening on the other end.
		 * return 0 and have it unlinked by the server process.
		 */
		dbg ("can't connect to socket: %s", strerror(e));
		return 0;
	}
	
	/* if we've come here, the socket exists and is live */
	dbg ("socket appears to be working");
	close (s);
	return 1;
}

mmmux_sctx_t* mmmuxd_init (int in_debug, char* in_sockname)
{
	mmmux_sctx_t *sc; /* server context */
	int i,r;

	sc = malloc (sizeof(mmmux_sctx_t));
	memset (sc, 0x00, sizeof(mmmux_sctx_t));

	if (in_debug == MDBG_NONE)
	{
		v = open ("/dev/null", O_WRONLY);
	} else if (in_debug == MDBG_STDOUT || in_debug == MDBG_STDERR)
	{
		v = in_debug * -1;
		v--;
	} else if (in_debug == MDBG_FILE)
	{
		v = open ("/tmp/mux-debug-log", O_WRONLY | O_APPEND);
	} else
	{
		v = in_debug;
	}
	sc->debugfd = v;
	
	if (in_sockname != NULL)
	{
		strncpy (sc->sockname, in_sockname, MIN(strlen(in_sockname), sizeof(sc->sockname)));
	} else
	{
		strncpy (sc->sockname, SOCKNAME_DEFAULT, strlen(SOCKNAME_DEFAULT));
	}
	
	r = socket_ping(sc->sockname);

	if (r < 0)
	{
		err ("socket_ping() returned an error - won't start.");
		return NULL;
	} else if (r != 0)
	{
		dbg ("socket already exists, using client mode");
	} else
	{
		dbg ("starting fs socket process, socket name: %s", sc->sockname);
		r = mmmux_server_sock_task(sc);
		if (r != 0)
		{
			err ("server task ended");
			return NULL;
		}
		
		dbg ("waiting for socket");
		/* wait until the client can conenct */
		for (i=0;i<MMMUX_SOCK_WAIT_DELAY;i += (MMMUX_SOCK_WAIT_DELAY / 256))
		{
			usleep (MMMUX_SOCK_WAIT_DELAY / 256);
			if (socket_ping(sc->sockname) == 1)
				break;
		}

		if (i >= MMMUX_SOCK_WAIT_DELAY)
		{
			err ("connection failed for above reason.\n");
			return NULL;
		}
		dbg ("server ready, starting client");
	}
	mmmux_client_connect (sc);

	return sc;
}
