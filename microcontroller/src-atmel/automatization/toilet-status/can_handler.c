#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"
#include "protocol.h"
#include "lights.h"


static uint8_t myaddr;

extern void can_handler()
{
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()) == 0)
		return;
	
	if ((rx_msg->addr_dst != myaddr))
		return;

	if (rx_msg->port_dst == PORT_MGT)
	{
		switch (rx_msg->data[0])
		{
		case FKT_MGT_RESET:
			TCCR2 = 0;
			wdt_enable(0);
			while(1);
	
		case FKT_MGT_PING:

			msg.addr_src = myaddr;
			msg.addr_dst = rx_msg->addr_src;
			can_transmit(&msg);
			break;
		}
	} else if (rx_msg->port_dst == 1)
	{
		static can_message msg = {0, 0, 0x0, PORT_REMOTE, 6, {0}};
		switch (rx_msg->data[0])
		{
			case TOILET_SET_RAW:
				lights_set_raw (rx_msg->data[1]);
				break;

			default:
				PORTC &= 0xF0;
				break;
		}
	}
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(0x00);
}
