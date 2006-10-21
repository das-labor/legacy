/*
 * error-handling-test
 * 
 * TODO:
 *  -remove ugly casts
 *  -make things easier to use (MACROS?!)
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

void* ehQ(void* p){
	uart_putstr("\r\n+-----------+"
				"\r\n|doing eh Q |"
				"\r\n+-----------+\r\n");
	return (void*)23;
}

/*****************************************************************************
 *  error-handling block													 *
 *****************************************************************************/


const errorBlockId_t id0 PROGMEM = { 4, 0x2, 0x3, 0x4, 0x5};
const errorBlockId_t id1 PROGMEM = { 6, 0x2, 0x3, 0x4, 0x5, 0x1, 0xA0};
const errorBlockId_t id2 PROGMEM = { 2, 0x41, 0x23};
const errorBlockId_t id3 PROGMEM = {10, 0x32, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA};
const errorBlockId_t idMaster PROGMEM = {0,0};

errorblock_t errorblock1 = {4,{{(uint8_t*)id0, eh0}, 
							{(uint8_t*)id1, eh1},
							{(uint8_t*)id2, eh2},
							{(uint8_t*)id3, eh3} }};


errorblock_t ex2 = {3, {{(uint8_t*)id2, eh2},
					    {(uint8_t*)id3, eh3},
						{(uint8_t*)idMaster, ehQ} }};

/* The following won't work since anonymous objects in progmem are not supported :-(
errorblock_t blub PROGMEM = {3, {
						 { {4, 1, 2, 3, 4}, eh2},
						 { {0}, ehQ},
						 { {3, 0xA, 0xB, 0xC}, eh0}
						}};
*/

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
	error_register_handlerblock((void*)&errorblock1);
	uart_putstr("\r\nstill allive ;-) 4");
	error_register_handlerblock((void*)&ex2);
	uart_putstr("\r\nstill allive ;-) 4");
	error("beep", 9, 0x32, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9);
	 /* this will cause an a call to ehQ */
	
	while(1){ 
		
	} /* while (1) */
}

