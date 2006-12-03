#ifndef CONFIG_H
#define CONFIG_H

// util.[ch] defines
// #define F_CPU 16000000         // Oszillator-Frequenz in Hz

// uart.[ch] defines
#define UART_INTERRUPT 1
#define UART_BAUD_RATE 19200
#define UART_RXBUFSIZE 16
#define UART_TXBUFSIZE 16
#define UART_LINE_BUFFER_SIZE 40
//#define UART_LEDS 1


#define TCNT0_INIT (0xFF-F_CPU/256/TICKRATE)

#define BAUDRATE 19200L
#define UBRR_INIT (F_CPU/(16*BAUDRATE)-1)

#define TMC8_CK256 (1<<CS02)


#define F_MCP F_CPU

#define TICKRATE 1000



#endif
