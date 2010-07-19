
#include <string.h>
#include "uart/uart.h"
#include "i2csw.h"

char buf[1024];

char zero[1];
	

int main(){
	int x;
	uart_init();
	i2cInit();

	//test that the uart works
	uart_putstr("Hello\r\n");


	//read 1024 bytes from eeprom and dump to uart as csv
	i2cEeRead(buf, 0, 0x20);
		
	for(x=0;x<0x20;x++){
		char foo[10];
		itoa(buf[x], foo, 10);
		uart_putstr(foo);
		uart_putc(',');
	
	}

	//zero out the thinkpad password
	
	for(x=0x338;x<0x348;x++){
		i2cEeWrite(x, zero, 1);
	}
	
	//that's all folks
	
	while(1);
}