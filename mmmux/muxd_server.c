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

#include "muxd_server.h"
static mmmux_sctx_t *my_ctx = NULL;

void mmmux_sigh_cleanup (int in_s)
{
	if (my_ctx == NULL)
		return;

	dbg ("signal %i received - cleaning up", in_s);
	close (my_ctx->listenfd);
	/* XXX we won't be able to clean up this one when in chroot */
	unlink (my_ctx->sockname);
	/* this will fail when in chroot */
	rmdir (my_ctx->chdir);
	/* good thing, rmdir does not delete non-empty dirs... */
	rmdir ("."); 
	free (my_ctx);
}

int mmmux_server_sock_task (mmmux_sctx_t *c)
{
	struct timeval tv;
	int e, rv, nfds;

	pid_t my_pid;

	#if 1
	/* fork away from process that called mmmuxd_init() */
	my_pid = fork();
	e = errno;
	if (my_pid < 0)
	{
		dbg ("can't create socket task: %s", strerror(e));
		return my_pid; /* error */
	}

	if (my_pid > 0) /* master: return to main */
	{
		dbg ("sock task: pid %i", my_pid);
		return 0;
	}

	my_pid = setsid ();
	dbg ("new session id: %i", my_pid);
	#else
	daemon(0,1);
	#endif

	/* pipe hw -> socket server */
	pipe(c->pfds_sock);
	
	my_ctx = c;
	
	FD_ZERO (&c->fds_master);
	FD_ZERO (&c->fds_read);
	FD_ZERO (&c->fds_write);
	
	mmmux_hw_init (c);
	v = c->debugfd;
	

	/* remove socket file when killed */
	signal (SIGKILL, mmmux_sigh_cleanup);
	signal (SIGINT, mmmux_sigh_cleanup);
	signal (SIGHUP, mmmux_sigh_cleanup);
	signal (SIGTERM, mmmux_sigh_cleanup);


	/* add pipe */
	FD_SET (c->pfds_sock[0], &c->fds_master);
	c->nfds = MAX(c->pfds_sock[0], c->nfds);

	dbg("sock task: initializing, ctx: %p", c);
	if (mmmux_server_create_socket(c) != 0)
	{
		dbg("sock task: can't create socket");
		return - __LINE__;
	}
	mmmux_server_drop_privs (c);
	dbg("sock task: socket created, starting main loop");

	while (23)
	{
		c->fds_read = c->fds_master;
		tv.tv_usec = 0;
		tv.tv_sec = 10;
		
		rv = select (c->nfds+1, &c->fds_read, NULL, NULL, &tv);
		e = errno;

		if (rv < 0 && e == EINTR)
			continue;

		if (rv < 0)
		{
			dbg ("select() returned %i (%s), bailing out", rv, strerror(e));
			return - __LINE__;
		}

		if (rv == 0)
		{
			dbg ("sock task stats: %i clients", c->nclients);
			continue;
		}
		
		/* data from client */
		mmmux_server_handle_data(c);
	}
	return 0;
}


void mmmux_server_drop_privs (mmmux_sctx_t *in_c)
{
	uid_t u;
	char new_dir[128];
	int fp, e;
	
	fp = open ("/dev/urandom", O_RDONLY);
	e = errno;
	if (fp >= 0)
	{
		read (fp, &u, sizeof(uid_t)); 
		close (fp);
	} else
	{
		dbg ("can't open /dev/urandom (error: \"%s\") -> UIDs won't be random!", strerror(e));
		u = random();
	}

	if (u < 30000)
		u += 30000;
	
	sprintf (new_dir, "/tmp/mmmuxd-chroot-%u", u);
	strncpy (in_c->chdir, new_dir, strlen(new_dir));
	
	/*
	   error conditions intentionally not handled, since these calls
	   may fail when not invoked by mr. r00t
	 */

	chown (in_c->sockname, u, u);
	chmod (in_c->sockname, S_IWUSR | S_IRUSR | S_IWOTH | S_IROTH);
	mkdir (new_dir, 0);
	chown (new_dir, u, u);
	chmod (new_dir, S_IRUSR | S_IXUSR | S_IWUSR);
	chdir (new_dir);
	chroot (new_dir);
	setgid(u);
	if (!setuid(u))
	{
		dbg ("task : new UID is %u", u);
	} else
	{
		e = errno;
		dbg ("couldn't set UID (%s)", strerror(e));
	}
}

