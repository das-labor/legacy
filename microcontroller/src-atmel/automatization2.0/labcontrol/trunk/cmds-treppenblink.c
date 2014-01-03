#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <lib-host/can.h>
#include <lib/lap.h>

#include "cmds-treppenblink.h"


void cmd_treppenblink_mode(int argc, char *argv[]) 
{
	pdo_message *msg;
	int i;
	if (argc != 2) goto argerror;

	msg = (pdo_message *)can_buffer_get();

	msg->addr_src = 0x00;
	msg->addr_dst = 0x25;
	msg->port_src = 0x00;
	msg->port_dst = 0x0f;
	msg->dlc      = 1;


	// dst
	if ( sscanf(argv[1], "%x", &i) != 1)
		goto argerror;
	msg->cmd      = i;

	can_transmit((can_message*)msg);

	return;

argerror:
	printf("modes %s:\n",argv[0]);
	printf("\t 0x00 \t - default (blau)\n");
	printf("\t 0x01 \t - stop\n");
	printf("\t 0x02 \t - Weiss voll\n");
	printf("\t 0x03 \t - Weiss halb\n");
	printf("\t 0x04 \t - aus\n");
	printf("\t 0x05 \t - gruen\n");
	printf("\t 0x06 \t - rot\n");
	printf("\t 0x07 \t - epi\n");

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


cmd_t treppenblink_cmds[] = {
  { &cmd_treppenblink_mode, "mode", "mode", "switch treppenblink to mode" },
  { NULL, NULL, NULL, NULL }
};


void cmd_treppenblink(int argc, char **argv)
{

  char *arg;
	cmd_t *cmd;
	cmd = treppenblink_cmds;
	arg = argv[1];
	if (argc > 1)
	  {
		    while(cmd->fkt) 
	      {
			if (strcmp(arg, cmd->cmd) == 0) 
		  {
			    (*(cmd->fkt))(argc-1, &(argv[1]));
		    goto done;
		  }
		cmd++;
	      }
	  }
	/* show help */
	printf( "\nUsage: lapcontrol [OPTIONS] treppenblink <SUBCOMMAND>\n\n" );
	printf( "Available Subcommands:\n\n" );

	cmd = treppenblink_cmds;
	while(cmd->fkt) {
		printf( "   %-30s %s\n", cmd->sig, cmd->desc );
		cmd++;
	}
	printf("type 'lapcontrol [OPTIONS] treppenblink <SUBCOMMAND> help' for more informations");
	printf( "\n" );

done: 
	return;
}


