
#include <avr/io.h>

#define FAXPORT_D_DDR DDRA
#define FAXPORT_D PORTA
#define FAXPORT_C PORTD
#define FAXPORT_C_DDR DDRD
#define FAXPORT_C_DDR_V 0x1C


#define CLK_U1_PIN PD4
#define CLK_U2_PIN PD3
#define NOE_U3_PIN PD2


static unsigned char LEDS1;

void fax_init(){
	FAXPORT_C |= 1<<NOE_U3_PIN ;
	FAXPORT_C &= ~((1<<CLK_U1_PIN)|(1<<CLK_U2_PIN));

	FAXPORT_D_DDR = 0xFF;
	FAXPORT_C_DDR |= FAXPORT_C_DDR_V;

	fax_led_off(0);
}


void fax_led_on(unsigned char num){
	LEDS1 |= 1<<num;
	FAXPORT_D = ~LEDS1;
	FAXPORT_C |= 1<<CLK_U1_PIN;
	FAXPORT_C &= ~(1<<CLK_U1_PIN);
}

void fax_led_off(unsigned char num){
	LEDS1 &= ~(1<<num);
	FAXPORT_D = ~LEDS1;
	FAXPORT_C |= 1<<CLK_U1_PIN;
	FAXPORT_C &= ~(1<<CLK_U1_PIN);
}
