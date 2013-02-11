#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"
#include "../include/PowerCommander.h"
#include "virt_lamp.h"
#include "switch.h"
#include "i2c_funktionen.h"

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
			else if (rx_msg->port_dst == 1) // old proto
			{
				switch (rx_msg->data[0]) {
					case C_SW: // SET LAMP
						if (rx_msg->data[2] < F_SW_STATUS)
						{
							switch (rx_msg->data[1])
							{
								case SWL_TAFEL:
									set_lamp(ROOM_VORTRAG, 0, rx_msg->data[2]);
									break;
								case SWL_BEAMER:
									set_lamp(ROOM_VORTRAG, 1, rx_msg->data[2]);
									break;
								case SWL_SCHRANK:
									set_lamp(ROOM_VORTRAG, 2, rx_msg->data[2]);
									break;
								case SWL_FLIPPER:
									set_lamp(ROOM_VORTRAG, 3, rx_msg->data[2]);
									break;
								case SWL_LOUNGE:
									set_lamp(ROOM_LOUNGE, 0, rx_msg->data[2]); // XXX rm
									break;
								case SWL_KUECHE:
									set_lamp(ROOM_KUECHE, 0, rx_msg->data[2]);
									break;
								case SWA_BEAMER:
									set_lamp(ROOM_VORTRAG, 4, rx_msg->data[2]);
									break;
							}
						}
						else if (rx_msg->data[2] == F_SW_TOGGLE && rx_msg->data[1] == SWL_KUECHE)
						{
							set_lamp_all(ROOM_KUECHE, ((outputdata.ports >> SWL_KUECHE) & 0x01)^1);
						}
						break;
					case C_PWM: // PWM F_PWM_SET
						switch (rx_msg->data[2])
						{
							case F_PWM_SET:
								if (rx_msg->data[2] == 0)
								{
									switch (rx_msg->data[1])
									{
										case PWM_TAFEL:
											set_bright(ROOM_VORTRAG, 0, rx_msg->data[3]);
											break;
										case PWM_BEAMER:
											set_bright(ROOM_VORTRAG, 1, rx_msg->data[3]);
											break;
										case PWM_SCHRANK:
											set_bright(ROOM_VORTRAG, 2, rx_msg->data[3]);
											break;
										case PWM_FLIPPER:
											set_bright(ROOM_VORTRAG, 3, rx_msg->data[3]);
											break;
										case PWM_KUECHE:
											set_bright(ROOM_KUECHE, 0, rx_msg->data[3]);
											break;
									}
								}
								break;
							case F_PWM_MOD: // TODO
								dim_vortrag();
								break;
							case F_PWM_DIR: // TODO
								tog_dimdir_vortrag();
								break;
						}
						break;
					case C_VIRT: // VIRT
						switch (rx_msg->data[1])
						{
							case VIRT_VORTRAG:
								if (rx_msg->data[2] < F_SW_STATUS)
									set_lamp_all(ROOM_VORTRAG, rx_msg->data[2]);
								else if (rx_msg->data[2] == F_SW_TOGGLE)
									toggle_vortrag();
								break;
							case VIRT_VORTRAG_PWM:
								switch (rx_msg->data[2])
								{
									case F_PWM_SET:
										set_bright_all(ROOM_VORTRAG, rx_msg->data[3]);
										break;
									case F_PWM_MOD:
										dim_vortrag();
										break;
									case F_PWM_DIR:
										tog_dimdir_vortrag();
										break;
								}
								break;
						}
						break;
				}
			}
			else if (rx_msg->port_dst == 2)
			{
				switch (rx_msg->data[0])
				{
					case 0: // switch lamp
						if(rx_msg->data[1] < 4)
							set_lamp(ROOM_VORTRAG, rx_msg->data[1], rx_msg->data[2]);
						else
							set_lamp(ROOM_KUECHE, 0, rx_msg->data[2]);
						send_status(rx_msg->addr_src);
						break;
					case 1: // set brightness lamp
						if(rx_msg->data[1] < 4)
							set_bright(ROOM_VORTRAG, rx_msg->data[1], rx_msg->data[2]);
						else
							set_bright(ROOM_KUECHE, 0, rx_msg->data[2]);
						send_status(rx_msg->addr_src);
						break;
					case 2: // request status packet
						send_status(rx_msg->addr_src);
						break;
					case 3: // set all lamps
						set_lamp_all(ROOM_VORTRAG, rx_msg->data[2]);
						set_lamp_all(ROOM_KUECHE, rx_msg->data[2]);
						send_status(rx_msg->addr_src);
						break;
					case 4: // set brightness all lamps
						set_bright_all(ROOM_VORTRAG, rx_msg->data[2]);
						set_bright_all(ROOM_KUECHE, rx_msg->data[2]);
						send_status(rx_msg->addr_src);
						break;
				}
			}
			else if (rx_msg->port_dst == 3)
			{
				if (rx_msg->addr_src == 0x61) /* lounge lamp 1 */
					set_lounge_lamp_1(rx_msg->data[0]);

				if (rx_msg->addr_src == 0x60) /* lounge lamp 2 */
					set_lounge_lamp_2(rx_msg->data[0]);
			}
		}
		// sleepmode zustand abfangen
		else if (rx_msg->addr_src == 0x04 && rx_msg->port_dst == 0x01 && rx_msg->data[1] == 0x01)
		{
			// = rx_msg->data[1];
		}
		can_free(rx_msg);
	}
}

void send_status(uint8_t addr)
{
	can_message *msg = can_buffer_get();
	msg->data[0] = get_channel_status();
	msg->data[1] = get_channel_brightness(0); /* SWL_TAFEL */
	msg->data[2] = get_channel_brightness(1); /* SWL_BEAMER */
	msg->data[3] = get_channel_brightness(2); /* SWL_SCHRANK */
	msg->data[4] = get_channel_brightness(3); /* SWL_FLIPPER */
	msg->data[5] = get_channel_brightness(4); /* SWL_KUECHE */
	msg->addr_src = myaddr;
	msg->addr_dst = addr;
	msg->port_dst = 3;
	msg->port_src = 3;
	msg->dlc = 6;
	can_transmit(msg);
}

void read_can_addr()
{
	myaddr = 0x02;//eeprom_read_byte(0x00);
}

