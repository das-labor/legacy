#include <avr/io.h>
#include <stdio.h>

#include "can_handler.h"
#include "can/spi.h"
#include "can/can.h"



void sync_osc() {
	/*32 MHz Oszillator starten und 32.768kHz Oszillator für DFLL starten*/
	OSC.CTRL |= OSC_RC32MEN_bm | OSC_RC32KEN_bm ;
	
	/*Wenn Oscillator stabil wird das Flag RC32MRDY
	 * gesetzt und 32Mhz können benutzt werden*/
	while (!(OSC.STATUS & OSC_RC32MRDY_bm));


	/* auto kalibierung ein */
	DFLLRC32M.CTRL = DFLL_ENABLE_bm;

	/*I/O Protection*/
	CCP = CCP_IOREG_gc;
	/*Clock auf 32Mhz einstellen*/
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
}

void start_mcp_clock(){
	//init the timmer for mcp2515 clock
	PORTD.DIRSET = (1<<2);
	TCD0.CTRLB = TC0_CCCEN_bm | 3; //single slope pwm, OC0C as output
	TCD0.PER = 1;
	TCD0.CCC = 1;
	TCD0.CNT = 0;
	TCD0.CTRLA = 1; //clk/1
}


#define LED_RED _BV(PIN2)
#define LED_GRUEN _BV(PIN1)
#define LED_BLAU _BV(PIN0)



int main(void)
{
	sync_osc();
	start_mcp_clock();
	spi_init();
	can_init();
	read_can_addr();

	PORTC.DIR = LED_RED | LED_GRUEN | LED_BLAU;  // LED Ports als Ausgang
	PORTC.OUT |= LED_RED | 0 | LED_BLAU; // LEDS aus
	
	uint32_t x;
	while (1) {
		can_handler();
		x++;
		if(x == 50000){
			PORTC.OUT ^= LED_RED | LED_GRUEN;
			x = 0;
		}
	}
}

