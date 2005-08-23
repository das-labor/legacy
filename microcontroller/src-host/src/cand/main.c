#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include "cann.h"
#include "debug.h"

// Atmel ; LAP includes
#include "config.h"

#include "uart-host.h"
#include "can-uart.h"

#ifndef max
 #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif


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
   -p, --port PORT         use specified TCP/IP port (default: 2342)\n\n" );
}

void process_uart_msg( rs232can_msg *msg )
{
}

void process_client_msg( cann_conn_t *client, rs232can_msg *msg )
{
	cann_conn_t *ac;
	int x;

	debug(3, "Processing message from network..." );

	switch(msg->cmd) {
		case RS232CAN_SETFILTER:
			/* XXX */
			break;
		case RS232CAN_SETMODE:
			/* XXX */
			break;
		case RS232CAN_PKT:
		default:
			// to UART
			canu_transmit(msg);

			// foreach client
			ac = cann_conns_head;
			while(ac) {
//XXX				if ( cann_match_filter(ac, msg) ) 
				if ( ac != client )
					cann_transmit(ac, msg);

				ac = ac->next;
			}
	}
}

void new_client( cann_conn_t *client )
{
	// XXX
}

void event_loop()
{
	for(;;) {
		int num;
		int highfd;
		fd_set rset;
		cann_conn_t *client;

		FD_ZERO(&rset);

		highfd = uart_fd();
		FD_SET(uart_fd(), &rset);
		highfd = max(highfd, cann_fdset(&rset));

		num = select(highfd+1, &rset, (fd_set *)NULL, (fd_set *)NULL, NULL);
		debug_assert( num >= 0, "select faild" );
		debug( 10, "Select returned %d", num);

		// check activity on uart_fd
		if (FD_ISSET(uart_fd(), &rset)) {
			debug( 10, "Activity on uart_fd" );
			rs232can_msg *msg = canu_get_nb();
			if (msg) {
				process_uart_msg(msg);
				free(msg);
			}
		}
		
		// new connections
		if( client = cann_accept(&rset) ) {
			debug( 2, "New connection (fd=%d)", client->fd );
		}

		// check client activity 
		while( client = cann_activity(&rset) ) {
			rs232can_msg *msg = cann_get_nb(client);
			if(msg) {
				process_client_msg(client, msg);
				// free(msg);
			}
		}

		// close errorous connections
		cann_close_errors();
	}
}


int main(int argc, char *argv[])
{
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
					debug_level = atoi(optarg);
				else 
					debug_level = 3;
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
			case 'h':
				help();
				exit(0);
			default:
				help();
				exit(1);
		}
	} // while

	// setup serial communication
	canu_init(serial);
	debug(1, "Serial CAN communication established" );

	// setup network socket
	cann_listen(tcpport);
	debug(1, "Listenig for network connections on port %d", tcpport );

	event_loop();  // does not return

	return 1;  

old:

	if(!strcmp(argv[1], "p")){
		print_packets();
	}else if(!strcmp(argv[1], "g")){
		int sa,da,sp,dp,d[8];
		int num = sscanf(argv[2], "%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x",&sa,&sp,&da,&dp,&d[0],&d[1],&d[2],&d[3],&d[4],&d[5],&d[6],&d[7]) - 4;
		if(num >= 0){
			can_message *msg = can_buffer_get();
			msg->dlc = num;
			msg->addr_src = sa;
			msg->addr_dst = da;
			msg->port_src = sp;
			msg->port_dst = dp;
			unsigned int x;
			for(x=0;x<8;x++){
				msg->data[x] = (unsigned char) d[x];
			}
			can_transmit(msg);
		}else{
			printf("error: not enough bytes provided");
		}
	
	}
	
	

}
