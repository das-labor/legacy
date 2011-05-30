#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#include "can_handler.h"
#include "can/spi.h"
#include "can/can.h"
#include "adc_driver.h"



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

#define PORT_GRAPH 0x36

void send_graph(uint8_t id, uint16_t * data, uint16_t len){
	static can_message msg = {0, 0, PORT_GRAPH, PORT_GRAPH, 4, {}};
	can_message * txmsg;
	
	msg.addr_src = myaddr;
	msg.dlc = 4;
	msg.data[0] = 0;   //counter (0=start message)
	msg.data[1] = id; 
	msg.data[2] = len;
	msg.data[3] = len>>8;

	
	txmsg = can_buffer_get();
	memcpy(txmsg, &msg, sizeof(can_message));
	can_transmit(txmsg);
	
	msg.dlc = 8;

	uint16_t x;
	for(x=0;x<len;x+=4){
		txmsg = can_buffer_get();
		memcpy(txmsg, &msg, sizeof(can_message) );//header
		memcpy(txmsg->data, &data[x], 8); //data
		can_transmit(txmsg);
	}

}

int main(void)
{
	sync_osc();
	start_mcp_clock();
	spi_init();
	can_init();
	read_can_addr();
	adc_init();
	
	/*I/O Protection*/
	CCP = CCP_IOREG_gc;
	PMIC.CTRL = PMIC_MEDLVLEN_bm; 
	sei();

	PORTC.DIR = LED_RED | LED_GRUEN | LED_BLAU;  // LED Ports als Ausgang
	PORTC.OUT |= LED_RED | 0 | LED_BLAU; // LEDS aus
	
	uint16_t testd[1024];
	
	uint16_t y;
	for(y=0;y<1024;y++){
		testd[y] = y;
	
	}
	
	uint32_t x;
	while (1) {
		can_handler();
		x++;
		if(x == 500000){
			PORTC.OUT ^= LED_RED | LED_GRUEN;
			x = 0;
			
			for(y=0;y<64;y++){
				while(!ADC_Ch_Conversion_Complete(&ADCA.CH0));
				testd[y] = ADC_ResultCh_GetWord(&ADCA.CH0) + 3;
			
			}
			
			
			send_graph(0x23, testd, 100);
		}
	}
}

