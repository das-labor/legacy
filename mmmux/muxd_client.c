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

