/* Utility functions for receiving and transmitting data from and to the target device.
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

/* cleanup these please */
#include "../../common/console.h"
#include "../../common/configvars.h"
#include "../../common/requests.h"
#include "../common/opendevice.h"

#include "../../firmware/usbconfig.h"
#include "../../firmware/requests.h"

//working includes
#include "termio.h"
#include "dump.h"

//include c driver interface
#include "../../../rfm12usb/host/CDriver/RfmUsb.h

/* defines for the expect function */
#define EXP_ADD   0x00 /* add an entry to the list */
#define EXP_MATCH 0x01 /* match against current list */
#define EXP_DEL   0x02 /* delete a single entry */
#define EXP_REP   0x03 /* replace entire list */

//rfmusb packetbuffer; stores received packets
rfmusb_packetbuffer packetBuffer;

usb_dev_handle *udhandle = NULL;

void nf_help()
{
	printf(
		"NakkaFlash Utility\r\n\r\n"
		"  Usage: nakkaflash -f firmware-file -a device_address\r\n\r\n"
		"  -f Specifies the Firmware file (binary formwat)\r\n"
		"  -a Specifies the device Address (in hex)\r\n\r\n"
		"Example: nakkaflash foo.bin 0xba\r\n"
	);
}


void nf_exit (int in_signal)
{
        printf ("\r\nNakkaflash closing...\r\n");
        exit (in_signal);
}

/* @description this function maintains a list of valid packet types for each state of the
 * flasher and can be used to match a given packet against them.
 */
uint_fast8_t nl_packet_match (uint_fast8_t in_len, rfmusb_packetbuffer *in_packet, uint_fast8_t in_function)
{
	static uint8_t *valid_packets = malloc(256);
	static uint_fast8_t listlen = 0;
	uint_fast8_t i;

	switch (in_function)
	{
		case EXP_ADD:
			if (in_len + listlen > 256) return 0; /* list is full (unlikely) */
			memcpy (valid_packets[listlen], in_packet, in_len);
			return 1;

		case EXP_MATCH:
			if (in_len < 2) return 0;
			for (i=0;i<listlen;i++)
				if (valid_packets[i] == (uint8_t) in_packet->buffer[0]) return valid_packets[i];
			return 0;

		case EXP_DEL:
			for (i=0;i<listlen;i++)
			{
				if (valid_packets[i] == (uint8_t) *in_packet)
				{
					if (i+1 < listlen)
						valid_packets[i] = valid_packets[listlen];
					listlen--;
					return listlen;
				}
			}
			return 0;

		case EXP_REP:
			listlen = in_len;
			memcpy (valid_packets, in_packet, listlen);
			return 1;
	}
}

int main (int argc, char* argv[])
{
	uint8_t tmpchar = 0x00;
	uint_fast16_t i;
	ssize_t l;
	uint_fast8_t ptype;
	int vid, pid, tmp;
	uint_fast32_t packetcounter = 0;
	nf_config_t *myconfig;

	myconfig = malloc(sizeof(nf_config_t));
	myconfig->verbosity = 0;

	const unsigned char rawVid[2] =
	{
		USB_CFG_VENDOR_ID
	},
	rawPid[2] =
	{
		USB_CFG_DEVICE_ID
	};

	char vendor[] = {
	{ 
		USB_CFG_VENDOR_NAME, 0
	},
		product[] =
		{
			USB_CFG_DEVICE_NAME, 0
		}
	};
	
	usb_init();


	/* */
	// l = read(*myconfig->fname, &tmpchar, 1);
	// if (l > 0) ;
	
	/* usb setup */
	vid = rawVid[1] * 256 + rawVid[0];
	pid = rawPid[1] * 256 + rawPid[0];

	if (usbOpenDevice (&udhandle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
	{
		printf ("Can't find USB Device w/ uid %04X, pid %04X\r\n", vid, pid);
		nf_exit(__LINE__ * -1);
		return __LINE__ * -1;
	}

	signal (SIGINT, nf_exit);
	signal (SIGKILL, nf_exit);
	signal (SIGHUP, nf_exit);

	tmp=0;
	i=0;

	nf_parse_args (argc, argv, myconfig);

	printf ("\r\n");

	printf ("Connection established.\r\n");

	while (23)
	{
		//try to fetch a packet from the air
		tmp = rfmusb_RxPacket (&packetBuffer);

		//if an error occures...
		if (tmp < 0)
		{
			fprintf (stderr, "USB error: %s\r\n", usb_strerror());
			nl_exit (__LINE__ * -1);
		}
		
		//verify that this is a valid packet
		else if (ptype = nl_packet_match(tmp, &packetBuffer, EXP_MATCH))
		{		
			//see what to do for given packet type
			switch (ptype)
			{
				//slave has sent it's configuration
				case NLPROTO_SLAVE_CONFIG:
					printf("got slave config!\n");
				break;
			}

			//transmit packet prototype
			//int rfmusb_TxPacket (unsigned char type, unsigned char len, unsigned char * data);
		}
		
		usleep (1000);
	}
}
