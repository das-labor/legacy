/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"

static uint8_t myaddr;

extern void can_handler()
{
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()) != 0) //get next canmessage in rx_msg
	{
		if (rx_msg->addr_dst == myaddr)
		{
			if (rx_msg->port_dst == PORT_MGT)
			{
				switch (rx_msg->data[0])
				{
					case FKT_MGT_RESET:
						//TCCR2 = 0;
						wdt_enable(0);
						while (1);
			
					case FKT_MGT_PING:
						msg.addr_src = myaddr;
						msg.addr_dst = rx_msg->addr_src;
						can_transmit(&msg);
						break;
				}
			}
		}
	}
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(0x00);
}

void can_send(uint8_t port, uint8_t *p)
{
	static can_message msg = {0x30, 0x30, 0x01, 0x01, 2, {0}};
	uint8_t i;
	for (i = 0; i < 3; i++)
		msg.data[i] = p[i];
	msg.addr_src = myaddr;
	msg.port_dst = port;
	can_transmit(&msg);
}

