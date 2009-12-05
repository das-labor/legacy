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

#include "ir_code.h"

/*
 * IR codes are stored in an array of on/off pulse lengths.
 * As transmissions must always start with a one and consist
 * of alternating ones and zeroes, every even index in the
 * array will be a one-pulse and every odd array index
 * will be a zero-pulse.
 */

//test code
uint16_t ir_testCode[] = {100, 50, 200};

//teufel test code
//mute = 010 010 100 000
uint16_t ir_testTeufel[] =
{PT_OFF, PT_ON, PT_OFF, PT_OFF, PT_ON, PT_OFF, PT_ON, PT_OFF, PT_OFF, PT_OFF, PT_OFF, PT_OFF};


//set OC1B to input
#define FREQGEN_OFF() DDRB &= ~(_BV(1))

//set OC1B to output and off, reset timer count
#define FREQGEN_ON() PORTB &= ~(_BV(1)); TCNT1 = 0; DDRB |= _BV(1);

//infrared current code array
uint16_t *volatile ir_curCode;
//length of current code
volatile uint8_t ir_curCodeLen;
//index into current code
volatile uint8_t ir_curCodeIdx;
//ir tick counter
volatile uint16_t ir_tickCnt;

//setup timer interrupt for tick counting
//timer runs with full ioclk (hopefully 16MHz)
//timer overflow will be every 255 counts,
//resulting in an interrupt frequency of 62.5KHz
void timer0_init()
{
	//reset ir tick counter
	ir_tickCnt = 0;

	//setup timer
	TCCR0 = _BV(CS00); //clk / 1 (int call @ clk/256)
	TCNT0 = 0; //reset counter
	TIMSK |= _BV(TOIE0); //timer 0 overflow int on
	TIFR |= _BV(TOV0); //clear overflow flag
}

//setup timer1 for frequency generation
//our frequency is 38kHz, that's a 26µS period
//timer1 will be in CTC mode and toggle outputs on compare match,
//meaning we have to run @ double frequency
//the timer base frequency will be the full ioclk (hopefully 16MHz)
void ir_freqInit()
{
	//disable frequency output
	FREQGEN_OFF();
	
	//timer1 TOP = 210, we're toggling OC1B at TOP
	//so this is 38.095KHz
	OCR1A = 210;

	//toggle OC1A on match
	TCCR1A = _BV(COM1A0);
	
	//reset counter
	TCNT1 = 0;

	//CTC mode using OCR1A as TOP
	//ioclk / 1 -> full ioclk (hopefully 16MHz)
	TCCR1B = (_BV(WGM12) | _BV(CS10)); //counter is running now
}

//system initialization
void init()
{
	//disable ir code generator
	ir_curCodeLen = 0;
	ir_curCodeIdx = 255;
	ir_curCode = 0;
	
	//disable analog comparator (to save power)
	ACSR |= _BV(ACD);	
	
	//enable frequency generator
	ir_freqInit();
	
	//enable our tick counter / ir code generator
	timer0_init();
	
	//turn on interrupts
	sei();
}

//handle infrared code sending via timer 0
ISR(TIMER0_OVF_vect)
{	
	//see if current code index is below length
	if(ir_curCodeIdx < ir_curCodeLen)
	{
		//update tick counter
		ir_tickCnt++;
		
		//if tick count is reached
		if(ir_tickCnt >= ir_curCode[ir_curCodeIdx])
		{
			//advance to next pulse length
			ir_curCodeIdx++;
			
			//see if we have to switch on or off frequency generation
			if(ir_curCodeIdx & 0x01)
			{
				FREQGEN_OFF();
			}
			else
			{
				FREQGEN_ON();
			}
			
			//reset tick counter
			ir_tickCnt = 0;
		}				
	}
}

