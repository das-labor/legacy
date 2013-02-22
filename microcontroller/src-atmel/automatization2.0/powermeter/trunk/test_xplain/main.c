#include <avr/io.h>
#include <stdio.h>
#include "avr_compiler.h"
#include "adc_driver.h"


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
	uint8_t bsel = (F_CPU / (8 * speed) - 1);
	USARTD0.BAUDCTRLA = bsel & 0xff;
	USARTD0.BAUDCTRLB = (USARTD0.BAUDCTRLB & 0xf0) + ((bsel >> 8) & 0x0f);
}

uint8_t usart_canSend() {
	return USARTD0.STATUS & USART_TXCIF_bm;
}

int uart_putc(char byte, FILE *fp) {
	USARTD0.DATA = byte;
	while((USARTD0.STATUS & USART_TXCIF_bm) == 0);
	USARTD0.STATUS = USART_TXCIF_bm;
	return 0;	
}

void uart_putstr(char *str) {
	while(*str) {
		uart_putc(*str++,0);
	}
}


volatile int8_t offset;
int main(void)
{
	sync_osc();
	usart_init();
	
	fdevopen(uart_putc,0);
	


	

	PORTE.DIRSET = PIN3_bm | PIN4_bm;
	PORTE.OUTCLR = PIN3_bm;
	usart_setSpeed(115200L);
	PORTE.OUTCLR = PIN4_bm;
	
	printf("Hallo\r\n");
	printf("%d\r\n", offset);
	
	while (1) {
		int x;
		int32_t akk;
		for (x = 0; x < 10000; x++) {
			int16_t adcSample, adcSample1;
			while (!ADC_Ch_Conversion_Complete(&ADCA.CH0));
			adcSample = ADC_ResultCh_GetWord_Signed(&ADCA.CH0, offset);
			while (!ADC_Ch_Conversion_Complete(&ADCA.CH1));
			adcSample1 = ADC_ResultCh_GetWord_Signed(&ADCA.CH1, offset);

			akk += (int32_t) adcSample * adcSample1;
		}


		akk = akk / 26580;

		printf("%d\r\n", akk);
	}
}

