#include <util/delay.h>
#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "lab-irkit2.h"

//system initialization
void init()
{
	//disable analog comparator (to save power)
	//ACSR |= _BV(ACD);

	//debug LED output
	DDRB |= _BV(PB7);
	
	// Must be output else SPI will fail
//	DDRB |= _BV(PB2);

	//initialize ir subsystem
        ir_init(IR_NEC_Carrier, IR_NEC_pulselength);

	//initialize spi port
	//spi_init();
	
	//turn on interrupts
	sei();
}

int main(void)
{	
	//system initialization
	init();
	uint8_t codelength=0;
	ir_genHeader(0, IR_NEC_header, IR_NEC_headerlength);
	codelength=ir_genCode(IR_NEC_headerlength,0,IR_NEC_oneCode,IR_NEC_oneCodelength,IR_NEC_zeroCode,IR_NEC_zeroCodelength,0b00010000110010001110000100011110, 32);
	ir_sendCode(codelength);	
	//the main loop continuously handles can messages
	while (1)
	{	
		//check uart: TODO

		PORTB &= ~_BV(PB7);
		ir_sendCode(codelength);
		_delay_ms(50);
	}
}
