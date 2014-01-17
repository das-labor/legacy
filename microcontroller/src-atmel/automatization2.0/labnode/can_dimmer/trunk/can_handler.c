#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"
#include "config.h"

#include "dimmer.h"

uint8_t myaddr;

uint8_t status[10][10];

#define VIRT_PWM_MAXVAL 255 //4096

static uint8_t virt_pwm_dir = 0;
static uint8_t virt_pwm_val = VIRT_PWM_MAXVAL;
static uint8_t virt_stat = 0;

extern void start_counter(uint16_t countdown);
extern uint8_t get_counter_status(void);

static void virt_pwm_set_all(uint8_t val) {
	set_dimmer(0, val);
	set_dimmer(1, val);
	set_dimmer(2, val);
	set_dimmer(3, val);
}

void can_handler()
{
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()))			//get next canmessage in rx_msg
	{
		if ((rx_msg->addr_dst == myaddr))
		{
			PORTB ^= _BV(PB0);		//LAPNODE LED blink
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
							//check if the button was pressed in the last $seconds
							//if yes continue, if not just toggle the lights
							if (!get_counter_status()) {	//button wasn't pressed in the last $seconds
								//if virt_stat == 1 lights are on
								//if virt_stat == 2 lights are on
								//if virt_stat == 3 lights are on
								//if virt_stat == 0 lights are off
								if (virt_stat)	//turn all lamps off
								{
									enable_channel(0, 0);
									enable_channel(1, 0);
									enable_channel(2, 0);
									enable_channel(3, 0);
									virt_stat = 0;
								}
								else	//turn all lamps on
								{
									enable_channel(0, 1);
									enable_channel(1, 1);
									enable_channel(2, 1);
									enable_channel(3, 1);
									virt_stat = 3;
								}
							} else {
								switch (virt_stat++) {
								case 0:
								
									enable_channel(0, 1);
									enable_channel(1, 1);
									enable_channel(2, 1);
									enable_channel(3, 0);
									break;
								case 1:
							
									enable_channel(0, 0);
									enable_channel(1, 0);
									enable_channel(2, 0);
									enable_channel(3, 1);
									break;
								case 2:
								
									enable_channel(0, 1);
									enable_channel(1, 1);
									enable_channel(2, 1);
									enable_channel(3, 1);
									break;
								case 3:
								
									enable_channel(0, 0);
									enable_channel(1, 0);
									enable_channel(2, 0);
									enable_channel(3, 0);
									virt_stat = 0;
									break;
								}
							}
							can_send_status(rx_msg->addr_src);
							start_counter(305);	//countdown 5 seconds
							break;
						case 1://C_PWM:	set LAMP rx_msg->data[1] to rx_msg->data[2] 

							if (rx_msg->data[1] < NUM_DIMMER_CHANNELS)
							{
                                set_dimmer(rx_msg->data[1], rx_msg->data[2]);
								can_send_status(rx_msg->addr_src);
							}
							break;
						case 2://PWM_MOD

							if (virt_pwm_dir == 1)
							{
								if (virt_pwm_val == VIRT_PWM_MAXVAL)
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
							can_send_status(rx_msg->addr_src);
							break;
						case 3: //PWM_DIR
							if (virt_pwm_dir)
								virt_pwm_dir = 0;
							else
								virt_pwm_dir = 1;
							break;
						case 4: //C_TOGGLE
							if (rx_msg->data[1] < NUM_DIMMER_CHANNELS)
							{
									if (rx_msg->data[2])	//lamp on
										enable_channel((rx_msg->data[1]), 1);
									else
										enable_channel((rx_msg->data[1]), 0);
								can_send_status(rx_msg->addr_src);
							}
							break;
						case 5: //request state
							can_send_status(rx_msg->addr_src);
							break;
					}
				break;
				case 2:	// Port 2 for lamp control
					if (rx_msg->data[1] >= NUM_DIMMER_CHANNELS)	// skip if lamp index out of range
						return;

					switch (rx_msg->data[0]) {
						case 0: // Lamp ON/OFF
							enable_channel(rx_msg->data[1],rx_msg->data[2]);
							can_send_status(rx_msg->addr_src);
							break;

						case 1: // Lamp brightness
                            set_dimmer(rx_msg->data[1], rx_msg->data[2]); 
							can_send_status(rx_msg->addr_src);
							break;

						case 2: // send status
							can_send_status(rx_msg->addr_src);
							break;

						case 3: // All Lamp ON/OFF
							enable_channel(0,rx_msg->data[2]);
							enable_channel(1,rx_msg->data[2]);
							enable_channel(2,rx_msg->data[2]);
							enable_channel(3,rx_msg->data[2]);
							can_send_status(rx_msg->addr_src);
							break;

						case 4: // All Lamp brightness
                            virt_pwm_set_all(rx_msg->data[2]);
							can_send_status(rx_msg->addr_src);
							break;

						case 5: // set all bright diff
							set_dimmer(0, rx_msg->data[2]);
							set_dimmer(1, rx_msg->data[3]);
							set_dimmer(2, rx_msg->data[4]);
							set_dimmer(3, rx_msg->data[5]);
							can_send_status(0);
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

void can_send_status(uint8_t addr)
{
	can_message *msg = can_buffer_get();
	msg->addr_src = myaddr;
	msg->port_src = 0x03;
	msg->addr_dst = addr;
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

