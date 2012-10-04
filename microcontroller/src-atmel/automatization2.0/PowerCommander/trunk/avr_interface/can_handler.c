#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"
#include "twi_master/twi_master.h"
#include "../include/PowerCommander.h"
#include "i2c_funktionen.h"


uint8_t myaddr;

void can_handler()
{
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()) != 0)			//get next canmessage in rx_msg
	{
		if (rx_msg->addr_dst == myaddr)
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
			else if (rx_msg->port_dst == 1)
			{
				switch (rx_msg->data[0]) {
					case C_SW: // switch // eingangs variable füllen  <<
						//rx_msg->data[1];
						
						break;
					case C_PWM: // PWM F_PWM_SET
						switch (rx_msg->data[2]) {
							case F_PWM_SET:
								if (rx_msg->data[1] < 6)
									outputdata.pwmval[rx_msg->data[1]] = rx_msg->data[3];
								break;
							case F_PWM_MOD:
								break;
							case F_PWM_DIR:
								break;
						}
						break;
					case C_VIRT: // VIRT
						switch (rx_msg->data[1]) {
							case VIRT_VORTRAG:
								//exec
								break;
							case VIRT_VORTRAG_PWM:
								
								break;
						}
						break;
				}
			}
		}
		// sleepmode zustand abfangen
		if (rx_msg->addr_src == 0x04 && rx_msg->port_dst == 0x01 && rx_msg->data[1] == 0x01) {
			// = rx_msg->data[1];
		}
	}
}

void can_send(uint8_t port, uint8_t *p)
{
	static can_message msg = {0x03, 0x00, 0x00, 0x01, 1, {0}};
	uint8_t i;
	for (i = 0; i < 1; i++)
		msg.data[i] = p[i];
	msg.addr_src = myaddr;
	msg.port_dst = port;
	can_transmit(&msg);
}

void can_send2(uint8_t p)
{
	static can_message msg = {0x03, 0x00, 0x00, 0x01, 1, {0}};

	msg.data[0] = p;
	msg.addr_src = myaddr;
	msg.addr_dst = 0x6f;
	msg.port_dst = 1;
	can_transmit(&msg);
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(0x00);
}

