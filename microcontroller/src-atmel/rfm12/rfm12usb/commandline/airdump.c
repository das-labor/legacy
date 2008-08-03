#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <usb.h>
#include <unistd.h>
#include <signal.h>

#include "opendevice.h"

#include "../firmware/usbconfig.h"
#include "../firmware/requests.h"

#define BUF_IN 0
#define BUF_OUT 1

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

int main (int argc, char *argv[])
{
	uint_fast16_t i;
	int vid, pid, tmp;

	uint8_t buffer[2][64];
	uint16_t buflen[2] = { 0, 0 };
	uint_fast32_t packetcounter = 0;

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
#if 0	
	/* socket handling stuff */
	int sockfd, new_fd;		// listen on sock_fd, new connection on new_fd
	struct sockaddr_in my_addr;	// my address information
	struct sockaddr_in their_addr;	// connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes = 1;
#endif
	
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

#if 0
	/* networking setup */
	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror ("socket");
		exit (1);
	}

	if (setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int)) == -1)
	{
		perror ("setsockopt");
		exit (1);
	}

	my_addr.sin_family = AF_INET;		// host byte order
	my_addr.sin_port = htons (MYPORT);	// short, network byte order
	my_addr.sin_addr.s_addr = INADDR_ANY;	// automatically fill with my IP
	memset (my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

	if (bind (sockfd, (struct sockaddr *) &my_addr, sizeof my_addr) == -1)
	{
		perror ("bind");
		exit (1);
	}

	if (listen (sockfd, BACKLOG) == -1)
	{
		perror ("listen");
		exit (1);
	}

	sa.sa_handler = sigchld_handler;	// reap all dead processes
	sigemptyset (&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if (sigaction (SIGCHLD, &sa, NULL) == -1)
	{
		perror ("sigaction");
		exit (1);
	}
#endif


	while (42)
	{
		sleep(1000);
		memset (buffer[BUF_IN], 0x00, sizeof(buffer[BUF_IN]));
		buffer[BUF_IN][0] = 1;
		tmp = usb_control_msg (udhandle,
				USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
				CUSTOM_RQ_GET_DATA, 0, 0, buffer[BUF_IN], sizeof (buffer[BUF_IN]),
				5000);
		if (tmp > 0)
		{
			packetcounter++;
			buflen[BUF_IN] = tmp;
			printf ("%03i bytes received, packet #%010u --------\r\n", tmp, packetcounter);
			for (i=0;i<tmp;i++)
			{
				printf("%c", buffer[BUF_IN][i]);
//				if (i % 20 == 0) printf("\r\n");
			}
			printf("\r\n");
		} else if (tmp < 0)
		{
			fprintf (stderr, "USB error: %s\r\n", usb_strerror());
			return __LINE__ * -1;
		}
	}
}
