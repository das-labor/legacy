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

#include <util/delay.h>

#include "lab-irkit.h"


//having these arrays global seems to solve problems
static uint16_t code[128];
static uint8_t codeLen;

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

void sendTeufelIR(uint8_t cmd)
{
	//verify if command number is within bounds
	if (cmd < TEUFEL_CODE_CNT) {
		//lookup command and generate the pulse length array
		codeLen = ir_genCode(code, PT_ON, PT_OFF, teufelCodes[cmd], 12);

		//send code and repeat it after 35ms
		//to please the teufel system
		ir_sendCode(code, codeLen);
		_delay_ms(40); //is in reality 35ms
		ir_sendCode(code, codeLen);
	}
}
