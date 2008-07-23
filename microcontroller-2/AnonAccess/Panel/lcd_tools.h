/* lcd_tools.h */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef LCD_TOOLS_H_
#define LCD_TOOLS_H_

#include "config.h"

#include <stdint.h>
#include <avr/pgmspace.h>

//void lcd_flash_e (uint8_t addr);
//void lcd_write (uint8_t data, uint8_t rs);
void lcd_cls (void);
void lcd_control(uint8_t display, uint8_t cursor, uint8_t blink);
void lcd_setcgaddr(uint8_t addr);
void lcd_setddaddr(uint8_t addr);
void lcd_loadfont(const void* font);
void lcd_loadfont_P(PGM_VOID_P font);
void lcd_writechar (char c);
void lcd_writestr (const char *str);
void lcd_writestr_P(PGM_P str);
void lcd_writestrn (const char *text, uint16_t len);
void lcd_writestrn_P(PGM_P text, uint16_t len);
void lcd_gotoline (uint8_t line);
void lcd_gotopos (uint8_t y, uint8_t x);
//void lcd_set4bit (void);
void lcd_port_ini (void);
void lcd_init (void);
void lcd_hexdump(const volatile void* data, uint8_t length);


void lcd_cursor(uint8_t blink, uint8_t cursor);
void lcd_display_onoff(uint8_t onoff);
	

#define LCD_CURSOR_ON   lcd_cursor(0,1)
#define LCD_CURSOR_OFF  lcd_cursor(0,0)
#define LCD_DISPLAY_ON  lcd_display_onoff(1)
#define LCD_DISPLAY_OFF lcd_display_onoff(0)


#endif /* LCD_TOOLS_H_ */

