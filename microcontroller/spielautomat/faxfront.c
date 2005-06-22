
#include <avr/io.h>
#include "faxfront.h"

#define FAXPORT_D_DDR DDRA
#define FAXPORT_D PORTA
#define FAXPORT_C PORTD
#define FAXPORT_C_DDR DDRD


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

void fax_init(){
	FAXPORT_C |= 1<<NOE_U3_PIN ;
	FAXPORT_C &= ~((1<<CLK_U1_PIN)|(1<<CLK_U2_PIN));

	FAXPORT_C_DDR |= (1<<CLK_U1_PIN)|(1<<CLK_U2_PIN)|(1<<NOE_U3_PIN);

	set_leds();
}


