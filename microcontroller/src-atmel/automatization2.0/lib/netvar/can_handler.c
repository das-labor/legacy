
#ifdef AVR
	#include <avr/eeprom.h>
	#include "can/can.h"
	#include "can/lap.h"
#else
	#include <stdint.h>
	#include <stdio.h>
	#define eeprom_read_byte(a) 0x88
	#include "can_pc/can.h"
	#include "can_pc/lap.h"
#endif

#define CAN_HANDLER_C

#include "can_handler.h"
#include "netvar/netvar.h"
#include "hardware.h"


uint8_t myaddr;

void can_handler()
{
	//static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message * msg;
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
						hardware_reset();
						break;
					case FKT_MGT_PING:
						msg = can_buffer_get();
						msg->addr_src = myaddr;
						msg->addr_dst = rx_msg->addr_src;
						msg->port_src = PORT_MGT;
						msg->port_dst = PORT_MGT;
						msg->dlc = 1;
						msg->data[0] = FKT_MGT_PONG;
						can_transmit(msg);
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

