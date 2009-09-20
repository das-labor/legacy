#ifndef LS20_H
#define LS20_H

#include <stdint.h>
#include <avr/pgmspace.h>

/*
 * Init LCD.
 */
extern void ls020_init_lcd(void);

/*
 * Schreibe 8Bit Daten auf Display.
 */
extern void ls020_wrdata8(char data);

/*
 * 
 */
extern void ls020_8bit_mode(char BGR);

/*
 * 
 */
extern void ls020_16bit_mode(void);

/*
 * Versetzt LCD in 8Bit Farb Modus
 */
extern void ls020_8_color_mode(void);

/*
 * Versetzt LCD in 65k Farb Modus
 */
extern void ls020_65k_color_mode(void);

/*
 * 
 */
extern void ls020_set_window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

/*
 * Scroll Displayinhalt um x nach ?.
 */
extern void ls020_scroll(uint8_t offset);


#endif // LS20_H
