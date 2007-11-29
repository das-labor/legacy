

#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "adc.h"

extern uint16_t werte[];
extern uint8_t adc_update;




void printwert(uint16_t wert){
	char sbuf[10];
	utoa(wert,sbuf,10);
	uart_putstr(sbuf);
	uart_putstr("\r\n");	
}


#define CHARGE_ON() DDRD|=(1<<4)


int main(){
	DDRD = 0x2C;
		
	uart_init();
	sei();
	uart_putstr("\r\nALM>\r\n");

	adc_init();
	pwm_init();
	
	CHARGE_ON();
	
	while(1){
		uint16_t wert;
		if(adc_update){
			adc_update = 0;
			wert = werte[0];
			wert /= 3;
			printwert(wert);
						
		}		
	}

}
