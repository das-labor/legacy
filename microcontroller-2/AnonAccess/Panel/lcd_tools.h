#include "config.h"

#include <stdint.h>

void lcd_flash_e (void);
void lcd_write (uint8_t, uint8_t);
void lcd_cls (void);
void lcd_writechar (char);
void lcd_writetext (char *);
void lcd_gotoline (uint8_t);
void lcd_gotopos (uint8_t y, uint8_t x);
void lcd_writezahl (int32_t, uint8_t, uint8_t);
void lcd_set4bit (void);
void lcd_port_ini (void);
void lcd_init (void);
void lcd_hexdump(void* data, uint8_t length);

