#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h> 

#include "lcap/can/can.h"
#include "lcap/can_handler.h"
#include "lcap/can/lap.h"

#include "dimmer.h"

uint8_t myaddr;

void twi_get(uint8_t *p);
uint8_t status[10][10];

#define VIRT_PWM_MAXVAL 255 //4096
#define NUM_CHANNELS 4

uint8_t virt_pwm_dir = 0;
uint8_t virt_pwm_val = VIRT_PWM_MAXVAL;
uint8_t virt_stat = 0;

extern void start_counter(uint16_t countdown);
extern uint8_t get_counter_status(void);

extern void enable_channel(uint8_t channel, uint8_t enable);

//lookuptable for gamma corretiob
const uint8_t exptab[256] PROGMEM =
{
0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,5,5,
5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,7,7,7,7,7,7,8,8,8,8,8,8,9,9,9,9,9,10,10,10,
10,10,11,11,11,11,12,12,12,12,13,13,13,14,14,14,14,15,15,15,16,16,16,17,
17,18,18,18,19,19,20,20,21,21,21,22,22,23,23,24,24,25,25,26,27,27,28,28,
29,30,30,31,32,32,33,34,35,35,36,37,38,39,39,40,41,42,43,44,45,46,47,48,
49,50,51,52,53,55,56,57,58,59,61,62,63,65,66,68,69,71,72,74,76,77,79,81,
82,84,86,88,90,92,94,96,98,100,102,105,107,109,112,114,117,119,122,124,
127,130,133,136,139,142,145,148,151,155,158,162,165,169,172,176,180,184,
188,192,196,201,205,210,214,219,224,229,234,239,244,250,255,
};

void virt_pwm_set_all(uint8_t val) {
	//do gammacorrection
	
	set_dimmer(0, val);
	set_dimmer(1, val);
	set_dimmer(2, val);
	set_dimmer(3, (255-pgm_read_byte(exptab+255-val)));
}


extern void can_handler()
{
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message *rx_msg;
	if ((rx_msg = can_get_nb()) != 0)			//get next canmessage in rx_msg
	{
		if ((rx_msg->addr_dst == myaddr))
		{
			PORTB ^= _BV(PB0);		//CAN LED blink
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
					case 0: //C_SW: ALL ON/ ALL OFF
						//check if the button was pressed in the last $seconds
						//if yes continue, if not just toggle the lights
						if(!get_counter_status()){	//button wasn't pressed in the last $seconds
							//if virt_stat == 1 lights are on
							//if virt_stat == 2 lights are on
							//if virt_stat == 3 lights are on
							//if virt_stat == 0 lights are off
							if(virt_stat)	//turn all lamps off
							{
								enable_channel(0,0);
								enable_channel(1,0);
								enable_channel(2,0);
								enable_channel(3,0);
								virt_stat=0;
							}
							else	//turn all lamps on
							{
								enable_channel(0,1);
								enable_channel(1,1);
								enable_channel(2,1);
								enable_channel(3,1);
								virt_stat=3;
							}
							
						}else{
							switch (virt_stat++){
							case 0:
								
								enable_channel(0,1);
								enable_channel(1,1);
								enable_channel(2,1);
								enable_channel(3,0);
								break;
							case 1:
							
								enable_channel(0,0);
								enable_channel(1,0);
								enable_channel(2,0);
								enable_channel(3,1);
								break;
							case 2:
								
								enable_channel(0,1);
								enable_channel(1,1);
								enable_channel(2,1);
								enable_channel(3,1);
								break;
							case 3:
								
								enable_channel(0,0);
								enable_channel(1,0);
								enable_channel(2,0);
								enable_channel(3,0);
								virt_stat = 0;
								break;
							}
						}
						start_counter(305);	//countdown 5 seconds
						break;
					case 1://C_PWM:	set LAMP rx_msg->data[1] to rx_msg->data[2] 

						if (rx_msg->data[1] < NUM_CHANNELS)
						{
							if (rx_msg->data[1] == 3)	//channel 3
								set_dimmer(3, 255-rx_msg->data[2]);	//invert neon tube
							else	
								set_dimmer(rx_msg->data[1], rx_msg->data[2]);
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
						break;
					case 3: //PWM_DIR
						if (virt_pwm_dir)
							virt_pwm_dir = 0;
						else
							virt_pwm_dir = 1;
						break;
					
					case 4: //C_TOGGLE
						if (rx_msg->data[1] < NUM_CHANNELS)
						{
								if(rx_msg->data[2])	//lamp on
									enable_channel((rx_msg->data[1]),1);
								else
									enable_channel((rx_msg->data[1]),0);
						}
						break;					
					
				}
			}
		}
	}
}

void can_send(uint8_t port, uint8_t *p)
{
	static can_message msg = {0xa9, 0x00, 0x00, 0x01, 1, {0}};
	uint8_t i;
	for (i = 0; i < 2; i++)
		msg.data[i] = p[i];
	msg.addr_src = myaddr;
	msg.port_dst = port;
	can_transmit(&msg);
}

void read_can_addr()
{
	myaddr = eeprom_read_byte(0x00);
}

