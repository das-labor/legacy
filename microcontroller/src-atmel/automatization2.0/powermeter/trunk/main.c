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
	uint8_t bsel = (F_CPU / (8 * speed)-1);
	USARTD0.BAUDCTRLA = bsel & 0xff;
	USARTD0.BAUDCTRLB = (USARTD0.BAUDCTRLB & 0xf0) + ((bsel >> 8) & 0x0f);
}

uint8_t usart_canSend() {
	return USARTD0.STATUS & USART_TXCIF_bm;
}

int uart_putc(char byte, FILE * fp) {
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
	


	/* Move stored calibration values to ADC A. */
	ADC_CalibrationValues_Load(&ADCA);

	/* Set up ADC A to have signed conversion mode and 12 bit resolution. */
  	ADC_ConvMode_and_Resolution_Config(&ADCA, ADC_ConvMode_Signed, ADC_RESOLUTION_12BIT_gc);

	/* Set sample rate. */
	ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV32_gc);

	/* Set reference voltage on ADC A to be 1.00 V.*/
	ADC_Reference_Config(&ADCA, ADC_REFSEL_INT1V_gc);

	/* Setup channel 0, 1 with different inputs. */
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH0,
	                                 ADC_CH_INPUTMODE_DIFF_gc,
	                                 ADC_DRIVER_CH_GAIN_NONE);
	/* Setup channel 0, 1 with different inputs. */
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH1,
	                                 ADC_CH_INPUTMODE_DIFF_gc,
	                                 ADC_DRIVER_CH_GAIN_NONE);

   	/* Get offset value for ADC A. */
   	ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_MUXPOS_PIN1_gc, ADC_CH_MUXNEG_PIN1_gc);

	ADC_Enable(&ADCA);
	/* Wait until common mode voltage is stable. Default clk is 2MHz and
	 * therefore below the maximum frequency to use this function. */
	ADC_Wait_8MHz(&ADCA);
 	offset = ADC_Offset_Get_Signed(&ADCA, &ADCA.CH0, false);
    ADC_Disable(&ADCA);
	
	ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_MUXPOS_PIN1_gc, ADC_CH_MUXNEG_PIN0_gc);
	ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_MUXPOS_PIN2_gc, ADC_CH_MUXNEG_PIN0_gc);

	/* Setup sweep of all four virtual channels. */
	ADC_SweepChannels_Config(&ADCA, ADC_SWEEP_01_gc);
	
        /* Enable ADC A .*/
	ADC_Enable(&ADCA);

	/* Wait until common mode voltage is stable. Default clk is 2MHz and
	 * therefore below the maximum frequency to use this function. */
	ADC_Wait_8MHz(&ADCA);
	
	/* Enable free running mode. */
	ADC_FreeRunning_Enable(&ADCA);

	PORTE.DIRSET = PIN3_bm | PIN4_bm;
	PORTE.OUTCLR = PIN3_bm;
	usart_setSpeed(115200L);
	PORTE.OUTCLR = PIN4_bm;
	
	printf("Hallo\r\n");
	
	while (1) {
	        int x;
	        int32_t akk;
	        for(x=0;x<10000;x++){
	                int16_t adcSample, adcSample1;
                	while (!ADC_Ch_Conversion_Complete(&ADCA.CH0));
                	adcSample = ADC_ResultCh_GetWord_Signed(&ADCA.CH0, offset);
                	while (!ADC_Ch_Conversion_Complete(&ADCA.CH1));
                	adcSample1 = ADC_ResultCh_GetWord_Signed(&ADCA.CH1, offset);
                	
                	
                	akk += (int32_t)adcSample * adcSample1;
                }	
        	
        	
        	akk = akk / 26580;
        	
        	printf("%d\r\n",akk);
        	
	}

}

