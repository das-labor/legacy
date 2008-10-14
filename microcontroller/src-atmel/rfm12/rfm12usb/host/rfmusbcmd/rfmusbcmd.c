#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <usb.h>
#include <unistd.h>
#include <signal.h>

//for simple usbOpenDevice
#include "../common/opendevice.h"

//common includes
#include "../../common/usb_id.h"
#include "../../common/requests.h"

#ifdef WIN32
#define	usleep(x) Sleep(x)
#endif

#define RFM12_MAX_PACKET_LENGTH 30
#define RADIO_TXBUFFER_HEADER_LEN 2
#define DEFAULT_USB_TIMEOUT 1000

//tx and rx raw packet buffer struct
typedef struct{
	unsigned char len;			                       //length byte - number of bytes in buffer
	unsigned char type;			                       //type field for airlab
	unsigned char buffer[RFM12_MAX_PACKET_LENGTH];     //generic buffer
} radio_packetbuffer;


//globals
usb_dev_handle *udhandle = NULL;
radio_packetbuffer packetBuffer;

//forward declarations
void sig_cleanup(int in_signum);
int radio_rx_dump(void);
int radio_rx(void);
int radio_tx(unsigned char len, unsigned char type, unsigned char *data);
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

        //clear buffer
        memset (&packetBuffer, 0x00, sizeof(packetBuffer));

        //request raw packet
        packetLen = usb_control_msg (udhandle,
        		USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
        		RFMUSB_RQ_RFM12_GET, 0, 0, (char *)&packetBuffer, sizeof(packetBuffer),
        		DEFAULT_USB_TIMEOUT);

        //if something has been received
        if (packetLen > 0)
        {
            //increment packet counter
        	packetCnt++;

        	//dump packet
        	printf ("--RX--  len: %02i, type: %02x, num: #%010u  --RX--\r\n", packetBuffer.len, packetBuffer.type, packetCnt);
        	for (i = 0;(i < packetBuffer.len) && (i < RFM12_MAX_PACKET_LENGTH); i++)
        	{
        		printf("%c", packetBuffer.buffer[i]);
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


//receive a packet into the standard packet buffer
int radio_rx(void)
{
    //clear buffer
    memset (&packetBuffer, 0x00, sizeof(packetBuffer));

    //request raw packet and return length
    return usb_control_msg (udhandle,
            USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
            RFMUSB_RQ_RFM12_GET, 0, 0, (char *)&packetBuffer, sizeof(packetBuffer),
            DEFAULT_USB_TIMEOUT);
}


//transmit a packet
int radio_tx(unsigned char len, unsigned char type, unsigned char *data)
{
    radio_packetbuffer buf;
    int packetLen;

    //trim packet length
    packetLen = RADIO_TXBUFFER_HEADER_LEN + ((len > RFM12_MAX_PACKET_LENGTH)?RFM12_MAX_PACKET_LENGTH:len);

    //setup buffer
    buf.len = len;
    buf.type = type;

    //copy data
    memcpy(buf.buffer, data, len);

    //request to send packet and return result
    return usb_control_msg (udhandle,
        USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
        RFMUSB_RQ_RFM12_PUT, 0, 0, (char *)&buf, packetLen,
        DEFAULT_USB_TIMEOUT);
}


//ask the user for the packet to transmit
void UI_send_raw()
{
    unsigned char length, type, buf[RFM12_MAX_PACKET_LENGTH];
    int i, tmp;

    printf("Packet length? ");
    scanf("%u", &tmp);
    fflush(stdin);
    length = tmp & 0xff;

    printf("Packet type? ");
    scanf("%u", &tmp);
    fflush(stdin);
    type =  tmp & 0xff;

    if(length > RFM12_MAX_PACKET_LENGTH)
    {
        length = RFM12_MAX_PACKET_LENGTH;
    }

    for(i = 0; i < length; i++)
    {
        printf("byte: ");
        scanf("%hd", &tmp);
        fflush(stdin);
        buf[i] =  tmp & 0xff;
    }

    //directly tx packet
    radio_tx(length, type, buf);
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
    int choice, run;

    run = 23 + 42 + 31337;
    while(run)
	{
        UI_menu_show();

        scanf("%i", &choice);
        fflush(stdin);

        switch(choice)
        {
            case 0:
                run = 0;
                break;

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

    printf("Exiting.\n");
}


int main(int argc, char *argv[])
{
	int vid, pid;

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

	/* signals */
#ifndef WIN32
	signal (SIGKILL, sig_cleanup);
	signal (SIGINT, sig_cleanup);
	signal (SIGHUP, sig_cleanup);
#endif

	usb_init();

	/* usb setup */
	vid = rawVid[1] * 256 + rawVid[0];
	pid = rawPid[1] * 256 + rawPid[0];

    //try to open the device
	if (usbOpenDevice (&udhandle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
	{
		printf ("Can't find USB Device w/ uid %04X, pid %04X\r\n", vid, pid);
		sig_cleanup(__LINE__ * -1);
		return __LINE__ * -1;
	}

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
