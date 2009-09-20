#ifndef GLCD_H
#define GLCD_H

#include <stdint.h>

/*
 * Mit Farbe gefülltes Rechteck zwischen 2 Punkten zeichnen.
 */
extern void ls020_rectangle8(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char color);

/*
 * Char aus dem Flash an Position x, y schreiben.
 * 
 */
extern void ls020_put_char8(uint8_t x, uint8_t y, char symbol, char color, char bkcolor);

/*
 * String aus dem Flash an Position x, y schreiben.
 * 
 */
extern void ls020_put_string8(uint8_t x, uint8_t y, PGM_P text, char color, char bkcolor);


#endif // GLCD_H
