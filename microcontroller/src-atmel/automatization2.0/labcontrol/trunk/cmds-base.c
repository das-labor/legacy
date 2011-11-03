#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <can.h>
#include <lap.h>

#include "cmds-base.h"
#include "proto_lampe.h"
void cmd_loopback(int argc, char *argv[])
{
	int mode;

	if (argc != 2) goto argerror;

	if (sscanf(argv[1], "%i", &mode) != 1)
		goto argerror;

	if (mode)
	{
		printf("Activating loopack mode\n");
		can_setmode(loopback);
	} else {
		printf("Deativating loopack mode\n");
		can_setmode(normal);
	}
	return;

argerror:
	debug(0, "loopback [0|1]");
}


void cmd_ping(int argc, char *argv[])
{
	int addr;
	can_message *msg;

	if (argc != 2) goto argerror;
       	if (sscanf(argv[1], "%i", &addr) != 1)
		goto argerror;

	lap_ping(addr);

	for (;;)
	{
		msg = can_get();

		if (msg->addr_src == addr)
		{
			printf("Pong from 0x%x\n", addr);
			free(msg);
			return;
		}
	}

	return;
argerror:
	debug(0, "ping <addr>");
};

void cmd_reset(int argc, char *argv[])
{
	int addr;

	if (argc != 2) goto argerror;
       	if (sscanf(argv[1], "%i", &addr) != 1)
		goto argerror;

	lap_reset(addr);
	return;
argerror:
	debug(0, "reset <addr>");
};



void hexdump(unsigned char * addr, int size)
{
	unsigned char x=0, sbuf[3];

	while (size--)
	{
		printf("%02x ", *addr++);
		if (++x == 16)
		{
			printf("\n");
			x = 0;
		}
	}
}


extern unsigned int debug_level;

void dump_packet_v2(can_message_v2 *msg){
	time_t muh = time(0);
	struct tm *tme = localtime(&muh);
	printf( "%02d:%02d.%02d:  %03x,%x:  %02x -> %02x    ",
		tme->tm_hour, tme->tm_min, tme->tm_sec,
		msg->channel, msg->subchannel,
		msg->addr_src, msg->addr_dst );
	hexdump(msg->data, msg->dlc);
	printf("\n");
}

void dump_packet(can_message *msg){
	time_t muh = time(0);
	struct tm *tme = localtime(&muh);
	printf( "%02d:%02d.%02d:  %02x:%02x -> %02x:%02x    ",
		tme->tm_hour, tme->tm_min, tme->tm_sec,
		msg->addr_src, msg->port_src,
		msg->addr_dst, msg->port_dst );
	hexdump(msg->data, msg->dlc);
	printf("\n");
}

void dump_control_packet(can_message *msg){
	time_t muh = time(0);
	struct tm *tme = localtime(&muh);
	printf( "%02d:%02d.%02d:  %02x:%02x -> %02x:%02x    ",
		tme->tm_hour, tme->tm_min, tme->tm_sec,
		msg->addr_src, msg->port_src,
		msg->addr_dst, msg->port_dst );
	hexdump(msg->data, msg->dlc);
	printf("\n");
}

void cmd_dump(int argc, char *argv[])
{

	if (argc > 1)
	{
		can_message_v2 *msg;

		while (1)
		{
			msg = can_get_v2_nb();

			if (msg)
			{
				dump_packet_v2(msg);
				can_free_v2(msg);
			}
			usleep(100);
		}
	} else
	{
		can_message *msg;

		while (1)
		{
			msg = can_get();

			if (msg)
			{
				if(msg->cmd == 0x11) //can packet
					dump_packet(msg);
				else
					dump_control_packet(msg);
				can_free(msg);
			}
		}
	}
};


void cmd_packet(int argc, char *argv[])
{
	int src_addr, src_port;
	int dst_addr, dst_port;
	int i;
	char *tok;
	can_message *msg;

	if (argc != 4)
		goto argerror;

	if (sscanf(argv[1], "%i:%i", &src_addr, &src_port ) != 2)
		goto argerror;

	if (sscanf(argv[2], "%i:%i", &dst_addr, &dst_port ) != 2)
		goto argerror;

	msg = can_buffer_get();
	msg->addr_src = src_addr;
	msg->addr_dst = dst_addr;
	msg->port_src = src_port;
	msg->port_dst = dst_port;
	msg->dlc = 0;

	tok = strtok( argv[3], ",");
	while (tok)
	{
		if (msg->dlc >= 8 ) goto argerror;
		if (sscanf(tok, "%i", &i) != 1) goto argerror;
		msg->data[msg->dlc++] = i;

		tok = strtok(NULL, ",");
	}

	printf("%02x:%02x -> %02x:%02x (length=%d)\n", src_addr, src_port, dst_addr, dst_port, msg->dlc);
	can_transmit(msg);

	return;
argerror:
	debug(0, "packet <src-addr>:<src-port> <dst-addr>:<dst-port> <data>,<data>,....");
}

void cmd_lamp(int argc,char *argv[])
{

	if (argc != 4)
		goto argerror;

	int dst;
	int lamp;
	int value;
	sscanf(argv[1],"%x",&dst);
	sscanf(argv[2],"%x",&lamp);
	sscanf(argv[3],"%x",&value);

	lampe_set_lampe((can_addr)dst, lamp, value);

	return;

argerror:
	debug(0, "lamp <addr> <lamp> <value>");
}

