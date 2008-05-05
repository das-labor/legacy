
#include <stdint.h>
#include "lcd_tools.h"
#include "ui_primitives.h"
#include "keypad_charset.h"

void read_flash(void){
	ui_hexdump_P(1,2,LCD_WIDTH,3,0,1024);
}

void read_decimal(void){
	char str[9];
	lcd_cls();
	lcd_gotopos(1,1);
	lcd_writestr_P(PSTR("Enter number:"));
	read_decimaln(1,2,str,8);
	lcd_gotopos(3,1);
	lcd_writestr(str);
	ui_waitforkeypress();
}

void read_hex(void){
	char str[9];
	lcd_cls();
	lcd_gotopos(1,1);
	lcd_writestr_P(PSTR("Enter hexnumber:"));
	read_hexn(1,2,str,8);
	lcd_gotopos(3,1);
	lcd_writestr(str);
	ui_waitforkeypress();
}


void read_str(void){
	char name[11];
	ui_printstatusline();
	lcd_gotopos(2,1);
	lcd_writestr_P(PSTR("name:"));
	read_strn(1, 3, alphanum_cs, name, 10);
	lcd_gotopos(4,1);
	lcd_writestr(name);
	ui_waitforkey('E');
	lcd_gotopos(2,1);
	lcd_writestr_P(PSTR("name:"));
	read_strn(1, 3, alphanumext_cs, name, 10);
	lcd_gotopos(4,1);
	lcd_writestr(name);
	ui_waitforkey('E');
}


void display_analysis(void){
	uint16_t offset=0,i;
	char c;
	lcd_cls();
	lcd_gotopos(1,1);
	lcd_writestr_P(PSTR("-=display analysis=-"));
	lcd_gotopos(2,2);
	lcd_writestr_P(PSTR("position:"));
	while((c=ui_waitforkeypress())!='C'){
		switch (c){
			case '0': if(offset>0) --offset; break;
			case '3': if(offset<256-LCD_WIDTH) ++offset; break;
			default: break;
		}
		if(c!=' '){
			lcd_gotopos(3,4);
			lcd_hexdump(&offset, 1);
			lcd_gotopos(4,1);
			for(i=offset; i<offset+LCD_WIDTH; ++i){
				lcd_writechar((char)i);
			}
			_delay_ms(3);
		}
	}
}

void read_gpl(void){
	PGM_P gpl_text= PSTR(
    "Copyright (C) 2008 \007 Daniel Otte\n"
    "\n"
    "This program is free software: you can redistribute it and/or modify\n"
    "it under the terms of the GNU General Public License as published by\n"
    "the Free Software Foundation, either version 3 of the License, or\n"
    "(at your option) any later version.\n"
    "\n"
    "This program is distributed in the hope that it will be useful,\n"
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    "GNU General Public License for more details.\n"
    "\n"
    "You should have received a copy of the GNU General Public License\n"
    "along with this program.  If not, see <http://www.gnu.org/licenses/>.");
	ui_textwindow_P(1,1,LCD_WIDTH,LCD_HEIGHT, gpl_text);
}

const char debug_menux_PS[] PROGMEM =   "debug menu";
const char test_hexview_PS[] PROGMEM =  "test hexview";
const char test_textview_PS[] PROGMEM = "test textview";
const char test_readdec_PS[] PROGMEM =  "read decimal";
const char test_readhex_PS[] PROGMEM =  "read hexadecimal";
const char test_readstr_PS[] PROGMEM =  "read string";
const char test_display_PS[] PROGMEM =  "displayanalysis";

menu_t ui_test_menu_mt[] PROGMEM ={
	{debug_menux_PS, back, (superp)NULL},	
	{test_hexview_PS, execute, (superp)read_flash},
	{test_textview_PS, execute, (superp)read_gpl},
	{test_readdec_PS, execute, (superp)read_decimal},
	{test_readhex_PS, execute, (superp)read_hex},
	{test_readstr_PS, execute, (superp)read_str},
	{test_display_PS, execute, (superp)display_analysis},
	{NULL, terminator, (superp)NULL}
};
