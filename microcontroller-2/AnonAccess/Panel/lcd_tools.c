/**
 * 
 * Author: Martin Hermsen & Daniel Otte
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

// toggel enable
void lcd_flash_e(void){
	PORT_LCD |= ( 1<<ENABLE_LCD );
	_delay_us (10);
	PORT_LCD &= ~( 1<<ENABLE_LCD );
	_delay_us (60);	
}

// send commands and chars: rs=0 => Command, rs=1 => char
void lcd_write(uint8_t data, uint8_t rs){
	uint8_t dataBits = 0;
  
	if(rs){   					// write data (RS=1, RW=0) 
		dataBits=0x10;			// RS liegt an Pin 4 = B 0001 0000 = H 10
	}else{     					// write instruction (RS=0, RW=0) 
		dataBits=0; 			
	}

	PORT_LCD = dataBits |(data>>RS_LCD); 		// output high nibble first
	lcd_flash_e ();    
	PORT_LCD = dataBits | (data&0x0F);		// output low nibble
	lcd_flash_e ();
	_delay_us (1);
}
void lcd_cursor(uint8_t blink, uint8_cursor){
	uint8_t i;
	i = 1<<2 | cursor<<1 | blink;
	i &= 0x07;
	i |= 0x08;
	lcd_write(i,0);
}

void lcd_display_onoff(uint8_t onoff){
	uint8_t i;
	i = onoff?_BV(2):0;
	i &= 0x07;
	i |= 0x08;
	lcd_write(i,0);
}

void lcd_control(uint8_t display, uint8_t cursor, uint8_t blink){
	uint8_t i;
	i = display<<2 | cursor<<1 | blink;
	i &= 0x07;
	i |= 0x08;
	lcd_write(i,0);
}

void lcd_setcgaddr(uint8_t addr){
	addr &= 0x3F;
	addr |= 0x40;
	lcd_write(addr, 0);
}

void lcd_setddaddr(uint8_t addr){
	addr &= 0x7F;
	addr |= 0x80;
	lcd_write(addr, 0);
}

void lcd_loadfont(const void* font){
	uint8_t i;
	for(i=0; i<8; ++i){
		lcd_write(((uint8_t*)font)[i],1);
	}
}

void lcd_loadfont_P(PGM_VOID_P font){
	uint8_t i;
	for(i=0; i<8; ++i){
		lcd_write(pgm_read_byte(((PGM_P)font)+i),1);
	}
}

// clear display
void lcd_cls (void){
	lcd_write(0x02,0);   				// 0000 0010 => del Display
	_delay_ms(2);					// wait a moment
	lcd_write(0x01,0);   				// B 0000 0001 => Cursor Home
	_delay_ms(2);					// wait a moment
}

// char output
void lcd_writechar (char c){
  lcd_write (c, 1);   
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

void lcd_hexdump(const void* data, uint8_t length){
	while(length){
		lcd_writechar(pgm_read_byte(hexdigit_tab_P+((*((uint8_t*)data))>>4)));
		lcd_writechar(pgm_read_byte(hexdigit_tab_P+((*((uint8_t*)data))&0x0F)));
		data = (uint8_t*)data +1;
		length--;
	}
}

uint8_t lcd_line_lut[] PROGMEM = {0x80, 0x80, 0xC0, 0x80+20, 0xC0+20};	

// goto pos
void lcd_gotoline (uint8_t line){
/*
  if (zeile == 1) lcd_write(0x80,0);   			// B 1000 0000 => DD-RAM Adress Set 1. Zeile/1.Spalte 
  if (zeile == 2) lcd_write(0xC0,0);   			// B 1100 0000 => DD-RAM Adress Set 2. Zeile/1.Spalte (B x100 0000 = H 40)
  if (zeile == 3) lcd_write(0x80+20,0);   		// B 1110 0000 => DD-RAM Adress Set 3. Zeile/1.Spalte (B x100 0000 = H 40)
  if (zeile == 4) lcd_write(0xC0+20,0);   		// B 1111 0000 => DD-RAM Adress Set 4. Zeile/1.Spalte (B x100 0000 = H 40)
*/
	if(line<=4)
		lcd_write(pgm_read_byte(lcd_line_lut+line),0);
}

// courser positioning
void lcd_gotopos (uint8_t line, uint8_t col){
/*	
  if (zeile == 1) lcd_write(0x80+spalte-1,0);   	// DD-RAM Adress 1. Zeile + Spalte
  if (zeile == 2) lcd_write(0xC0+spalte-1,0);   	// DD-RAM Adress 2. Zeile + Spalte 
  if (zeile == 3) lcd_write(0x80+20+spalte-1,0);	// DD-RAM Adress 3. Zeile + Spalte
  if (zeile == 4) lcd_write(0xC0+20+spalte-1,0);	// DD-RAM Adress 4. Zeile + Spalte
*/
	if(line<=4)
		lcd_write(pgm_read_byte(lcd_line_lut+line)+col-1,0);
}

// Display initialisieren. Einmal als erstes aufrufen
void lcd_init (void){
	DDRC_LCD = 0x3F;				// init ports
	PORT_LCD = 0x00;				// set all ports low
	_delay_ms(20);					// wait a moment	

	// 8-Bit-Mode then 4-Bit-Mode
	lcd_write(0x33,0);
	lcd_write(0x33,0);
	lcd_write(0x32,0); 				// B 0010 1000 => 8-Bit-Mode
	_delay_ms(1);					// wait a moment

	// now 4-Bit-Mode
	lcd_write(0x28,0);     				// B 0010 1000 => Function Set: 2x16 or 4x16
	lcd_write(0x0C,0);     				// B 0000 1100 => display on, coursor off
	lcd_write(0x06,0);     				// B 0000 0110 => Entry Mode Set: DD-RAM autom. inkr., Cursor bewegen
	_delay_ms(2);					// wait a moment

	lcd_cls();					// clear display
	_delay_ms(2);					// wait a moment
}
