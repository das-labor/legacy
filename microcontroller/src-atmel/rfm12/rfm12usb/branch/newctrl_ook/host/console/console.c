/* Dump packets onto the console.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Soeren Heisrath <forename@surname.org>
 */



#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdint.h>
#include <usb.h>
#include <unistd.h>

#include "termio.h"
#include "../../common/console.h"
#include "../../common/configvars.h"
#include "../../common/requests.h"
#include "dump.h"
#include "../common/opendevice.h"

#include "../../firmware/usbconfig.h"
#include "../../firmware/requests.h"

usb_dev_handle *udhandle = NULL;
int mytty;

void print_intro ()
{
	printf("RFM Console\r\n");
	printf("\r\nPress CTRL-A for the Config menue\r\n");
}


void console_exit (int in_signal)
{
        printf ("\nCansole closing...\n");
        close (mytty);
	reset_keypress ();
        exit (in_signal);
}

/* @description Formats a given chunk of data accordingly and hands it over to the
 * transmitting function. */
int rc_tx_packet (uint8_t in_type, uint8_t in_len, uint8_t *in_data)
{
	uint8_t tmpbuf[512];
	if (in_len)
		memcpy (tmpbuf + 2, in_data, in_len);
	
	tmpbuf[0] = in_type;
	tmpbuf[1] = in_len;

	return rc_tx_cb (in_len + 2, tmpbuf);
}

/* @description Waits for a packet of given type and copies it to the buffer.
 * @return The return value indicates the length of the packet received. This function
 * will return negative values on errors - e.g. if the timeout has been hit.
 */
int rc_expect_packet (uint8_t in_type, uint8_t *in_buffer)
{
	uint8_t rxbuf[512];
	uint8_t rxlen = 0;
	uint_fast16_t timetick = 0;
	
	while (timetick < CONF_RX_TIMEOUT && rxlen <= 0)
	{
		rxlen = rc_rx_cb (&rxbuf);
		timetick++;
	}

	if (timetick == CONF_RX_TIMEOUT) return RC_E_TIMEOUT;

	if (rxlen > 0)
	{
		if (rxbuf[0] != in_type) return (-1) * RC_E_WRONGTYPE;
		
		if (in_buffer != NULL)
			memcpy (in_buffer, rxbuf, rxlen);

		return rxlen;
	}
}

int main (int argc, char* argv[])
{
	uint8_t tmpchar = 0x00;
	uint_fast16_t i;
	ssize_t l;
	uint8_t buf[512];
	int vid, pid, tmp;
	uint_fast32_t packetcounter = 0;

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
	
	usb_init();
	
	/* usb setup */
	vid = rawVid[1] * 256 + rawVid[0];
	pid = rawPid[1] * 256 + rawPid[0];

	if (usbOpenDevice (&udhandle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
	{
		printf ("Can't find USB Device w/ uid %04X, pid %04X\r\n", vid, pid);
		console_exit(__LINE__ * -1);
		return __LINE__ * -1;
	}

	signal (SIGINT, console_exit);
	signal (SIGKILL, console_exit);
	signal (SIGHUP, console_exit);

	mytty = open ("/dev/tty", O_NONBLOCK | O_RDWR );
	
//	print_intro();

	set_keypress();
	printf ("Enter device address to connect to or \"*\" for promiscious mode: ");

	tmp=0;
	i=0;

	while (i < 12 && 0)
	{
		l = read(mytty, &tmpchar, 1);
		if (l == 1)
		{
			/* determine if input is hex or decimal */
			if ( (tmpchar >= 'a' && tmpchar <= 'f')
				|| (tmpchar >= 'A' && tmpchar <= 'F')
				|| ((tmpchar == 'X' || tmpchar == 'x') && i == 1))
			{
				tmp=1; /* set to hex mode */
				i++;
				buf[i] = tmpchar;
				printf("%c", tmpchar);
			} else if (tmpchar >= '0' && tmpchar <= '9')
			{
				buf[i] = tmpchar;
				printf("%c", tmpchar);
				i++;
			} else if (tmpchar == 10)  /* return key pressed */
			{
				i = 0xff;
			} else if (tmpchar == '*') /* the "any-key" ;-) */
			{
				i = 0xff;
			}
		}
		termflush (mytty);
		usleep (1000);
	}

	printf ("\r\n");

	printf ("Connection established.\r\n");

	while (23)
	{
		tmp = usb_control_msg (udhandle,
				USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
				RFMUSB_RQ_RADIO_GET, 0, 0, buf, sizeof (buf),
				5000);

		if (tmp > 0) /* data received from usb bus */
		{
			packetcounter++;
			for (i=0;i<tmp;i++)
			{
				printf("%c", sanitize_char(buf[i]));
				termflush (mytty);
			}
		} else if (tmp < 0)
		{
			fprintf (stderr, "USB error: %s\r\n", usb_strerror());
			return __LINE__ * -1;
		}
		
		l = read(mytty, &tmpchar, 1);
		if (l == 1) /* character from stdin */
		{
			buf[0] = USB_SENDCHAR;
			buf[1] = tmpchar;

		//	printf("%c", tmpchar); /* local echo */

			tmp = usb_control_msg (udhandle,
					USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
					RFMUSB_RQ_RADIO_PUT, USB_TXPACKET, 0, buf, 2,
					5000);
					5000);
		}
		termflush (mytty);
		
		usleep (1000);
	}
}
