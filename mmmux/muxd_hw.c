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
