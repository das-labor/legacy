
#include <avr/io.h>
#include <avr/interrupt.h>

#include "ioport.h"
#include "uart/uart.h"

#include "stepper.h"
#include "pinout.h"

// 4096 motor-clocks/revolution


volatile uint8_t ms_over;

//Interrupt Handler
ISR (TIMER0_COMP_vect) {
	//Jede Millisekunde diese Variable einmal setzen
	ms_over = 1;
}

void timer_init () {
	//CTC Modus, Takt/64
	TCCR0 = (1<<WGM01) | (1<<CS01) | (1<<CS00);
	
	//Timer vergleichen mit 250, dann auf 0 setzen (CTC)
	//und Interrupt auslösen.
	OCR0  = 250;
	
	TIMSK |= (1<<OCIE0); //OC Inteerupt aktivieren
	
	//Der Interrupt kommt 1000 mal Pro Sekunde (ms Takt)
	//Weil  (16MHz / 64) / 250 = 1000Hz
}






int main(){
	static uint16_t led_delay;
	static uint8_t led_state;
	
	stepper_init();
	timer_init();
	uart_init();
	
	sei();
	
	SET_DDR(LED);
	
	stepper_command = CMD_HOME;
	
	while(1){
		if (ms_over) {
			ms_over = 0;
			update_stepper();
			
			if(stepper_command == 0){
				position_request = 1200;
				stepper_command = CMD_GOTO;
			}
			
			if(led_delay){
				led_delay --;
			}else{
				uart_putstr("Hello world!\r\n");
				led_delay = 500;
				led_state ^= 1;
				if(led_state){
					OUTPUT_ON(LED);
				}else{
					OUTPUT_OFF(LED);
				}
			}
			
		}
	}

}


