
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
			msg.data[0] = PORT_TEMP;
			can_put(&msg);
			break;
		case FKT_MGT_WREEPR0M:
			eeprom_write_byte(rx_msg.data[1], rx_msg.data[2]);
			break;
	}
}

AVRX_GCC_TASKDEF(laptask, 55, 3) {
	while (1) {
		can_get();			//get next canmessage in rx_msg
		if (rx_msg.addr_dst == myaddr) {
			if (rx_msg.port_dst == PORT_MGT) {
				process_mgt_msg();	
			}
		}
	}
};

void xlap_init() {
	myaddr = eeprom_read_byte(0x00);
	spi_init();
	can_init();
}
