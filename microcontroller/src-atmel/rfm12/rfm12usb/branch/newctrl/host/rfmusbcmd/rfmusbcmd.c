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
#include <time.h>

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
	uint_fast32_t packetCnt = 0;
	int packetLen;
	time_t cur_time;
	char *timeStr;

	printf("dumping packets:\n");

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

		//get time
		cur_time = time(NULL);
		timeStr = ctime(&cur_time);

        	//dump packet
        	printf ("--RX--  len: %02i, type: %02x, num: #%010u  --RX--\r\n", packetBuffer.len, packetBuffer.type, packetCnt);
		timeStr[strlen(timeStr)-1] = 0;
		printf("%s:\t", timeStr);
        	for (i = 0;(i < packetBuffer.len) && (i < RFM12_BUFFER_SIZE); i++)
        	{
        		printf("%.2x ", packetBuffer.buffer[i]); //hex
        		//printf("%c", packetBuffer.buffer[i]); //char
        	}
        	printf("\r\n----\r\n");
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
        scanf("%hd", (short *)&tmp);
        fflush(stdin);
        buf[i] =  tmp & 0xff;
    }

    //directly tx packet
	rfmusb_TxPacket (udhandle, type, length, buf);
}


void UI_config()
{
	const unsigned int num_cmd = rfmusb_rfm12_get_cmd_count();
	char buf[256];
    int i, tmp;
    unsigned char cmd;
    unsigned short val;
    
    //print cmd menu
    printf("     setting    | current value\n");
    printf("----------------+--------------\n");
    for(i = 0; i < num_cmd; i++)
    {
		rfmusb_rfm12_cmd_to_string(i, buf);
		printf("%i. %12s | ", i, buf);
		rfmusb_rfm12_get_parameter_string(i, buf);
		printf("%12s\n", buf);
	}

    printf("\nsetting num? ");
    scanf("%u", &tmp);
    fflush(stdin);
    cmd = tmp & 0xff;

    printf("Value (hex)? ");
    scanf("%04x", &tmp);
    fflush(stdin);
    val =  tmp & 0xffff;

    if(cmd > RFM12_BUFFER_SIZE)
    {
        printf("command not understoord\n");
        return;
    }

	rfmusb_rfm12_config(udhandle, cmd, val);
	
	printf("new setting:\n");
	rfmusb_rfm12_cmd_to_string(cmd, buf);
	printf("%s : ", buf);
	rfmusb_rfm12_get_parameter_string(cmd, buf);
	printf("%12s\n", buf);
}

#define JOY_UP 0x01
#define JOY_DOWN 0x02
#define JOY_LEFT 0x04
#define JOY_RIGHT 0x08
#define JOY_FIRE1 0x10
#define JOY_FIRE2 0x20

//be a joystick emulator!
void UI_joystick()
{
    unsigned char length, type, joy;
    char c;

    length = 1;
    type =  0x69;

    printf("Use 'wsad' to set the direction bits, q+e for fire1+2, any other key to clear and x to quit.\n");

/*    while((c = getch()) != 'x')
    {
        switch(c)
        {
            case 'w':
                joy = JOY_UP;
                break;

            case 's':
                joy = JOY_DOWN;
                break;

            case 'a':
                joy = JOY_LEFT;
                break;

            case 'd':
                joy = JOY_RIGHT;
                break;

            case 'q':
                joy = JOY_FIRE1;
                break;

            case 'e':
                joy = JOY_FIRE2;
                break;

            default:
                joy = 0;
                break;
        }

        printf(" %.2x ", joy);

        //tx packet
        rfmusb_TxPacket (udhandle, type, length, &joy);
    }*/
}


//show the menu
void UI_menu_show(void)
{
     printf("Menu:\n");
     printf("1\tairdump\n");
     printf("2\tsend raw packet\n");
     printf("3\tconfigure\n");
//     printf("4\tjoystick mode\n");
     printf("0\texit\n");
     printf("\n> ");
}


void UI_main_menu(void)
{
    char choice;

    do
	{
        UI_menu_show();

#ifdef WIN32
        choice = getch();
#else
		scanf("%c", &choice);
    	fflush(stdin);
#endif

        switch(choice)
        {
            case '1':
                radio_rx_dump();
                break;

            case '2':
                UI_send_raw();
                break;

			case '3':
                UI_config();
                break;
            case '4':
                UI_joystick();
                break;

            default:
                break;
        }
    }
	while(choice != '0');

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

    //////////HACKHACK FOR MULTIPLE DEVICES
	int vid, pid;
	unsigned tmp = 0;

	const unsigned char rawVid[2] =
	{
		USB_CFG_VENDOR_ID
	},
	rawPid[2] =
	{
		USB_CFG_DEVICE_ID
	};

	vid = rawVid[1] * 256 + rawVid[0];
	pid = rawPid[1] * 256 + rawPid[0];

	usb_init();

    int devcnt = usbCountDevices(vid, pid);
    printf("Found %i devices..\n", devcnt);

    if(devcnt == 0)
    {
        printf ("Can't find RfmUSB Device\r\n");
		sig_cleanup(__LINE__ * -1);
		return __LINE__ * -1;
    }

    struct usb_device **devices = malloc(sizeof(void *) * devcnt);
    usbListDevices(devices, vid, pid);

    if(devcnt > 1)
    {
        printf("Which device (num)? ");
        scanf("%u", &tmp);
        fflush(stdin);
    }

    udhandle = usb_open(devices[tmp]);

    ///////////////////HACK END


    //try to open the device
	/*if (rfmusb_Connect(&udhandle) != 0)
	{
		printf ("Can't find RfmUSB Device\r\n");
		sig_cleanup(__LINE__ * -1);
		return __LINE__ * -1;
	}*/
#ifndef WIN32
	setuid(19928);
#endif

    //kick in main menu loop
    UI_main_menu();
}


void sig_cleanup (int in_signum)
{
	printf("DOES NOT COMPUTE! (Signal %i)\r\n", in_signum);	
	if (udhandle != NULL) usb_close ( udhandle );
	
#ifdef WIN32
	system("pause");
#endif

	exit (1);
}
