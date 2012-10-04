#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"

#include "dimmer.h"

uint8_t myaddr;

void twi_get(uint8_t *p);
uint8_t status[10][10];

uint8_t virt_pwm_dir = 0;
uint8_t virt_pwm_val = 255;
uint8_t virt_stat = 0;

void virt_pwm_set_all(uint8_t val) {

	set_dimmer(0, val);
	set_dimmer(1, val);
	set_dimmer(2, val);
}


extern void can_handler()
{
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()) != 0)			//get next canmessage in rx_msg
	{
		if ((rx_msg->addr_dst == myaddr))
		{
			PORTB ^= _BV(PB0);
			if (rx_msg->port_dst == PORT_MGT)
			{
				switch (rx_msg->data[0])
				{
					case FKT_MGT_RESET:
//						TCCR2 = 0
						wdt_enable(0);
						while (1);
			
					case FKT_MGT_PING:

						msg.addr_src = myaddr;
						msg.addr_dst = rx_msg->addr_src;
						can_transmit(&msg);
						break;
				}
			}
/*		}
		if ((rx_msg->addr_dst == 2)) {*/
			if (rx_msg->port_dst == 1)
			{
				switch (rx_msg->data[0]) {
					case 0: //C_SW:
						if (virt_stat==0)
							virt_stat = 255;
						else 
							virt_stat = 0;
						set_dimmer(0, virt_stat);
						set_dimmer(1, virt_stat);
						set_dimmer(2, virt_stat);
						set_dimmer(3, virt_stat);
						break;
					case 1://C_PWM:
						if (rx_msg->data[1] < 4)
							set_dimmer(rx_msg->data[1], rx_msg->data[2]);
						else if (rx_msg->data[1] == 4)
							OCR0 = rx_msg->data[2];
						break;
					case 2://PWM_MOD

						if (virt_pwm_dir == 1)
						{
							if (virt_pwm_val == 255)
							{
								virt_pwm_dir = 0;
							} else
							{
								virt_pwm_set_all(++virt_pwm_val);
							}
						} else
						{

							if (virt_pwm_val == 0)
							{
								virt_pwm_dir = 1;
							} else
							{
								virt_pwm_set_all(--virt_pwm_val);
							}
						}
						break;
					case 3: //PWM_DIR
						if (virt_pwm_dir)
							virt_pwm_dir = 0;
						else
							virt_pwm_dir = 1;
						break;
				}
			}
		}
	}
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(0x00);
}

