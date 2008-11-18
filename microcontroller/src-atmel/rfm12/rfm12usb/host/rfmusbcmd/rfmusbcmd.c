/* Utility functions for receiving and transmitting data from and to the target device.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Hans-Gert Dahmen <sexyludernatascha@gmail.com>, Soeren Heisrath <forename@surname.org>
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <usb.h>
#include <unistd.h>
#include <signal.h>

//for simple usbOpenDevice
#include "../common/opendevice.h"

//use my usb driver
#include "../CDriver/RfmUsb.h"

//common includes
#include "../../common/usb_id.h"
#include "../../common/requests.h"
#include "../../common/rfm12_buffer_size.h"

#ifdef WIN32
#define	usleep(x) Sleep(x)
#else
#include <ulimit.h>
#endif


//globals
usb_dev_handle *udhandle = NULL;
rfmusb_packetbuffer packetBuffer;


//forward declarations
void sig_cleanup(int in_signum);
int radio_rx_dump(void);
void UI_main_menu(void);
void UI_menu_show(void);
void UI_send_raw(void);


//dump packets
int radio_rx_dump(void)
{
    uint_fast16_t i;
	uint_fast32_t packetCnt = 0, packetLen;

    do
	{
        //rate limit (don't be faster than 10us for a 16MHz device
        usleep(10);

        //request raw packet
        packetLen = rfmusb_RxPacket (udhandle, &packetBuffer);

        //if something has been received
        if (packetLen > 0)
        {
            //increment packet counter
        	packetCnt++;

        	//dump packet
        	printf ("--RX--  len: %02i, type: %02x, num: #%010u  --RX--\r\n", packetBuffer.len, packetBuffer.type, packetCnt);
        	for (i = 0;(i <= packetBuffer.len) && (i < RFM12_BUFFER_SIZE); i++)
        	{
        		printf("%.2x ", packetBuffer.buffer[i]);
        	}
        	printf("\r\n");
        }
        else if (packetLen < 0)
        {
        	fprintf (stderr, "USB error: %s\r\n", usb_strerror());
        	return __LINE__ * -1;
        }
    }
    //FIXME: implement nicer way to quit the packetdump than ctrl+c
    while(42);
}


//ask the user for the packet to transmit
void UI_send_raw()
{
    unsigned char length, type, buf[RFM12_BUFFER_SIZE];
    int i, tmp;

    printf("Packet length? ");
    scanf("%u", &tmp);
    fflush(stdin);
    length = tmp & 0xff;

    printf("Packet type? ");
    scanf("%u", &tmp);
    fflush(stdin);
    type =  tmp & 0xff;

    if(length > RFM12_BUFFER_SIZE)
    {
        length = RFM12_BUFFER_SIZE;
    }

    for(i = 0; i < length; i++)
    {
        printf("byte: ");
        scanf("%hd", &tmp);
        fflush(stdin);
        buf[i] =  tmp & 0xff;
    }

    //directly tx packet
	rfmusb_TxPacket (udhandle, type, length, buf);
}


//show the menu
void UI_menu_show(void)
{
     printf("Menu:\n");
     printf("1\tairdump\n");
     printf("2\tsend raw packet\n");
     printf("0\texit\n");
     printf("\n> ");
}


void UI_main_menu(void)
{
    int choice;

    do
	{
        UI_menu_show();

        scanf("%i", &choice);
        fflush(stdin);

        switch(choice)
        {
            case 1:
                radio_rx_dump();
                break;

            case 2:
                UI_send_raw();
                break;

            default:
                break;
        }
    }
	while(choice);

    printf("Exiting.\n");
}


int main(int argc, char *argv[])
{
	/* signals */
#ifndef WIN32
	signal (SIGKILL, sig_cleanup);
	signal (SIGINT, sig_cleanup);
	signal (SIGHUP, sig_cleanup);
#endif

    //try to open the device
	if (rfmusb_Connect(&udhandle) != 0)
	{
		printf ("Can't find RfmUSB Device\r\n");
		sig_cleanup(__LINE__ * -1);
		return __LINE__ * -1;
	}
#ifndef WIN32
	setuid(19928);
#endif

    //kick in main menu loop
    UI_main_menu();
}


void sig_cleanup (int in_signum)
{
#ifndef WIN32
	printf("DOES NOT COMPUTE! (Signal %i)\r\n", in_signum);
#endif
	if (udhandle != NULL) usb_close ( udhandle );
#ifdef WIN32
	system("pause");
	exit (1);
#endif
}
