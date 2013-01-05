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
#include "virt_lamp.h"

uint8_t myaddr;

void send_status(uint8_t addr);

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
			else if (rx_msg->port_dst == 2)
			{
				switch (rx_msg->data[0]) {
					case 0: // switch lamp
						if(rx_msg->data[1] < 4)
							set_lamp(ROOM_VORTRAG,rx_msg->data[1],rx_msg->data[2]);
						else
							set_lamp(ROOM_KUECHE,0,rx_msg->data[2]);
						send_status(rx_msg->addr_src);
						break;
					case 1: // set brightness lamp
						if(rx_msg->data[1] < 4)
							set_bright(ROOM_VORTRAG,rx_msg->data[1],rx_msg->data[2]);
						else
							set_bright(ROOM_KUECHE,0,rx_msg->data[2]);
						send_status(rx_msg->addr_src);
						break;
					case 2: // request status packet
						send_status(rx_msg->addr_src);
						break;
					case 3: // set all lamps
						set_lamp_all(ROOM_VORTRAG,rx_msg->data[2]);
						set_lamp_all(ROOM_KUECHE,rx_msg->data[2]);
						send_status(rx_msg->addr_src);
						break;
					case 4: // set brightness all lamps
						set_bright_all(ROOM_VORTRAG,rx_msg->data[2]);
						set_bright_all(ROOM_KUECHE,rx_msg->data[2]);
						send_status(rx_msg->addr_src);
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

void send_status(uint8_t addr)
{
	static can_message msg = {0x00, 0x00, 0x03, 0x03, 6, {0}};
	msg.data[0] = get_channel_status();
	msg.data[1] = get_channel_brightness(0); /* SWL_TAFEL */
	msg.data[2] = get_channel_brightness(1); /* SWL_BEAMER */
	msg.data[3] = get_channel_brightness(2); /* SWL_SCHRANK */
	msg.data[4] = get_channel_brightness(3); /* SWL_FLIPPER */
	msg.data[5] = get_channel_brightness(4); /* SWL_KUECHE */
	msg.addr_src = myaddr;
	msg.addr_dst = addr;
	can_transmit(&msg);
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(0x00);
}

