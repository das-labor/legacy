#include <avr/pgmspace.h>
#include <stdlib.h>

#include "uart/uart.h"
#include "can_handler.h"


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
//	POWER_ON = 0,
//	POWER_OFF,
	SOURCE = 1,
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

static void beamer_nachlauf_detection(uint8_t src);

static void read_string(char *buf, size_t i)
{
	// Lese die Adresse des i-ten Strings aus array[]
	//const char *parray = (const char*) pgm_read_word (&acer_cmds[i]);
	// Kopiere den Inhalt der Zeichenkette vom Flash ins RAM
	strcpy_P(buf, acer_cmds[i]);
}


static uint8_t lamp_status = 0, video_source = 0;


void beamer_send_command(uint8_t cmd)
{
	char buf[12];

	if (cmd < RS232_CMD_COUNT) {
		if (cmd == 0) { // power off
			if (lamp_status)
				cmd = 1; // power on
		}
		else
			cmd += 1;
		read_string(buf, cmd);
		uart_putstr(buf);
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
		case '*':
			/*if (buf[4] == '0')
				printf("ack\n");
			else if (buf[4] == '1')
				printf("nack\n");*/
			break;
		default:
			if (((buf[0] >= 0x30) && buf[0] <= 0x39) && (buf[1] >= 0x30) && buf[1] <= 0x39) {
				buf[4] = '\0';
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

void set_beamer_power(uint8_t status)
{
	beamer_power = status;
}

void beamer_poll_state(void)
{
	static uint16_t beamer_poll_delay = 0;
	if (beamer_power && beamer_poll_delay++ > 50) {
		beamer_poll_delay = 0;
		beamer_send_command(QUERY_LAMP_STATUS);
		if (shutdown_progress && !lamp_status)
			beamer_send_command(QUERY_VIDEO_SOURCE);
	}
}

/*
 * while cooling Lamp 0 + Src 0
 * after cooling Lamp 0 + Src 1
 */

static void beamer_nachlauf_detection(uint8_t src) {
	if (shutdown_progress && !lamp_status && src)
	{
		lap_switch_beamer_relais(0);
		PORTD ^= _BV(PD7); // XXX
		shutdown_progress = 0;
	}
}

void beamer_start_shutdown(void)
{
	if (beamer_power && lamp_status) {
		beamer_send_command(0);
		shutdown_progress = 1;
	}
}
