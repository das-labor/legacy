#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "lib-host/can-encap.h"
#include "lib-host/can.h"

#define DEFAULT_TIMEOUT 2000
#define ADC_RES ((double)(1<<10))
#define ADC_REF_MV 5000.0
#define VOLTAGE_DIVIDER_R1 2700.0
#define VOLTAGE_DIVIDER_R2 1000.0
#define VSHUNT_AMPLIFICATION 10.0
#define SHUNT_OHMS 0.01

/**
 * Available commands array
 */
typedef struct {
	void (*fkt)(int, char**);
	char *cmd;
	char *sig;
	char *desc;
} cmd_t;


static void send_gateway_command(uint8_t cmd, uint8_t *data, uint8_t len)
{
	rs232can_msg rmsg;
	rmsg.cmd = cmd;
	rmsg.len = (len > RS232CAN_MAXLENGTH) ? RS232CAN_MAXLENGTH : len;
	if (len > 0 && data != NULL)
		memcpy((void *)rmsg.data, (void *)data, rmsg.len);
	can_transmit_raw_gateway_message(&rmsg);
}


static rs232can_msg* anticipate_gateway_reply(uint8_t cmd)
{
	rs232can_msg *rmsg_in;
	unsigned int i = 0;
	//anticipate reply and timeout after 2s
	while (((rmsg_in = can_get_raw_gateway_message_nb()) == NULL || rmsg_in->cmd != cmd) && i++ < DEFAULT_TIMEOUT) usleep(1000);
	return (i < DEFAULT_TIMEOUT) ? rmsg_in : NULL;
}


static void cmd_gateway_ping(int argc, char *argv[])
{
	rs232can_msg *rmsg;

	//create and send gateway ping request
	printf("Pinging gateway.. ");
	send_gateway_command(RS232CAN_PING_GATEWAY, NULL, 0);

	//anticipate reply and timeout after 2s
	rmsg = anticipate_gateway_reply(RS232CAN_PING_GATEWAY);
	printf(rmsg ? "and got a reply!\n" : "but timed out after 2 seconds :-(\n");
}


static void cmd_gateway_version(int argc, char *argv[])
{
	rs232can_msg *rmsg;
	unsigned int maj, min, svn;
	char idstring[RS232CAN_MAXLENGTH+1];

	//create and send gateway version request
	printf("Requesting version..\n");
	send_gateway_command(RS232CAN_VERSION, NULL, 0);

	//anticipate reply and timeout after 2s
	rmsg = anticipate_gateway_reply(RS232CAN_VERSION);
	if (!rmsg) goto timeout;

	//store values
	maj = (unsigned char)rmsg->data[0];
	min = (unsigned char)rmsg->data[1];
	svn = ((unsigned char)rmsg->data[2] << 8) | (unsigned char)rmsg->data[3];

	//create and send gateway id request
	printf("Requesting id-string..\n");
	send_gateway_command(RS232CAN_IDSTRING, NULL, 0);

	//anticipate reply and timeout after 2s
	rmsg = anticipate_gateway_reply(RS232CAN_IDSTRING);
	if (!rmsg) goto timeout;

	//store string
	rmsg->len = (rmsg->len < sizeof(idstring)) ? rmsg->len : 20;
	memcpy(idstring, rmsg->data, rmsg->len);
	idstring[rmsg->len] = 0;

	//print answers
	printf("Version:\t%u.%u\n", maj, min);
	printf("SVN-Rev:\t%u\n", svn);
	printf("Identifier:\t%s\n", idstring);
	return;

timeout:
	printf(" but timed out after 2 seconds :-(\n");
}


static void cmd_gateway_packetstats(int argc, char *argv[])
{
	rs232can_msg *rmsg;

	//create and send gateway request
	printf("Pinging packet counters..\n");
	send_gateway_command(RS232CAN_PACKETCOUNTERS, NULL, 0);

	//anticipate reply and timeout after 2s
	rmsg = anticipate_gateway_reply(RS232CAN_PACKETCOUNTERS);
	if (!rmsg) goto timeout;

	//print answers
	printf("TX Packets:\t\t%u\n", *((uint32_t *)&rmsg->data[0]));
	printf("RX Packets:\t\t%u\n", *((uint32_t *)&rmsg->data[4]));
	printf("TX total payload data:\t%u bytes\n", *((uint32_t *)&rmsg->data[8]));
	printf("RX total payload data:\t%u bytes\n", *((uint32_t *)&rmsg->data[12]));
	return;

timeout:
	printf(" but timed out after 2 seconds :-(\n");
}

#define MCP251X_ERRFLG_EWARN	1
#define MCP251X_ERRFLG_RXWAR	2
#define MCP251X_ERRFLG_TXWAR	4
#define MCP251X_ERRFLG_RXEP	8
#define MCP251X_ERRFLG_TXEP	16
#define MCP251X_ERRFLG_TXBO	32
#define MCP251X_ERRFLG_RX0OVR	64
#define MCP251X_ERRFLG_RX1OVR	128

