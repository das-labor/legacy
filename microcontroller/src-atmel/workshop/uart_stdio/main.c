
#include <avr/io.h>
#include <stdio.h>
#include "uart/uart.h"



int putc(char c, FILE * f){
	uart_putc(c);
	return 0;
}

int getc(FILE * f){
	return uart_getc();
}


int main(){
	uart_init();
	fdevopen(putc, getc);
	
	printf("Hallo\r\n");


}