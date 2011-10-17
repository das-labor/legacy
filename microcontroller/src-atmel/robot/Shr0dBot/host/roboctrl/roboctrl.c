#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>ch
#include <usb.h>
#include <unistd.h>
#include <signal.h>

#include "opendevice.h"

#include "common/usb_id.h"
#include "rfmusb/CDriver/RfmUsb.h"

#define BUF_IN 0
#define BUF_OUT 1

#ifdef WIN32
#define	usleep(x) sleep(x)
#endif

#define USB_TXPACKET 0x42
#define RFM12_BUFFER_SIZE 30

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

#define RADIO_TXBUFFER_HEADER_LEN 2

//dump packets
int radio_rx_dump(void)
{
	uint_fast16_t i;
	uint_fast32_t packetCnt = 0;
	int packetLen;
	rfmusb_packetbuffer packetBuffer;

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

        	//dump packet
        	printf ("--RX--  len: %02i, type: %02x, num: #%010u  --RX--\r\n", packetBuffer.len, packetBuffer.type, packetCnt);
        	for (i = 0;(i < packetBuffer.len) && (i < RFM12_BUFFER_SIZE); i++)
        	{
        		printf("%.2x ", packetBuffer.buffer[i]); //hex
        		//printf("%c", packetBuffer.buffer[i]); //char
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


//temporary solution
typedef struct
{
	int16_t		acc_curX;			//current x acceleration
	int16_t		acc_curY;			//current y acceleration
	int16_t		acc_curZ;			//current z acceleration
	uint16_t	battVolts;		//battery voltage
	uint16_t	currentDrain;	//motor current usage
	int16_t		curSpeed;		//the robots current speed	
	int32_t		encAPos;		//the encoder A count
} radio_sensorData;


//dump sensor data
int dump_sensor(void)
{
	uint_fast16_t i;
	uint_fast32_t packetCnt = 0;
	int packetLen;
	rfmusb_packetbuffer packetBuffer;
	radio_sensorData sd;

	printf("dumping sensordata:\n");

    do
	{
        //rate limit (don't be faster than 10us for a 16MHz device
        usleep(10);

        //request raw packet
        packetLen = rfmusb_RxPacket (udhandle, &packetBuffer);

        //if something has been received
        if (packetLen > 0 && packetBuffer.type == 0x42)
        {
		//increment packet counter
        	packetCnt++;

		memcpy(&sd, packetBuffer.buffer, packetBuffer.len);

        	//dump packet
        	printf ("#:%u\taccX:%h\taccY:%h\taccZ:%h\tbat:%uh\tpwr:%uh\r\n", packetCnt, sd.acc_curX, sd.acc_curY, sd.acc_curZ, sd.battVolts, sd.currentDrain);
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
	printf("3\tjoystick\n");
	printf("4\tsee robot data\n");
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

void connect()
{
	int vid, pid;
	int devcnt;
	struct usb_device **devices;

	const unsigned char rawVid[2] =
	{
		USB_CFG_VENDOR_ID
	},
	rawPid[2] =
	{
		USB_CFG_DEVICE_ID
	};

	/* usb setup */
	vid = rawVid[1] * 256 + rawVid[0];
	pid = rawPid[1] * 256 + rawPid[0];

	devcnt = usbCountDevices(vid, pid);
	printf("Found %i devices..\n", devcnt);

	if(devcnt == 0)
	{
		printf ("Can't find USB Device w/ uid %04X, pid %04X\r\n", vid, pid);
		sig_cleanup(__LINE__ * -1); //exit
	}

	devices = malloc(sizeof(void *) * devcnt);
	usbListDevices(devices, vid, pid);

	if(devcnt > 1)
	{
		printf("Which device (num)? ");
		scanf("%u", &devcnt);
		fflush(stdin);
	}

	udhandle = usb_open(devices[devcnt]);
}

int main(int argc, char *argv[])
{
	/* signals */
	#ifndef WIN32
		signal (SIGKILL, sig_cleanup);
		signal (SIGINT, sig_cleanup);
		signal (SIGHUP, sig_cleanup);
	#endif

	usb_init();

	connect();

    	main_menu();
}
