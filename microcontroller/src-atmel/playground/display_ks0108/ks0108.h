#ifndef	KS0108_H
#define KS0108_H

#include <avr/io.h>




extern void dispInit(void);         //Initialize LCD
extern void dispClear(void);

extern void dispSetPix(uint8_t x, uint8_t y, uint8_t color);
extern void dispHLine(uint8_t x, uint8_t y, uint8_t x2, uint8_t color);
extern void dispVLine(uint8_t x, uint8_t y, uint8_t y2);
extern void dispFillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
extern void dispDrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);



#endif //KS0108_H
