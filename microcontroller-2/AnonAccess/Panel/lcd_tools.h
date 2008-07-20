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
void lcd_hexdump(const void* data, uint8_t length);


#define LCD_CURSOR_ON   lcd_control(1,1,0)
#define LCD_CURSOR_OFF  lcd_control(1,0,0)
#define LCD_DISPLAY_ON  lcd_control(1,0,0)
#define LCD_DISPLAY_OFF lcd_control(0,0,0)


#endif /* LCD_TOOLS_H_ */

