/**
 * lcd_tools2.c for the larger (4x27) LCD
 * Author: Peter Fuhrmann, Martin Hermsen & Daniel Otte
 * License: GPLv3
 * 
 * 
 */

#include "config.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "lcd_tools.h"
#include "hexdigit_tab.h"

#define PORT_LCD PORTA
#define DDR_LCD  DDA
#define DDRC_LCD DDRA
#define PIN_LCD  PINA

#define LCD_RS      7
#define LCD_RW      6
#define LCD_ENABLE1 5
#define LCD_ENABLE2 4

#define LCD_WIDTH 27
#define LCD_HEIGHT 4


#define asm __asm__

#ifndef DISPLAY_TIMEOUT
 #define DISPLAY_TIMEOUT 0xffff
#endif

//set this for 4bit mode
//The Data Lines 7:4 shall be connected to the Display Port 3:0 then
#define IF4BIT
#undef FAXFRONT


#define PIN_RS PA7
#define PIN_RW PA6
#define PIN_E1 PA5
#define PIN_E2 PA4

//Port, to which the Datalines of the Display are connected
#define DISP_DDR_D DDRA
#define DISP_PORT_D PORTA
#define DISP_PIN PINA

//Port, to which the Control Lines of the Display are connected
#define DISP_DDR_C DDRA
#define DISP_PORT_C PORTA

#define DISP_LEN 27
#define DISP_LINES 2


/* Instructions */
#define CLEAR 		0x01	/* no params */
#define HOME 		0x02	/* no params */
#define ENTRY_MODE	0x04	/* B1 = cursor direction(1=right) B0 = do display shift */
#define ON_OFF		0x08	/* B2 = Display B1 = Cursor B0 = Blink */
#define SHIFT		0x10	/* B3 = Move Cursor(1)/ Shift Display(0) B2 = Shift Direction */
#define FUNCTION	0x20	/* B4 = Data Length (0=4, 1=8) B3 = Num of lines B2 = Font(0) */
#define SET_CGRAM	0x40	/* Character Generator Address */
#define SET_DDRAM	0x80	/* Character Address */


static __inline__ 
void wait200us(void){
	_delay_us(200);
}

uint8_t lcd_ptr=0;

uint8_t hd44780_send(uint8_t data, uint8_t addr){
	uint8_t busy;
	uint16_t timeout=DISPLAY_TIMEOUT;
	uint8_t xpin = _BV(addr?PIN_E2:PIN_E1);
#ifdef IF4BIT	
	DISP_PORT_C |= xpin;
	DISP_PORT_D &= 0xF0;
	DISP_PORT_D |= ((data >> 4) & 0x0F);
	DISP_PORT_C &= ~xpin;
	DISP_PORT_C |= xpin;
	DISP_PORT_D &= 0xF0;
	DISP_PORT_D |= (data & 0x0F);
	DISP_PORT_C &= ~xpin;
#else
	DISP_PORT_C |= xpin;
	DISP_PORT_D = (data);
	DISP_PORT_C &= ~xpin;
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
		DISP_PORT_C |= xpin;
		asm volatile ("nop\n\tnop\n\t"::);
#ifdef IF4BIT		
		busy = (DISP_PIN & 0x08);
		DISP_PORT_C &= ~xpin;
		DISP_PORT_C |= xpin;
		DISP_PORT_C &= ~xpin;
#else
		busy = (DISP_PIN & 0x80);
		DISP_PORT_C &= ~xpin;
#endif	
	}while(busy && timeout--);

	DISP_PORT_C &= ~(1<<PIN_RW | 1<<PIN_RS );
#ifdef IF4BIT	
	DISP_DDR_D |= 0x0F;		//Data Lines to Output
#else
	DISP_DDR_D = 0xFF;
#endif
	return (timeout?0:1);
}

// send Data to display without checking the Busy Flag, needed for iniializing Display
void hd44780_send_nobusy(uint8_t data, uint8_t addr){
	uint8_t xpin = _BV(addr?PIN_E2:PIN_E1);
#ifdef IF4BIT	
	DISP_PORT_C |= xpin;
	DISP_PORT_D &= 0xF0;
	DISP_PORT_D |= (data >> 4);
	DISP_PORT_C &= ~xpin;
	wait200us();
	DISP_PORT_C |= xpin;
	DISP_PORT_D &= 0xF0;
	DISP_PORT_D |= (data & 0x0F);
	DISP_PORT_C &= ~xpin;
	wait200us();
#else
	DISP_PORT_C |= xpin;
	DISP_PORT_D = data;
	DISP_PORT_C &= ~xpin;
	wait200us();
#endif
}

