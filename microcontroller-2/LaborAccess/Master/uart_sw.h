#ifndef _UART_H_
#define _UART_H_

#define SUART_TXD
#define SUART_RXD

void InitSerial1();

#ifdef SUART_TXD
int put_c1(char c);		// Non blocking, returns -1 when full
int put_char1(char c);	// Blocking, always returns 0.
#endif // SUART_RXD

#ifdef SUART_RXD
int get_c1(void);		// Non blocking, returns -1 when empty
int get_char1(void);	// Blocking, always returns character
#endif // SUART_RXD






#endif /* _UART_H_ */

