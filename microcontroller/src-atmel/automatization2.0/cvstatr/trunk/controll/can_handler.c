#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "can_handler.h"
#include "can/can.h"
#include "can/lap.h"
#include "i2c_temp.h"
#include "temp_regler.h"

uint8_t myaddr;

void can_handler()
{
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message *rx_msg;
	uint8_t data[2] = {0, 0};
	if ((rx_msg = can_get_nb()) != 0) //get next canmessage in rx_msg
	{
		if (rx_msg->addr_dst == myaddr)
		{
			switch (rx_msg->port_dst)
			{
				case PORT_MGT:
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
					break;
				/*case 0x04:
					temp_soll = rx_msg->data[0];
					break;
				case 0x05:
					set_ventil(rx_msg->data[0]);
					break; */
				case 0x10:
					get_temp(data, 0x96);
					msg.data[0] = data[0];
					msg.data[1] = data[1];
					msg.dlc = 2;
					msg.port_dst = 0x10;
					msg.port_src = 0x10;
					msg.addr_src = myaddr;
					msg.addr_dst = rx_msg->addr_src;
					can_transmit(&msg);
				break;
			}
		}
		/*if (rx_msg->addr_dst == 0x23 && rx_msg->port_dst == 0x10)
		{
			temp_ist = rx_msg->data[0];
		} */
	}
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(EEPROM_CAN_ADDR);
}

