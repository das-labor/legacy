//      lab-irkit.h
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

#ifndef _H_LAB_IRKIT
#define _H_LAB_IRKIT

#include <avr/io.h>

//one tick is 16 µS long
#define IR_TICK_US 16

//teufel signal definitions
//see datasheet of PT2248 chip
//all calculations are done in µS
// a is the base time unit
// a = (1/fOSC) * 192 -> fOSC typically is 455KHz
#define PT2248_A 431 //431 fits best, empirically
// bit 0 = a on, 3*a off
#define PT_OFF (PT2248_A / IR_TICK_US), ((3*PT2248_A) / IR_TICK_US)
// bit 1 = 3*a on, a off
#define PT_ON ((3*PT2248_A) / IR_TICK_US), (PT2248_A / IR_TICK_US)


// Extended NEC Protocol
// Pulse distance encoding
// one additional bit for distance

// AGC Burst 9ms + 4,5ms pause
#define PNEC_AGC_ON  (9000 / IR_TICK_US)
#define PNEC_AGC_OFF (4500 / IR_TICK_US)
#define PNEC_AGC_BURST (PNEC_AGC_ON), (PNEC_AGC_OFF)

// bit 0 560µs on + 560µs off
#define PNEC_OFF (560 / IR_TICK_US), (560 / IR_TICK_US)

// bit 1 560µs on + 1,69ms off
#define PNEC_ON (560 / IR_TICK_US), (1690 / IR_TICK_US)

//macro for generating  extended nec encodings
//x is the destination array, y is the input code, z is the bit count
#define IR_GEN_NECEXT(x, y, z) (ir_genCode((uint16_t *)(x+4), PNEC_ON, PNEC_OFF, y, z) + 4); x[0] = PNEC_AGC_ON; x[1] = PNEC_AGC_OFF


// release timer unit from pin
#define FREQGEN_OFF() TCCR1A = 0;

// connect timer to pin, reset timer count
#define FREQGEN_ON() TCNT1 = 0; TCCR1A = _BV(COM1A0);

//function prototypes
extern uint8_t ir_genCode(uint16_t *destCode, uint16_t oneOntime, uint16_t oneOfftime, uint16_t zeroOntime, uint16_t zeroOfftime, uint32_t bitCode, uint8_t codeLen);
extern void ir_sendCode(uint16_t *code, uint8_t codeLen);
extern void ir_init(void);

//work in progress
/*typedef struct {
uint16_t oneOntime;
uint16_t oneOfftime;
uint16_t zeroOntime;
uint16_t zeroOfftime;

} ir_proto;*/


//function to generate extended nec encodings
//destCode is the destination array
//bitCode is the input code
//codeLen is the bit count
static uint8_t ir_genENEC(uint16_t *destCode, uint32_t bitCode, uint8_t codeLen)
{
	//create nec preamble
	destCode[0] = PNEC_AGC_ON;
	destCode[1] = PNEC_AGC_OFF;

	//add a short trailing on pulse
	destCode[codeLen * 2 + 2] = (560 / IR_TICK_US);

	//generate the code, add 4 to the length (for preamble and footer) and return
	return ir_genCode(&destCode[2], PNEC_ON, PNEC_OFF, bitCode, codeLen) + 4;
}

#endif //_H_LAB_IRKIT

