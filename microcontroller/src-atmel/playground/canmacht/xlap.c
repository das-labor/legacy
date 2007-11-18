
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
#include "xlap.h"
#include "string.h"
#include "menu.h"
#include "stdlib.h"

uint8_t myaddr;
sensor_t sensor = {0, NULL, NULL};
//#################
TimerControlBlock switchtimer2;
//#################
void process_mgt_msg() {
	static can_message_t msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	switch (rx_msg.data[0]) {
		case FKT_MGT_RESET:
			TCCR2 = 0;
			wdt_enable(0);
			while(1);
		case FKT_MGT_PING:
			msg.addr_src = myaddr;
			msg.addr_dst = rx_msg.addr_src;
			can_put(&msg);
			break;
	}
}

void process_data() {
	sensor_t *nextsensor = &sensor;
	uint8_t *wert = malloc(sizeof(uint8_t[2]));
	wert[0] = rx_msg.data[1];
	wert[1] = rx_msg.data[2];

	can_setled(0, 1);
	AvrXDelay(&switchtimer2, 100);
	can_setled(0, 0);

	while (nextsensor->next != NULL) {
		if (nextsensor->typ == rx_msg.data[0]) {
			nextsensor->wert = wert;
			return;
		}
		nextsensor = nextsensor->next;
	}
	sensor_t *newsensor = malloc(sizeof(sensor_t));
	newsensor->typ = rx_msg.data[0];
	newsensor->wert = wert;
	newsensor->next = NULL;
	nextsensor->next = newsensor;
}

AVRX_GCC_TASKDEF(laptask, 55, 3) {
	while (1) {
		can_get();			//get next canmessage in rx_msg
		if((rx_msg.addr_dst == myaddr) || (rx_msg.addr_dst == 0xff)) {
			if(rx_msg.port_dst == PORT_MGT) {
				process_mgt_msg();
			}
			else if(rx_msg.port_dst == PORT_REMOTE) {  // Temperaturen empfangen
				process_data();
			}
		}
	}
}

void xlap_init() {
	myaddr = eeprom_read_byte(0x00);
	spi_init();
	can_init();
}
