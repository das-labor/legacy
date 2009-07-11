#include <avr/io.h>
#include <util/delay.h>
#include "../makros.h"
#include "../config.h"

#define		NES_PAD_DDR_PL DDR(NES_PAD_PORT_PL)
#define		NES_PAD_DDR_CLK DDR(NES_PAD_PORT_CLK)
#define		NES_PAD_PIN_DS PIN(NES_PAD_PORT_DS)
#define		NES_PAD_DDR_DS DDR(NES_PAD_PORT_DS)
volatile unsigned char fakeport;  
unsigned char waitForFire;


void joy_init(){

	NES_PAD_DDR_PL |= 1<<NES_PAD_BIT_PL;
	NES_PAD_DDR_CLK |= 1<<NES_PAD_BIT_CLK;
	NES_PAD_PORT_DS |= (1<<NES_PAD_BIT_DS);//pullup	
	NES_PAD_DDR_DS &= ~(1<<NES_PAD_BIT_DS);

}

void readButtons(){
	
	NES_PAD_PORT_PL |= 1<<NES_PAD_BIT_PL;
	asm volatile("nop\r\t");
	asm volatile("nop\r\t");
	asm volatile("nop\r\t");

	NES_PAD_PORT_PL &= ~ (1<<NES_PAD_BIT_PL);

	uint8_t i, port=0;
	for(i=0; i<8; i++){

		port >>= 1;
		_delay_us(1);
		if(NES_PAD_PIN_DS & (1 << NES_PAD_BIT_DS))
			port |=0x80;

		NES_PAD_PORT_CLK |= 1<<NES_PAD_BIT_CLK;
	asm volatile("nop\r\t");
	asm volatile("nop\r\t");
		NES_PAD_PORT_CLK &= ~(1<<NES_PAD_BIT_CLK);
	}
	fakeport= ~port;
}
