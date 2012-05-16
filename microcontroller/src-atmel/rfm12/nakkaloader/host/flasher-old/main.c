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
//#include "../../../rfm12usb/host/CDriver/RfmUsb.h"

//project specific
#include "../../common/nl_protocol.h"
#include "config.h"

//include the crc function
#include "crc.h"

//include the flasher funcs
#include "nl_flash.h"


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


#ifdef WIN32
void winexit(void)
{
    myexit(0);
}
#endif


int main (int argc, char* argv[])
{
        //usb stuff
	int vid, pid, tmp;

	//various variables
        uint8_t tmpchar = 0x00;
	uint_fast8_t ptype;
	uint_fast32_t packetcounter = 0;
	nf_config_t *myconfig;
	nl_config slave_cfg;
	int istate = 0;

        //config stuff
	myconfig = malloc(sizeof(nf_config_t));
	myconfig->verbosity = 0;


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




	if(nf_parse_args (argc, argv, myconfig) == 0)
	{
		usage();
		//exit(0);
	}

	free(myconfig->fname);	//see parse args

	myconfig->fname = malloc (255);
	strncpy (myconfig->fname, argv[1], strlen(argv[1])+1);



	myconfig->addr = 0xff;

	printf ("Waiting for slave %x\r\n\r\n", myconfig->addr);

	nl_packet_match(NLPROTO_SLAVE_CONFIG, NULL, EXP_ADD);
	nl_packet_match(NLPROTO_MASTER_EHLO, NULL, EXP_ADD);


	while (23)
	{
		//try to fetch a packet from the air
		tmp = rfmusb_RxPacket (udhandle, &packetBuffer);

		if(tmp > 0)
		{
            printf("RX 0x%.2x\n", packetBuffer.buffer[0]);
		}

		//if an error occurs...
		if (tmp < 0)
		{
			fprintf (stderr, "USB error: %s\r\n", usb_strerror());

			exit (__LINE__ * -1);
		}

		//verify that this is a valid packet from the right slave with the right type
		else if ((packetBuffer.buffer[1] == myconfig->addr) && (packetBuffer.type == NL_PACKETTYPE) && ( ptype = nl_packet_match(tmp, &packetBuffer, EXP_MATCH)))
		{
			//see what to do for given packet type
			switch (ptype)
			{
				//slave has sent it's configuration
				case NLPROTO_SLAVE_CONFIG:
                    if((istate == 0) || (istate == 1))
                    {
                        istate = 1;
                        memcpy((void *)&slave_cfg, (void *)(packetBuffer.buffer + 2), sizeof(nl_config));
			/*slave_cfg.pagesize = be16toh(packetBuffer.buffer[2]);
			slave_cfg.pagesize = 64;
			slave_cfg.rxbufsize = 20; /* Fixed val for now... */

                        //printf("got slave config!\nPagesize: %i\nAir Packet Size: %i\nVersion: %i\n", ((nl_config *)(packetBuffer.buffer + 2))->pagesize, ((nl_config *)(packetBuffer.buffer + 2))->rxbufsize, ((nl_config *)(packetBuffer.buffer + 2))->version);
                        printf("Got slave config!\nPagesize: %i\nAir Packet Size: %i\nVersion: %i\nAddress: %i\n\n", slave_cfg.pagesize, slave_cfg.rxbufsize, slave_cfg.version, packetBuffer.buffer[1]);

                        //reply is not active in newest firmware, goto flash immediately
                        //nl_tx_packet(udhandle, NLPROTO_MASTER_EHLO, myconfig->addr, 0, NULL);
						//printf("reply sent\n");
						
						nl_flash(udhandle, myconfig->fname, myconfig->addr , slave_cfg.pagesize, 64, /*slave_cfg.rxbufsize*/ 20);
                    }
				break;

				//slave has ACKed EHLO, ready to accept pagefills
				case NLPROTO_MASTER_EHLO:
                    if(istate == 1)
                    {
                        printf("Slave is ready to be flashed now.\n");

                        //flash
                        nl_flash(udhandle, myconfig->fname, myconfig->addr , slave_cfg.pagesize, 64, /*slave_cfg.rxbufsize*/ 30);

                        //ready again
                        istate = 2;
                    }
				break;
			}
		}

		//flashing done
		if(istate == 2)
		{
		    printf("Done!\n");

            break;
		}

		//this is done to prevent stressing the usb connection too much
		usleep (1000);
	}
}
