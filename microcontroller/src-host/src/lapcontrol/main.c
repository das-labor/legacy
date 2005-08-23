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

/**
 * Available Subcommands
 */
typedef struct {
	void (*fkt)(int, char**);
	char *cmd;
	char *sig;
	char *desc;
} cmd_t;

void fkt_ping(int argc, char *argv[]) 
{
	can_addr addr = atoi(argv[1]);
	lap_ping(addr);
};

void fkt_reset(int argc, char *argv[]) 
{
};

void fkt_dump(int argc, char *argv[]) 
{
	can_message *msg;

	while(1) {
		msg = can_get_nb();
		
		if (msg) {
			printf( "%10d: %02x:%02x -> %02x:%02x\n", localtime(),
					msg->addr_src, msg->port_src,
					msg->addr_dst, msg->port_dst );
		}
		can_free(msg);
	}
};



cmd_t cmds[] = {
  { &fkt_dump,  "dump", "dump", "Packet dump from CAN bus" },
  { &fkt_reset, "reset", "reset <addr>", "Send reset to <addr>" },
  { &fkt_ping,  "ping",	"ping <addr>", "Send ping to <addr>" },
//  { &ftk_lampe, "lampe", "lampe ...",  "Control lampe-device" }
  { NULL, NULL, NULL, NULL }
};


void help()
{
	cmd_t *ncmd;

	printf("\nUsage: %s [OPTIONS] <COMMAND>\n", progname);
	printf("\n\
Options:\n\n\
   -h, --help              display this help and exit\n\
   -v, --verbose           be more verbose and display a CAN packet dump\n\
   -s, --server HOST       use specified server (default: localhost)\n\
   -p, --port PORT         use specified TCP/IP port (default: 2342)\n\n\
Commands:\n\n" );

	ncmd = cmds;
	while(ncmd->fkt) {
		printf( "   %-20s       %s\n", ncmd->sig, ncmd->desc );

		ncmd++;
	}
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


	if (optind == argc) {
		help();
		exit(1);
	}

	can_init(server, tcpport);
	debug(1, "CAN communication established" );

	char *arg = argv[optind];

	cmd_t *cmd = cmds;
	while(cmd->fkt) {
		if (strcmp(arg, cmd->cmd) == 0) {
			(*(cmd->fkt))(argc-optind, &(argv[optind]));
			goto done;
		}
		cmd++;
	}

done:
	cann_close(0);

	return 0;
}
