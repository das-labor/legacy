#include <setjmp.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "borg_hw.h"
#include "pixel.h"
#include "scrolltext.h"

jmp_buf newmode_jmpbuf;
uint8_t text[] PROGMEM = "IT'S COLDER THAN BEFORE\n";

int main (void){
	unsigned char mode = 0;
	clear_screen(0);
	borg_hw_init();
	sei();

	mode = setjmp(newmode_jmpbuf);
	for(;;){
		alice_putstr(text);
	}
}
