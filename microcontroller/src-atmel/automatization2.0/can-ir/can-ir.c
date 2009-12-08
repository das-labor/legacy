//      can-ir.c
//      
//      Authors: Hans-Gert Dahmen  <sexyludernatascha@gmail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "can/can.h"
#include "can/spi.h"
#include "can/lap.h"
#include "lab-irkit.h"

/*
 * IR codes are stored in an array of on/off pulse lengths.
 * As transmissions must always start with a one and consist
 * of alternating ones and zeroes, every even index in the
 * array will be a one-pulse and every odd array index
 * will be a zero-pulse.
 */

//having these arrays global seems to solve problems
uint16_t code[128];
uint8_t codeLen;

//message handler
//(to be beautified [with lookup tables])
void can_handler()
{
	static can_message msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	can_message *rx_msg;

	//get next canmessage in rx_msg that is destined for us
	if(((rx_msg = can_get_nb()) != 0) && (rx_msg->addr_dst == 0x10))
	{
		//handle management functions
		if(rx_msg->port_dst == PORT_MGT)
		{
			switch(rx_msg->data[0])
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

				default:
					break;
			}
		}
		//handle ir commands
		else if(rx_msg->port_dst == PORT_REMOTE)
		{
			//switch the remote device type
			switch(rx_msg->data[0])
			{
				//this is a message for the teufel system
				case 0:
					//see which code we need to send
					switch (rx_msg->data[1])
					{
						//volume down
						case 0:	
							codeLen = ir_genCode(code, PT_ON, PT_OFF, 0b010100010000, 12);
							ir_sendCode(code, codeLen);
							_delay_ms(40);
							ir_sendCode(code, codeLen);
							break;

						//volume up
						case 1:
							codeLen = ir_genCode(code, PT_ON, PT_OFF, 0b010100100000, 12);
							ir_sendCode(code, codeLen);
							_delay_ms(40);
							ir_sendCode(code, codeLen);

						default:
							break;
					}
					break;

				//this is a message for the acer beamer
				case 1:
					//see which code we need to send
					switch (rx_msg->data[1])
					{
						//power
						case 0:
							codeLen = ir_genENEC(code, 0b00010000110010001110000100011110, 32);
							ir_sendCode(code, codeLen);
							break;

						default:
							break;
					}
					break;

				default:
					break;
			}
		}
	}
}

//system initialization
void init()
{
	//disable analog comparator (to save power)
	ACSR |= _BV(ACD);

	//initialize ir subsystem
	ir_init();

	//initialize spi port
	spi_init();
	
	//initialize can communication
	can_init();
	
	//turn on interrupts
	sei();
}
	 
int main(void)
{	
	//system initialization
	init();
		
	//the main loop continuously handles can messages
	while(1)
	{	
		can_handler();
	}
}
