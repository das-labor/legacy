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

/* close the usb connection, remove the hardware from the list */
void rfm12usb_close (void *in_ctx)
{
	rfm12usb_t *rs = ((mmmux_hw_t*) in_ctx)->udata;
	usb_close (rs->uhandle);
	rs->uhandle = NULL;
	mmmux_hw_remove ((mmmux_hw_t*) in_ctx);
}

ssize_t rfm12usb_tx (void *in_ctx, size_t in_len, void* in_data)
{
	size_t txlen = in_len;
	int rv;
	rfm12usb_t *rs = ((mmmux_hw_t*) in_ctx)->udata;

	if (txlen > rs->txlen)
		txlen = rs->txlen;
	
	rv = usb_control_msg (rs->uhandle,
		USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
		RFMUSB_RQ_RFM12_PUT, 0, 0, (char *) in_data, txlen,
		DEFAULT_USB_TIMEOUT);

	if (rv > 0)
		((mmmux_hw_t*) in_ctx)->txcount += rv;

	return (ssize_t) rv;
}

ssize_t rfm12usb_rx (void *in_ctx, size_t in_maxlen, void* out_data)
{
	rfm12usb_t *rs = ((mmmux_hw_t*) in_ctx)->udata;
	int rv;

	memset (out_data, 0x00, in_maxlen);

	rv = usb_control_msg (rs->uhandle,
		USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
		RFMUSB_RQ_RFM12_GET, 0, 0, (char *) out_data, in_maxlen,
		DEFAULT_USB_TIMEOUT);

	if (rv > 0)
		((mmmux_hw_t*) in_ctx)->rxcount += rv;

	return (ssize_t) rv;
}

/* walk along the usb busses and devices, search for rfm12usb devices
 * and add them to the hardware list.
 */
int rfm12usb_find ()
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
			memset (hws, 0x00, sizeof(mmmux_hw_t));

			/* store rfm12usb specific data in the udata area */
			hws->udata = malloc (sizeof(rfm12usb_t));
			rs = (rfm12usb_t *) hws->udata;

			rs->vid = dev->descriptor.idVendor;
			rs->pid = dev->descriptor.idProduct;
			rs->txlen = 64; /* XXX hardcoded values for now */
			rs->rxlen = 64;
			rs->uhandle = h;
			sprintf (hws->name, "RFM12USB device #%u", c++);
			hws->init = NULL;
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
