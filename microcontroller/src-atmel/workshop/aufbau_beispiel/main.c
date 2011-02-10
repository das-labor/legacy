
// Header für AVR-Register wie PORTA, TCNT0, usw...
// Außerdem werden die Type uint8_t, uint16_t ... deklariert
#include <avr/io.h>

//Wird für Interrupt Handler gebraucht
#include <avr/interrupt.h>

void taster_init () {
	PORTB |= 0x0f; //Untere 4 bits auf high setzen
				   //Für Taster-Pullups
}


//Globale Variablen für entprellte Taster-Zustände
uint8_t tast0, tast1, tast2, tast3;

//Taster entprellen und globalen Variablen zuweisen
void taster_update () {
	//statische Variablen behalten ihren Wert zwischen Funktionsaufrufen
	//(wirkt wie globale Variable, die außerhalb der Funktion nicht sichtbar ist)
	static uint8_t old_pinb, equal_count;
	
	//Lokale Variable
	uint8_t pinb;
	
	pinb = PINB & 0x0f; //untere 4 bit von PINB lesen
	
	//entprellen
	if (pinb == old_pinb) {
		if (equal_count < 5){
			//nicht weiter als 5 zählen
			equal_count ++;
		}
	} else {
		old_pinb = pinb;
		equal_count = 0;
	}
	
		
	if (equal_count == 5){
		//Zustand war oft genug gleich
		//Kann ausgewertet werden
				
		//Globale Variablen entsprechend den Bits setzen
		if (pinb & (1<<0) ) tast0 = 0; else tast0 = 1;
		if (pinb & (1<<1) ) tast1 = 0; else tast1 = 1;
		if (pinb & (1<<2) ) tast2 = 0; else tast2 = 1;
		if (pinb & (1<<3) ) tast3 = 0; else tast3 = 1;
	}
}


//Zustände für power state
#define PS_OFF  0
#define PS_WAIT 1
#define PS_ON   2

uint8_t power_state;

//Power soll mit tast1 anschaltbar sein mit 2 Sekunden delay
//Aus Taster schaltet sofort aus

void power_statemachine () {
	static uint16_t power_delay;
	
	switch (power_state) {
		case PS_OFF:
			if (tast1) {
				//An Taster gedrückt
				power_state = PS_WAIT; //gehe in warte Zustand
				power_delay = 2000;    //Warte delay initialisieren
			}
			break;
		case PS_WAIT:
			power_delay --;
			if (power_delay == 0) {
				//delay abgelaufen
				power_state = PS_ON;
			}
			if (tast0) {
				//Aus Taster gedrückt
				power_state = PS_OFF;
			}
			break;
		case PS_ON:
			if (tast0) {
				//Aus Taster gedrückt
				power_state = PS_OFF;
			}
			break;
	}
}

uint8_t pulse_out;

//Wenn Power an ist sollen mit tast2 100ms Pulse ausgelöst werden
//oder mit tast3 500ms Pulse
void pulse_statemachine () {
	//speicher für alte Taster-zustände, damit flanken erkannt werden können
	static uint8_t tast2_old, tast3_old;
	
	static uint16_t pulse_delay;
	
	if(power_state == PS_ON){
		//Power ist an
		
		if ( tast2 && ! tast2_old) { //Flanke an Taster2
			pulse_delay = 100;
		}
		
		if ( tast3 && ! tast3_old) { //Flanke an Taster3
			pulse_delay = 500;
		}
		
		if (pulse_delay) {
			//delay nicht abgelaufen
			pulse_delay --;
			pulse_out = 1;
		} else {
			pulse_out = 0;
		}
	} else {
		//Power ist aus - alles zurücksetzen
		pulse_delay = 0;
		pulse_out = 0;
	}

	tast2_old = tast2;
	tast3_old = tast3;
}

void led_init () {
	//Port C komplett auf Ausgang
	DDRC = 0xff;
}

void led_update () {
	static uint8_t blink_delay;
		
	//Power über LED 7 und 6 anzeigen
	switch (power_state) {
		case PS_OFF:
			//beide leds aus
			PORTC &= ~( (1<<7) | (1<<6) );
			
			//blink_delay zurücksetzen
			blink_delay = 0;
			break;
		case PS_WAIT:
			//LED 7 blinkt
			if(blink_delay == 0){
				blink_delay = 250;
				PORTC ^= (1<<7);
			} else {
				blink_delay --;
			}
			break;
		case PS_ON:
			//beide LEDs an
			PORTC |= ( (1<<7) | (1<<6) );
			break;
	}
	
	//Pulse auf LED 0 ausgeben
	if (pulse_out) {
		PORTC |=  (1<<0);
	} else {
		PORTC &= ~(1<<0);
	}
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


//Variablen, die Schnittstellen zum Interrupt-Handler darstellen
//müssen volatile deklariert werden.
volatile uint8_t ms_over;

//Interrupt Handler
ISR (TIMER0_COMP_vect) {
	//Jede Millisekunde diese Variable einmal setzen
	ms_over = 1;
}

//Hauptprogramm
int main(){
	//Initialisierung
	taster_init ();
	led_init ();
	timer_init ();
	
	//Interrupts global aktivieren
	sei();
	
	
	//Hauptschleife
	while(1){
		if (ms_over) {
			ms_over = 0;
			
			//Alle Millisekunde alle Statemachines einmal laufen lassen
			taster_update ();
			power_statemachine ();
			pulse_statemachine ();
			led_update ();
		}
	}
	
}
