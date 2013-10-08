#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"
#include "animationen.h"

uint8_t myaddr;

void can_handler()
{
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()))			//get next canmessage in rx_msg
	{
		if (rx_msg->addr_dst == myaddr)
		{
			if (rx_msg->port_dst == PORT_MGT)
			{
				switch (rx_msg->data[0])
				{
					case FKT_MGT_RESET:
						wdt_enable(0);
						while (1);
					case FKT_MGT_PING:
						{
							can_message *tx_msg = can_buffer_get();
							tx_msg->port_src = PORT_MGT;
							tx_msg->port_dst = PORT_MGT;
							tx_msg->addr_src = myaddr;
							tx_msg->addr_dst = rx_msg->addr_src;
							tx_msg->dlc = 1;
							tx_msg->data[0] = FKT_MGT_PONG;
							can_transmit(tx_msg);
						}
						break;
				}
			}
			else if (rx_msg->port_dst == 0x0f)
			{
				switch(rx_msg->data[0]) { // alte nummerierung in neue
					case 4:
						animation = 0;
						break;
					case 5:
						animation = 1;
						break;
					case 8:
						animation = 2;
						break;
				}
			}
		}
		can_free(rx_msg);
	}
}


void send_status(void)
{
	can_message *msg = can_buffer_get();
	msg->data[0] = animation;
	msg->addr_src = myaddr;
	msg->addr_dst = 0x00;
	msg->port_dst = 0x00;
	msg->port_src = 0x0f;
	msg->dlc = 1;
	can_transmit(msg);
}

static const uint8_t EE_lap_addr EEMEM = EE_LAP_ADDR;

void read_can_addr()
{
	myaddr = eeprom_read_byte(&EE_lap_addr);
}

