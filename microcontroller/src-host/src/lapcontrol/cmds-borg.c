#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <can.h>
#include <lap.h>

#include "cmds-borg.h"


void cmd_borg_mode(int argc, char *argv[]) 
{
	pdo_message *msg;
	int i;

	if (argc != 3) goto argerror;

	msg = (pdo_message *)can_buffer_get();

	msg->addr_src = 0x00;
	msg->addr_dst = 0x00;
	msg->port_src = 0x23;
	msg->port_dst = 0x23;
	msg->dlc      = 2;
	msg->cmd      = FKT_BORG_MODE;
	msg->data[0]  = 0;

	// dst
	if ( sscanf(argv[1], "%x", &i) != 1)
		goto argerror;
	msg->addr_dst = i;

	// mode
	if ( sscanf(argv[2], "%i", &i) != 1)
		goto argerror;
	msg->data[0] = i;

	can_transmit((can_message*)msg);

	return;

argerror:
	debug(0, "mode <number>");
}


void cmd_borg_scroll(int argc, char *argv[]) 
{
	pdo_message *msg;
	int i;

	if (argc != 3) goto argerror;

	msg = (pdo_message *)can_buffer_get();

	msg->addr_src = 0x00;
	msg->addr_dst = 0x00;
	msg->port_src = 0x23;
	msg->port_dst = 0x23;
	msg->dlc      = 2;
	msg->cmd      = 1;
	msg->data[0]  = 0;

	// dst
	if ( sscanf(argv[1], "%x", &i) != 1)
		goto argerror;
	msg->addr_dst = i;

	can_transmit((can_message*)msg);

	return;

argerror:
	debug(0, "borg scroll <number>");
};

/**
 * Available commands array
 */
typedef struct {
	void (*fkt)(int, char**);
	char *cmd;
	char *sig;
	char *desc;
} cmd_t;


cmd_t borg_cmds[] = {
  { &cmd_borg_mode, "mode", "mode", "switch bord to mode" },
  { &cmd_borg_scroll, "scroll", "scroll", "set scrolltext" },
  { NULL, NULL, NULL, NULL }
};


void cmd_borg(int argc, char **argv)
{
	char *arg = argv[1];
	cmd_t *cmd;
	
	cmd = borg_cmds;
	while(cmd->fkt) {
		if (strcmp(arg, cmd->cmd) == 0) {
			(*(cmd->fkt))(argc-1, &(argv[1]));
			goto done;
		}
		cmd++;
	}

	/* show help */
	printf( "\nUsage: lapcontrol [OPTIONS] borg <SUBCOMMAND>\n\n" );
	printf( "Available Subcommands:\n\n" );

	cmd = borg_cmds;
	while(cmd->fkt) {
		printf( "   %-30s %s\n", cmd->sig, cmd->desc );
		cmd++;
	}
	printf( "\n" );

done: 
	return;
}


