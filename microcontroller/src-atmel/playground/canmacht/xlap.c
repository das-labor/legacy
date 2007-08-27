
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
#include "string.h"
#include "menu.h"
#include "stdlib.h"

uint8_t myaddr;


void process_mgt_msg() {
	static can_message_t msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	switch(rx_msg.data[0]) {
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
	if(rx_msg.data[0] == 0x10)
		
	// if temp = tempbuffer  		// ariatne 
typedef struct {
		uint8_t typ;
		uint8_t *wert;
		uint_t *next;
	} sensor_t;
	
	sensor sensor_t = 0;

	addsensor( wert, typ) {
		sensor_t nextsensor = sensor;
		sensor_t newsensor;
		if (sensor != 0) {
			while (nextsensor != 0)
				nextsensor = sensor.next;
		{
		newsensor.typ = ;
		newsensor.wert = ;
		newsensor.next = 0;
		sensor->next = newsensor;
	}
	
	
	//AvrXSetSemaphore(&men_mutex);
}

AVRX_GCC_TASKDEF(laptask, 55, 3) {
	while (1) {
		can_get();			//get next canmessage in rx_msg
		if(rx_msg.addr_dst == myaddr) {
			if(rx_msg.port_dst == PORT_MGT) {
				process_mgt_msg();
			}
			else if(rx_msg.port_dst == PORT_REMOTE) {  // Temperaturen empfangen  -  getrennt empfangen - port?
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
