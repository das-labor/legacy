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
#include <avr/eeprom.h>

#include "can/can.h"
#include "can/spi.h"
#include "can/lap.h"
#include "lab-irkit.h"
#include "uart/uart.h"
#include "beamer_rs232.h"
#include "twi_master/twi_master.h"
#include "teufel-i2c.h"

//having these arrays global seems to solve problems
static uint16_t code[128];
static uint8_t codeLen;
static uint8_t myaddr;

//these are the commando codes for the teufel system
static uint16_t teufelCodes[] =
{
	//volume down
	0b010100010000,
	//volume up
	0b010100100000,
	//mute
	0b010010100000,
	//main
	0b010001100000,
	//front
	0b010001010000,
	//rear
	0b010001001000,
	//side
	0b010001000100,
	//sub
	0b010010010000,
	//center
	0b010010001000,
};

//the length of the teufel code array
#define TEUFEL_CODE_CNT (sizeof(teufelCodes) / sizeof(uint16_t))

//message handler
//(to be beautified [with lookup tables])
static void can_handler(void)
{
	can_message *rx_msg;

	//get next canmessage in rx_msg that is destined for us
	if ((rx_msg = can_get_nb())) {
		if (rx_msg->addr_dst == myaddr) {
			//handle management functions
			if (rx_msg->port_dst == PORT_MGT) {
				switch (rx_msg->data[0]) {
					case FKT_MGT_RESET:
						wdt_enable(WDTO_15MS);
						while (1);
					case FKT_MGT_PING: {
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
					default:
						break;
				}
			}
			//handle ir commands
			else if (rx_msg->port_dst == PORT_REMOTE) { // 0x21
				//switch the remote device type
				switch (rx_msg->data[0]) {
					//this is a message for the teufel system
					case 0:
						//verify if command number is within bounds
						if (rx_msg->data[1] < TEUFEL_CODE_CNT) {
							//lookup command and generate the pulse length array
							codeLen = ir_genCode(code, PT_ON, PT_OFF, teufelCodes[rx_msg->data[1]], 12);

							//send code and repeat it after 35ms
							//to please the teufel system
							ir_sendCode(code, codeLen);
							_delay_ms(40); //is in reality 35ms
							ir_sendCode(code, codeLen);
						}
						break;
					//this is a message for the acer beamer
					case 1:
						PORTD |= _BV(PD7); // Enable debug LED
						rs232_send_command(rx_msg->data[1]);
						break;
					// 2 was beamer IR
					case 3:
						setAllChannels(rx_msg->data[1]);
						break;
					case 4:
						setSingleChannel(rx_msg->data[1], rx_msg->data[2]);
						break;
				}
			}
		}
		can_free(rx_msg);
	}
}

static volatile uint8_t tickscounter;

//system initialization
static void init(void)
{
	//disable analog comparator (to save power)
	ACSR |= _BV(ACD);

	//debug LED output
	DDRD |= _BV(PD7);

	//initialize ir subsystem
	ir_init();

	//initialize spi port
	spi_init();

	// initialize uart
	uart_init();

	// initialize twi
	if (!TWIM_Init())
	{
		while (1);
	}

	myaddr = eeprom_read_byte(0x00);
	//initialize can communication
	can_init();

	//turn on interrupts
	sei();
}

int main(void)
{
	//system initialization
	init();

	setDefaultAfterPoweron();

	//the main loop continuously handles can messages
	while (1) {
		can_handler();
		rs232_receive_handler();
		if (tickscounter > 10) {
			tickscounter = 0;
			poll_beamer_state();
		}
		PORTD &= ~_BV(PD7); // Disable debug LED
	}
}
