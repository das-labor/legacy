#include <avr/pgmspace.h>
#include <stdlib.h>

#include "uart/uart.h"
#include "can/can.h"

static const char power_on[] PROGMEM = "OKOKOKOKOK\r";
static const char power_off[] PROGMEM = "* 0 IR 002\r";
static const char query_lamp_status[] PROGMEM = "* 0 Lamp ?\r";
static const char query_lamp_hours[] PROGMEM = "* 0 Lamp\r";
static const char query_video_source[] PROGMEM = "* 0 Src ?\r";
static const char query_video_res[] PROGMEM = "* 0 IR 036\r";
static const char source[] PROGMEM = "* 0 IR 031\r";
static const char source_vga[] PROGMEM = "* 0 IR 015\r";
static const char source_dvi[] PROGMEM = "* 0 IR 016\r";
static const char source_svideo[] PROGMEM = "* 0 IR 018\r";
static const char source_composite[] PROGMEM = "* 0 IR 019\r";
static const char blank[] PROGMEM = "* 0 IR 030\r";
/*static const char [] PROGMEM = "";
static const char [] PROGMEM = "";
static const char [] PROGMEM = "";*/

static const PGM_P acer_cmds[] = {
	power_on,
	power_off,
	source,
	source_vga,
	source_dvi,
	source_svideo,
	source_composite,
	blank,
	query_lamp_status,
	query_lamp_hours,
	query_video_source
};

enum {
	POWER_ON = 1,
	POWER_OFF,
	SOURCE,
	SOURCE_VGA,
	SOURCE_DVI,
	SOURCE_SVIDEO,
	SOURCE_COMPOSITE,
	BLANK,
	QUERY_LAMP_STATUS,
	QUERY_LAMP_HOURS,
	QUERY_VIDEO_SOURCE,
	RS232_CMD_COUNT
} rs232_cmd;


static void read_string(char *buf, size_t i)
{
	// Lese die Adresse des i-ten Strings aus array[]
	//const char *parray = (const char*) pgm_read_word (&acer_cmds[i]);
	// Kopiere den Inhalt der Zeichenkette vom Flash ins RAM
	strcpy_P(buf, acer_cmds[i]);
}


static uint8_t lamp_status = 0;
static uint8_t last_cmd = 0;


void rs232_send_command(uint8_t cmd)
{
	char buf[RS232_CMD_COUNT];

	if (cmd < RS232_CMD_COUNT) {
		if (cmd == 0) {
			if (lamp_status)
				cmd = 1;
		}
		else
			cmd += 1;
		read_string(buf, cmd);
		uart_putstr(buf);
		last_cmd = cmd;
	}
}

void lap_send_msg(uint8_t type, uint8_t data)
{
	can_message *tx_msg = can_buffer_get();
	tx_msg->addr_src = 0x10;
	tx_msg->port_src = 0x05;
	tx_msg->addr_dst = 0;
	tx_msg->port_dst = 0;
	tx_msg->dlc = 2;
	tx_msg->data[0] = type;
	tx_msg->data[1] = data;
	can_transmit(tx_msg);
}

/*
 rueckgabe strings
 "Res XXXXX"
 "Lamp X" (0 / 1)
 "Src X" (0 - 7)
 "XXXX"
*/
static void parse_string(char *buf)
{
	switch (buf[0]) {
		/*case 'R':
			if (buf[1] == 'e' && buf[2] == 's') {

			}
			break;*/
		case 'L':
			if (buf[1] == 'a' && buf[2] == 'm' && buf[3] == 'p') {
				if (buf[5] == '0')
					lamp_status = 0;
				else if (buf[5] == '1')
					lamp_status = 1;
				lap_send_msg(1, lamp_status);
			}
			break;
		case 'S':
			if (buf[1] == 'r' && buf[2] == 'c') {
				lap_send_msg(2, buf[4] - 0x30); // ascii
			}
			break;
		default:
			if ((buf[0] >= 0x30) && buf[0] <= 0x39) {
				lap_send_msg(3, atoi(buf)); //
			}
			break;
	}
}

void rs232_receive_handler(void)
{
	char c;
	static char ret_buf[12];
	static uint8_t i = 0;

	if (uart_getc_nb(&c)) {
		ret_buf[i++] = c;
		if (c == '\r' || i == 11) {
			i = 0;
			parse_string(ret_buf);
		}
	}
}

static uint8_t shutdown_progress;


void poll_beamer_state(void)
{
	static uint16_t beamer_poll_delay;
	if (beamer_poll_delay++ > 3000) {
		rs232_send_command(QUERY_LAMP_STATUS);
		if (shutdown_progress && !lamp_status)
			rs232_send_command(QUERY_VIDEO_SOURCE);
		beamer_poll_delay = 0;
	}
}


static void sendBeamerPowerOffMsg(void)
{
	can_message *tx_msg = can_buffer_get();
	tx_msg->addr_src = 0x10;
	tx_msg->port_src = 0x00;
	tx_msg->addr_dst = 0x02;
	tx_msg->port_dst = 0x01;
	tx_msg->dlc = 3;
	tx_msg->data[0] = 0x00; // C_SW
	tx_msg->data[1] = 0x01; // SWA_BEAMER
	tx_msg->data[2] = 0x23; // AUS - special value
	can_transmit(tx_msg);
}

/* while cooling Lamp 0 + Src 0
 * after cooling Lamp 0 + Src 1
 */

static uint8_t shutdown_progress;

void start_shutdown(void)
{
	shutdown_progress = 1;
}
