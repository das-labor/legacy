/* HD44780 Library by Peter Fuhrmann
 * this uses 4bit mode only for now.
 * The Data Lines 7:4 shall be connected to the Display Port 3:0
*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "hd44780.h"
#include "config.h"
//set this for 4bit mode
//The Data Lines 7:4 shall be connected to the Display Port 3:0 then

#define IF4BIT

#ifdef FAXFRONT

#undef IF4BIT
#define PIN_RS PD7
#define PIN_E  PD6
#define PIN_RW PD5

//Port, to which the Datalines of the Display are connected
#define DISP_DDR_D DDRC
#define DISP_PORT_D PORTC
#define DISP_PIN PINC

//Port, to which the Control Lines of the Display are connected
#define DISP_DDR_C DDRD
#define DISP_PORT_C PORTD

#else

#define PIN_RS PA5
#define PIN_RW PA6
#define PIN_E  PA7

//Port, to which the Datalines of the Display are connected
#define DISP_DDR_D DDRA
#define DISP_PORT_D PORTA
#define DISP_PIN PINA

//Port, to which the Control Lines of the Display are connected
#define DISP_DDR_C DDRA
#define DISP_PORT_C PORTA

#endif


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
	uint16_t x; char y;
	for (y=1;y<8;y++){
		for (x=1;x<50000;x++){};
	}
}
void wait200us(){
	uint8_t x;
	for (x=1;x<200;x++){};
}

void hd44780_send(uint8_t data){
	uint8_t busy;
	
#ifdef IF4BIT	
	DISP_PORT_C |= _BV(PIN_E);
	DISP_PORT_D &= 0xF0;
	DISP_PORT_D |= ((data >> 4) & 0x0F);
	DISP_PORT_C &= ~_BV(PIN_E);
	DISP_PORT_C |= _BV(PIN_E);
	DISP_PORT_D &= 0xF0;
	DISP_PORT_D |= (data & 0x0F);
	DISP_PORT_C &= ~_BV(PIN_E);
#else
	DISP_PORT_C |= _BV(PIN_E);
	DISP_PORT_D = (data);
	DISP_PORT_C &= ~_BV(PIN_E);
#endif

#ifdef IF4BIT
	DISP_DDR_D &= 0XF0;		//Data lines 3:0 to Input
	DISP_PORT_D |= 0x0F;		//Pullups on on Data lines
#else
	DISP_DDR_D = 0x00;		//Data lines to Input
	DISP_PORT_D = 0xFF;		//Pullups on
#endif

	DISP_PORT_C = (DISP_PORT_C | 1<<PIN_RW) & ~(1<<PIN_RS);

	do{
		DISP_PORT_C |= _BV(PIN_E);
		asm volatile ("nop\n\tnop\n\t"::);
#ifdef IF4BIT		
		busy = (DISP_PIN & 0x08);
		DISP_PORT_C &= ~_BV(PIN_E);
		DISP_PORT_C |= _BV(PIN_E);
		DISP_PORT_C &= ~_BV(PIN_E);
#else
		busy = (DISP_PIN & 0x80);
		DISP_PORT_C &= ~_BV(PIN_E);
#endif	
	}while(busy);

	DISP_PORT_C &= ~(1<<PIN_RW | 1<<PIN_RS );
#ifdef IF4BIT	
	DISP_DDR_D |= 0x0F;		//Data Lines to Output
#else
	DISP_DDR_D = 0xFF;
#endif
}

// send Data to display without checking the Busy Flag, needed for iniializing Display
void hd44780_send_nobusy(uint8_t data){
#ifdef IF4BIT	
	DISP_PORT_C |= _BV(PIN_E);
	DISP_PORT_D &= 0xF0;
	DISP_PORT_D |= (data >> 4);
	DISP_PORT_C &= ~_BV(PIN_E);
	wait200us();
	DISP_PORT_C |= _BV(PIN_E);
	DISP_PORT_D &= 0xF0;
	DISP_PORT_D |= (data & 0x0F);
	DISP_PORT_C &= ~_BV(PIN_E);
	wait200us();
#else
	DISP_PORT_C |= _BV(PIN_E);
	DISP_PORT_D = data;
	DISP_PORT_C &= ~_BV(PIN_E);
	wait200us();
#endif
}

/* reads and returns the address counter */
uint8_t hd44780_read(){
	uint8_t address;
#ifdef IF4BIT
	DISP_DDR_D &= 0xF0;
	DISP_PORT_D |= 0x0F;
	
	DISP_PORT_C &= ~(1<<PIN_RS);
	DISP_PORT_C |= (1<<PIN_RW);
	
	DISP_PORT_C |= 1<<PIN_E;
	asm volatile ("nop\n\tnop\n\t"::);
	address = DISP_PIN & 0x07; //dont read busy flag
	DISP_PORT_C &= ~_BV(PIN_E);
	address <<= 4;
	DISP_PORT_C |= _BV(PIN_E);
	asm volatile ("nop\n\tnop\n\t"::);
	address |= DISP_PIN & 0x0F;
	DISP_PORT_C &= ~_BV(PIN_E);

	DISP_PORT_C &= ~(1<<PIN_RW);	
	DISP_DDR_D |= 0x0F;
#else	
	DISP_DDR_D = 0x00;
	DISP_PORT_D = 0xFF;
	
	DISP_PORT_C &= ~(1<<PIN_RS);
	DISP_PORT_C |= (1<<PIN_RW);
	
	DISP_PORT_C |= 1<<PIN_E;
	asm volatile ("nop\n\tnop\n\t"::);
	address = DISP_PIN & 0x7F; //dont read busy flag
	DISP_PORT_C &= ~_BV(PIN_E);
	
	DISP_PORT_C &= ~(1<<PIN_RW);
	DISP_DDR_D = 0xFF;
#endif
	return address;
}

void hd44780_command(uint8_t command){
	DISP_PORT_C &= ~(1<<PIN_RS);
	hd44780_send(command);
}

void hd44780_data(uint8_t data){
	DISP_PORT_C |= 1<<PIN_RS;
	hd44780_send(data);
}

/* init the controller and clear the Display */
void hd44780_init(){

#ifdef IF4BIT	
	DISP_DDR_D |= 0x0F;
	DISP_PORT_D &= 0xF0;
#else
	DISP_DDR_D = 0xFF;
	DISP_PORT_D = 0x00;
#endif
	DISP_DDR_C |= (1<<PIN_E|1<<PIN_RW|1<<PIN_RS);
	DISP_PORT_C &= ~(1<<PIN_E|1<<PIN_RW|1<<PIN_RS);
	
	wait50ms();
#ifdef IF4BIT
	hd44780_send_nobusy(0x33);
	hd44780_send_nobusy(0x32);// Put Display in 4bit mode
	hd44780_send_nobusy(0x28);// Put Display in 4bit mode
	hd44780_command(0x28);// 2 Lines 5x7 DOT
	
#else
	hd44780_send_nobusy(0x30);
	hd44780_send_nobusy(0x30);
	hd44780_send_nobusy(0x30);
	hd44780_send_nobusy(0x30);// Put Display in 8bit mode
	hd44780_command(0x30);
	hd44780_command(0x38);// 2 Lines 5x7 DOT
#endif

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
