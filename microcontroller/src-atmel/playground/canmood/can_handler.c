#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"
#include "pwm.h"
#include "static_scripts.h"

static uint8_t myaddr;

extern void can_handler()
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
				static can_message msg = {0, 0, 0x0, PORT_REMOTE, 6, {0}};
				switch (rx_msg->data[0])
				{
					case 0x00: //FKT_MOOD_SET_B
						global_pwm.channels[rx_msg->data[1]].target_brightness = rx_msg->data[2];
						break;
					case 0x01://speed
						global_pwm.channels[rx_msg->data[1]].speed = 100 * rx_msg->data[2];
						break;
					case 0x03://FKT_MOOD_GET_B
						msg.addr_src = myaddr;
						msg.addr_dst = rx_msg->addr_src;
						msg.dlc = 3;
						msg.data[0] = global_pwm.channels[0].brightness;
						msg.data[1] = global_pwm.channels[1].brightness;
						msg.data[2] = global_pwm.channels[2].brightness;
						can_transmit(&msg);
						break;
				}
			}
		}
#ifdef LABOR_MOOD
		if (rx_msg->addr_src == 0x02 && rx_msg->addr_dst == 0x00 && rx_msg->port_dst == 0x01)
		{
			if (!(rx_msg->data[0] & 0x01))
			{
				script_threads[0].flags.disabled = 1;
				global_pwm.channels[0].target_brightness = 0;
				global_pwm.channels[1].target_brightness = 0;
				global_pwm.channels[1].target_brightness = 0;
			}
			if (rx_msg->data[0] & 0x01)
				script_threads[0].flags.disabled = 0;
		}
#endif
	}
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(0x00);
}

