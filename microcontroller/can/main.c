
#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"
#include "spi.h"
#include "can.h"

int main(){
	uart_init();
	spi_init();
	can_init();
	unsigned char tmp = mcp_read(0x0e);
	char buf[4];
	itoa(tmp,buf,10);
	uart_putstr(buf);
	uart_putc(' ');
	
	tmp = mcp_read(0x38);
	itoa(tmp,buf,10);
	uart_putstr(buf);
	uart_putc(' ');

	mcp_write(0x38,0xBB);



	tmp = mcp_read(0x38);
	itoa(tmp,buf,10);
	uart_putstr(buf);	
	uart_putc(' ');

	return 0;
}
