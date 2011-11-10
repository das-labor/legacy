#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <signal.h>
#include <unistd.h>

#include "can.h"
#include "can-tcp.h"
#include "uart-host.h"
#include "can-uart.h"
#include "debug.h"

// Atmel ; LAP includes
#include "config.h"

#ifndef max
 #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif


char *progname;
char *serial;
char *logfile = NULL;
char *scriptfile = NULL;
char *debugfile = NULL;


static char *optstring = "hdv::S:p:l:s:D:";
struct option longopts[] =
{
  { "help", no_argument, NULL, 'h' },
  { "daemon", no_argument, NULL, 'd' },
  { "verbose", optional_argument, NULL, 'v' },
  { "serial", required_argument, NULL, 'S' },
  { "port", required_argument, NULL, 'p' },
  { "logfile", required_argument, NULL, 'l'},
  { "scriptfile", required_argument, NULL, 's'},
  { "debugfile", optional_argument, NULL, 'D'},
  { NULL, 0, NULL, 0 }
};

void help()
{
	printf("\nUsage: %s [OPTIONS]\n", progname);
	printf("\
   -h, --help              display this help and exit\n\
   -v, --verbose           be more verbose and display a CAN packet dump\n\
   -d, --daemon            become daemon\n\
   -S, --serial PORT       use specified serial port\n\
   -s, --scriptfile FILE   use a scripte file to execute cmds on package\n\
   -l, --logfile FILE      log every package to a logfile\n\
   -p, --port PORT         use specified TCP/IP port (default: 2342)\n\
   -D, --debugfile FILE    log debug messages to this file\n\n" );
}


void hexdump(unsigned char * addr, int size)
{
	unsigned char x = 0;

	printf( "Size: %d\n", size);
	while (size--) {
		printf("%02x ", *addr++);
		if (++x == 16) {
			printf("\n");
			x = 0;
		}
	}
	printf("\n");
}

