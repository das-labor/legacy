#include "muxd.h"

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

int mmmuxd_init (int in_debug, char* in_sockname)
{
	mmmux_sctx_t *sc; /* server context */
	int i,r;
	v = in_debug;

	sc = malloc (sizeof(mmmux_sctx_t));
	memset (sc, 0x00, sizeof(mmmux_sctx_t));
	
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
		return - __LINE__;
	} else if (r != 0)
	{
		dbg ("socket already exists, using client mode");
		mmmux_client_connect (sc);
	} else
	{
		dbg ("starting fs socket process, socket name: %s", sc->sockname);
		r = mmmux_server_sock_task(sc);
		if (r != 0)
		{
			err ("server task ended");
			return r;
		}
	}

	return 0;
}
