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
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "can/can.h"
#include "can/spi.h"
#include "can/can_handler.h"
#include "lab-irkit.h"

/*
 * IR codes are stored in an array of on/off pulse lengths.
 * As transmissions must always start with a one and consist
 * of alternating ones and zeroes, every even index in the
 * array will be a one-pulse and every odd array index
 * will be a zero-pulse.
 */

/*
//teufel test code
//mute = 010 010 100 000
uint16_t ir_testTeufel[] =
{PT_OFF, PT_ON, PT_OFF, PT_OFF, PT_ON, PT_OFF, PT_ON, PT_OFF, PT_OFF, PT_OFF, PT_OFF, PT_OFF};
*/

//system initialization
void init()
{
	//disable analog comparator (to save power)
	ACSR |= _BV(ACD);

	//disable ir code generator
	ir_disable();
	
	//enable frequency generator
	ir_freqInit();
	
	//enable our tick counter / ir code generator
	ir_timer0Init();
	
	//turn on interrupts
	sei();
}

void read_code_to_array(uint16_t *array, uint16_t *pgmData, uint8_t pos, uint8_t len)
{
	uint8_t i;
	for (i = pos; pos < len; i++)
		array[i] = pgm_read_word(pgmData++);


}

//having these arrays global seems to solve the problem
uint16_t code[128];
uint8_t codeLen;
	 
int main(void)
{	
	//system initialization
	init();
	
	spi_init();
	
	can_init();
	
	//generate teufel code automatically
	//this code is evil, as PT_ON & PT_OFF
	//are actually four parameters disguised as two
	//volume down = 010 100 010000
	codeLen = ir_genCode(code, PT_ON, PT_OFF, 0b010100100000, 12);
	
	//this macro generates nec codes automatically
	//power 0001 0000 1100 1000 + 1110 0001 0001 1110	
	codeLen = ir_genENEC(code, 0b00010000110010001110000100011110, 32);
		
	//test loop turns down volume
	while(1)
	{	
		//can_handler();
		
		//remote control always sends the code twice with some delay
		//ir_sendCode(teufelCode, teufelLen);
		
		// repeat delay for custom protocol SIGNAL MUST BE REPEATED !!
		//_delay_ms(40); // must be 35ms --- IMPORTANT 40ms are in real 35ms
		
		ir_sendCode(code, codeLen);
		
		_delay_ms(500);
	}
}
