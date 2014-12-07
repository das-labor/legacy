#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "config.h"
#include "lib-host/can.h"
#include "lib-host/can-tcp.h"
#include "lib-host/can-uart.h"
#include "lib-host/debug.h"

#include "cansole.h"
#include "cmds-base.h"
#include "cmds-test.h"
#include "cmds-borg.h"
#include "cmds-mood.h"
#include "cmds-musicd.h"
#include "cmds-flash.h"
#include "cmds-treppenblink.h"
#include "cmds-canir.h"
#include "cmds-cantemp.h"
#include "cmds-powercommander.h"
#include "cmds-bastel.h"
#include "cmds-powermeter.h"
#include "cmds-gateway.h"
// Atmel ; LAP includes
// #include "config.h"


#ifndef max
 #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

/**
 * Available commands array
 */
typedef struct {
	void (*fkt)(int, char**);
	char *cmd;
	char *sig;
	char *desc;
} cmd_t;

cmd_t cmds[] = {
	{ &cmd_loopback,	 "loopback", "loopback [0|1]", "Enable/disable loopback mode" },
	{ &cmd_test, "test", "test <subcommand>", "Test commands" },
	{ &cmd_packet, "packet", "packet <src> <dst> <data>", "Send arbitrary packets" },
	{ &cmd_dump,	 "dump", "dump", "Packet dump from CAN bus" },
	{ &cmd_reset,	"reset", "reset <addr>", "Send reset to <addr>" },
	{ &cmd_ping,	 "ping", "ping <addr>", "Send ping to <addr>" },
	{ &cmd_flash,	"flash", "flash <addr> <file>" ,"flash file to device"},
	{ &cmd_lamp,	"lamp", "lamp <addr> <lamp> <value>" ,"set lamp on device to value"},
	{ &cmd_borg,	"borg", "borg <subcommand>" ,"control bord device"},
	{ &cmd_mood,	"mood", "mood <subcommand>" ,"control mood device"},
	{ &cmd_cansole, "cansole", "cansole <addr> <chan> <subchannel>", "connect to cansole at given address, channel and subchannel" },
	{ &cmd_musicd, "musicd", "musicd", "starts a music control daemon in foreground" },
	{ &cmd_treppenblink, "treppenblink", "treppenblink", "mode des treppenblinks" },
	{ &cmd_canir, "canir", "canir", "control thinks supposed to accept ir-commands only via can" },
	{ &cmd_powercommander, "powercommander", "powercommander", "powercommander class object function value ... wiki->Powercommander" },
	{ &cmd_cantemp, "cantemp", "cantemp", "Temperatur fuer Sensor" },
	{ &cmd_bastel, "bastelcmd", "bastelcmd", "bastelcontrol class object function value ... wiki->Bastelraum" },
	{ &cmd_canpowermeter, "powermeter", "powermeter", "Energieverbrauch des Labors" },
	{ &cmd_gateway, "gw", "gw <subcommand>", "CAN-Gateway Control (ping currently)" },
	{ NULL, NULL, NULL, NULL }
};


static char *progname;

static char *optstring = "hv::S:s:p:";
struct option longopts[] =
{
	{ "help", no_argument, NULL, 'h' },
	{ "verbose", optional_argument, NULL, 'v' },
	{ "server", required_argument, NULL, 's' },
	{ "serial", required_argument, NULL, 'S' },
	{ "baudrate", required_argument, NULL, 'b' },
	{ "port", required_argument, NULL, 'p' },
	{ NULL, 0, NULL, 0 }
};


void help()
{
	cmd_t *ncmd;

	printf("\nUsage: %s [OPTIONS] <COMMAND>\n", progname);
	printf("\n\
Options:\n\n\
	 -h, --help							display this help and exit\n\
	 -v, --verbose					 be more verbose and display a CAN packet dump\n\
	 -s, --server HOST			 use specified server (default: localhost)\n\
	 -p, --port PORT				 use specified TCP/IP port (default: 2342)\n\
	 -S, --serial PORT			 use specified serial port\n\n\
Commands:\n\n" );

	ncmd = cmds;
	while(ncmd->fkt) {
		printf( "	 %-30s %s\n", ncmd->sig, ncmd->desc );
		ncmd++;
	}
	printf( "\n" );
}

/**
 * Main
 */
int main(int argc, char *argv[])
{
	char *tcpport	= "2342";				 // TCP Port
	char *server = "localhost";
	char *serial = NULL;
	char *baud = NULL;
	int optc;

	progname = argv[0];
	int option_index = 0;

	while ((optc = getopt_long(argc, argv, optstring, longopts, &option_index)) != EOF) {
		switch (optc) {
			case 'v':
				if (optarg)
					debug_level = atoi(optarg);
				else
					debug_level = 3;
				break;
			case 'S':
				serial = optarg;
				break;
			case 'b':
				baud = optarg;
				break;
			case 's':
				server = optarg;
				break;
			case 'p':
				tcpport = optarg;
				break;
			case 'h':
				help();
				exit(EXIT_SUCCESS);
			default:
				help();
				exit(EXIT_FAILURE);
		}
	} // while
	cann_conn_t *conn;

	//init debugging to console, otherwise we'll experience crashes...
	debug_init(NULL);

	if (optind == argc) {
		help();
		exit(EXIT_FAILURE);
	}

	char *arg = argv[optind];

	cmd_t *cmd = cmds;
	while (cmd->fkt) {
		if (!strcmp(arg, cmd->cmd)) {
			//connect only if a valid command has been detected
			if (serial) {
				debug(1, "Trying to establish CAN communication via serial %s", serial);
				canu_init(serial, baud);
				can_init_posix(NULL);		// use serial
			} else {
				debug(1, "Trying to establish CAN communication via cand (%s:%s)", server, tcpport);
				conn = cann_connect(server, tcpport);
				can_init_posix(conn);		// use specified connection to cand
			}

			(*(cmd->fkt))(argc-optind, &(argv[optind]));
			goto disconnect;
		}
		cmd++;
	}

	debug(0, "Command not understood");
	help();
	goto done;

disconnect:
	//cann_close(0);
	usleep(1000); //sleep a little, so last data on socket can be sent before closing
	close(conn->fd);
	//cann_close_errors();
	free(conn);
done:
	debug_close();
	return 0;
}
