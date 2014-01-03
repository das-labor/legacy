#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <lib-host/can.h>
#include <lib/lap.h>

#include "cmds-canir.h"


void cmd_canir_teufel(int argc, char *argv[])
{
	can_message *msg;
	unsigned int i;
	if (argc != 2) goto argerror;
	
	// dst
	if ( sscanf(argv[1], "%x", &i) != 1)
		goto argerror;

	msg = can_buffer_get();

	msg->addr_src = 0x00;
	msg->addr_dst = 0x10;
	msg->port_src = 0x00;
	msg->port_dst = 0x21;
	msg->dlc      = 2;
	msg->data[0]  = 0x00;
	msg->data[1]  = i;

	can_transmit(msg);

	return;

argerror:
	printf("teufel %s:\n",argv[0]);
	printf("\t 0x00 \t - leiser\n");
	printf("\t 0x01 \t - lauter\n");
	printf("\t 0x02 \t - mute\n");
	printf("\t 0x03 \t - select main-channel\n");
	printf("\t 0x04 \t - select front-channel\n");
	printf("\t 0x05 \t - select rear-channel\n");
	printf("\t 0x06 \t - select side-channel\n");
	printf("\t 0x07 \t - select sub-channel\n");
	printf("\t 0x08 \t - select center-channel\n");

}


void cmd_canir_beamer(int argc, char *argv[]) 
{
	can_message *msg;
	unsigned int i;
	if (argc != 2) goto argerror;

	// dst
	if (sscanf(argv[1], "%x", &i) != 1)
		goto argerror;

	msg = can_buffer_get();

	msg->addr_src = 0x00;
	msg->addr_dst = 0x10;
	msg->port_src = 0x00;
	msg->port_dst = 0x21;
	msg->dlc      = 2;
	msg->data[0]  = 0x01;

	msg->data[1]  = i;

	can_transmit(msg);

	return;

argerror:
	printf("beamer %s:\n",argv[0]);
	printf("\t 0x00 \t - Power On/OFF\n");
	printf("\t 0x01 \t - Source\n");
	printf("\t 0x02 \t - VGA\n");
	printf("\t 0x03 \t - DVI\n");
	printf("\t 0x04 \t - s-video\n");
	printf("\t 0x05 \t - component\n");
	printf("\t 0x06 \t - blank\n");

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


cmd_t canir_cmds[] = {
	{ &cmd_canir_beamer, "beamer", "beamer", "control beamer" },
	{ &cmd_canir_teufel, "teufel", "teufel", "control teufelamp" },
	{ NULL, NULL, NULL, NULL }
};


void cmd_canir(int argc, char **argv)
{

	char *arg;
	cmd_t *cmd;
	cmd = canir_cmds;
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
	printf( "\nUsage: lapcontrol [OPTIONS] canir <SUBCOMMAND>\n\n" );
	printf( "Available Subcommands:\n\n" );

	cmd = canir_cmds;
	while(cmd->fkt) {
		printf( "   %-30s %s\n", cmd->sig, cmd->desc );
		cmd++;
	}
	printf("type 'lapcontrol [OPTIONS] canir <SUBCOMMAND> help' for more informations");
	printf( "\n" );

done: 
	return;
}

