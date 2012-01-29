#include "muxd_server.h"
static mmmux_sctx_t *my_ctx = NULL;

void mmmux_sigh_cleanup (int in_s)
{
	if (my_ctx == NULL)
		return;

	dbg ("signal %i received - cleaning up", in_s);
	close (my_ctx->listenfd);
	unlink (my_ctx->sockname);
	rmdir (my_ctx->chdir);
	free (my_ctx);
}

int mmmux_server_sock_task (mmmux_sctx_t *c)
{
	pid_t my_pid;
	struct timeval tv;
	int e, rv, nfds;

	my_pid = fork();
	e = errno;
	if (my_pid < 0)
	{
		dbg ("can't create task: %s", strerror(e));
		return my_pid; /* error */
	}

	if (my_pid > 0) /* master: return to main */
	{
		dbg (" fs task: pid %i", my_pid);
		return 0;
	}

	pipe(c->pfds_hw);
	pipe(c->pfds_sock);

	mmmux_hw_init (c);

	/* remove socket file when killed */
	signal (SIGKILL, mmmux_sigh_cleanup);
	signal (SIGINT, mmmux_sigh_cleanup);
	signal (SIGHUP, mmmux_sigh_cleanup);
	signal (SIGTERM, mmmux_sigh_cleanup);

	my_ctx = c;
	
	FD_ZERO (&c->fds_master);
	FD_ZERO (&c->fds_read);
	FD_ZERO (&c->fds_write);

	/* add pipe */
	FD_SET (c->pfds_sock[0], &c->fds_master);
	c->nfds = MAX(c->pfds_sock[0], c->nfds);

	dbg(" fs task: initializing, ctx: %p", c);
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
	chmod (in_c->sockname, S_IWOTH | S_IROTH);
	mkdir (new_dir, 0);
	chmod (new_dir, S_IWOTH | S_IROTH);
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
	int i;
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
			for (k=0;k<=in_c->nfds;k++)
			{
				if (k == i)
					continue;
				
				send (k, buf, rv, 0);
			}
		} else
		/* data from client */
		{
			
			rv = recv (i, buf, sizeof(buf), 0);

			if (rv <= 0)
			{
				mmmux_server_disconnect (in_c, i);
				continue;
			}

			dbg ("%i bytes from client: %08X", rv, buf);
			write (in_c->pfds_hw[1], buf, rv);
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


