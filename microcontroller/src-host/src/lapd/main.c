#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>


// Atmel ; LAP includes

#include "config.h"

#include "uart-host.h"
#include "can.h"
#include "rs232can.h"

static char *progname;

static char *optstring = "hdv::s:p:";
struct option longopts[] =
{
  { "help", no_argument, NULL, 'h' },
  { "daemon", no_argument, NULL, 'd' },
  { "verbose", optional_argument, NULL, 'v' },
  { "serial", required_argument, NULL, 's' },
  { "port", required_argument, NULL, 'p' },
  { NULL, 0, NULL, 0 }
};

void help()
{
	printf("\nUsage: %s [OPTIONS]\n", progname);
	printf("\
   -h, --help              display this help and exit\n\
   -v, --verbose           be more verbose and display a CAN packet dump\n\
   -d, --daemon            become daemon\n\
   -s, --serial PORT       use specified serial port (default: /dev/ttyS0)\n\
   -p, --port PORT         use specified TCP/IP port (default: 2342)\n" );
}

void print_packets(){
	while(1) {
		can_message *msg = can_get_nb();
		if(msg)printf("\nmsg from %X:%X\tto %X:%X\t dlc %d\tdata %X\n",msg->addr_src, msg->port_src, msg->addr_dest, msg->port_dest, msg->dlc, msg->data[0]);
		sleep(1);
	}
}


int main(int argc, char *argv[])
{
	int v = 0;                   // verbose
	int d = 0;                   // daemon
	int tcpport  = 2342;         // TCP Port
	char *serial = "/dev/ttyS0"; // serial port
	int optc;

	progname = argv[0];

	while ((optc=getopt_long(argc, argv, optstring, longopts, (int *)0))
		!= EOF) {
		switch (optc) {
			case 'v':
				if (optarg)
					v = atoi(optarg);
				else 
					v = 1;
				break;
			case 'd':
				d = 1;
				break;
			case 's':
				break;
			case 'p':
				break;
			case 'h':
				help();
				exit(0);
			default:
				help();
				exit(1);
		}
	} // while

	

	uart_init();
	can_init();

	rs232can_reset();

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
	
	
	

	return 0;
}
