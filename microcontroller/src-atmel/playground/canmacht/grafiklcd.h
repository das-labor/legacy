#include <inttypes.h>

#define NUM_ROWS 64
#define NUM_COLS 128

#define DISPSETPAGE(y) dispWriteCommand(0xB0|(y))
#define DISPLINESTART(start) dispWriteCommand(((start) & 63) | 64)


extern void dispSetPix(uint8_t x, uint8_t y, uint8_t color);
extern void dispHLine(uint8_t x, uint8_t y, uint8_t x2, uint8_t color);
extern void dispVLine(uint8_t x, uint8_t y, uint8_t y2);
extern void fillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
extern void dispDrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
extern void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
