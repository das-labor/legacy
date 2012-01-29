#include "muxd_hw.h"
static mmmux_hw_t *hw_first = NULL;

/* add a hardware device to the internal list
 */
int mmmux_hw_add (mmmux_hw_t *in_h)
{
	mmmux_hw_t *hw;
	in_h->next = NULL;

	if (hw_first == NULL)
	{
		hw_first = in_h;
		return;
	}

	for (hw=hw_first; hw->next != NULL; hw = hw->next);

	hw->next = in_h;
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

int mmmux_hw_init ()
{
	int rv;
#if MMMUX_USE_RFM12USB == 1
	rv = rfm12usb_find ();
	if (rv != 0)
	{
		err ("failed to initialize rfm12usb device");
		return rv;
	}
#endif
	return 0;
}
