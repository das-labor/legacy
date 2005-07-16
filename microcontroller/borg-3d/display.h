#include <avr/io.h>
#include <inttypes.h>

// Display lib for 4x16 Display form an old Lexmark Laserprinter
// It uses the M50530 Chip in 4bit mode with serial comunication

#define DISPLAY_PORT PORTA		// defines the port the LCD ist connected to
#define DISPLAY_DDR DDRA

// INITPARAMERTER

#define DISPLAY_LINES 4			// defines the number of lines of your display 1 2 and 4 are posible

#define DISPLAY_CHARS 16		// chars per line
#define DISPLAY_8BIT 0		 	// 0 for 4-bitmode
#define DISPLAY_8FONT 1    		// 1 for 5x8 font  0 for 5x12 font depends on the display
#define DISPLAY_OWN_CHARS 0		// for 5x8  font: 0 = no, 1 = 4, 2 = 8, 3 = 12 own chars
					 			// for 5x12 font: 0 = no, 1 = 2, 2 = 4, 3 = 6 own chars
#define DISPLAY_CURSOR 1		// enable/disables the cursor			 	
#define DISPLAY_CURSOR_BLINK 1
#define DISPLAY_CHAR_BLINK 0	
#define DISPLAY_UNDERLINE 0

#define DISPLAY_START_DEC 1 	  // defines if the startadress schould be incremented = 0 or decrementet = 1
#define DISPLAY_START_AUTOINC_READ 0 	// inc/dec by write
#define DISPLAY_START_AUTOINC_WRITE 0	// inc/dec by read

#define DISPLAY_CURSOR_DEC 0 	  // defines if the cursor schould be incremented = 0 or decrementet = 1
#define DISPLAY_CURSOR_AUTOINC_READ 0 	// inc/dec by write
#define DISPLAY_CURSOR_AUTOINC_WRITE 1	// inc/dec by read

void delay(uint8_t time);
void display_send(uint8_t data);
void display_clear();
void display_init();
void display_cursor(uint8_t line, uint8_t ch);
void display_print(char *data);
void display_printChar(char data);
void display_newChar5x8(uint8_t number, uint8_t rows[7]); 
