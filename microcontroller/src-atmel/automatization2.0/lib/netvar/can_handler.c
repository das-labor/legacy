
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
	can_message *tx_msg;
	can_message *rx_msg;
	while ((rx_msg = can_get_nb())) //get next canmessage in rx_msg
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
						tx_msg = can_buffer_get();
						tx_msg->addr_src = myaddr;
						tx_msg->addr_dst = rx_msg->addr_src;
						tx_msg->port_src = PORT_MGT;
						tx_msg->port_dst = PORT_MGT;
						tx_msg->dlc = 1;
						tx_msg->data[0] = FKT_MGT_PONG;
						can_transmit(tx_msg);
						break;
				}
			}
		}
		if (rx_msg->port_dst == PORT_NETVAR)
		{
			netvar_received(rx_msg);
		}
		can_free(rx_msg);
	}
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(EEP_MY_ADDR);
}

