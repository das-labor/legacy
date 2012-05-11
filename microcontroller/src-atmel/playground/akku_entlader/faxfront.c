
#include <avr/io.h>
#include "faxfront.h"

//Data Port
#define FAXDDR_D DDRC
#define FAXPORT_D PORTC
#define FAXPIN_D PINC

//Control Port
#define FAXPORT_C PORTD
#define FAXDDR_C DDRD


#define CLK_U1_PIN PD4
#define CLK_U2_PIN PD3
#define NOE_U3_PIN PD2


static unsigned int LEDS;

static void set_leds(){
	FAXPORT_D = ~(unsigned char)LEDS;
	FAXPORT_C |= 1<<CLK_U1_PIN;
	FAXPORT_C &= ~(1<<CLK_U1_PIN);
	
	FAXPORT_D = ~(unsigned char)(LEDS>>4);
	FAXPORT_C |= 1<<CLK_U2_PIN;
	FAXPORT_C &= ~(1<<CLK_U2_PIN);
}

void fax_led_set_pattern(unsigned int pattern){
	LEDS = pattern;
	set_leds();
}

void fax_led_on(unsigned char num){
	LEDS |= 1<<num;
	set_leds();
}

void fax_led_off(unsigned char num){
	LEDS &= ~(1<<num);
	set_leds();
}

unsigned char fax_get_key(){
	unsigned char pin, mask, x, y;
	for(x=0;x<4;x++){
		FAXPORT_D = ~((unsigned char)LEDS|(1<<x));
		FAXPORT_C |= 1<<CLK_U2_PIN;
		FAXPORT_C &= ~(1<<CLK_U2_PIN);
	
		FAXDDR_D = 0;
		FAXPORT_C &= ~(1<<NOE_U3_PIN);
		asm volatile ("nop\n\tnop\n\t"::);
		pin = FAXPIN_D;
		FAXPORT_C |= 1<<NOE_U3_PIN;
		FAXDDR_D = 0xFF;

		mask = 0x01;
		for(y=0;y<8;y++){
			if(~pin & mask){
				return((x<<3) + y +1);
			}
			mask<<=1;
		}
	}
	return 0;
}

void fax_init(){
	FAXPORT_C |= 1<<NOE_U3_PIN ;
	FAXPORT_C &= ~((1<<CLK_U1_PIN)|(1<<CLK_U2_PIN));

	FAXDDR_C |= (1<<CLK_U1_PIN)|(1<<CLK_U2_PIN)|(1<<NOE_U3_PIN);

	FAXDDR_D = 0xFF;
	
	set_leds();
}


