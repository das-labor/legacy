#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include <iostream>
#include <string>

#include "inet.h"

// Atmel ; LAP includes
#include "config.h"

#include "uart-host.h"
#include "can.h"
#include "rs232can.h"

#ifndef max
 #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

static InetConnections *conns;
static char *progname;
static int verbose = 0;

static char *optstring = "hdv::s:p:l";
struct option longopts[] =
{
  { "help", no_argument, NULL, 'h' },
  { "daemon", no_argument, NULL, 'd' },
  { "verbose", optional_argument, NULL, 'v' },
  { "serial", required_argument, NULL, 's' },
  { "port", required_argument, NULL, 'p' },
  { "loopback", no_argument, NULL, 'l' },
  { NULL, 0, NULL, 0 }
};

void debug(int level, char *msg)
{
	if (level >= verbose) {
		fprintf(stderr, msg);
	}
}

void help()
{
	printf("\nUsage: %s [OPTIONS]\n", progname);
	printf("\
   -h, --help              display this help and exit\n\
   -v, --verbose           be more verbose and display a CAN packet dump\n\
   -d, --daemon            become daemon\n\
   -s, --serial PORT       use specified serial port (default: /dev/ttyS0)\n\
   -p, --port PORT         use specified TCP/IP port (default: 2342)\n\
   -l, --loopback          set CAN gateway to loopback mode\n\n" );
}

void print_packets(){
	while(1) {
		can_message *msg = can_get_nb();
		if(msg)printf("\nmsg from %X:%X\tto %X:%X\t dlc %d\tdata %X\n",msg->addr_src, msg->port_src, msg->addr_dest, msg->port_dest, msg->dlc, msg->data[0]);
		sleep(1);
	}
}


void process_rs232_msg(rs232can_msg *msg)
{
	conns->sendPacket(msg);
}

void process_inet_msg(LapClient *client, rs232can_msg *msg)
{
	conns->sendPacket(msg);
}

int main(int argc, char *argv[])
{
	int d = 0;                   // daemon
	int tcpport  = 2342;         // TCP Port
	char *serial = "/dev/ttyS0"; // serial port
	bool loopback = false;       // activate loopback
	int optc;

	progname = argv[0];

	while ((optc=getopt_long(argc, argv, optstring, longopts, (int *)0))
		!= EOF) {
		switch (optc) {
			case 'v':
				if (optarg)
					verbose = atoi(optarg);
				else 
					verbose = 3;
				break;
			case 'd':
				d = 1;
				break;
			case 's':
				serial = optarg;
				break;
			case 'p':
				tcpport = atoi(optarg);
				break;
			case 'l':
				loopback = true;
				break;
			case 'h':
				help();
				exit(0);
			default:
				help();
				exit(1);
		}
	} // while

	
	// setup serial communication
	uart_init(serial);
	can_init();
	rs232can_reset();
	debug(1, "CAN communication established...\n" );

	// setup network socket
	conns = new InetConnections(tcpport);
	conns->addWatchFd(uart_fd());
	debug(1, "Listenig for network connections...\n" );

	// eventloop
	/*
	for(;;) {
		int num;
		int highfd;
		fd_set rset;
		connection_t conn;

		FD_ZERO(&rset);

		highfd = uart_fdset(&rset);
		highfd = max(highfd, net_fdset(&rset));

		num = select(highfd+1, &rset, (fd_set *)NULL, (fd_set *)NULL, NULL);

		debug(9, "Select returned\n");

		if (num == 0) continue;
		if (num < 0) {
			perror("select failed: ");
			exit(2);
		}

		while( conn = net_after_select(&rset) ) {
		}
	}
	*/

	for(;;) {
		conns->waitForActivity();

		// data on uart?
		if (conns->fdHasActivity(uart_fd())) {
			rs232can_msg *msg = rs232can_get_nb();
			if (msg) {
				// complete packet arrived
				process_rs232_msg(msg);
			}
		}

		// handle inet connections
		for(InetConnections::ClientList::iterator i = conns->clients.begin(); i != conns->clients.end(); i++) {
			if (!conns->hasActivity(*i)) continue;

			rs232can_msg *msg = (*i)->getPacket();
			if (msg) {
				// complete packet arrived
				process_inet_msg(*i, msg);
			}
		}
	}

	/*
	if(!strcmp(argv[1], "p")){
		print_packets();
	}else if(!strcmp(argv[1], "g")){
		int sa,da,sp,dp,d[8];
		int num = sscanf(argv[2], "%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x",&sa,&sp,&da,&dp,&d[0],&d[1],&d[2],&d[3],&d[4],&d[5],&d[6],&d[7]) - 4;
		if(num >= 0){
			can_message *msg = can_buffer_get();
			msg->dlc = num;
			msg->addr_src = sa;
			msg->addr_dest = da;
			msg->port_src = sp;
			msg->port_dest = dp;
			unsigned int x;
			for(x=0;x<8;x++){
				msg->data[x] = (unsigned char) d[x];
			}
			can_transmit(msg);
		}else{
			printf("error: not enough bytes provided");
		}
	
	}
	
	*/
	

	return 0;
}
