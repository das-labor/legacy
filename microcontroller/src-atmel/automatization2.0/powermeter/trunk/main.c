#include <avr/io.h>
#include <stdio.h>



void sync_osc() {
	/*Oscillator auf 32Mhz einstellen*/
	OSC.CTRL |= OSC_RC32MEN_bm;
	/*Wenn Oscillator stabil wird das Flag RC32MRDY
	 * gesetzt und 32Mhz können benutzt werden*/
	while (!(OSC.STATUS & OSC_RC32MRDY_bm)) {
	}

	/*I/O Protection*/
	CCP = CCP_IOREG_gc;
	/*Clock auf 32Mhz einstellen*/
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;

	/* auto kalibierung ein */
	DFLLRC32M.CTRL = DFLL_ENABLE_bm;
}

#define LED_RED _BV(PIN2)
#define LED_GRUEN _BV(PIN1)
#define LED_BLAU _BV(PIN0)

int main(void)
{
	sync_osc();

	PORTC.DIR = LED_RED | LED_GRUEN | LED_BLAU;  // LED Ports als Ausgang
	PORTC.OUT |= LED_RED | LED_GRUEN | LED_BLAU; // LEDS aus
	
	while (1) {
		
	}
}

