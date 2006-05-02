#include <inttypes.h>
#include "config.h"

#ifdef	KS0108
#include "ks0108.h"
#endif //KS0108_H

#ifdef	SED1565
#include "sed1565.h"
#endif //SED1565

#define NUM_ROWS 64
#define NUM_COLS 128

void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
void dispDrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
