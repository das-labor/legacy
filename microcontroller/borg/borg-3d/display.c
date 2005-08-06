#include "display.h"

#if  DISPLAY_LINES == 4
#	if DISPLAY_OWN_CHARS == 0
#		define DISPLAY_SFL 	0x08	
#		define DISPLAY_LINELEN 0x40
#	elif DISPLAY_OWN_CHARS == 1	
#		define DISPLAY_SFL 	0x09
#		define DISPLAY_CGSTART 0xE0
#		define DISPLAY_LINELEN 0x38	
#	elif DISPLAY_OWN_CHARS == 2
#		define DISPLAY_SFL 	0x0a
#		define DISPLAY_CGSTART 0xC0
#		define DISPLAY_LINELEN 0x30
#	elif DISPLAY_OWN_CHARS == 3
#		define DISPLAY_SFL 	0x0b
#		define DISPLAY_CGSTART 0xa0
#		define DISPLAY_LINELEN 0x28	
#	endif
#endif


/*
 Bit Funktion                   Standart
 0	 EX    Instruction Start    0  (1 for execution)
 1   VCC /RST   49	    VDD 5V  1
 2   A & B                      X (from data, must be serialiesed)
 3   GND        R/W=0 GND 0V    0
 4   CLK                        X (clock for the seriel to parallel converter)
 5   R/W=0 GND 0V               0
 6   Serielle Ausgabe           0    
 7   Helligjeit analog          1
 
 The first 6 bits of data will be send to the serial -> paralle converter on
 the Display.

 Nur die oberen 6 bit werden verwendet
 
 74164
 
 QA	DB4
 QB DB5
 QC DB6
 QD DB7
 QE IO/C2
 QF IO/C1
 QG -
 QH -
 
*/
void display_send(uint8_t data) {
	uint8_t i = 6, help;
	for (; i; --i) {
		help = (data & 0x20) >> 3;
		DISPLAY_PORT = 0x12 | help;    // two times for clock
		DISPLAY_PORT = 0x02 | help;
		data <<= 1;	
	}
	DISPLAY_PORT = 0x12;
	delay(5);
	DISPLAY_PORT = 0x13;
	delay(30);
	DISPLAY_PORT = 0x12;
}

void display_clear() {
	display_send(0x00); // clear display
	display_send(0x01);
	delay(8);
}

void display_init() {
//	uint8_t ch11[7] = {0xAA, 0xFF, 0xAA, 0xFF, 0xAA, 0xFF, 0xAA};

	delay(30);  // wait for hardware
	DISPLAY_DDR = 0xFF;
	
	//SF
	display_send(0x0C | (DISPLAY_8BIT<<1) | DISPLAY_8FONT);
	display_send(DISPLAY_SFL); 
	
	//SE
 	display_send(0x04 | (DISPLAY_CURSOR_DEC<<1) | DISPLAY_CURSOR_AUTOINC_WRITE);
 	
	display_send((DISPLAY_CURSOR_AUTOINC_READ<<3) | (DISPLAY_START_DEC<<2) |
	             (DISPLAY_START_AUTOINC_WRITE<<1) | DISPLAY_CURSOR_AUTOINC_READ);
	
	//SD
	display_send(0x03); // SD H  0  0  1  1=DSP_ON
	display_send((DISPLAY_CURSOR<<3) | (DISPLAY_UNDERLINE<<2) | 
				 (DISPLAY_CURSOR_BLINK<<1) | DISPLAY_CHAR_BLINK); 

	//display_newChar5x8(11, ch11);
	
	display_clear();
	display_send(0x20); // set display startadress to 0
	display_send(0x20);
	display_send(0x30); // set cursor startadress to 0
	display_send(0x30);
}

void display_printChar(char data) {					
	display_send(0x10 | data >> 4);
	display_send(0x10 | (data & 0x0f));
}

void display_cursor(uint8_t line, uint8_t ch) {
	uint8_t data = DISPLAY_LINELEN * line + ch;
	display_send(0x30 | data >> 4);
	display_send(0x30 | (data & 0x0f));
}

void display_print(char *data) {
	while (*data != '\0') { 
		display_printChar(*data);
		data++;
	}
}


/** a new userdefinable charcter, tha is defined in rows[7] can be tranferd 
 * to the display-controller. The format is following:
 *        bit 7 6 5 4 3 2 1 0
 *  row[0]          0 1 1 1 0
 *  row[1]          1 0 0 0 1
 *  row[2]          1 0 0 0 1
 *  row[3]          1 1 1 1 1
 *  row[4]          1 0 0 0 1
 *  row[5]          1 0 0 0 1
 *  row[6]          1 0 0 0 1
 * This is an example for an A. only the lower 5 bits of the 
 * array are used for character
 */
