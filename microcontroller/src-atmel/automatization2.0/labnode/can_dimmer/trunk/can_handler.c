#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"
#include "config.h"

#include "dimmer.h"

uint8_t myaddr;

static void virt_pwm_set_all(uint8_t val)
{
	set_dimmer(0, val);
	set_dimmer(1, val);
	set_dimmer(2, val);
	set_dimmer(3, val);
}


void can_handler()
{
	can_message *rx_msg;

	if ((rx_msg = can_get_nb()))			// get next canmessage in rx_msg
	{
		if (rx_msg->addr_dst == myaddr)
		{
			PORTB ^= _BV(PB0);		// LAPNODE LED blink on packet
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
				case 1:
					switch (rx_msg->data[0]) {
						case 0: //C_SW: ALL ON/ ALL OFF
							virt_pwm_set_all(rx_msg->data[2]);
							can_send_status();
							break;
						case 1://C_PWM:	set LAMP rx_msg->data[1] to rx_msg->data[2] 

							if (rx_msg->data[1] < NUM_DIMMER_CHANNELS)
							{
								set_dimmer(rx_msg->data[1], rx_msg->data[2]);
								can_send_status();
							}
							break;
						case 5: //request state
							can_send_status();
							break;
					}
					break;
				case 2:	// Port 2 for lamp control
					if (rx_msg->data[1] >= NUM_DIMMER_CHANNELS)	// skip if lamp index out of range
						return;

					switch (rx_msg->data[0]) {
						case 0: // Lamp ON/OFF
							enable_channel(rx_msg->data[1], rx_msg->data[2]);
							can_send_status();
							break;

						case 1: // Lamp brightness
							set_dimmer(rx_msg->data[1], rx_msg->data[2]);
							can_send_status();
							break;

						case 2: // send status
							can_send_status();
							break;

						case 3: // All Lamp ON/OFF
							enable_channel(0, rx_msg->data[2]);
							enable_channel(1, rx_msg->data[2]);
							enable_channel(2, rx_msg->data[2]);
							enable_channel(3, rx_msg->data[2]);
							can_send_status();
							break;

						case 4: // All Lamp brightness
							virt_pwm_set_all(rx_msg->data[2]);
							can_send_status();
							break;

						case 5: // set all bright diff
							set_dimmer(0, rx_msg->data[2]);
							set_dimmer(1, rx_msg->data[3]);
							set_dimmer(2, rx_msg->data[4]);
							set_dimmer(3, rx_msg->data[5]);
							can_send_status();
							break;

						case 255: //received status packet
							/* nothing to do here */
							break;
					}
				break;
			}
		}
		can_free(rx_msg);
	}
}

void can_send_status()
{
	can_message *msg = can_buffer_get();
	msg->addr_src = myaddr;
	msg->port_src = 0x03;
	msg->addr_dst = 0x00;
	msg->port_dst = 0x00;
	msg->dlc = 5;
	msg->data[0] = get_channel_status();
	msg->data[1] = dim_vals_8bit[0];
	msg->data[2] = dim_vals_8bit[1];
	msg->data[3] = dim_vals_8bit[2];
	msg->data[4] = dim_vals_8bit[3];
	can_transmit(msg);
}

static const uint8_t EE_lap_addr EEMEM = EEPROM_LAP_ADDR;

void can_read_addr()
{
	myaddr = eeprom_read_byte(&EE_lap_addr);
}

