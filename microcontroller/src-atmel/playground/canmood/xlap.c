
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/eeprom.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "config.h"
#include "spi.h"
#include "xcan.h"
#include "lap.h"
#include "pwm.h"
#include "string.h"
#include "mood.h"

uint8_t myaddr;

void process_mgt_msg() {
	static can_message_t msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	switch (rx_msg.data[0]) {
		case FKT_MGT_RESET:
			wdt_enable(0);
			while(1);
		case FKT_MGT_PING:
			msg.addr_src = myaddr;
			msg.addr_dst = rx_msg.addr_src;
			can_put(&msg);
			break;
		case FKT_MGT_DESC:
			msg.addr_src = myaddr;
			msg.addr_dst = rx_msg.addr_src;
			msg.dlc = 1;
			msg.data[0] = PORT_MOOD;
			can_put(&msg);
			break;
		case FKT_WR_ADDR:
			eeprom_write_byte(0, rx_msg.data[1]);
			break;
	}	
}

void process_mood_msg() {
	static can_message_t msg = {0, 0, PORT_MOOD, PORT_REMOTE, 6, {0}};
	switch(rx_msg.data[0]) {
		case FKT_MOOD_SET_B:
			global_pwm.channels[rx_msg.data[1]].target_brightness = rx_msg.data[2];
			break;
		case FKT_SPEED:
			global_pwm.channels[rx_msg.data[1]].speed = 100 * rx_msg.data[2];
			break;
		case FKT_MOOD_GET_B:
			msg.addr_src = myaddr;
			msg.addr_dst = rx_msg.addr_src;
			msg.dlc = 3;
			msg.data[0] = global_pwm.channels[0].brightness;
			msg.data[1] = global_pwm.channels[1].brightness;
			msg.data[2] = global_pwm.channels[2].brightness;
			can_put(&msg);
			break;
	}
}

void process_prog_msg() {
	uint8_t i;
	mprog = rx_msg.data[0];
	if (!mprog) {
		AvrXSuspend(PID(mood));
		for (i = 0; i < 3; i++) {
			global_pwm.channels[i].speed = 0x00200;
			global_pwm.channels[i].target_brightness = 0;
		}
	}
	else {
		AvrXResume(PID(mood));
	}
}

AVRX_GCC_TASKDEF(laptask, 55, 3) {
	while (1) {
		can_get();			//get next canmessage in rx_msg
		if (rx_msg.addr_dst == myaddr) {
			if (rx_msg.port_dst == PORT_MGT) {
				process_mgt_msg();	
			}
			else if (rx_msg.port_dst == PORT_MOOD) {
				process_mood_msg();
			}
			else if (rx_msg.port_dst == 0x06) {
				process_prog_msg();
			}
		}
	}
};

void xlap_init() {
	myaddr = eeprom_read_byte(0x00);
	spi_init();
	can_init();
}
