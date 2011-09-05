
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "can/can.h"
#include "can/lap.h"

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
						/*I/O Protection*/
						CCP = CCP_IOREG_gc;
						//Reset
						RST.CTRL = RST_SWRST_bm;
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
		can_free(rx_msg);
	}
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(0x00);
}

