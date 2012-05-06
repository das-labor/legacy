#include "can.h"
#include "lap.h"
#include "borg_can.h"
#include "spi.h"
#include "../borg_hw/borg_hw.h"

#include <setjmp.h>

#ifdef __AVR__
#  include <avr/pgmspace.h>
#  include <avr/interrupt.h>
#  include <avr/eeprom.h>
#else
#  include "../compat/eeprom.h"
#endif

#include <string.h>

can_addr_t myaddr;
extern jmp_buf newmode_jmpbuf;

#ifdef LAP_TIME_EXTENSION
//variables to save the last received hours and  minutes
//(accessible via lap.h)
uint8_t lap_time_h, lap_time_m, lap_time_update = 0;
#endif

void bcan_init()
{
	spi_init();
	can_init();

	myaddr = eeprom_read_byte(0x00);
	pdo_message *msg = (pdo_message *)can_buffer_get();
	msg->addr_src = myaddr;
	msg->addr_dst = 0xff;
	msg->port_src = PORT_BORG;
	msg->port_dst = PORT_BORG;
	msg->dlc      = 4;
	msg->cmd      = FKT_BORG_INFO;
	msg->data[0]  = NUM_ROWS;
	msg->data[1]  = NUM_COLS;
	msg->data[2]  = NUMPLANE;

	can_transmit((can_message *)msg);
}

void process_mgt_msg(pdo_message *msg)
{
	pdo_message *rmsg;

	switch(msg->cmd) {
	case FKT_MGT_RESET:
#ifdef __AVR__
		timer0_off();
		cli();
		while(1);
#endif
		break;
	case FKT_MGT_PING:
		rmsg = (pdo_message *)can_buffer_get();
		rmsg->addr_dst = msg->addr_src;
		rmsg->addr_src = myaddr;
		rmsg->port_dst = msg->port_src;
		rmsg->port_src = msg->port_dst;
		rmsg->cmd = FKT_MGT_PONG;
		rmsg->dlc = 1;
		can_transmit((can_message *)rmsg);
		break;

	#ifdef LAP_TIME_EXTENSION
	//if we get a time reply, save it
	case FKT_MGT_TIMEREPLY:
		lap_time_h = msg->data[0];
		lap_time_m = msg->data[1];
		lap_time_update = 1;
		break;
	#endif
	}
}

void process_borg_msg(pdo_message *msg)
{
	unsigned char i, j;

	switch (msg->cmd) {
	case FKT_BORG_MODE:
		longjmp(newmode_jmpbuf, msg->data[0]);
		break;

	case FKT_BORG_SCROLLTEXT_RESET:
		for (i = 0; i < msg->dlc - 1; i++) {
			scrolltext_text[i] = msg->data[i];
		}
		scrolltext_text[i] = 0;
		break;

	case FKT_BORG_SCROLLTEXT_APPEND:
		j = 0;
		while (scrolltext_text[j])
			j++;

		for (i = 0; i < msg->dlc - 1; i++) {
			scrolltext_text[i + j] = msg->data[i];
		}
		scrolltext_text[i + j] = 0;
		break;

#ifdef Hansi_hat_gelernt_Werte_vorher_zu_definieren

	//========== blinkenstuff

	//clear the blinkenbackbuffer to color
	case FKT_BLINK_CLEARBUF:
			blink_clearbuf(msg->data[0]);
		break;

	//set auto position increment flag
	case FKT_BLINK_SETAUTOPOS:
			blink_setautopos(msg->data[0]);
		break;

	//set the current blinkenbuffer offset position
	case FKT_BLINK_SETPOS:
			blink_setpos(msg->data[0]);
		break;

	//puts the current blinkenbuffer to the frontbuffer
	case FKT_BLINK_SHOW:
			blink_show();
		break;

	//puts data into the blinkenbuffer
	case FKT_BLINK_DATA:
			blink_data(msg->data, msg->dlc - 1);
		break;
#endif
	}
}

void bcan_process_messages() {
	pdo_message *msg = (pdo_message*) can_get_nb();

	while (msg) {
		if (!msg)
			return;

		if (msg->addr_dst == myaddr && msg->port_dst == PORT_MGT)
			process_mgt_msg(msg);

		if (msg->addr_dst == myaddr && msg->port_dst == PORT_BORG)
			process_borg_msg(msg);

		msg = (pdo_message*) can_get_nb();
	}
}
