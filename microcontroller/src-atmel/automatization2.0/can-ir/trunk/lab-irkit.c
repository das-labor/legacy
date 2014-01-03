//      lab-irkit.c
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


/*
 * IR codes are stored in an array of on/off pulse lengths.
 * As transmissions must always start with a one and consist
 * of alternating ones and zeroes, every even index in the
 * array will be a one-pulse and every odd array index
 * will be a zero-pulse.
 */

#include "lab-irkit.h"
#include <avr/interrupt.h>


//infrared current code array
static uint16_t *volatile ir_curCode;
//length of current code
static volatile uint8_t ir_curCodeLen;
//index into current code
static volatile uint8_t ir_curCodeIdx;
//ir tick counter
static volatile uint16_t ir_tickCnt;




//setup timer interrupt for tick counting
//timer runs with full ioclk (hopefully 16MHz)
//timer overflow will be every 255 counts,
//resulting in an interrupt frequency of 62.5KHz
static void ir_timer0Init(void)
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
static void ir_freqInit(void)
{
	//disable frequency output
	FREQGEN_OFF();

	//timer1 TOP = 210, we're toggling OC1B at TOP
	//so this is 38.095KHz
	OCR1A = 210;

	//toggle OC1A on match  - not needet here only doku
	//TCCR1A = _BV(COM1A0);

	//reset counter
	TCNT1 = 0;

	//CTC mode using OCR1A as TOP
	//ioclk / 1 -> full ioclk (hopefully 16MHz)
	TCCR1B = (_BV(WGM12) | _BV(CS10)); //counter is running now
}

//handle infrared code sending via timer 0
ISR(TIMER0_OVF_vect)
{	
	//see if current code index is below length
	if (ir_curCodeIdx < ir_curCodeLen)
	{
		//update tick counter
		ir_tickCnt++;

		//if tick count is reached
		if (ir_tickCnt >= ir_curCode[ir_curCodeIdx])
		{
			//advance to next pulse length
			ir_curCodeIdx++;

			//see if we have to switch on or off frequency generation
			if (ir_curCodeIdx & 0x01)
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

//disable code output
static void ir_disable(void)
{
	//disable ir code generator
	ir_curCodeLen = 0;
	ir_curCodeIdx = 255;
	ir_curCode = 0;
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
//0 means an error has occured
uint8_t ir_genCode(uint16_t *destCode, uint16_t oneOntime, uint16_t oneOfftime, uint16_t zeroOntime, uint16_t zeroOfftime, uint32_t bitCode, uint8_t codeLen)
{
	uint8_t i;

	//failsafe
	if (codeLen > 32)
	{
		return 0;
	}

	i = codeLen;

	//convert bitcode
	while (i--)
	{
		if (bitCode & 1)
		{
			//encode a one
			destCode[i * 2] = oneOntime;
			destCode[(i * 2) + 1] = oneOfftime;
		}
		else
		{
			//encode a zero
			destCode[i * 2] = zeroOntime;
			destCode[(i * 2) + 1] = zeroOfftime;
		}

		bitCode >>= 1;
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

//all-in-one initialization
void ir_init(void)
{
	//ir LED output - NPN tansistor needs low output or LED will die - overcurrent driven
	DDRB |= _BV(PB1);

	//disable ir code generator
	ir_disable();

	//enable frequency generator
	ir_freqInit();

	//enable our tick counter / ir code generator
	ir_timer0Init();
}