#define MCP251X_ERRFLG_EWARN_STR	"Error Warning "
#define MCP251X_ERRFLG_RXWAR_STR	"Receive Error Warning "
#define MCP251X_ERRFLG_TXWAR_STR	"Transmit Error Warning "
#define MCP251X_ERRFLG_RXEP_STR		"Receive Error-Passive "
#define MCP251X_ERRFLG_TXEP_STR		"Transmit Error-Passive "
#define MCP251X_ERRFLG_TXBO_STR		"Bus-Off Error "
#define MCP251X_ERRFLG_RX0OVR_STR	"Receive Buffer 0 Overflow "
#define MCP251X_ERRFLG_RX1OVR_STR	"Receive Buffer 1 Overflow "


static void sprint_mcp251x_errorflags(char *buf, unsigned char error_flags)
{
	sprintf(buf, "%s%s%s%s%s%s%s%s",
		(error_flags & MCP251X_ERRFLG_EWARN) ? MCP251X_ERRFLG_EWARN_STR : "",
		(error_flags & MCP251X_ERRFLG_RXWAR) ? MCP251X_ERRFLG_RXWAR_STR : "",
		(error_flags & MCP251X_ERRFLG_TXWAR) ? MCP251X_ERRFLG_TXWAR_STR : "",
		(error_flags & MCP251X_ERRFLG_RXEP) ? MCP251X_ERRFLG_RXEP_STR : "",
		(error_flags & MCP251X_ERRFLG_TXEP) ? MCP251X_ERRFLG_TXEP_STR : "",
		(error_flags & MCP251X_ERRFLG_TXBO) ? MCP251X_ERRFLG_TXBO_STR : "",
		(error_flags & MCP251X_ERRFLG_RX0OVR) ? MCP251X_ERRFLG_RX0OVR_STR : "",
		(error_flags & MCP251X_ERRFLG_RX1OVR) ? MCP251X_ERRFLG_RX1OVR_STR : "");
		
}

static void cmd_gateway_errorstats(int argc, char *argv[])
{
	rs232can_msg *rmsg;
	char buf[sizeof(MCP251X_ERRFLG_EWARN_STR) + sizeof(MCP251X_ERRFLG_RXWAR_STR) +
	     sizeof(MCP251X_ERRFLG_TXWAR_STR) + sizeof(MCP251X_ERRFLG_RXEP_STR) +
	     sizeof(MCP251X_ERRFLG_TXEP_STR) + sizeof(MCP251X_ERRFLG_TXBO_STR) +
	     sizeof(MCP251X_ERRFLG_RX0OVR_STR) + sizeof(MCP251X_ERRFLG_RX1OVR_STR)];

	//create and send gateway request
	printf("Requesting error counters..\n");
	send_gateway_command(RS232CAN_ERRORCOUNTERS, NULL, 0);

	//anticipate reply and timeout after 2s
	rmsg = anticipate_gateway_reply(RS232CAN_ERRORCOUNTERS);
	if (!rmsg) goto timeout;

	//print answers
	printf("TX Errors:\t%u\n", (unsigned char)rmsg->data[0]);
	printf("RX Errors:\t%u\n", (unsigned char)rmsg->data[1]);
	sprint_mcp251x_errorflags(buf, rmsg->data[2]);
	printf("Error Flags:\t%s\n", buf);
	return;

timeout:
	printf(" but timed out after 2 seconds :-(\n");
}


//convert an adc voltage to millivolts
static inline double adc_to_mv(double adc_resolution, double reference_mv, double value)
{
	return (value * reference_mv) / adc_resolution;
}


//compute how much millivolts are going into a voltage divider using r1, r2 and output millivolts
static inline double voltage_divider_mv_in(double r1, double r2, double outmv)
{
	return (outmv * (r1 + r2)) / r2;
}


static void cmd_gateway_powerdraw(int argc, char *argv[])
{
	rs232can_msg *rmsg;
	double v, ma;

	//create and send gateway request
	printf("Requesting power draw..\n");
	send_gateway_command(RS232CAN_POWERDRAW, NULL, 0);

	//anticipate reply and timeout after 2s
	rmsg = anticipate_gateway_reply(RS232CAN_POWERDRAW);
	if (!rmsg) goto timeout;

	//print answers
	v = voltage_divider_mv_in(VOLTAGE_DIVIDER_R1, VOLTAGE_DIVIDER_R2, adc_to_mv(ADC_RES, ADC_REF_MV, *((uint16_t *)&rmsg->data[0]))) / 1000.0;
	ma = adc_to_mv(ADC_RES, ADC_REF_MV, *((uint16_t *)&rmsg->data[2])) / (VSHUNT_AMPLIFICATION * SHUNT_OHMS);
	printf("Voltage:\t%2.3lfV\n", v);
	printf("Current:\t%4.1lfmA\n", ma);
	printf("Power:\t\t%2.3lfW\n", (ma * v) / 1000.0);
	printf("Bandgap:\t%u (10-Bit ADC)\n", *((uint16_t *)&rmsg->data[4]));
	printf("GND:\t\t%u (10-Bit ADC)\n", *((uint16_t *)&rmsg->data[6]));
	return;

timeout:
	printf(" but timed out after 2 seconds :-(\n");
}

