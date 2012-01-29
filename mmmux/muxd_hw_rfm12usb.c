#include "muxd_hw_rfm12usb.h"

#if MMMUX_USE_RFM12USB == 1
/* match usb pid/vid numbers
 */
int rfm12usb_match_usbid (int in_vid, int in_pid)
{
	if (in_vid == 0x16C0 && in_pid == 0x05DC)
		return 1;
	
	return 0;
}

int rfm12usb_find (void *in_ctx)
{
	struct usb_bus *bus;
	struct usb_device *dev;
	uint_fast8_t c = 0;
	usb_dev_handle *h = NULL;

	mmmux_hw_t *hws;
	rfm12usb_t *rs;

	usb_init();
	usb_find_busses();
	usb_find_devices();

	bus = usb_get_busses();
	while (bus != NULL)
	{
		for (dev = bus->devices; dev != NULL; dev = dev->next)
		{
			h = usb_open (dev);
			if (h == NULL)
				continue;

			if (!rfm12usb_match_usbid (dev->descriptor.idVendor, dev->descriptor.idProduct))
				continue;

			dbg ("found rfm12usb device: vid %04X, pid %04X",
				dev->descriptor.idVendor, dev->descriptor.idProduct);

			hws = malloc (sizeof(mmmux_hw_t));

			/* store rfm12usb specific data in the udata area */
			hws->udata = malloc (sizeof(rfm12usb_t));
			rs = (rfm12usb_t *) hws->udata;

			rs->vid = dev->descriptor.idVendor;
			rs->pid = dev->descriptor.idProduct;
			rs->uhandle = h;
			sprintf (hws->name, "RFM12USB device #%u", c++);
			hws->init = rfm12usb_init;
			hws->close = rfm12usb_close;
			hws->tx = rfm12usb_tx;
			hws->rx = rfm12usb_rx;

			mmmux_hw_add (hws);
		}
		bus = bus->next;
	}
	return 0;
}
#endif
