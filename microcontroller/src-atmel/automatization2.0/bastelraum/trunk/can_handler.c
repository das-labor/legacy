#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "config.h"
#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"
#include "io.h"
#include "Bastelcmd.h"

uint8_t myaddr;

void can_handler()
{
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()))			//get next canmessage in rx_msg
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
				case PORT_BASTEL:
					switch (rx_msg->data[0])
					{
						case C_SW:
							set_output(rx_msg->data[1], rx_msg->data[2]);
							break;
						case C_SW_MASTER:
							set_output_all(rx_msg->data[1]);
							break;
						case C_PWM:
							set_pwm(rx_msg->data[1], rx_msg->data[2]);
							break;
						case C_PWM_MASTER:
							set_pwm_all(rx_msg->data[1]);
							break;
						case C_SET_MOTION_T:
							eeprom_write_byte ((uint8_t *) EEPROM_MOTION_TRESH, rx_msg->data[2]);
							break;
						case C_GET_STATE:
							can_send_output_status();
							break;
					}
					break;
			}
		}
		can_free(rx_msg);
	}
}

void can_send_output_status()
{
	can_message *msg = can_buffer_get();
	msg->addr_src = myaddr;
	msg->port_src = 0x03;
	msg->addr_dst = 0x00;
	msg->port_dst = 0x00;
	msg->dlc = 4;
	msg->data[0] = get_outputs();
	msg->data[1] = get_pwm(F_PWM_FENSTER);
	msg->data[2] = get_pwm(F_PWM_MITTE);
	msg->data[3] = get_pwm(F_PWM_NISCHE);
	can_transmit(msg);
}


static const uint8_t EE_lap_addr EEMEM = EEPROM_LAP_ADDR;

void can_read_addr()
{
	myaddr = eeprom_read_byte(&EE_lap_addr);
}

