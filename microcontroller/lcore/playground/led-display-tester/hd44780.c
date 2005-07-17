/* HD44780 Library by Peter Fuhrmann
 * this uses 4bit mode only for now.
 * The Data Lines 7:4 shall be connected to the Display Port 3:0
*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "hd44780.h"

#define PIN_RS PA5
#define PIN_RW PA6
#define PIN_E  PA7

#define DISP_DDR DDRA
#define DISP_PIN PINA
#define DISP_PORT PORTA

/* Instructions */
#define CLEAR 		0x01	/* no params */
#define HOME 		0x02	/* no params */
#define ENTRY_MODE	0x04	/* B1 = cursor direction(1=right) B0 = do display shift */
#define ON_OFF		0x08	/* B2 = Display B1 = Cursor B0 = Blink */
#define SHIFT		0x10	/* B3 = Move Cursor(1)/ Shift Display(0) B2 = Shift Direction */
#define FUNCTION	0x20	/* B4 = Data Length (0=4, 1=8) B3 = Num of lines B2 = Font(0) */
#define SET_CGRAM	0x40	/* Character Generator Address */
#define SET_DDRAM	0x80	/* Character Address */


void wait50ms(){
uint16_t x;
for (x=1;x<50000;x++){};
}

void wait200us(){
	uint8_t x;
	for (x=1;x<200;x++){};
}

void hd44780_send(uint8_t data){
	uint8_t busy;
	
	DISP_PORT |= _BV(PIN_E);
	DISP_PORT &= 0xF0;
	DISP_PORT |= ((data >> 4) & 0x0F);
	DISP_PORT &= ~_BV(PIN_E);
	DISP_PORT |= _BV(PIN_E);
	DISP_PORT &= 0xF0;
	DISP_PORT |= (data & 0x0F);
	DISP_PORT &= ~_BV(PIN_E);


	DISP_DDR = 0XF0;
	DISP_PORT = 0x0F | _BV(PIN_RW);
	
	do{
		DISP_PORT |= _BV(PIN_E);
		asm volatile ("nop\n\tnop\n\t"::);
		busy = (DISP_PIN & 0x08);
		DISP_PORT &= ~_BV(PIN_E);
		DISP_PORT |= _BV(PIN_E);
		DISP_PORT &= ~_BV(PIN_E);
	}while(busy);

	DISP_PORT = 0;
	DISP_DDR = 0xFF;
}

void hd44780_send_nobusy(uint8_t data){
	DISP_PORT |= _BV(PIN_E);
	DISP_PORT &= 0xF0;
	DISP_PORT |= ((data >> 4) & 0x0F); //to be optimized
	DISP_PORT &= ~_BV(PIN_E);
	wait200us();
	DISP_PORT |= _BV(PIN_E);
	DISP_PORT &= 0xF0;
	DISP_PORT |= (data & 0x0F);
	DISP_PORT &= ~_BV(PIN_E);
	wait200us();
}

/* reads and returns the address counter */
uint8_t hd44780_read(){
	uint8_t address;
	DISP_DDR = 0xF0;
	DISP_PORT = 0x0F | _BV(PIN_RW);

	DISP_PORT |= _BV(PIN_E);
	asm volatile ("nop\n\tnop\n\t"::);
	address = DISP_PIN & 0x07; //dont read busy flag
	DISP_PORT &= ~_BV(PIN_E);
	address <<= 4;
	DISP_PORT |= _BV(PIN_E);
	asm volatile ("nop\n\tnop\n\t"::);
	address |= DISP_PIN & 0x0F;
	DISP_PORT &= ~_BV(PIN_E);
	
	DISP_DDR = 0xFF;
	return address;
}

void hd44780_command(uint8_t command){
	DISP_PORT = _BV(PIN_E);
	hd44780_send(command);
}

void hd44780_data(uint8_t data){
	DISP_PORT = (_BV(PIN_E) | _BV(PIN_RS));
	hd44780_send(data);
}

/* init the controller and clear the Display */
void hd44780_init(){

	DISP_DDR = 0xFF;
	DISP_PORT = 0;

	wait50ms();

	hd44780_send_nobusy(0x33);
	hd44780_send_nobusy(0x32);// Put Display in 4bit mode
	
	hd44780_command(0x28);// 2 Lines 5x7 DOT
	hd44780_command(0x10);// cursor move
	hd44780_command(0x0F);//Display on, cursor on, cursor blink

	hd44780_command(0x01);//clear Display
}

void hd44780_set_cursor(uint8_t row, uint8_t col){
	hd44780_command(0x80|(uint8_t)(row<<6)|col);
}

void hd44780_backspace(uint8_t num){
	uint8_t curs_pos = hd44780_read();
	uint8_t x;
	hd44780_command(SET_DDRAM|(curs_pos-num));
	for(x=0;x<num;x++){
		hd44780_data(' ');
	}
	hd44780_command(SET_DDRAM|(curs_pos-1));
}

void hd44780_clear_line(uint8_t line){
	uint8_t x;
	hd44780_set_cursor(line,0);
	for(x=0;x<DISP_LEN;x++){
		hd44780_data(' ');
	}
	hd44780_set_cursor(line,0);
}

void hd44780_print(char * string){
	char c;
	while ((c = *string++)){
		hd44780_data(c);
	}
}

void hd44780_print_P(PGM_P string){
	char c;
	while ((c = pgm_read_byte(string++))){
		hd44780_data(c);
	}
} 

void hd44780_load_font_P(uint8_t charnum, PGM_P data){
	uint8_t x, address;
	address = hd44780_read();
	hd44780_command((uint8_t)(charnum<<3)|0x40); //set cgram address
	for (x=0;x<8;x++){
		hd44780_data(pgm_read_byte(data++));	
	}
	/*restore position where cursor was */
	hd44780_command(0x80|address);
}
