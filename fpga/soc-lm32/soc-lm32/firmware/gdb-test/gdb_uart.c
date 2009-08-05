#include "soc-hw.h"

/* Write a single character to debug host */
void 
_gdb_write_char (char c)
{
	uart_putchar(c);
}

/* Read and return a single character from debug host */
char 
_gdb_read_char (void)
{    
	return uart_getchar();
}

/* Acknowledge specified interrupt that caused CPU to enter debug mode */
void 
_gdb_ack_interrupt (void) 
{
  /* ACK interrupt */
  /*__asm__ ("wcsr\tIA, %0" : : "d" (1 << interrupt));*/
}
