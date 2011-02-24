
#include <avr/io.h>
#include <stdio.h>
#include "uart/uart.h"

int my_putc(char c, FILE * f){
	uart_putc(c);
	return 0;
}

int my_getc(FILE * f){
	char c;
	c = uart_getc();
	//uart_putc(c); //lokales Echo wenn gewünscht
	return c;
}

int main(){
	uart_init();
	fdevopen(my_putc, my_getc);
	
	printf("Hallo\r\n");

	while(1){
		int i;
		scanf("%d", &i);
		
		printf("Die Zahl %d ist %X in hexadecimal\r\n", i, i);
	
	}

	return 0;
}
