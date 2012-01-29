#include "muxd_hw.h"
static mmmux_hw_t *hw_first = NULL;

int mmmux_hw_task (mmmux_sctx_t *in_c, mmmux_hw_t *in_h)
{
	int e, rv;
	pid_t p;
	uint8_t buf[1024];

	p = fork();
	e = errno;

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

	mmmux_server_drop_privs (in_c);
	/* ~~~~ actual task starts here ~~~~ */

	if (in_h->init != NULL)
		in_h->init ((void*) in_h);
	
	fcntl (in_c->pfds_hw[0], F_SETFL, O_NONBLOCK);
	
	while (23)
	{
		usleep (in_h->sdelay);
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
				dbg ("hw task: received %i bytes", rv);
				write (in_c->pfds_sock[1], buf, rv);
			}
		}

		if (in_h->tx != NULL)
		{
			rv = read(in_c->pfds_hw[0], buf, sizeof(buf));
			e = errno;
			
			if (rv > 0 && (e == EAGAIN || e == EWOULDBLOCK))
			{
				dbg ("hw task: got %i bytes via pipe", rv);
				in_h->tx (in_h, rv, buf);
			}

			if (rv <= 0 && !(e == EAGAIN || e == EWOULDBLOCK))
			{
				err ("read() returned error: %s", strerror(e));
				return - __LINE__;
			}
		}

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
	}

	for (hw=hw_first; hw->next != NULL; hw = hw->next);

	hw->next = in_h;
	mmmux_hw_task (in_c, in_h);
}

void mmmux_hw_remove (mmmux_hw_t *in_h)
{
	mmmux_hw_t *last, *current = hw_first;

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
	
#if MMMUX_USE_RFM12USB == 1
	rv = rfm12usb_find (in_c);
	if (rv != 0)
	{
		err ("failed to initialize rfm12usb device");
		return rv;
	}
#endif
	return 0;
}
