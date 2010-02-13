#ifndef _H_LCD_HARDWARE
#define _H_LCD_HARDWARE

#define pixmap ((uint8_t*)0x2000)

void init_lcd_hardware();

void lcd_on();


#define X_SIZE 320ul
#define Y_SIZE 240ul

#define LCD_WIDTH  X_SIZE
#define LCD_HEIGHT Y_SIZE

#define BIG_DISPLAY

#define BIG_DISPLAY

#ifdef BIG_DISPLAY
#define INTERFACE_BITS 8
#else
#define INTERFACE_BITS 4

#endif

#endif //_H_LCD_HARDWARE