/* reads and returns the address counter */
uint8_t hd44780_read(uint8_t addr){
	uint8_t address;
	uint8_t xpin = _BV(addr?PIN_E2:PIN_E1);
#ifdef IF4BIT
	DISP_DDR_D &= 0xF0;
	DISP_PORT_D |= 0x0F;
	
	DISP_PORT_C &= ~_BV(PIN_RS);
	DISP_PORT_C |= _BV(PIN_RW);
	
	DISP_PORT_C |= xpin;
	asm volatile ("nop\n\tnop\n\t"::);
	address = DISP_PIN & 0x07; //dont read busy flag
	DISP_PORT_C &= ~xpin;
	address <<= 4;
	DISP_PORT_C |= xpin;
	asm volatile ("nop\n\tnop\n\t"::);
	address |= DISP_PIN & 0x0F;
	DISP_PORT_C &= ~xpin;

	DISP_PORT_C &= ~_BV(PIN_RW);	
	DISP_DDR_D |= 0x0F;
#else	
	DISP_DDR_D = 0x00;
	DISP_PORT_D = 0xFF;
	
	DISP_PORT_C &= ~_BV(PIN_RS);
	DISP_PORT_C |= _BV(PIN_RW);
	
	DISP_PORT_C |= 1<<PIN_E1;
	asm volatile ("nop\n\tnop\n\t"::);
	address = DISP_PIN & 0x7F; //dont read busy flag
	DISP_PORT_C &= ~xpin;
	
	DISP_PORT_C &= ~_BV(PIN_RW);
	DISP_DDR_D = 0xFF;
#endif
	return address;
}

uint8_t hd44780_command(uint8_t command, uint8_t addr){
	DISP_PORT_C &= ~_BV(PIN_RS);
	return hd44780_send(command, addr);
}

void hd44780_data(uint8_t data, uint8_t addr){
	DISP_PORT_C |= _BV(PIN_RS);
	hd44780_send(data, addr);
}



// send commands and chars: rs=0 => Command, rs=1 => char
static
void lcd_write(uint8_t data, uint8_t rs, uint8_t addr){
	//((rs!=0)?(&hd44780_data):(&hd44780_command))(data, addr);
	if(rs){
		hd44780_data(data, addr);
    }else{	
    	hd44780_command(data,addr);
	}
}

void lcd_cursor(uint8_t blink, uint8_t cursor){
	uint8_t i;
	i = 1<<2 | cursor<<1 | blink;
	i &= 0x07;
	i |= 0x08;
	lcd_write(i,0,(lcd_ptr>=2*LCD_WIDTH)?1:0);
}

void lcd_display_onoff(uint8_t onoff){
	uint8_t i;
	i = onoff?_BV(2):0;
	i &= 0x07;
	i |= 0x08;
	lcd_write(i,0,0);
	lcd_write(i,0,1);
}

void lcd_control(uint8_t display, uint8_t cursor, uint8_t blink){
	uint8_t i;
	i = display<<2 | cursor<<1 | blink;
	i &= 0x07;
	i |= 0x08;
	lcd_write(i,0,0);
	lcd_write(i,0,1);
}

void lcd_setcgaddr(uint8_t addr){
	addr &= 0x3F;
	addr |= 0x40;
	lcd_write(addr, 0, 0);
	lcd_write(addr, 0, 1);
}

void lcd_setddaddr(uint8_t addr){
	addr &= 0x7F;
	addr |= 0x80;
	lcd_write(addr, 0, 0);
	lcd_write(addr, 0, 1);
}

void lcd_loadfont(const void* font){
	uint8_t i;
	for(i=0; i<8; ++i){
		lcd_write(((uint8_t*)font)[i], 1, 0);
		lcd_write(((uint8_t*)font)[i], 1, 1);
	}
}

