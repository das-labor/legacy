#include "config.h"

#include <stdint.h>
#include <avr/pgmspace.h>

void lcd_flash_e (void);
void lcd_write (uint8_t, uint8_t);
void lcd_cls (void);
void lcd_writechar (char);
void lcd_writestr (char *);
void lcd_writestr_P(PGM_P str);
void lcd_writestrn (char *text, uint16_t len);
void lcd_gotoline (uint8_t);
void lcd_gotopos (uint8_t y, uint8_t x);
void lcd_set4bit (void);
void lcd_port_ini (void);
void lcd_init (void);
void lcd_hexdump(void* data, uint8_t length);

