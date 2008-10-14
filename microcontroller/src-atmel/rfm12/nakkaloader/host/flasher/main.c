/* Utility functions for receiving and transmitting data from and to the target device.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Soeren Heisrath <forename@surname.org>
 */

//from stdinclude path
#include <stdio.h>
#include <stdint.h>
#include <usb.h>

//rfm12usb includes
#include "../../../rfm12usb/host/common/opendevice.h"
#include "../../../rfm12usb/firmware/usbconfig.h"
#include "../../../rfm12usb/common/usb_id.h"

//include c driver interface
#include "../../../rfm12usb/host/CDriver/RfmUsb.h"

//project specific
#include "../../common/nl_protocol.h"
#include "config.h"


#ifdef WIN32
#define	usleep(x) Sleep(x)
#endif


/* defines for the expect function */
#define EXP_ADD   0x00 /* add an entry to the list */
#define EXP_MATCH 0x01 /* match against current list */
#define EXP_DEL   0x02 /* delete a single entry */

/**
   GLOBALS
**/

//rfmusb packetbuffer; stores received packets
rfmusb_packetbuffer packetBuffer;
usb_dev_handle *udhandle = NULL;

//usage
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


#ifdef WIN32
void winexit(void)
{
    nf_exit(128);
}
#endif


uint8_t *read_buf_from_hex(FILE *f, size_t *size, size_t *offset)
{
	int i, tt;
	uint8_t *buf;

	if (fscanf(f, ":%2x", size) != 1)
		goto error;

	if (fscanf(f, "%4x", offset) != 1)
		goto error;

	if (fscanf(f, "%2x", &tt) != 1)
		goto error;

	if(tt == 1) {
		*size  = 0;
		return 0;
	}

	i = *size;
	buf = malloc(*size);
	uint8_t *ptr = buf;
	for(;i > 0; i--) {
		if (fscanf(f, "%2x", ptr++) != 1)
			goto error;
	}
	if (fscanf(f, "%2x", &tt) != 1)	 // checksum
		goto error;

	fscanf(f, "\n");

	return buf;

error:
	*size = -1;
	return 0;
}


/* @description this function maintains a list of valid packet types for each state of the
 * flasher and can be used to match a given packet against them.
 */
uint_fast8_t nl_packet_match (uint_fast8_t in_len, rfmusb_packetbuffer *in_packet, uint_fast8_t in_function)
{
	static uint8_t valid_packet_types[256];
	static uint_fast8_t listlen = 0;
	uint_fast8_t i;

	switch (in_function)
	{
		case EXP_ADD:
			if (in_len + listlen > 256) return 0; /* list is full (unlikely) */

			//add type to list
			valid_packet_types[listlen] = in_len;
			return 1;

		case EXP_MATCH:
			if (in_len < 2) return 0;
			for (i=0;i<listlen;i++)
				if (valid_packet_types[i] == (uint8_t) in_packet->buffer[0]) return valid_packet_types[i];
			return 0;

		case EXP_DEL:
			for (i=0;i<listlen;i++)
			{
				if (valid_packet_types[i] == in_len)
				{
					if (i+1 < listlen)
						valid_packet_types[i] = valid_packet_types[listlen];
					listlen--;
					return listlen;
				}
			}
			return 0;
	}
}

int main (int argc, char* argv[])
{
    //usb stuff
	int vid, pid, tmp;

	//various variables
    uint8_t tmpchar = 0x00;
	uint_fast16_t i;
	ssize_t l;
	uint_fast8_t ptype;
	uint_fast32_t packetcounter = 0;
	nf_config_t *myconfig;

    //config stuff
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

	char vendor[] =
	{
		USB_CFG_VENDOR_NAME, 0
	},
	product[] =
    {
        USB_CFG_DEVICE_NAME, 0
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

#ifndef WIN32
	signal (SIGINT, nf_exit);
	signal (SIGKILL, nf_exit);
	signal (SIGHUP, nf_exit);
#else
    atexit((void * )winexit);
#endif

	tmp=0;
	i=0;

	nf_parse_args (argc, argv, myconfig);

	printf ("\r\n");

	printf ("Connection established.\r\n");

	while (23)
	{
		//try to fetch a packet from the air
		tmp = rfmusb_RxPacket (udhandle, &packetBuffer);

		//if an error occurs...
		if (tmp < 0)
		{
			fprintf (stderr, "USB error: %s\r\n", usb_strerror());

			nf_exit (__LINE__ * -1);
		}

		//verify that this is a valid packet
		else if (ptype = nl_packet_match(tmp, &packetBuffer, EXP_MATCH))
		{
			//see what to do for given packet type
			switch (ptype)
			{
				//slave has sent it's configuration
				case NLPROTO_SLAVE_CONFIG:
					printf("got slave config!\nPagesize: %i\n", ((nl_config *)(packetBuffer.buffer + 2))->pagesize);
				break;
			}

			//transmit packet prototype
			//int rfmusb_TxPacket (udhandle, unsigned char type, unsigned char len, unsigned char * data);
		}

		//this is done to prevent stressing the usb connection too much
		usleep (1000);
	}
}
