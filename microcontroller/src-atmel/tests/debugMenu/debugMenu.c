/**
 * 2011 Martin Ongsiek
 *  
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "debugMenu.h"

#define TIME_ENTPRELL 5

typedef struct {
	uint8_t wert;
	uint8_t format;
} debugMenu_t;

typedef enum {
	ST_MENU_START,
	ST_MENU_ON,
	ST_MENU_OFF,
	ST_MENU_WERT,
} states_menu_t;

static uint8_t menuEntry;
static states_menu_t stateMenu;
static debugMenu_t debugMenu[8];
static uint8_t entprellTimer[4];
static uint8_t tasterCur, timer, alterWert, updateWert;
static uint8_t tasterEntprellt;

void debugMenuInit() {
	DDRC = 0xff; 
	// Taster als Eingaenge
	DDRB  &= ~(_BV(PB0) | _BV(PB1) | _BV(PB2) | _BV(PB3));
	// Pullups
	PORTB |= _BV(PB0) | _BV(PB1) | _BV(PB2) | _BV(PB3);
	menuEntry = 0;
	stateMenu = ST_MENU_START;

#ifdef DEBUG_MIT_INTTERUPT
	// Timer initialisieren
	GICR   = 0;
	TCCR0  = _BV(CS01) | _BV(CS00);
	TIMSK |= (1 << TOIE0);
	sei();
#endif // DEBUG_MENU
}

static uint8_t tastenEntprellen() {
	uint8_t i, changes, mask, event = 0;
	changes = (PINB & 0x0f) ^ tasterCur;
	tasterCur = (PINB & 0x0f);
	mask = 1;
	for (i = 0; i < 4; i++) {
		if (changes & mask) { // Eingang geändert
			entprellTimer[i] = 0;
		} else if (entprellTimer[i] < TIME_ENTPRELL) { // Entprellzeit nicht abgelaufen
			entprellTimer[i]++;
		} else if ((tasterCur & mask) != (tasterEntprellt & mask)) { // Änderung
	  	if (tasterCur & mask) { // Setzt Event
				tasterEntprellt |=  mask;
			} else { // Rücksetztevent, (Taster gedrückt)
				tasterEntprellt &= ~mask;
				event |= mask;
			}
		}
		mask <<= 1;
	}
	return event;
}

void debugMenuCont() {
	uint8_t i, help;
	uint8_t tastenDruckEvent = tastenEntprellen();
 	
	if (tastenDruckEvent & _BV(PB0)) { // Taste Menu +
		menuEntry++;
		if (menuEntry >= 8)
			menuEntry = 0;
		stateMenu = ST_MENU_START;	
	} else if (tastenDruckEvent & _BV(PB1)) { // Taste Menu +
		if (menuEntry)
			menuEntry--;
		else
			menuEntry = 7;
		stateMenu = ST_MENU_START;	
	}	
	switch (stateMenu) {
		case ST_MENU_START:
			PORTC     = (1 << menuEntry);
			timer = 50;
			stateMenu = ST_MENU_ON;
			break;
			
		case ST_MENU_ON:
			if (timer) {
				timer--;
				PORTC = 0;
				stateMenu = ST_MENU_OFF; // Wechsel zu ST_MENU_OFF damit das Menu dunkler erscheint.
			} else {
				stateMenu = ST_MENU_WERT;
				updateWert = 1;
			}
			break;	

		case ST_MENU_OFF:
			PORTC     = (1 << menuEntry);
			stateMenu = ST_MENU_ON;
			break;

		case ST_MENU_WERT:
		  
		  if (tastenDruckEvent & _BV(PB2)) { // Taste Wert +
				updateWert = 1;
				debugMenu[menuEntry].wert++;
			} else if (tastenDruckEvent & _BV(PB3)) { // Taste Wert -
				updateWert = 1;
				debugMenu[menuEntry].wert--;
			}
			if (alterWert != debugMenu[menuEntry].wert)   // Wert geändert
		    updateWert = 1;
		  alterWert = debugMenu[menuEntry].wert;
		  if (updateWert)  {
		    updateWert = 0;
  			switch (debugMenu[menuEntry].format) {  // Welches Ausgabeformat
  				case DM_FRMT_VOLUME:
  					help = 0;
  					if (debugMenu[menuEntry].wert) {
  						help = 1;
  						for (i = 1; i < debugMenu[menuEntry].wert && i < 8; i++) {
  							help |= help << 1;
  						}
  					}
  					PORTC = help;
  					break;
  					
  				case DM_FRMT_BIN:	
  				default:
  					PORTC = debugMenu[menuEntry].wert;
  			}
			}
			break;
	}
}

void debugMenuSetValue(uint8_t led, uint8_t wert) {
	if (led < 8)
		debugMenu[led].wert = wert;
}

void debugMenuSetFormat(uint8_t led, format_t format) {
	if (led < 8)
		debugMenu[led].format = format;
}

uint8_t debugMenuGetValue(uint8_t led) {
	if (led < 8)
		return debugMenu[led].wert;
	return 0; 
}

#ifdef DEBUG_MIT_INTTERUPT
static volatile uint8_t logicTimer;
ISR(TIMER0_OVF_vect) {
	TCNT0 = 255-125;
	logicTimer++;
	if (logicTimer > 9) {
		logicTimer = 0;
		debugMenuCont();
	}
}
#endif // DEBUG_MENU
