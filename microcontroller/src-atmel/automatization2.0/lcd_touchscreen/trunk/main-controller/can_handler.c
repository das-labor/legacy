
#ifdef AVR
	#include <avr/io.h>
	#include <avr/wdt.h>
	#include <avr/eeprom.h>
	#include <util/delay.h>
	#include "can/can.h"
	#include "can/lap.h"
#else
	#include <stdint.h>
	#include <stdio.h>
	uint8_t dummy;
	#define TCCR2 printf("reset\r\n"); dummy
	#define wdt_enable(a)
	#define eeprom_read_byte(a) 0x88
	#include "can_pc/can.h"
	#include "can_pc/lap.h"
#endif

#include "can_handler.h"

uint8_t myaddr;

void can_handler()
{
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()) != 0) //get next canmessage in rx_msg
	{
		if ((rx_msg->addr_dst == myaddr))
		{
			if (rx_msg->port_dst == PORT_MGT)
			{
				switch (rx_msg->data[0])
				{
					case FKT_MGT_RESET:
						TCCR2 = 0;
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
		if (rx_msg->port_dst == 0x37)
		{
			//printf("netvar received\r\n");
			netvar_received(rx_msg);
		}
	}
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(0x00);
}

