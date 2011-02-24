#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "rfm12.h"
#include "ioport.h"

#define TASTER_PORT C
#define TASTER_BIT  5

#define LED_RED_PORT D
#define LED_RED_BIT  6

#define LED_GREEN_PORT D
#define LED_GREEN_BIT  7


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
	
	uint8_t tx_data[] = "foobar\x00";

	SET_DDR(LED_RED);
	SET_DDR(LED_GREEN);
	OUTPUT_ON(TASTER); //pullup

	_delay_ms(250);
	_delay_ms(250);
	rfm12_init();
	timer_init ();
	
	sei();
	
	
	//Hauptschleife
	while(1){
		if (ms_over) {
			static uint8_t old_taster;
			uint8_t taster;
			
			if (rfm12_rx_status() == STATUS_COMPLETE)
			{
	
				bufcontents = rfm12_rx_buffer();
					
				// tell the implementation that the buffer
				// can be reused for the next data.
				rfm12_rx_clear();
	
			}
			
			taster = ! INPUT(TASTER);
			
			if (taster != old_taster)
			{
				old_taster = taster;
				
				if(taster){
					OUTPUT_ON(LED_RED);
				}else{
					OUTPUT_OFF(LED_RED);
				}
				
				tx_data[6] = taster;//taster wert hinter "foobar"
				
				rfm12_tx (sizeof(tx_data), 0, tx_data);
				
			}
	
			rfm12_tick();
		}
	}
}
