#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib-host/can.h"

#define LOOPBACK_MSGCOUNT 100

void test_loopback(int argc, char **argv)
{
	can_message *msg;
	int i;

	printf( "* Activating loopback mode\n" );
	can_setmode(loopback);

	for(i=0; i<LOOPBACK_MSGCOUNT; i++) {
		// sending message
		msg = can_buffer_get();
		msg->addr_src = 0x00;
		msg->port_src = 0x23;
		msg->addr_dst = 0x00;
		msg->port_dst = 0x23;
		msg->dlc      = 2;
		msg->data[0]  = 0x00;
		msg->data[1]  = i;
		can_transmit(msg);

		msg = can_get_nb();
		if (msg) {
			printf( " * got msg!\n" );
		}
	}

	printf( "* Deactivating loopback mode\n" );
	can_setmode(loopback);
}



/**
 * Available commands array
 */
typedef struct {
	void (*fkt)(int, char**);
	char *cmd;
	char *sig;
	char *desc;
} cmd_t;

cmd_t test_cmds[] = {
  { &test_loopback, "loopback", "loopback", "Loopback test" },
  { NULL, NULL, NULL, NULL }
};

void cmd_test(int argc, char **argv)
{
	char *arg = argv[1];
	cmd_t *cmd;

	cmd = test_cmds;
	while(argc > 1 && cmd->fkt) {
		if (strcmp(arg, cmd->cmd) == 0) {
			(*(cmd->fkt))(argc-1, &(argv[1]));
			goto done;
		}
	}

	/* show help */
	printf( "\nUsage: lapcontrol [OPTIONS] test <SUBCOMMAND>\n\n" );
	printf( "Available Subcommands:\n\n" );

	cmd = test_cmds;
	while(cmd->fkt) {
		printf( "   %-30s %s\n", cmd->sig, cmd->desc );
		cmd++;
	}
	printf( "\n" );

done:
	return;
}


