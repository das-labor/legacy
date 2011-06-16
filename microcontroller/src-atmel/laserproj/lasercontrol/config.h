#ifndef _CONFIG_H
#define _CONFIG_H

#define PIN_POWER PD6
#define PIN_BLINKLED PD5
#define PIN_SAFTY PC5

#define POWER_BUTTON_PRESSED !(PIND & _BV(PIN_POWER))
//*** kb

//*** UART
#define F_CPU 8000000        // Oszillator-Frequenz in Hz
#define UART_INTERRUPT 1
#define UART_BAUD_RATE 38400UL
#define UART_RXBUFSIZE 16
#define UART_TXBUFSIZE 16
#define UART_LINE_BUFFER_SIZE 40
//#define UART_LEDS   

//*** GLCD
#define LCD_HEIGHT 64
#define LCD_WIDTH 128

#if 0
// Ports
#define LCD_CMD_PORT		PORTC		// Command Output Register
#define LCD_CMD_DIR		DDRC		// Data Direction Register for Command Port

#define LCD_DATA_IN		PINA		// Data Input Register
#define LCD_DATA_OUT		PORTA		// Data Output Register
#define LCD_DATA_DIR		DDRA		// Data Direction Register for Data Port

// Command Port Bits
#define D_I					PC2		// D/I Bit Number
#define R_W					PC3		// R/W Bit Number
#define EN					PC4		// EN Bit Number
#define CSEL1				PC0		// CS1 Bit Number
#define CSEL2				PC1		// CS2 Bit Number
#endif
//Command Ports
#define ENPORT PORTC
#define CSEL1PORT PORTC 
#define CSEL2PORT PORTC
#define RWPORT PORTC
#define DIPORT PORTC

//*** menu
#if 1
	#define BUTTON_UP kbchar == 'w'
	#define BUTTON_DOWN kbchar == 's'
	#define BUTTON_RIGHT kbchar == 'd'
	#define BUTTON_LEFT kbchar == 'a'
	#define BUTTON_ENTER kbchar == 'e'
	#define BUTTON_ISNUMMERIC ((kbchar > ('0'-1)) && (kbchar < ('9'+1)))
#else
	#define BUTTON_UP PIND & 0x01
	#define BUTTON_DOWN PIND & 0x02
	#define BUTTON_RIGHT PIND & 0x04
	#define BUTTON_LEFT PIND & 0x08
#endif

#endif // ifndef CONFIG_H