static void cmd_gateway_control(int argc, char *argv[])
{
	rs232can_msg *rmsg;

	if (argc == 2)
	{
		uint8_t val = atoi(argv[1]);

		//create and send gateway request
		printf("Writing control register..\n");
		send_gateway_command(RS232CAN_WRITE_CTRL_REG, &val, 1);

		//anticipate reply and timeout after 2s
		rmsg = anticipate_gateway_reply(RS232CAN_WRITE_CTRL_REG);
	}
	else
	{
		//create and send gateway request
		printf("Reading control register..\n");
		send_gateway_command(RS232CAN_READ_CTRL_REG, NULL, 0);

		//anticipate reply and timeout after 2s
		rmsg = anticipate_gateway_reply(RS232CAN_READ_CTRL_REG);
	}

	if (!rmsg) goto timeout;

	//print new value
	printf("Control register value:\t%02x\n", (unsigned char)rmsg->data[0]);
	return;

timeout:
	printf(" but timed out after 2 seconds :-(\n");
}


#define MEGA8_RESETCAUSE_PORF 	1
#define MEGA8_RESETCAUSE_EXTRF	2
#define MEGA8_RESETCAUSE_BORF	4
#define MEGA8_RESETCAUSE_WDRF	8
#define RESETCAUSE_PORF_STR "power_on "
#define RESETCAUSE_EXTRF_STR "reset_pin "
#define RESETCAUSE_BORF_STR "brown_out "
#define RESETCAUSE_WDRF_STR "watchdog "

static void sprint_atmega8_resetcause(char *buf, unsigned char reset_flags)
{
	sprintf(buf, "%s%s%s%s",
		(reset_flags & MEGA8_RESETCAUSE_PORF) ? RESETCAUSE_PORF_STR : "",
		(reset_flags & MEGA8_RESETCAUSE_EXTRF) ? RESETCAUSE_EXTRF_STR : "",
		(reset_flags & MEGA8_RESETCAUSE_BORF) ? RESETCAUSE_BORF_STR : "",
		(reset_flags & MEGA8_RESETCAUSE_WDRF) ? RESETCAUSE_WDRF_STR : "");
}


static void cmd_gateway_resetcause(int argc, char *argv[])
{
	rs232can_msg *rmsg;
	char buf[sizeof(RESETCAUSE_PORF_STR) + sizeof(RESETCAUSE_EXTRF_STR) + sizeof(RESETCAUSE_BORF_STR) + sizeof(RESETCAUSE_WDRF_STR)];

	//create and send gateway request
	printf("Requesting reset cause..\n");
	send_gateway_command(RS232CAN_GET_RESETCAUSE, NULL, 0);

	//anticipate reply and timeout after 2s
	rmsg = anticipate_gateway_reply(RS232CAN_GET_RESETCAUSE);
	if (!rmsg) goto timeout;

	//print answer
	sprint_atmega8_resetcause(buf, rmsg->data[0]);
	printf("Reset cause: %s\n", buf);
	return;

timeout:
	printf(" but timed out after 2 seconds :-(\n");
}

static void cmd_gateway_reset(int argc, char *argv[])
{
	//create and send gateway request
	printf("Reseting gateway ...\n");
	send_gateway_command(RS232CAN_RESET, NULL, 0);
}

static cmd_t gateway_cmds[] = {
	{ &cmd_gateway_ping, "ping", "ping", "ping the gateway itself (not a CAN ping)" },
	{ &cmd_gateway_version, "version", "version", "show firmware version" },
	{ &cmd_gateway_packetstats, "pstats", "pstats", "show packet counters" },
	{ &cmd_gateway_errorstats, "estats", "estats", "show MCP2515 error counters" },
	{ &cmd_gateway_powerdraw, "powerdraw", "powerdraw", "show current bus power draw" },
	{ &cmd_gateway_resetcause, "resetcause", "resetcause", "question gateway about the cause of its last reset" },
	{ &cmd_gateway_control, "control", "control [value]", "read or write control register (advanced users only!)" },
	{ &cmd_gateway_reset, "reset", "reset", "reset gateway" },
	{ NULL, NULL, NULL, NULL }
};


void cmd_gateway(int argc, char **argv)
{
	char *arg = argv[1];
	cmd_t *cmd;

	cmd = gateway_cmds;
	while (cmd->fkt && argc > 1) {
		fflush(stdout);
		if (strcmp(arg, cmd->cmd) == 0) {
			(*(cmd->fkt))(argc-1, &(argv[1]));
			goto done;
		}
		cmd++;
	}

	/* show help */
	printf( "\nUsage: lapcontrol [OPTIONS] gw <SUBCOMMAND>\n\n" );
	printf( "Available Subcommands:\n\n" );

	cmd = gateway_cmds;
	while (cmd->fkt) {
		printf( "   %-30s %s\n", cmd->sig, cmd->desc );
		cmd++;
	}
	printf( "\n" );

done:
	return;
}

