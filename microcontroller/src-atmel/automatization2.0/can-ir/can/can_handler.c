#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "can.h"
#include "can_handler.h"
#include "lap.h"
#include "../can-ir.h"
#include "../ir_code.h"

//teufel test code
//volume down = 010 100 010000
uint16_t ir_testTeufel2[] PROGMEM =
{PT_OFF, PT_ON, PT_OFF, PT_ON, PT_OFF, PT_OFF, PT_OFF, PT_ON, PT_OFF, PT_OFF, PT_OFF, PT_OFF}; 

//teufel test code
//volume up = 010 100 100000
uint16_t ir_testTeufel3[] PROGMEM =
{PT_OFF, PT_ON, PT_OFF, PT_ON, PT_OFF, PT_OFF, PT_ON, PT_OFF, PT_OFF, PT_OFF, PT_OFF, PT_OFF}; 

// nec test
// address 16bit          command 8bit - power
//                                  inverted 8bit cmd
// 0001 0000 1100 1000 + 1110 0001 (0001 1110)

uint16_t ir_acer_address[] PROGMEM =
{PNEC_AGC_BURST, PNEC_OFF, PNEC_OFF, PNEC_OFF, PNEC_ON, PNEC_OFF, PNEC_OFF, 
 PNEC_OFF, PNEC_OFF, PNEC_ON, PNEC_ON, PNEC_OFF, PNEC_OFF, PNEC_ON, PNEC_OFF,
 PNEC_OFF, PNEC_OFF};

uint16_t ir_acer_power[] PROGMEM =
{PNEC_ON, PNEC_ON, PNEC_ON, PNEC_OFF, PNEC_OFF, PNEC_OFF, PNEC_OFF, PNEC_ON,
 PNEC_OFF, PNEC_OFF, PNEC_OFF, PNEC_ON, PNEC_ON, PNEC_ON, PNEC_ON, PNEC_OFF,
 PNEC_OFF};


extern void can_handler()
{
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message *rx_msg;
	uint16_t code[68];
	if ((rx_msg = can_get_nb()) != 0)			//get next canmessage in rx_msg
	{
		if ((rx_msg->addr_dst == 0x10))
		{
			if (rx_msg->port_dst == PORT_MGT)
			{
				switch (rx_msg->data[0])
				{
				case FKT_MGT_RESET:
					TCCR2 = 0;
					wdt_enable(0);
					while(1);
			
				case FKT_MGT_PING:

					msg.addr_src = 0x10;
					msg.addr_dst = rx_msg->addr_src;
					can_transmit(&msg);
					break;
				}
			}
			else if (rx_msg->port_dst == PORT_REMOTE)
			{
				if (rx_msg->data[0] == 0)
				{
					if (rx_msg->data[1] == 0)
					{
						ir_sendCode(ir_testTeufel2, 23);
						_delay_ms(40);
						ir_sendCode(ir_testTeufel2, 23);
					}
					if (rx_msg->data[1] == 1)
					{
						ir_sendCode(ir_testTeufel3, 23);
						_delay_ms(40);
						ir_sendCode(ir_testTeufel3, 23);
					}
				}
				if (rx_msg->data[0] == 1)
				{
					if (rx_msg->data[1] == 0)
					{
						read_code_to_array(code, ir_acer_address, 0, 34);
						read_code_to_array(code, ir_acer_power, 34, 68);
						ir_sendCode(code, 67);
					}
				}
			}
		}
	}
}