void customscripts(rs232can_msg *msg)
{
	FILE *logFP;
	FILE *scriptFP;
	char tmpstr[80];
	char tmpstr2[80];
	time_t mytime;
	char as_args[9][5];
	struct tm *tm;

	int result;

	uint8_t i;

	setlocale (LC_ALL, "C");
	mytime = time(NULL);
	tm = localtime(&mytime);

	char line[300];
	can_message_raw *in_msg = (can_message_raw*)(msg->data);
	can_message_match match_msg = {0x00,0x00,0x00,0x00,0x00,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
	can_message_match dec_msg = {0x00,0x00,0x00,0x00,0x00,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};


	// decoding in_msg to readable format
	dec_msg.addr_src = (uint8_t) (in_msg->id >> 8);
	dec_msg.addr_dst = (uint8_t) (in_msg->id);
	dec_msg.port_src = (uint8_t) ((in_msg->id >> 23) & 0x3f);
	dec_msg.port_dst = (uint8_t) (((in_msg->id >> 16) & 0x0f) | ((in_msg->id >> 17) & 0x30));
	dec_msg.dlc = in_msg->dlc;
	for (i = 0; i < dec_msg.dlc; i++)
	{
		dec_msg.data[i] = in_msg->data[i];
	}

	// logging to file - 'logfile' is global

	strftime(tmpstr2, 79, "%c", tm);

	if ( logfile != NULL)
	{
		// open logfile to append - use logrotate
		if ( (logFP=fopen(logfile,"a")) != NULL)
		{
			memset(tmpstr,0,80);

			// print time and metadata to buffer
			result = snprintf(tmpstr,80,"%s : src: 0x%.2x:0x%.2x dst:0x%.2x:0x%.2x data: ",
				    tmpstr2,
				    dec_msg.addr_src,dec_msg.port_src,
				    dec_msg.addr_dst,dec_msg.port_dst );
			// result must be in range - also snprintf limits range
			if (result <= 80 && result >= 0)
			{
				// write to logfile
				fwrite(tmpstr,result,1,logFP);
				memset(tmpstr,0,80);
				memset(tmpstr2,0,80);

				for (i = 0; i < dec_msg.dlc; i++)
				{
					result = snprintf(tmpstr,80,"0x%.2x ",dec_msg.data[i]);
					if (result == 5)
					{
						// append the data
						fwrite(tmpstr,result,1,logFP);
					}
				}
				// append newline
				fwrite("\n",1,1,logFP);
			}
			else
			{
				// this should not happen
				fwrite("logfail\n",8,1,logFP);
			}
			fflush(logFP);
			fclose(logFP);
		}
	}


	// 'scriptfile' is global
	if ( scriptfile != NULL)
	{
		if ( (scriptFP=fopen(scriptfile,"r")) != NULL)
		{
			// we only support full match - on src/dst and dlc
			// example:
			// 0x00:0x00 0x00:0x00 0x04  -> command
			// src:srcport dst:dstport len -> executable
			while (fgets(line, 300, scriptFP) != NULL)
			{
			// read instructions
				memset(tmpstr,0,80);
				memset(tmpstr2,0,80);

				result = sscanf(line,"0x%hhx:0x%hhx 0x%hhx:0x%hhx 0x%hhx -> %s",
					 &(match_msg.addr_src),&(match_msg.port_src),
					 &(match_msg.addr_dst),&(match_msg.port_dst),
					 &(match_msg.dlc),
					 tmpstr2);
				memset(line,0,300);
				if (result !=6)
				{
					// got a wrong line
					continue;
				}
				// check for match
				if ( (match_msg.addr_dst == dec_msg.addr_dst) &&
					(match_msg.addr_src == dec_msg.addr_src) &&
					(match_msg.port_dst == dec_msg.port_dst) &&
					(match_msg.port_src == dec_msg.port_src) &&
					(match_msg.dlc == dec_msg.dlc))
				{
					// double fork a client to exec the command right under init
					if (vfork())
					{
						wait();// dump
					}
					else
					{
						//memset(as_args,0,45);

						// executing as a child
						if (vfork())
						{
							_exit(0);
						}
						else
						{
							snprintf(as_args[0],5,"0x%.2x",dec_msg.dlc);
							snprintf(as_args[1],5,"0x%.2x",dec_msg.data[0]);
							snprintf(as_args[2],5,"0x%.2x",dec_msg.data[1]);
							snprintf(as_args[3],5,"0x%.2x",dec_msg.data[2]);
							snprintf(as_args[4],5,"0x%.2x",dec_msg.data[3]);
							snprintf(as_args[5],5,"0x%.2x",dec_msg.data[4]);
							snprintf(as_args[6],5,"0x%.2x",dec_msg.data[5]);
							snprintf(as_args[7],5,"0x%.2x",dec_msg.data[6]);
							snprintf(as_args[8],5,"0x%.2x",dec_msg.data[7]);
							execl(tmpstr2,tmpstr2,
							as_args[0],
							as_args[1],as_args[2],as_args[3],as_args[4],
							as_args[5],as_args[6],as_args[7],as_args[8],NULL);
						}
					}
				}
			}
			fclose(scriptFP);
		}
	}

}

void msg_to_clients(rs232can_msg *msg)
{
	cann_conn_t *ac;
	if(msg->cmd == RS232CAN_PKT)
		customscripts(msg);

	// foreach client
	ac = cann_conns_head;
	while (ac) {
//XXX		if ( cann_match_filter(ac, msg) )
		cann_transmit(ac, msg);		//send to each client on the network
		ac = ac->next;
	}
}

#define MEGA8_RESETCAUSE_PORF 	1
#define MEGA8_RESETCAUSE_EXTRF	2
#define MEGA8_RESETCAUSE_BORF	4
#define MEGA8_RESETCAUSE_WDRF	8
#define RESETCAUSE_PORF_STR "power on "
#define RESETCAUSE_EXTRF_STR "reset pin "
#define RESETCAUSE_BORF_STR "brown out "
#define RESETCAUSE_WDRF_STR "watchdog "

void sprint_atmega8_resetcause(char *buf, unsigned char reset_flags)
{
	sprintf(buf, "%s%s%s%s",
		(reset_flags & MEGA8_RESETCAUSE_PORF)?RESETCAUSE_PORF_STR:"",
		(reset_flags & MEGA8_RESETCAUSE_EXTRF)?RESETCAUSE_EXTRF_STR:"",
		(reset_flags & MEGA8_RESETCAUSE_BORF)?RESETCAUSE_BORF_STR:"",
		(reset_flags & MEGA8_RESETCAUSE_WDRF)?RESETCAUSE_WDRF_STR:"");
}


void process_uart_msg()
{
	rs232can_msg *msg = canu_get_nb();	//get message from uart
	char buf[sizeof(RESETCAUSE_PORF_STR) + sizeof(RESETCAUSE_EXTRF_STR) + sizeof(RESETCAUSE_BORF_STR) + sizeof(RESETCAUSE_WDRF_STR)];

	debug( 10, "Activity on uart_fd" );

	if (!msg)
		return;
	else if(canu_failcnt > CANU_FAILTHRESH)
	{
		debug(0, "UART failure threshold exceeded (%u), resyncing gateway.", canu_failcnt);
		canu_reset();
		canu_transmit_cmd(RS232CAN_RESYNC);
		canu_failcnt = 0;
	}

	debug(3, "Processing message from uart..." );

	switch(msg->cmd)
	{
		case RS232CAN_PKT:
			msg_to_clients(msg);
			break;
		case RS232CAN_ERROR:
			debug(0, "GATEWAY: error");
			break;
		case RS232CAN_NOTIFY_RESET:
			sprint_atmega8_resetcause(buf, msg->data[0]);
			debug(0, "GATEWAY: reset, reset cause: %s\n", buf);
			break;
		case RS232CAN_PING_GATEWAY:
			debug(0, "GATEWAY: pong");
			msg_to_clients(msg);	//send pong to network clients
			break;
		case RS232CAN_RESYNC:
			debug(0, "GATEWAY: resync request");
			canu_reset();
			break;
		case RS232CAN_VERSION:
		case RS232CAN_IDSTRING:
		case RS232CAN_PACKETCOUNTERS:
		case RS232CAN_ERRORCOUNTERS:
		case RS232CAN_POWERDRAW:
		case RS232CAN_READ_CTRL_REG:
		case RS232CAN_WRITE_CTRL_REG:
			msg_to_clients(msg);	//pipe reply to network clients
			break;
		default:
			debug(0, "Whats going on? Received unknown type 0x%02x on Uart", msg->cmd);
			break;
	}

	canu_free(msg);
	debug(3, "...processing done.");
}

void process_client_msg( cann_conn_t *client )
{
	cann_conn_t *ac;

	debug( 10, "Activity on client %d", client->fd );

	rs232can_msg *msg = cann_get_nb(client);	//get message from network
	if(!msg) return;

	debug(3, "Processing message from network..." );
	if (debug_level >= 3) hexdump((void *)msg, msg->len + 2);

	customscripts(msg);

	switch(msg->cmd) {
		case RS232CAN_SETFILTER:
		case RS232CAN_SETMODE:
			/* XXX */
			break;
		case RS232CAN_PKT:
			// to UART
			if (serial) canu_transmit(msg);		//send to client on the can
			msg_to_clients(msg);				//send to all network clients
			break;
		case RS232CAN_PING_GATEWAY:
		case RS232CAN_VERSION:
		case RS232CAN_IDSTRING:
		case RS232CAN_PACKETCOUNTERS:
		case RS232CAN_ERRORCOUNTERS:
		case RS232CAN_POWERDRAW:
		case RS232CAN_READ_CTRL_REG:
			msg->len = 0;
			if (serial) canu_transmit(msg);		//send to client on the can
			break;
		case RS232CAN_WRITE_CTRL_REG:
			if (msg->len == 1 && serial) canu_transmit(msg);		//send to client on the can
			break;
		case RS232CAN_ERROR:
		case RS232CAN_NOTIFY_RESET:
		case RS232CAN_RESYNC:
			//don't react on these commands
			break;
		default:
			if (serial) canu_transmit(msg);		//send to client on the can
			break;
	}
	cann_free(msg);
	debug(3, "...processing done.");
}

void new_client( cann_conn_t *client )
{
	// XXX
}

void event_loop()
{
	for (;;) {
		int ret;
		int highfd = 0;;
		fd_set rset;
		cann_conn_t *client;

		FD_ZERO(&rset);

		if (serial) {
			highfd = uart_fd;
			FD_SET(uart_fd, &rset);
		};
		highfd = max(highfd, cann_fdset(&rset));


		debug( 9, "VOR SELECT" );
		cann_dumpconn();

		ret = select(highfd+1, &rset, (fd_set *)NULL, (fd_set *)NULL, NULL);
		if (ret < 0) {
			debug_perror(1, "select returned %d", ret);
			continue;
		}
		debug( 10, "Select returned %d", ret);

		// check activity on uart_fd
		if (serial && FD_ISSET(uart_fd, &rset))
			process_uart_msg();

		debug( 9, "AFTER UART" );
		cann_dumpconn();

		// check client activity
		//
		while ( client = cann_activity(&rset) ) {
			debug(5, "CANN actiity found" );
			process_client_msg(client);
			//cann_conn_free(client);
		}

		debug( 9, "AFTER CANN ACTIVITY" );
		cann_dumpconn();

		// new connections
		if ( client = cann_accept(&rset) ) {
			debug( 2, "===> New connection (fd=%d)", client->fd );
			//cann_conn_free(client);
		}

		debug( 9, "AFTER CANN NEWCONN" );
		cann_dumpconn();


		// close errorous connections
		cann_close_errors();

		debug( 9, "AFTER CANN CLOSE" );
		cann_dumpconn();
	}
}

unsigned int running = 0;

void shutdown_all()
{
	if(running)
	{
		cann_close_errors();
		cann_close(NULL);
		shutdown(listen_socket, SHUT_RDWR);
		close(listen_socket);
		debug_close();
	}
}

void signal_handler(int sig) {
	debug(0, "Stopping Cand (SIGNAL: %i)", sig);
	shutdown_all();
	signal (sig, SIG_DFL);
	if (sig == SIGQUIT)
		exit(EXIT_SUCCESS);
	else
		raise (sig);
}

int main(int argc, char *argv[])
{
	int d = 0;                   // daemon
	int tcpport  = 2342;         // TCP Port
	int optc;

	(void) signal(SIGTERM, signal_handler);
	(void) signal(SIGKILL, signal_handler);
	(void) signal(SIGSEGV, signal_handler);

	progname = argv[0];
	optc=getopt_long(argc, argv, optstring, longopts, (int *)0);
	if(optc == EOF)
	{
		printf("ERROR: no arguments given, serial port not specified\n");
		help();
		exit(EXIT_SUCCESS);
	}
	while (optc != EOF) {
		switch (optc) {
			case 'v':
				if (optarg)
					debug_level = atoi(optarg);
				else
					debug_level = 0;
				break;
			case 'd':
				d = 1;
				break;
			case 'S':
				serial = optarg;
				break;
			case 'p':
				tcpport = atoi(optarg);
				break;
			case 'h':
				help();
				exit(EXIT_SUCCESS);
			case 'l':
				logfile = optarg;
				break;
			case 's':
				scriptfile = optarg;
				break;
			case 'D':
				debugfile = optarg;
				break;
			default:
				help();
				exit(EXIT_SUCCESS);
		}
		optc=getopt_long(argc, argv, optstring, longopts, (int *)0);
	} // while

	if(!serial)
	{
		printf("ERROR: no serial port specified\n");
		help();
		exit(EXIT_SUCCESS);
	}

	debug_init(debugfile);
	debug(0, "Starting Cand");

	// setup serial communication
	canu_init(serial);
	debug(1, "Serial CAN communication established" );


	// setup network socket
	cann_listen(tcpport);
	debug(1, "Listenig for network connections on port %d", tcpport );

	running = 1;
	atexit(shutdown_all);
	event_loop();  // does not return

	debug(0, "Cand exited event loop.. XXX ERROR BLINK FIXME NOW");
	return EXIT_SUCCESS;
}
