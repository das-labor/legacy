#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <lib-host/can.h>
#include <lib/lap.h>

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
	debug(0, "borg mode <addr> <number>");
}

void blm_frame_send(int (* bmp)[8] ){

	pdo_message *msg;
	unsigned char x;
	for(x=0;x<8;x+=2){
		msg = (pdo_message *)can_buffer_get();

		msg->addr_src = 0x00;
		msg->addr_dst = 0x42;
		msg->port_src = 0x23;
		msg->port_dst = 0x23;
		msg->dlc      = 8;
		msg->cmd      = 4;

		msg->data[0] = x;

		(*bmp)[x] >>= 1;
		(*bmp)[x+1] >>= 1;
		msg->data[1] = (*bmp)[x] & 0xff;
		msg->data[2] = ((*bmp)[x]>>8) & 0xff;
		msg->data[3] = ((*bmp)[x]>>16) & 0xff;
		msg->data[4] = ((*bmp)[x+1]) & 0xff;
		msg->data[5] = ((*bmp)[x+1]>>8) & 0xff;
		msg->data[6] = ((*bmp)[x+1]>>16) & 0xff;

		can_transmit((can_message*)msg);
	}
}



void cmd_borg_blm(int argc, char *argv[]){
	pdo_message *msg;
	char * filename;
	FILE * blm;
	int lc, i, len, duration=0;
	char buf[100];
//	char *src, *dst;

//	int addr;

	if (argc != 2) goto argerror;

	filename = argv[1];

	if (strcmp (filename, "-") == 0)
		blm = stdin;
	else
		blm = fopen (filename, "r");

	if (!blm)
		{
			fprintf (stderr, "Can't open '%s': %s\n", filename, strerror (errno));
			return;
		}

	lc = 1;
	if (!bl_fgets (buf, sizeof (buf), blm) && lc++)
		goto blerror;

	if (buf[0] != '#')
		goto blerror;

	i = 1;
	while (isspace (buf[i]))
		i++;

	if (strncasecmp (buf + i, "BlinkenLights Movie", 19) != 0)
		goto blerror;

	unsigned int bmp[8];
	unsigned int linecnt=0;

	while (bl_fgets (buf, sizeof (buf), blm) && lc++)

	{
			len = strlen (buf);

			if (len == 0)
				continue;

			if (buf[0] == '#')
				continue;

			if (buf[0] == '@')
				{
					fflush (stdout);
					blm_frame_send(bmp);
					usleep (duration*1000);

					if (sscanf (buf+1, "%d", &duration) != 1 || duration < 0)
						duration = 0;

					printf ("\033[2J\033[H");
					linecnt = 0;
				}
			else
				{
					/* skip empty lines */
					for (i = 0; i < len; i++)
						if (!isspace (buf[i]))
							break;
					if (i == len)
						continue;

					/* special case last line */
					if (feof (blm))
						len++;

					bmp[linecnt] = 0;
					for (i = 0; i < len - 1; i++){
						bmp[linecnt] <<= 1;
						putchar (buf[i] == '0' ? ' ' : '#');
						bmp[linecnt] |= (buf[i] == '0' ? 0:1);
					}

					linecnt++;
					putchar ('\n');
				}
		}

	fclose (blm);
	blm_frame_send(bmp);
	usleep (duration*1000);
	return 0;

 blerror:
	fprintf (stderr, "Error parsing BlinkenLights movie '%s' (line %d).\n",
					 filename, lc);
	fclose (blm);
	return -1;
argerror:
	debug(0, "borg mode <addr> <number>");

}



void cmd_borg_scroll(int argc, char *argv[])
{
	pdo_message *msg;
	char *src, *dst;
	int addr;

	if (argc != 3) goto argerror;

	// dst
	if ( sscanf(argv[1], "%x", &addr) != 1)
		goto argerror;

	msg = (pdo_message *)can_buffer_get();

	msg->addr_src = 0x00;
	msg->addr_dst = addr;
	msg->port_src = 0x23;
	msg->port_dst = 0x23;
	msg->dlc      = 1;
	msg->cmd      = 2;

	src = argv[2];
	dst = msg->data;
	while( (*src) && (msg->dlc < 8)) {
		*dst++ = *src++;
		msg->dlc++;
	};

	// first packet
	can_transmit((can_message*)msg);

	while(*src) {
		msg = (pdo_message *)can_buffer_get();

		msg->addr_src = 0x00;
		msg->addr_dst = addr;
		msg->port_src = 0x23;
		msg->port_dst = 0x23;
		msg->dlc      = 1;
		msg->cmd      = 3;

		dst = msg->data;
		while( (*src) && (msg->dlc < 8)) {
			*dst++ = *src++;
			msg->dlc++;
		};

		can_transmit((can_message*)msg);
	}


	return;

argerror:
	debug(0, "borg scroll <addr> <number>");
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
  { &cmd_borg_mode, "mode", "mode", "switch borg to mode" },
  { &cmd_borg_scroll, "scroll", "scroll", "set scrolltext" },
  { &cmd_borg_blm, "blm", "blm", "play Blinkenmovie" },
  { NULL, NULL, NULL, NULL }
};


void cmd_borg(int argc, char **argv)
{
	char *arg = argv[1];
	cmd_t *cmd;

	cmd = borg_cmds;
	while(argc > 1 && cmd->fkt) {
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


