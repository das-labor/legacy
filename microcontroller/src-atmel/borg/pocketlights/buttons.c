#include <avr/io.h>
#include "buttons.h"

#include <setjmp.h>
extern jmp_buf newmode_jmpbuf;

void init_buttons(){
	// Button Input initialisieren, pull-up aus, weil pull-down in der Schaltung
	BUTTONDDR  &= ~(1<<BUTTONPIN);
	BUTTONPORT &=  ~(1<<BUTTONPIN);
}


void button_action(int button){
	longjmp(newmode_jmpbuf, 43);
}