int mmmux_server_connect (mmmux_sctx_t *in_c)
{
	struct sockaddr_un r;
	socklen_t len;
	int new_fd, e;

	len = sizeof(r);
	new_fd = accept(in_c->listenfd, (struct sockaddr *)&r, &len);
	e = errno;
	if (new_fd < 0)
	{
		dbg ("sock task: can't accept() connection: %s", strerror(e));
		return - __LINE__;
	}

	FD_SET (new_fd, &in_c->fds_master);
	in_c->nfds = MAX(new_fd, in_c->nfds);
	in_c->nclients++;
	dbg ("sock task: new client connection (%i clients total)", in_c->nclients);
}

void mmmux_server_disconnect (mmmux_sctx_t *in_c, int in_fd)
{
	close (in_fd);
	dbg ("sock task: client %i disconnected", in_fd);
	in_c->nclients--;
	FD_CLR (in_fd, &in_c->fds_master);
}

int mmmux_server_handle_data (mmmux_sctx_t *in_c)
{
	int i,k;
	char buf[1024];
	int rv;

	for (i=0;i<=in_c->nfds;i++)
	{
		if (!FD_ISSET(i, &in_c->fds_read))
			continue;
		
		/* new connection */
		if (i == in_c->listenfd)
		{
			mmmux_server_connect (in_c);
			continue;
		}
		
		/* data from pipe (hw device) */
		if (i == in_c->pfds_sock[0])
		{
			int k;

			rv = read (in_c->pfds_sock[0], buf, sizeof(buf));
			/* send to all sockets */
			for (k=0;k<=in_c->nfds;k++)
			{
				if (k == i)
					continue;
				
				send (k, buf, rv, 0);
			}
		} else
		/* data from client */
		{
			/* XXX ZARRO??! */
			rv = recv (i, buf, sizeof(buf), 0);

			if (rv <= 0)
			{
				mmmux_server_disconnect (in_c, i);
				continue;
			}

			dbg ("%i bytes from client: %02X%02X%02X%02X", rv, buf[0], buf[1], buf[2], buf[3]);
			for (k=0; in_c->fds_hw[k] >= 0; k++)
			{
				dbg ("sent to pipe #%i, index %i\n", in_c->fds_hw[k], k);
				write (in_c->fds_hw[k], buf, rv);
			}
		}
	}
}

int mmmux_server_create_socket (mmmux_sctx_t *in_c)
{
	size_t len;
	int e, yes = 1;

	in_c->listenfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (in_c->listenfd <= 0)
		return - __LINE__;
	
	in_c->sl.sun_family = AF_UNIX;
	strncpy (in_c->sl.sun_path, in_c->sockname, strlen(in_c->sockname));
	unlink (in_c->sl.sun_path);
	dbg ("sock task: creating socket: %s", in_c->sl.sun_path);	

	len = strlen(in_c->sl.sun_path) + sizeof(in_c->sl.sun_family);
	if (bind (in_c->listenfd, (struct sockaddr *)&in_c->sl, len) < 0)
	{
		e = errno;
		dbg("sock task: bind() failed: %s", strerror(e));
		return - __LINE__;
	}

	if (setsockopt(in_c->listenfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) < 0)
	{
		e = errno;
		dbg("sock task: setsockopt() failed: %s", strerror(e));
		return - __LINE__;
	}

	if (listen (in_c->listenfd, 4) < 0)
	{
		e = errno;
		dbg("sock task: listen() failed: %s", strerror(e));
		return - __LINE__;
	}

	in_c->nfds = MAX(in_c->nfds, in_c->listenfd);
	FD_SET (in_c->listenfd, &in_c->fds_master);

	return 0;
}


