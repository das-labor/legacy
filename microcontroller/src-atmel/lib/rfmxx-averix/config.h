#ifndef RFM12_CONFIG
	#define RFM12_CONFIG
	#define RF_PORT	PORTD
	#define RF_DDR	DDRD
	#define RF_PIN	PIND

	#define SDI		7
	#define SCK		6
	#define CS		5
	#define SDO		4
#endif

#ifndef F_CPU
	#define F_CPU 16000000L
#endif

#define UART_BAUD_RATE 9600
#define UART_INTERRUPT

#define UART_TXBUFSIZE 16
#define UART_RXBUFSIZE 16

/* use avrx timers */
#define RF_USE_AVRX

/* how long (in ms) to wait in rf12_ready() */
#define RF_WAITDELAY 5

/* how long (in ms) to wait before giving up on receiving a data array */
#define RF_RXTIMEOUT 100
