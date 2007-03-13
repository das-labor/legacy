#include "buttons.h"
#include "fifo.h"

//DEBUG
#include "pixel.h"

unsigned char buffer[BUF_SIZE];
fifo_t fifo;
 
button_value get_button(){
	int temp=fifo_get_nowait(&fifo);
	
	if( temp == -1 ) //fifo returns with no button in it
		return BUTTON_NONE;
	
	if ( temp & BUTTON_UP_MASK )
		return BUTTON_UP;
	if ( temp & BUTTON_DOWN_MASK )
		return BUTTON_DOWN;
	if ( temp & BUTTON_LEFT_MASK )
		return BUTTON_LEFT;
	if ( temp & BUTTON_RIGHT_MASK )
		return BUTTON_RIGHT;
	if ( temp & BUTTON_START_MASK )
		return BUTTON_START;
	if ( temp & BUTTON_SELECT_MASK )
		return BUTTON_SELECT;
	if ( temp & BUTTON_A_MASK )
		return BUTTON_A;
	if ( temp & BUTTON_B_MASK )
		return BUTTON_B;
	return BUTTON_NONE;
}

void init_buttons(){
	// Button Input initialisieren, pull-up aus, weil pull-down in der Schaltung
	BUTTONDDR  &= ~(1<<BUTTONPIN);
	BUTTONPORT &=  ~(1<<BUTTONPIN);
	fifo_init (&fifo, buffer, BUF_SIZE);
}
