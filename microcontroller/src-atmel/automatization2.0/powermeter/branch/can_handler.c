
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "can/can.h"
#include "can/lap.h"
#include "can_handler.h"

#include "led_driver.h"
#include "ursartC1_driver.h"
static uint8_t myaddr;

void can_handler()
{
	
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()) != 0) //get next canmessage in rx_msg
	{
#if DEBUGMODE
		sendUSARTC1_putstr("CAN\n\r");
#endif
		if ((rx_msg->addr_dst == myaddr))
		{
#if DEBUGMODE
			sendUSARTC1_putstr("myaddr ");	
			sendUSARTC1_char(myaddr);
			sendUSARTC1_putstr(" ");		

#endif
			if (rx_msg->port_dst == PORT_MGT)
			{
#if DEBUGMODE
			sendUSARTC1_putstr(" PORT_MGT\n\r");
#endif
				switch (rx_msg->data[0])
				{
					case FKT_MGT_RESET:
#if DEBUGMODE
						sendUSARTC1_putstr("reset\n\r");
#endif
						/*I/O Protection*/
						CCP = CCP_IOREG_gc;
						//Reset
						RST.CTRL = RST_SWRST_bm;
						while (1);
					case FKT_MGT_PING:
#if DEBUGMODE
						sendUSARTC1_putstr("ping\n\r");
#endif
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
			//netvar_received(rx_msg);
		}
		can_free(rx_msg);
	}
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(0x00);

#if DEBUGMODE
	sendUSARTC1_char(myaddr);
	sendUSARTC1_putstr(" \n");		
#endif
}

