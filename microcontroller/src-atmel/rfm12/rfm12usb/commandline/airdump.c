#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <usb.h>
#include <unistd.h>
#include <signal.h>

#include "opendevice.h"

#include "../firmware/usbconfig.h"
#include "../firmware/requests.h"

#define BUF_IN 0
#define BUF_OUT 1

usb_dev_handle *udhandle = NULL;

void sig_cleanup(int in_signum);

void sig_cleanup (int in_signum)
{
	printf("DOES NOT COMPUTE! (Signal %i)\r\n", in_signum);
	if (udhandle != NULL) usb_close ( udhandle );
}

int main (int argc, char *argv[])
{
	uint_fast16_t i;
	int vid, pid, tmp;

	uint8_t buffer[2][64];
	uint16_t buflen[2] = { 0, 0 };

	const unsigned char rawVid[2] =
	{
		USB_CFG_VENDOR_ID
	},
	rawPid[2] =
	{
		USB_CFG_DEVICE_ID
	};

	char vendor[] =
	{ 
		USB_CFG_VENDOR_NAME, 0
	},
		product[] =
		{
			USB_CFG_DEVICE_NAME, 0
		};

	signal (SIGKILL, sig_cleanup);
	signal (SIGINT, sig_cleanup);

	usb_init();
	
	/* compute VID/PID from usbconfig.h so that there is a central source of information */
	vid = rawVid[1] * 256 + rawVid[0];
	pid = rawPid[1] * 256 + rawPid[0];

	if (usbOpenDevice (&udhandle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
	{
		printf ("Can't find USB Device w/ uid %04X, pid %04X\r\n", vid, pid);
		sig_cleanup(__LINE__ * -1);
		return __LINE__ * -1;
	}

	while (42)
	{
		tmp = usb_control_msg (udhandle,
				USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
				CUSTOM_RQ_GET_DATA, 0, 0, buffer[BUF_IN], sizeof (buffer[BUF_IN]),
				5000);
		if (tmp > 0)
		{
			buflen[BUF_IN] = tmp;
			printf ("%03i bytes received --------\r\n", tmp);
			for (i=0;i<tmp;i++)
			{
				printf("%0X ", buffer[BUF_IN][i]);
				if (i % 20 == 0) printf("\r\n");
			}
			printf("\r\n");
		} else if (tmp < 0)
		{
			fprintf (stderr, "USB error: %s\r\n", usb_strerror());
		}
	}
}
