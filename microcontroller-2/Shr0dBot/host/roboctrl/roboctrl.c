#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <usb.h>
#include <unistd.h>
#include <signal.h>

#include "opendevice.h"

#include "../../../rfm12usb/firmware/usbconfig.h"
#include "../../../rfm12usb/common/requests.h"

#define BUF_IN 0
#define BUF_OUT 1

#ifdef WIN32
#define	usleep(x) sleep(x)
#endif

#define USB_TXPACKET 0x42
#define RFM12_MAX_PACKET_LENGTH 30

#define RADIO_TYPE_ROBO 0x42
#define RADIO_CMD_STOP 0x01
#define RADIO_CMD_GOTO 0x02

usb_dev_handle *udhandle = NULL;

void sig_cleanup(int in_signum);

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

int radio_rx_dump(void)
{
    int tmp;
    uint_fast16_t i;
	uint8_t buffer[2][64];
	uint16_t buflen[2] = { 0, 0 };
	uint_fast32_t packetcounter = 0;

    do
	{
        usleep(10);

        memset (buffer[BUF_IN], 0x00, sizeof(buffer[BUF_IN]));
        buffer[BUF_IN][0] = 1;

        tmp = usb_control_msg (udhandle,
        		USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
        		RFMUSB_RQ_RFM12_GET, 0, 0, buffer[BUF_IN], sizeof (buffer[BUF_IN]),
        		5000);

        if (tmp > 0)
        {
        	packetcounter++;
        	buflen[BUF_IN] = tmp;
        	printf ("%03i bytes received, packet #%010u --------\r\n", tmp, packetcounter);
        	for (i=0;i<tmp;i++)
        	{
        		printf("%c", buffer[BUF_IN][i]);
        	}
        	printf("\r\n");
        } else if (tmp < 0)
        {
        	fprintf (stderr, "USB error: %s\r\n", usb_strerror());
        	return __LINE__ * -1;
        }
    }
    while(42);
}

typedef struct{
	unsigned char len;			//length byte - number of bytes in buffer
	unsigned char type;			//type field for airlab
	unsigned char buffer[RFM12_MAX_PACKET_LENGTH]; //receive buffer
} radio_tx_buffer;
#define RADIO_TXBUFFER_HEADER_LEN 2

void radio_tx(unsigned char len, unsigned char type, unsigned char *data)
{
    radio_tx_buffer buf;
    int packetlength;

    packetlength = RADIO_TXBUFFER_HEADER_LEN + ((len > RFM12_MAX_PACKET_LENGTH)?RFM12_MAX_PACKET_LENGTH:len);

    buf.len = len;
    buf.type = type;

    memcpy(buf.buffer, data, len);

    usb_control_msg (udhandle,
        USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
        RFMUSB_RQ_RFM12_PUT, USB_TXPACKET, 0, (char *)&buf, packetlength,
        5000);
}

void send_raw()
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

    //printf("%u %u", length, type);

    for(i = 0; i < length; i++)
    {
        printf("byte: ");
        scanf("%hd", &tmp);
        fflush(stdin);
        buf[i] =  tmp & 0xff;
    }

    radio_tx(length, type, buf);
}

void movement_menu_show(void)
{
     printf("Menu:\n");
     printf("0\texit\n");
     printf("1\tstop\n");
     printf("2\tjoy_fwd\n");
	 printf("3\tJoy_back\n");
     printf("\n> ");
}

#define RADIO_TYPE_JOY 0x69
void movement_menu(void)
{
    int choice, run;
    char buf;

    run = 23 + 42 + 31337;
    while(run)
	{
        movement_menu_show();

        scanf("%i", &choice);
        fflush(stdin);

        switch(choice)
        {
            case 0:
                run = 0;
                break;

            case 1:
                buf = 0;
                radio_tx(1, RADIO_TYPE_JOY, &buf);
                break;

            case 2:
                buf = 0x01;
                radio_tx(1, RADIO_TYPE_JOY, &buf);
                break;
                
            case 3:
                buf = 0x02;
                radio_tx(1, RADIO_TYPE_JOY, &buf);
                break;                

            default:
                break;
        }
    }
}

void menu_show(void)
{
     printf("Menu:\n");
     printf("0\texit\n");
     printf("1\tairdump\n");
     printf("2\tsend raw\n");
	 printf("3\tmovement\n");
     printf("\n> ");
}

void main_menu(void)
{
    int choice, run;

    run = 23 + 42 + 31337;
    while(run)
	{
        menu_show();

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
                send_raw();
                break;
                
            case 3:
                movement_menu();
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

	if (usbOpenDevice (&udhandle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
	{
		printf ("Can't find USB Device w/ uid %04X, pid %04X\r\n", vid, pid);
		sig_cleanup(__LINE__ * -1);
		return __LINE__ * -1;
	}

    main_menu();
}
