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

uint8_t
	opt_daemon = 0,
	opt_reset = 0,
	dst_addr = 0,
	opt_dumpall = 0,
	opt_resetall = 0,
	opt_set_treshold = 0,
	opt_dump = 0;

void print_usage();

int parse_address_arg (char* in_str);

void dump_data (size_t in_len, uint8_t *in_data);
void dump_data (size_t in_len, uint8_t *in_data)
{
	uint_fast32_t i;
	for (i=0;i<in_len;i++)
	{
		printf("%02x", in_data[i]);
		if (i && !(i % 32))
			printf("\r\n");
		else if (!(i % 4))
			printf(" ");
	}
	printf("\r\n");
}

uint8_t gethexfromchar (char in_c)
{
	if (in_c >= '0' && in_c <= '9')
	{
		return in_c - '0';
	} else if (in_c >= 'A' && in_c <= 'F')
	{
		return 10 + (in_c - 'A');
	} else if (in_c >= 'a' && in_c<= 'f')
	{
		return 10 + (in_c - 'a');
	}

	return 0;
}

void print_usage()
{
	printf(
		"Alert Control Utility\r\n\r\n"
		"Invocation: alertctrl <options>\r\n"
		"Options:\r\n"
		"  -d run as daemon\r\n\r\n"
		"  -v dump alarm system packets\r\n"
		"  -a dump ALL packets\r\n"
		"  -r <addr> Reset device at addr (format: 0xXX (hex))\r\n"
	);
}

int parse_args (int argc, char* argv[])
{
	int i;
	uint8_t tmp;

	for (i=1;i<argc;i++)
	{
		if (argv[i][0] != '-')
		{
			printf ("Argument #%i not understood.\r\n", i);
			continue;
		}

		switch (argv[i][1])
		{
			case 'v':
				opt_dump = 1;
			break;
			case 'd':
				opt_daemon = 1;
				opt_reset = 0;
			break;
			case 'r':
				if (++i == argc)
				{
					printf("You must specify an address.\r\n");
					return -1;
				}
				dst_addr = parse_address_arg(argv[i]);
				
				opt_reset = 1;
				opt_daemon = 0;
				printf("dst addr: %2x\r\n", dst_addr);
			break;
			case 'R': /* reset all devices */
				opt_resetall = 1;
				opt_reset = 1;
				opt_daemon = 0;
			break;
			case 'a':
				opt_dumpall = 1;
				opt_dump = 1;
			break;
			case 't':
			{
				int tmpaddr;
				++i;
				if (i+1 == argc)
				{
					printf("syntax is: -t address treshold\r\n");
					return -1;
				}
				tmpaddr = parse_address_arg(argv[i]);
				if (tmpaddr < 0)
				{
					printf("error parsing address\r\n");
					return -1;
				}
				dst_addr = abs(tmpaddr);
				i++;
				opt_set_treshold = abs(atoi(argv[i]));
			}
			break;
			default:
				printf ("Argument #%i not understood.\r\n", i);
				print_usage();
				return -1;
			break;
		}
	}
	return 11;
}

void myexit (int in_signal)
{

        printf ("\r\nNakkaflash closing...\r\n");
        exit (in_signal);
}

int tx_packet (rfmusb_dev_handle *in_udh, uint8_t in_dst, uint8_t in_cmd, uint8_t in_len, uint8_t *in_data)
{
	uint8_t pkt[256];
	uint8_t i = 0;

	pkt[F_T0] = PROTO_TYPE0;
	pkt[F_T1] = PROTO_TYPE1;
	pkt[F_SRC] = 0x00;
	pkt[F_DST] = in_dst;
	pkt[F_CMD] = in_cmd;

	memcpy(pkt + F_DATA, in_data, in_len);
	if (opt_dump)
	{
		printf("TX:\r\n");
		dump_data (in_len, in_data);
		printf("\r\n");
	}

	return rfmusb_TxPacket (in_udh, RFM12_PROTO_TYPE, PACKET_MINLEN + in_len, (unsigned char *) pkt);
}


#ifdef WIN32
void winexit(void)
{
    myexit(0);
}
#endif

int parse_address_arg (char* in_str)
{
	uint8_t tmp;
	int retval;

	if (strlen(in_str) > 2 && in_str[0] == '0' &&
		tolower(in_str[1]) == 'x')
	{
		tmp = 2;
	} else if (strlen(in_str) == 2)
	{
		tmp = 0;
	} else
	{
		return -1;
	}

	retval = gethexfromchar (in_str[tmp]) * 16;
	retval += gethexfromchar (in_str[++tmp]);
	
	return retval;
}

uint8_t match_packet (uint8_t *in_packet)
{
	return 1;
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

	if (parse_args (argc, argv) < 0)
	{
		return -1;
	}

        //try to open the device
	if (rfmusb_Connect(&udhandle) != 0)
	{
		printf ("Can't find RfmUSB Device!\r\n");
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
			if (packetBuffer.buffer[4] == CMD_MOTION || packetBuffer.buffer[4] == CMD_PWRFAIL)
			{
				mc++;
				printf("**** MOTION **** (count = %10i)\n", mc);
				system ("su -c \"mocp -p\" sh");
			}
		}
		if (tmp > 0 && opt_dumpall)
		{
			
			printf("RX\t");
			for (i=0;i<tmp;i++)
			{
				printf("%02x ", packetBuffer.buffer[i]);
			}
			printf("\r\n");
		}

		//if an error occurs...
		if (tmp < 0)
		{
			fprintf (stderr, "USB error: %s\r\n", usb_strerror());

			exit (__LINE__ * -1);
		}
		
		if (opt_reset)
		{
			static uint8_t resetcount = 0, resetaddr = 0;

			if (opt_resetall)
				dst_addr = resetaddr;
				
			txbuf[0] = CMD_RESET;

			tx_packet (udhandle, dst_addr, CMD_RESET, 0, txbuf);
			printf("rst to %x\r\n", dst_addr);
			resetcount++;
			if (resetcount > 5)
			{
				if (!opt_resetall)
					return;
				resetaddr++;
				resetcount = 0;
			}
			if (resetcount > 1 && resetaddr == 0xff && opt_resetall)
				return;
		}
		
		if (opt_set_treshold)
		{
			txbuf[0] = opt_set_treshold;
			txbuf[1] = opt_set_treshold;
			txbuf[2] = opt_set_treshold;
			txbuf[3] = opt_set_treshold;

			printf("setting treshold value %x for address %2x\r\n", opt_set_treshold, dst_addr);

			tx_packet (udhandle, dst_addr, CMD_MOTION_TRESH_SET, 2, txbuf);
			return;
		}
		
		//this is done to prevent stressing the usb connection too much
		usleep (1000);
	}
}
