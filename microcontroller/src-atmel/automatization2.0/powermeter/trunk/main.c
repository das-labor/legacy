#include <avr/io.h>



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


void usart_init() {
	/* This PORT setting is only valid to USARTC0 if other USARTs is used a
	 * different PORT and/or pins is used. */
	/* PIN3 (TXD0) as output. */
	PORTD.DIRSET = PIN3_bm;

	/* PC2 (RXD0) as input. */
	PORTD.DIRCLR = PIN2_bm;

	// enable tx rx double clock
	USARTD0.CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm;

	// asynchronous, char size is 8 bit, no parity, one stop bit
	USARTD0.CTRLC = USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc;
}

void usart_setSpeed(uint32_t speed) {
	uint8_t bsel = (F_CPU / (8 * speed)-1);
	USARTD0.BAUDCTRLA = bsel & 0xff;
	USARTD0.BAUDCTRLB = (USARTC0.BAUDCTRLB & 0xf0) + ((bsel >> 8) & 0x0f);
}

uint8_t usart_canSend() {
	return USARTD0.STATUS & USART_TXCIF_bm;
}

void usart_sendByte(uint8_t byte) {
	USARTD0.DATA = byte;
}



int main(void)
{
	sync_osc();
	usart_init();




	PORTE.DIRSET = PIN3_bm | PIN4_bm;
	PORTE.OUTCLR = PIN3_bm;
	usart_setSpeed(19200L);
	PORTE.OUTCLR = PIN4_bm;
	
	while (1) {

			usart_sendByte('g');
	}
	
	return 1;
}

