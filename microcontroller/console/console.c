
#define CONSOLE_UART

#ifdef CONSOLE_UART
#include "uart.h"

#define stdin_getline_nb uart_getline_nb
#define stdout_putstr uart_putstr
#endif



void console(){
	char * cl;
	if((cl=stdin_getline_nb())){
		
		
		stdout_putstr("\r\nnode 1: >");	
	}
		
}