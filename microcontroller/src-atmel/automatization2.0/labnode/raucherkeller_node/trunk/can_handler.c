#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h> 

#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"
#include "config.h"
#include "light.h"

uint8_t myaddr;

void can_handler()
{
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()) != 0)			//get next canmessage in rx_msg
	{
		if ((rx_msg->addr_dst == myaddr))
		{
			PORTB ^= _BV(PB0);		//CAN LED blink
			if (rx_msg->port_dst == PORT_MGT)
			{
				switch (rx_msg->data[0])
				{
					case FKT_MGT_RESET:
						wdt_enable(0);
						while (1);
					case FKT_MGT_PING:
						msg.addr_src = myaddr;
						msg.addr_dst = rx_msg->addr_src;
						can_transmit(&msg);
						break;
				}
			}
			if (rx_msg->port_dst == 1)
			{
				switch (rx_msg->data[0]) {
					case 1: //C_TOGGLE
						enable_channel((rx_msg->data[1]), rx_msg->data[2]?1:0);
						break;
					case 4: //request state
						send_status();
						break;
				}
			}
		}
	}
}

void send_status()
{
	static can_message msg = { 0x00 };
	msg.data[0] = get_channel_status();
	msg.addr_src = myaddr;
	can_transmit(&msg);
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(0x00);
}

