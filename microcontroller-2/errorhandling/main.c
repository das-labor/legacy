/*
 * error-handling-test
 * 
*/


#include "config.h"
#include "uart.h"
#include "debug.h"
#include "avr-errorhandling.h"

#include <stdint.h>
#include <string.h>
#include <avr/pgmspace.h>

/*****************************************************************************
 *  error-handler																	 *
 *****************************************************************************/
void* eh0(void* p){
	uart_putstr("\r\n+-----------+"
				"\r\n|doing eh 0 |"
				"\r\n+-----------+\r\n");
	return (void*)23;
}

void* eh1(void* p){
	uart_putstr("\r\n+-----------+"
				"\r\n|doing eh 1 |"
				"\r\n+-----------+\r\n");
	return (void*)23;
}

void* eh2(void* p){
	uart_putstr("\r\n+-----------+"
				"\r\n|doing eh 2 |"
				"\r\n+-----------+\r\n");
	return (void*)23;
}

void* eh3(void* p){
	uart_putstr("\r\n+-----------+"
				"\r\n|doing eh 3 |"
				"\r\n+-----------+\r\n");
	return (void*)23;
}

/*****************************************************************************
 *  error-handling block													 *
 *****************************************************************************/
const uint8_t id0[] PROGMEM = { 4, 0x2, 0x3, 0x4, 0x5};
const uint8_t id1[] PROGMEM = { 6, 0x2, 0x3, 0x4, 0x5, 0x1, 0xA0};
const uint8_t id2[] PROGMEM = { 2, 0x41, 0x23};
const uint8_t id3[] PROGMEM = {10, 0x32, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA};

/*
struct {
	const PROGMEM uint8_t n;
	const PROGMEM PGM_P entry[8];
} errorblock PROGMEM  = {4,{id0, eh0, 
							id1, eh1,
							id2, eh2,
							id3, eh3} };
*/

volatile const PGM_P	 eb_3[2] PROGMEM = {id3, eh3};
volatile const PGM_P	 eb_2[2] PROGMEM = {id2, eh2};
volatile const PGM_P	 eb_1[2] PROGMEM = {id1, eh1};
volatile const PGM_P	 eb_0[2] PROGMEM = {id0, eh0};
//asm(".byte 4");
const uint8_t errorblock[] PROGMEM = {4};
const uint8_t dummy[] PROGMEM = {4};
//volatile prog_uint8_t errorblock  = 4;


/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

int main (void){
	
	DEBUG_INIT();
	
	uart_putstr("\r\n\r\nRESET");
	error_init();
	uart_putstr("\r\nstill allive ;-) 1");
	error_register_handler(eh0,1,"\x42");
	uart_putstr("\r\nstill allive ;-) 2");
	error("beep", 2, 0x42, 0x23);
	error_register_handler(eh1,2,"\x42\x23");
	uart_putstr("\r\nstill allive ;-) 3");
	error("beep", 2, 0x42, 0x23);
	error_register_handlerblock((&dummy) +1);
	uart_putstr("\r\nstill allive ;-) 4");
	error("beep", 3, 0x41, 0x23, 0x11);
	uart_putstr("\r\nstill allive ;-) 5");
	error("beep", 9, 0x32, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9); /* this will cause an reset because no
	  handler for this case is registered (eh_3 is more special)*/
	
	while(1){ 
		
	} /* while (1) */
}