//this function converts a bit-encoded code into
//the internal format used by the ir sending function
//it takes an array where to place the code (must be large enough),
//the time in timer ticks (1 tick should be 16µS) for the on/off pulse
//combination of a one and a zero, the bitcode to convert and its length
//as parameters
//
//the bitcode is assumed to be in MSB first format
//
//the function will return the length of the generated
//code, which is always bitcode length * 2
uint8_t ir_genCode(uint16_t *destCode, uint16_t oneOntime, uint16_t oneOfftime, uint16_t zeroOntime, uint16_t zeroOfftime, uint32_t bitCode, uint8_t codeLen)
{
	uint8_t i;
	
	//convert bitcode
	for(i = 0; i < codeLen; i++)
	{
		if(bitCode & (uint32_t)((uint32_t)1 << (uint32_t)(codeLen-1)))
		{
			//encode a one
			destCode[i*2] = oneOntime;
			destCode[(i*2)+1] = oneOfftime;
		}
		else
		{
			//encode a zero
			destCode[i*2] = zeroOntime;
			destCode[(i*2)+1] = zeroOfftime;
		}
		
		bitCode <<= 1;
	}
	
	return (codeLen * 2) - 1;
}

//send an ir code, please never use a code length of zero
void ir_sendCode(uint16_t *code, uint8_t codeLen)
{
	//turn off code sending completely while modifying the code
	//this is done by setting the current index to 255
	//the interrupt will then always skip code sending
	ir_curCodeIdx = 255;
	
	//the last code to send may never be a zero
	//-> it would not make any sense
	//-> the frequency generation would give a carrier for one timer
	//   overflow after the last bit has been sent
	//this makes even numbers the next smaller uneven ones
	ir_curCodeLen = (codeLen-1) | 0x01;
	
	//save code pointer
	ir_curCode = code;
	
	//reset tick counter
	ir_tickCnt = 0;
	
	//reset timer count to be in sync
	TCNT0 = 0;
	
	//enable frequency generation
	//the first pulse will be a few atmega cycles short,
	//as we're starting after the counter has begun from BOTTOM
	FREQGEN_ON();
	
	//enable code sending by setting the index to zero
	ir_curCodeIdx = 0;
}

void read_code_to_array(uint16_t *array, uint16_t *pgmData, uint8_t pos, uint8_t len)
{
	uint8_t i;
	for (i = pos; pos < len; i++)
		array[i] = pgm_read_word(pgmData++);


}

int main(void)
{	
//	uint16_t teufelCode[24];
//	uint8_t teufelLen;
//	uint16_t NECCode[68];
//	uint8_t NECLen;
	
	//system initialization
	init();
	
	spi_init();
	
	can_init();
	
	//generate teufel code automatically
	//this code is evil, as PT_ON & PT_OFF
	//are actually four parameters disguised as two
	//volume down = 010 100 010000
//	teufelLen = ir_genCode(teufelCode, PT_ON, PT_OFF, 0b010100100000, 12);
	
	//this macro generates nec codes automatically
	//power 0001 0000 1100 1000 + 1110 0001 0001 1110
	//hint: the manually defined test nec code has an additional trailing zero
	//hint2: maximum bit length is 32
//	NECLen = IR_GEN_NECEXT(NECCode, 0b00010000110010001110000100011110, 15);
	
	//NECLen = ir_genCode((NECCode + 2), PNEC_ON, PNEC_OFF, 0b0001000011001000, 15) + 2;

	/*NECLen = 4;
	NECCode[0] = PNEC_AGC_ON;
	NECCode[1] = PNEC_AGC_OFF;
	NECCode[2] = PNEC_AGC_ON;
	NECCode[3] = PNEC_AGC_OFF;*/
	
	//test loop turns down volume
	while(1)
	{	
		can_handler();
		//remote control always sends the code twice with some delay
//		ir_sendCode(ir_testTeufel2, 23);
		// repeat delay for custom protocol SIGNAL MUST BE REPEATED !!
//		_delay_ms(40); // must be 35ms --- IMPORTANT 40ms are in real 35ms
//		ir_sendCode(ir_testTeufel2, 23);
//		_delay_ms(500);
//		ir_sendCode(NECCode, NECLen);
//		ir_sendCode(ir_test_nec, 67);
	}
}
