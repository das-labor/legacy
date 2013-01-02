#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"

#include "config.h"


uint8_t myaddr;
void twi_get(uint8_t *p);
uint8_t status[10][10];

extern void can_handler()
{
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()) != 0)			//get next canmessage in rx_msg
	{
		if ((rx_msg->addr_dst == myaddr))
		{
		PORTB |= _BV(PB0);
			if (rx_msg->port_dst == PORT_MGT)
			{
				switch (rx_msg->data[0])
				{
					case FKT_MGT_RESET:
//						TCCR2 = 0;
						wdt_enable(0);
						while (1);
			
					case FKT_MGT_PING:
						msg.addr_src = myaddr;
						msg.addr_dst = rx_msg->addr_src;
						can_transmit(&msg);
						break;
				}
			}
			else if (rx_msg->port_dst == 0x02)
			{
				rfm12_tx(rx_msg->dlc, 0x24, rx_msg->data);
			}
		}
	}
}

void can_send(uint8_t *p)
{
	static can_message msg = {0x00, 0x00, 0x00, 0x00, 8, {0}};
	uint8_t i;
	for (i = 0; i < 8; i++)
		msg.data[i] = p[i];
	msg.addr_src = myaddr;
	can_transmit(&msg);
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(EEP_MY_ADDR);
}

