#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#include "rfm12.h"
#include "ioport.h"

#define TASTER_PORT C
#define TASTER_BIT  5

#define LED_RED_PORT B
#define LED_RED_BIT  0



void timer_init () {
	//CTC Modus, Takt/64
	TCCR2 = (1<<WGM21) | (1<<CS21) | (1<<CS20);
	
	//Timer vergleichen mit 250, dann auf 0 setzen (CTC)
	//und Interrupt auslösen.
	OCR2  = 250;
	
	TIMSK |= (1<<OCIE2); //OC Inteerupt aktivieren
	
	//Der Interrupt kommt 1000 mal Pro Sekunde (ms Takt)
	//Weil  (16MHz / 64) / 250 = 1000Hz
}


//Variablen, die Schnittstellen zum Interrupt-Handler darstellen
//müssen volatile deklariert werden.
volatile uint8_t ms_over;

//Interrupt Handler
ISR (TIMER2_COMP_vect) {
	//Jede Millisekunde diese Variable einmal setzen
	ms_over = 1;
}


int main ( void )
{
	uint8_t *bufcontents;
	
	SET_DDR(LED_RED);

	_delay_ms(250);
	_delay_ms(250);
	rfm12_init();
	timer_init ();
	
	sei();
	
	
	//Hauptschleife
	while(1){
		if (ms_over) {
			
			if (rfm12_rx_status() == STATUS_COMPLETE)
			{
				bufcontents = rfm12_rx_buffer();
				
				if(strncmp((char*)bufcontents, "foobar", 6) == 0){
					if(bufcontents[6]){
						OUTPUT_ON(LED_RED);
					}else{
						OUTPUT_OFF(LED_RED);
					}
	
				}
								
				// tell the implementation that the buffer
				// can be reused for the next data.
				rfm12_rx_clear();
	
			}
				
			rfm12_tick();
		}
	}
}
