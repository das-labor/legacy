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

void can_handler()
{
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()))	// get next canmessage in rx_msg
	{
		if (rx_msg->addr_dst == myaddr)
		{
			switch (rx_msg->port_dst)
			{
				case PORT_MGT:
					switch (rx_msg->data[0])
					{
						case FKT_MGT_RESET:
							wdt_enable(WDTO_15MS);
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
					break;
				case 1: // old proto
					switch (rx_msg->data[0])
					{
						case C_SW: // 0 SET LAMP
							if (rx_msg->data[2] < F_SW_STATUS || rx_msg->data[2] == 0x23)
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
									/*case SWL_LOUNGE:
										set_lamp(ROOM_LOUNGE, 0, rx_msg->data[2]); // XXX rm
										break;*/
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
								toggle_kueche();
							}
							break;
						case C_PWM: // PWM 0x01
							switch (rx_msg->data[2])
							{
								case F_PWM_SET:
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
									break;
								case F_PWM_MOD: // 0x02
									switch (rx_msg->data[1])
									{
										case PWM_KUECHE:
											dim_kueche();
											break;
									}
									break;
								case F_PWM_DIR: // 0x03
									switch (rx_msg->data[1])
									{
										case PWM_KUECHE:
											tog_dimdir_kueche();
											break;
									}
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
					break;
				case 2:
					switch (rx_msg->data[0])
					{
						case 0: // switch lamp
							if (rx_msg->data[1] < 5)
								set_lamp(ROOM_VORTRAG, rx_msg->data[1], rx_msg->data[2]);
							else
								set_lamp(ROOM_KUECHE, 0, rx_msg->data[2]);
							break;
						case 1: // set brightness lamp
							if (rx_msg->data[1] < 4)
								set_bright(ROOM_VORTRAG, rx_msg->data[1], rx_msg->data[2]);
							else
								set_bright(ROOM_KUECHE, 0, rx_msg->data[2]);
							break;
						case 2: // request status packet
							can_send_output_status();
							break;
						case 3: // set all lamps - XXX so ist das doch doof und ergibt keinen sinn
							set_lamp_all(ROOM_VORTRAG, rx_msg->data[2]);
							//set_lamp_all(ROOM_KUECHE, rx_msg->data[2]);
							break;
						case 4: // set brightness all lamps - XXX und dat auch
							set_bright_all(ROOM_VORTRAG, rx_msg->data[2]);
							//set_bright_all(ROOM_KUECHE, rx_msg->data[2]);
							break;
					}
					break;
			}
		}
		// sleepmode zustand abfangen vom hauptschalter gehäuse
		/*else if (rx_msg->addr_src == 0x04 && rx_msg->port_src == 0x01 && rx_msg->data[1] == 0x01)
		{
			// = rx_msg->data[1];
		}*/
		else if (rx_msg->addr_src == 0x61 && rx_msg->port_src == 0x03) { // lounge lamp 1
			set_lounge_lamp_1(rx_msg->data[0]);
			lamploungepwm[0] = rx_msg->data[1];
			lamploungepwm[1] = rx_msg->data[2];
			lamploungepwm[2] = rx_msg->data[3];
			lamploungepwm[3] = rx_msg->data[4];
		}
		else if (rx_msg->addr_src == 0x60 && rx_msg->port_src == 0x03) { // lounge lamp 2
			set_lounge_lamp_2(rx_msg->data[0]);
			lamploungepwm[4] = rx_msg->data[1];
			lamploungepwm[5] = rx_msg->data[2];
			lamploungepwm[6] = rx_msg->data[3];
			lamploungepwm[7] = rx_msg->data[4];
		}
		can_free(rx_msg);
	}
}

void can_send_input_stat(uint8_t pos, uint8_t val) {
	can_message *msg = can_buffer_get();
	msg->addr_src = myaddr;
	msg->port_src = 0x03;
	msg->addr_dst = 0x00;
	msg->port_dst = 0x00;
	msg->dlc = 2;
	msg->data[0] = val;
	msg->data[1] = pos;
	can_transmit(msg);
}

void can_send_output_status()
{
	can_message *msg = can_buffer_get();
	msg->addr_src = myaddr;
	msg->port_src = 0x02;
	msg->addr_dst = 0x00;
	msg->port_dst = 0x00;
	msg->dlc = 7;
	msg->data[0] = get_channel_status();
	msg->data[1] = get_output_status();
	msg->data[2] = get_channel_brightness(PWM_TAFEL);
	msg->data[3] = get_channel_brightness(PWM_BEAMER);
	msg->data[4] = get_channel_brightness(PWM_SCHRANK);
	msg->data[5] = get_channel_brightness(PWM_FLIPPER);
	msg->data[6] = get_channel_brightness(PWM_KUECHE);
	//msg->data[7] = get_channel_brightness(PWM_LOUNGE);
	can_transmit(msg);
}

static const uint8_t EE_lap_addr EEMEM = EEPROM_LAP_ADDR;

void read_can_addr()
{
	myaddr = eeprom_read_byte(&EE_lap_addr);
}
