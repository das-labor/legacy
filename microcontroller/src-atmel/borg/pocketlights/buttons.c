#include <avr/interrupt.h>
#include <avr/io.h>
#include "buttons.h"
#include "fifo.h"

//DEBUG
#include "pixel.h"

unsigned char buffer[BUF_SIZE];
fifo_t fifo;
 
void button_add(button_value button){
}

void button_register(unsigned char button_pin_state){
}

button_value get_button(){
	uint8_t temp=fifo_get_wait(&fifo);
	
	if ( temp & BUTTON_UP_MASK)
		return BUTTON_UP;
	if ( temp & BUTTON_DOWN_MASK)
		return BUTTON_DOWN;
	if ( temp & BUTTON_LEFT_MASK)
		return BUTTON_LEFT;
	if ( temp & BUTTON_RIGHT_MASK)
		return BUTTON_RIGHT;
	if ( temp & BUTTON_START_MASK)
		return BUTTON_START;
	if ( temp & BUTTON_SELECT_MASK)
		return BUTTON_SELECT;
	if ( temp & BUTTON_A_MASK)
		return BUTTON_A;
	if ( temp & BUTTON_B_MASK)
		return BUTTON_B;
	return 99;
}

void init_buttons(){
	// Button Input initialisieren, pull-up aus, weil pull-down in der Schaltung
	BUTTONDDR  &= ~(1<<BUTTONPIN);
	BUTTONPORT &=  ~(1<<BUTTONPIN);
	fifo_init (&fifo, buffer, BUF_SIZE);
}
