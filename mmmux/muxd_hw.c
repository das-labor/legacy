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

#include "muxd_hw.h"
static mmmux_hw_t *hw_first = NULL;

/* the hardware task is forked for each device found.
 */
int mmmux_hw_task (mmmux_sctx_t *in_c, mmmux_hw_t *in_h)
{
	int e, rv, i;
	pid_t p;
	uint8_t buf[1024];
	int pfds[2];

	fd_set m_set, r_set;
	struct timeval tv;
	
	/* each hw task has its own pipe */
	pipe (pfds);

	for (i=0;in_c->fds_hw[i] >= 0;i++);
	dbg ("adding pipe #%i at pos %i", pfds[1], i);
	in_c->fds_hw[i] = pfds[1];
	p = fork();
	e = errno;
	v = in_c->debugfd;

	if (p < 0)
	{
		err ("couldn't launch hardware task: %s", strerror(e));
		return - __LINE__;
	}

	if (p > 0)
	{
		dbg ("hardware task for device '%s' started, pid %i",
			in_h->name, p);
		return 0;
	}
	//p = setsid ();
	v = in_c->debugfd;

	mmmux_server_drop_privs (in_c);
	/* ~~~~ actual task starts here ~~~~ */

	if (in_h->init != NULL)
		in_h->init ((void*) in_h);
	
	//fcntl (pfds[0], F_SETFL, O_NONBLOCK);
	FD_ZERO (&m_set);
	FD_SET (pfds[0], &m_set);
	
	while (23)
	{
		if (in_h->rx != NULL)
		{
			rv = in_h->rx(in_h, sizeof(buf), buf);

			if (rv < 0)
			{
				err ("couldn't exec rx function; bailing out.");
				return - __LINE__;
			}
			
			if (rv > 0)
			{
				dbg ("received %i bytes", rv);
				write (in_c->pfds_sock[1], buf, rv);
			}
		}

		tv.tv_usec = 1000;
		tv.tv_sec = 0;
		r_set = m_set;	
		rv = select (pfds[0] + 1, &r_set, NULL, NULL, &tv);
		e = errno;

		if (rv == -1 && e == EINTR)
			continue;
		
		if (in_h->tx == NULL)
			continue;

		if (!FD_ISSET (pfds[0], &r_set))
			continue;
		
		rv = read(pfds[0], buf, sizeof(buf));
		e = errno;
		
		if (rv <= 0 && !(e == EAGAIN || e == EWOULDBLOCK))
		{
			err ("read() returned error: %s", strerror(e));
			return - __LINE__;
		}
		
		dbg ("hw task (%s): got %i bytes via pipe %02X%02X%02X%02X ...", in_h->name, rv,
			buf[0], buf[1], buf[2], buf[3]);
		in_h->tx (in_h, rv, buf);
	}
	return 0;
}

/* add a hardware device to the internal list,
 * launch a task to handle the data.
 *
 * this function is called by a device-finding routine (i.e. rfm12usb_find)
 */
int mmmux_hw_add (mmmux_sctx_t *in_c, mmmux_hw_t *in_h)
{
	mmmux_hw_t *hw;
	pid_t p;
	int rv;
	
	in_h->next = NULL;

	if (hw_first == NULL)
	{
		hw_first = in_h;
	} else
	{
		for (hw=hw_first; hw->next != NULL; hw = hw->next);

		hw->next = in_h;
	}
	mmmux_hw_task (in_c, in_h);
}

void mmmux_hw_remove (mmmux_hw_t *in_h)
{
	mmmux_hw_t *last, *current = hw_first;

	/* TODO pipefd remove */

	if (hw_first == NULL || in_h == NULL)
		return;
	
	if (in_h->udata != NULL)
		free (in_h->udata);

	if (hw_first == in_h)
	{
		hw_first = in_h->next;
		return;
	}

	while (current->next != in_h && current->next != NULL)
		current = current->next;
	
	if (current->next != in_h)
	{
		err ("hw descriptor not found.");
	}
	
	current->next = in_h->next;
	
	free (in_h);
}

int mmmux_hw_init (mmmux_sctx_t *in_c)
{
	int rv;
	
	/* init hw pipes to -1 */
	for (rv = 0; rv < sizeof(in_c->fds_hw) / sizeof(int); rv++)
		in_c->fds_hw[rv] = -1;
	
#if MMMUX_USE_RFM12USB == 1
	rv = rfm12usb_find (in_c);
	if (rv != 0)
	{
		err ("failed to initialize rfm12usb device");
		return rv;
	}
#endif

#if MMMUX_USE_DUMMYHW == 1 && 0
	rv = dummyhw_find (in_c);
#endif
	dbg ("wtf");
	return 0;
}
