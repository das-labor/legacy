#include <stdlib.h>
#include <avr/pgmspace.h>

#include "uart/uart.h"
#include "can_handler.h"
/*
 * TODO
 * on / off with power
 */

static const char query_lamp_status[] PROGMEM = "Lamp ?";
static const char query_lamp_hours[] PROGMEM = "Lamp";
static const char query_video_source[] PROGMEM = "Src ?";


static const PGM_P acer_query[] = {
	query_lamp_status,
	query_lamp_hours,
	query_video_source
};

enum {
	QUERY_LAMP_STATUS,
	QUERY_LAMP_HOURS,
	QUERY_VIDEO_SOURCE,
	ACER_QUERY_COUNT,
} e_acer_query;

static const uint8_t acer_cmds[] PROGMEM = {1, 2, 47, 30, 31, 15, 18, 19, 50, 8, 9, 10, 11, 12};

enum {
	ON,
	OFF,
	BLANK,
	SOURCE,
	SOURCE_VGA,
	SOURCE_SVIDEO,
	SOURCE_COMPOSITE,
	SOURCE_HDMI,
	MENU,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	E_KEY,
	ACER_CMD_COUNT,
} e_acer_cmd;

#define CMD_ON_OFF 0

static void beamer_nachlauf_detection(uint8_t src);


static uint8_t lamp_status = 0, video_source = 0, last_cmd = 0xff;

void beamer_send_command(uint8_t cmd)
{
	if (cmd < ACER_CMD_COUNT) {
		if (cmd == CMD_ON_OFF) { // power on / off
			if (lamp_status)
				cmd = pgm_read_word(&acer_cmds[OFF]); // power off
		}
		else
			cmd += 1;

		uart_putstr_P(PSTR("* 0 IR 0"));
		uart_putc(pgm_read_word(&acer_cmds[cmd]) / 10 + 0x30);
		uart_putc(pgm_read_word(&acer_cmds[cmd]) % 10 + 0x30);
		uart_putc('\r');
	}
}

static void beamer_query(uint8_t query)
{
	if (query < ACER_QUERY_COUNT) {
		char buf[8];
		strcpy_P(buf, acer_query[query]);
		uart_putstr_P(PSTR("* 0 "));
		uart_putstr(buf);
		uart_putc('\r');
		last_cmd = query;
	}
}

/*
 rueckgabe strings
 "Res XXXXX"
 "Lamp X" (0 / 1) "*000\r  Lamp 0\r" 2A 30 30 30 0D 4C 61 6D 70 20 30 0D
 "Src X" (0 - 7)
 "XXXX"
*/
static void parse_string(char *buf)
{
	switch (buf[0]) {
		case 'R': // Resolution
			if (buf[1] == 'e' && buf[2] == 's') {

			}
			break;
		case 'L': // Lamp X\r
			if (buf[1] == 'a' && buf[2] == 'm' && buf[3] == 'p') {
				if (buf[5] == '0')
					lamp_status = 0;
				else if (buf[5] == '1')
					lamp_status = 1;
				lap_send_beamer_status(1, 2, lamp_status);
			}
			break;
		case 'S': // Src X\r
			if (buf[1] == 'r' && buf[2] == 'c') {
				video_source = buf[4] - 0x30;  // ascii to dec
				lap_send_beamer_status(2, 2, video_source);
				beamer_nachlauf_detection(video_source);
			}
			break;
		case '*': // CMD ack: "*000\r" nack: "*001\r"
			lap_send_beamer_status(5, 2, buf[3] - 0x30);
			if ((last_cmd == QUERY_VIDEO_SOURCE) && (buf[3] - 0x30 == 1)) {
				beamer_nachlauf_detection(1);
			}
			last_cmd = 0xff;
			break;
		default: // "XXXX" lamp hours
			if (((buf[0] >= 0x30) && buf[0] <= 0x39) && (buf[1] >= 0x30) && buf[1] <= 0x39) {
				buf[4] = '\0'; // replace \r with \0 for atoi
				lap_send_beamer_status(3, 3, atoi(buf)); // XXX  16 bit  lamp time
			}
			break;
	}
}

void beamer_receive_handler(void)
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

static uint8_t shutdown_progress = 0;
static uint8_t beamer_power;

void set_status_beamer_power(uint8_t status)
{
	beamer_power = status;
}

void beamer_poll_state(void)
{
	static uint16_t beamer_poll_delay = 0;
	if (beamer_power && beamer_poll_delay++ > 50) {
		beamer_poll_delay = 0;
		beamer_query(QUERY_LAMP_STATUS);
	}
	if (shutdown_progress && (!lamp_status) && (beamer_poll_delay == 20)) {
		beamer_query(QUERY_VIDEO_SOURCE);
		last_cmd = QUERY_VIDEO_SOURCE;
	}
}

/*
 * while cooling Lamp 0 + Src 0
 * after cooling Lamp 0 + Src 1 - old beamer
 * new: after cooling Lamp 0 + Src ? returns nack
 */

static void beamer_nachlauf_detection(uint8_t src)
{
	if (shutdown_progress && !lamp_status && src) {
		lap_switch_beamer_relais(0);
		shutdown_progress = 0;
	}
}

void beamer_start_shutdown(void)
{
	if (beamer_power && lamp_status) // check if beamer has power and is running
		beamer_send_command(CMD_ON_OFF); // send off cmd
	shutdown_progress = 1;
}
