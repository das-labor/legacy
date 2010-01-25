/* Utility functions for receiving and transmitting data from and to the target device.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Soeren Heisrath <forename@surname.org>, Hans-Gert Dahmen <sexyludernatascha@gmail.com>
 */

//from stdinclude path
#include <stdio.h>
#include <stdint.h>
#include <string.h>
//#include <endian.h>

//include rfmusb c driver interface
#include "../../rfm12usb/host/CDriver/RfmUsb.h"
#include "../common/protocol.h"


#ifdef WIN32
#define	usleep(x) Sleep(x)
#else
#include <unistd.h>
#include <signal.h>
#endif


/**
   GLOBALS
**/

//rfmusb packetbuffer; stores received packets
rfmusb_packetbuffer packetBuffer;
rfmusb_dev_handle *udhandle = NULL;

//usage
void usage()
{
	printf(
		"NakkaFlash Utility\r\n\r\n"
		"  Usage: nakkaflash -f firmware-file -a device_address\r\n\r\n"
		"  -f Specifies the Firmware file (binary formwat)\r\n"
		"  -a Specifies the device Address (in hex)\r\n\r\n"
		"Example: nakkaflash foo.bin 0xba\r\n"
	);
}


void myexit (int in_signal)
{

        printf ("\r\nNakkaflash closing...\r\n");
        exit (in_signal);
}

int tx_packet (rfmusb_dev_handle *udhandle, uint8_t in_type, uint8_t addr, uint8_t len, uint8_t * data)
{
	uint8_t pkt[256];
	uint8_t i = 0;

	//form packet
	pkt[i++] = PROTO_TYPE0;
	pkt[i++] = PROTO_TYPE1;
	memcpy(pkt + i, data, len);

	//transmit packet
	printf("trans: %2x%2x%2x, len %i\n", pkt[0], pkt[1], pkt[2], i + len);
	return rfmusb_TxPacket (udhandle, in_type, len + i, (unsigned char *) pkt);
}


#ifdef WIN32
void winexit(void)
{
    myexit(0);
}
#endif

uint8_t match_packet (uint8_t *in_packet)
{
	if (in_packet[0] == PROTO_TYPE0 && in_packet[1] == PROTO_TYPE1)
		return 1;
	return 0;
}

int main (int argc, char* argv[])
{
        //usb stuff
	int vid, pid, tmp;
	uint_fast32_t mc = 0;

	//various variables
        uint8_t tmpchar = 0x00;
	uint_fast8_t ptype;
	uint_fast32_t packetcounter = 0;
	int istate = 0;
	uint8_t txbuf[256];
	uint_fast32_t i;

        //try to open the device
	if (rfmusb_Connect(&udhandle) != 0)
	{
		printf ("Can't find RfmUSB Device!\r\n");
                //FIXME -> what's this?
		//sig_cleanup(__LINE__ * -1);
		return __LINE__ * -1;
	}

#ifndef WIN32
	signal (SIGINT, myexit);
	signal (SIGKILL, myexit);
	signal (SIGHUP, myexit);
#else
	atexit((void * )winexit);
#endif

	while (23)
	{
		//try to fetch a packet from the air
		tmp = rfmusb_RxPacket (udhandle, &packetBuffer);

		if(tmp > 0 && match_packet(packetBuffer.buffer))
		{
			if (packetBuffer.buffer[2] == CMD_MOTION)
			{
				mc++;
				printf("**** MOTION **** (count = %10i\n", mc);
				if (mc > 5) system ("su -c \"mocp -p\" sh");
			}
			
			printf("RX\t");
			for (i=0;i<16;i++)
			{
				printf("%02x ", packetBuffer.buffer[i]);
			}
			printf("\t\tMC=%i", mc);
			printf("\n");
		}

		//if an error occurs...
		if (tmp < 0)
		{
			fprintf (stderr, "USB error: %s\r\n", usb_strerror());

			exit (__LINE__ * -1);
		}
		
		txbuf[0] = CMD_RESET;
		txbuf[1] = CMD_RESET;
		txbuf[2] = CMD_RESET;
		txbuf[3] = CMD_RESET;
		#if 0
		tx_packet (udhandle, PROTO_TYPE0, PROTO_TYPE1, 4, txbuf);
		return;
		#endif

		
		//this is done to prevent stressing the usb connection too much
		usleep (1000);
	}
}
