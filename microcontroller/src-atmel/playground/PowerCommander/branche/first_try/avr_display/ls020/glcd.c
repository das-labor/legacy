/*--------------------------------------
ls020 lib for avrgcc
Code from:

Juras-Projects 2009
LS020 LCD 8-bit palette demo program
Schematic diagram at http://www.juras-projects.org/eng/lcd.php
(S65 LCD Test Board)
Compiled with IAR for AVR
--------------------------------------*/

#include "ls020.h"
#include "glcd.h"
#include "ls020_fonts.h"

void ls020_rectangle8(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char color) {
    ls020_set_window(x1, y1, x2, y2);
    for (uint8_t y = y1; y <= y2; y++)
    {
        for (uint8_t x = x1; x <= x2; x++)
        {
            ls020_wrdata8(color);
        }
    }
}

//TODO: Multifont und Speicher
extern void ls020_put_char8(uint8_t x, uint8_t y, char symbol, char color, char bkcolor)
{
    ls020_set_window(x, y, x + 5, y + 7);
    uint16_t offset = 6 * (symbol - 0x20);
    for (uint8_t i = 0; i < 6; i++)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            if (((pgm_read_byte(font6x8 + offset + i) << j)&0x80) == 0x80)
            {
                ls020_wrdata8(color);
            } else
            {
                ls020_wrdata8(bkcolor);
            }
        }
    }
}

void ls020_put_string8(uint8_t x, uint8_t y, PGM_P text, char color, char bkcolor)
{
    uint8_t i = 0;
    uint8_t x0 = 0;
    while (pgm_read_byte(text + i) != 0)
    {
        ls020_put_char8(x + x0, y, pgm_read_byte(text + i), color, bkcolor);
        i++;
        x0 += 6;
    }
}

