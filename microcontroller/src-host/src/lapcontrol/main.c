#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include "can.h"
#include "cann.h"
#include "debug.h"

// Atmel ; LAP includes
// #include "config.h"


#ifndef max
 #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif


static char *progname;

static char *optstring = "hv::s:p:";
struct option longopts[] =
{
  { "help", no_argument, NULL, 'h' },
  { "verbose", optional_argument, NULL, 'v' },
  { "server", required_argument, NULL, 's' },
  { "port", required_argument, NULL, 'p' },
  { NULL, 0, NULL, 0 }
};

void help()
{
	printf("\nUsage: %s [OPTIONS]\n", progname);
	printf("\
   -h, --help              display this help and exit\n\
   -v, --verbose           be more verbose and display a CAN packet dump\n\
   -s, --server HOST       use specified server (default: localhost)\n\
   -p, --port PORT         use specified TCP/IP port (default: 2342)\n\n" );
}

void print_packets(){
//	while(1) {
//		can_message *msg = can_get_nb();
//		if(msg)printf("\nmsg from %X:%X\tto %X:%X\t dlc %d\tdata %X\n",msg->addr_src, msg->port_src, msg->addr_dest, msg->port_dest, msg->dlc, msg->data[0]);
//		sleep(1);
//	}
}
int main(int argc, char *argv[])
{
	int tcpport  = 2342;         // TCP Port
	char *server = "localhost"; 
	int optc;

	progname = argv[0];

	while ((optc=getopt_long(argc, argv, optstring, longopts, (int *)0))
		!= EOF) {
		switch (optc) {
			case 'v':
				if (optarg)
					debug_level = atoi(optarg);
				else 
					debug_level = 3;
				break;
			case 's':
				server = optarg;
				break;
			case 'p':
				tcpport = atoi(optarg);
				break;
			case 'h':
				help();
				exit(0);
			default:
				help();
				exit(1);
		}
	} // while

	can_init(server, tcpport);
	debug(1, "CAN communication established" );


	lap_ping( 0xff );

//	can_message *msg = can_buffer_get();
//	can_transmit(msg);
	cann_close(0);

	return 0;
}