void display_newChar5x8(uint8_t number, uint8_t rows[7]) {
#ifdef DISPLAY_CGSTART	
	uint8_t i = 0;
	uint8_t data = DISPLAY_CGSTART + number * 8;
	display_send(0x30 | data >> 4);
	display_send(0x30 | (data & 0x0f));
	for (; i < 7; ++i) {
		display_send(0x10 | rows[i] >> 4);
		display_send(0x10 | (rows[i] & 0x0f));
	}
#endif
}

void delay(uint8_t time) {
	uint16_t i;
	uint8_t j = time;
	while (--j) {
		i = 0x0FFF;
		while (--i);
	}
}

/*
void display_printUint8(uint8_t data) {
	uint8_t i = 3;
	char tmp[] = "   ";
	while (data > 0) {
		tmp[i--] = '0' + data % 10;
		data /= 10;
	}
	if (data == 0 && i == 3)
		tmp[i--] = '0';
	while (i <= 3) 
		tmp[i--] = ' ';
	display_print(tmp);
}

void display_printUint16(uint16_t data) {
	uint8_t i = 5;
	char tmp[] = "     ";
	while (data > 0) {
		tmp[i--] = '0' + data % 10;
		data /= 10;
	}
	if (data == 0 && i == 5)
		tmp[i--] = '0';
	while (i <= 5) 
		tmp[i--] = ' ';
	display_print(tmp);
}
*/
/*
// I think this is only waste of character, but looks nice
void drawBorder(uint8_t mode) {
	display_cursor(0, 0);
	display_printChar(mode == 0 ? 0x80 : ' ');
	display_printChar(mode == 1 ? 0x86 : ' ');
	display_printChar(mode == 0 ? 0x86 : ' ');
	display_printChar(mode == 1 ? 0x86 : ' ');
	display_printChar(mode == 0 ? 0x86 : ' ');
	display_printChar(mode == 1 ? 0x86 : ' ');
	display_printChar(mode == 0 ? 0x86 : ' ');
	display_printChar(mode == 1 ? 0x86 : ' ');
	display_printChar(mode == 0 ? 0x86 : ' ');
	display_printChar(mode == 1 ? 0x86 : ' ');
	display_printChar(mode == 0 ? 0x86 : ' ');
	display_printChar(mode == 1 ? 0x86 : ' ');
	display_printChar(mode == 0 ? 0x86 : ' ');
	display_printChar(mode == 1 ? 0x86 : ' ');
	display_printChar(mode == 0 ? 0x86 : ' ');	
	display_printChar(mode == 1 ? 0x81 : ' ');
	display_cursor(1, 0);
	display_printChar(mode == 1 ? 0x85 : ' ');
	display_cursor(1, 15);
	display_printChar(mode == 0 ? 0x84 : ' ');
	display_cursor(2, 0);
	display_printChar(mode == 0 ? 0x85 : ' ');
	display_cursor(2, 15);
	display_printChar(mode == 1 ? 0x84 : ' ');
	display_cursor(3, 0);
	display_printChar(mode == 1 ? 0x82 : ' ');
	display_printChar(mode == 0 ? 0x87 : ' ');
	display_printChar(mode == 1 ? 0x87 : ' ');
	display_printChar(mode == 0 ? 0x87 : ' ');
	display_printChar(mode == 1 ? 0x87 : ' ');
	display_printChar(mode == 0 ? 0x87 : ' ');
	display_printChar(mode == 1 ? 0x87 : ' ');
	display_printChar(mode == 0 ? 0x87 : ' ');
	display_printChar(mode == 1 ? 0x87 : ' ');
	display_printChar(mode == 0 ? 0x87 : ' ');
	display_printChar(mode == 1 ? 0x87 : ' ');
	display_printChar(mode == 0 ? 0x87 : ' ');
	display_printChar(mode == 1 ? 0x87 : ' ');
	display_printChar(mode == 0 ? 0x87 : ' ');
	display_printChar(mode == 1 ? 0x87 : ' ');
	display_printChar(mode == 0 ? 0x83 : ' ');
}
*/

/*
#define B0000 0x0 
#define B0001 0x1
#define B0010 0x2
#define B0011 0x3
#define B0100 0x4
#define B0101 0x5
#define B0110 0x6
#define B0111 0x7
#define B1000 0x8
#define B1001 0x9
#define B1010 0xa
#define B1011 0xb
#define B1100 0xc
#define B1101 0xd
#define B1110 0xe
#define B1111 0xf
*/