void lcd_loadfont_P(PGM_VOID_P font){
	uint8_t i;
	for(i=0; i<8; ++i){
		lcd_write(pgm_read_byte(((PGM_P)font)+i), 1, 0);
		lcd_write(pgm_read_byte(((PGM_P)font)+i), 1, 1);
	}
}


// clear display
void lcd_cls (void){
	lcd_ptr=0;
	lcd_write(0x02,0,0);   				// 0000 0010 => del Display
	lcd_write(0x02,0,1);   				// 0000 0010 => del Display
	_delay_ms(2);					// wait a moment
	lcd_write(0x01,0,0);   				// B 0000 0001 => Cursor Home
	lcd_write(0x01,0,1);   				// B 0000 0001 => Cursor Home
	_delay_ms(2);					// wait a moment
}

// char output
void lcd_writechar (char c){
  lcd_ptr++;
  lcd_write (c, 1, (lcd_ptr>(2*27))?1:0);   
}

// string output
void lcd_writestr (const char *text){
  while (*text!='\0') {
    lcd_writechar(*text++);
  }
}

void lcd_writestrn (const char *text, uint16_t len){
  while ((*text!='\0') && len--) {
    lcd_writechar(*text++);
  }
}

void lcd_writestrn_P(PGM_P text, uint16_t len){
  char c;
  while ((c=pgm_read_byte(text++)) && len--) {
    lcd_writechar(c);
  }
}

void lcd_writestr_P(PGM_P str){
	char c;
  while ((c=pgm_read_byte(str++))) {
    lcd_writechar(c);
  }
}

void lcd_hexdump(const volatile void* data, uint8_t length){
	while(length){
		lcd_writechar(pgm_read_byte(hexdigit_tab_P+((*((uint8_t*)data))>>4)));
		lcd_writechar(pgm_read_byte(hexdigit_tab_P+((*((uint8_t*)data))&0x0F)));
		data = (uint8_t*)data +1;
		length--;
	}
}

uint8_t lcd_line_lut[] PROGMEM = {0x80, 0x80, 0xC0, 0x80, 0xC0};	

void lcd_gotoline (uint8_t line){
	if(line==0)
		line=1;
	if(line<=4){
		lcd_write(pgm_read_byte(lcd_line_lut+line),0,(line>=2)?1:0);
		lcd_ptr=(line-1)*27;
	}
}

/* courser positioning */
void lcd_gotopos (uint8_t line, uint8_t col){
	if(line==0)
		line=1;
	if(col==0)
		col=1;
	if(line<=4){
		lcd_write(pgm_read_byte(lcd_line_lut+line)+col-1,0,(line>2)?1:0);
		lcd_ptr=(line-1)*27+col-1;
	}
}

/* display initialisation */
void lcd_init (void){
	DDRC_LCD = 0xFF;        // init ports
	PORT_LCD = _BV(LCD_RW); // set all ports low
	_delay_ms(20);          // wait a moment	

	// 8-Bit-Mode then 4-Bit-Mode
	lcd_write(0x33,0,0);
	lcd_write(0x33,0,0);
	lcd_write(0x32,0,0);   // B 0010 1000 => 8-Bit-Mode
	_delay_ms(1);          // wait a moment

	// 8-Bit-Mode then 4-Bit-Mode
	lcd_write(0x33,0,1);
	lcd_write(0x33,0,1);
	lcd_write(0x32,0,1);   // B 0010 1000 => 8-Bit-Mode
	_delay_ms(1);          // wait a moment

	// now 4-Bit-Mode
	lcd_write(0x28,0,0);   // B 0010 1000 => Function Set: 2x16 or 4x16
	lcd_write(0x0C,0,0);   // B 0000 1100 => display on, coursor off
	lcd_write(0x06,0,0);   // B 0000 0110 => Entry Mode Set: DD-RAM autom. inkr., Cursor bewegen
	_delay_ms(2);          // wait a moment

	// now 4-Bit-Mode
	lcd_write(0x28,0,1);   // B 0010 1000 => Function Set: 2x16 or 4x16
	lcd_write(0x0C,0,1);   // B 0000 1100 => display on, coursor off
	lcd_write(0x06,0,1);   // B 0000 0110 => Entry Mode Set: DD-RAM autom. inkr., Cursor bewegen
	_delay_ms(2);          // wait a moment

	lcd_cls();             // clear display
	_delay_ms(2);          // wait a moment
}




