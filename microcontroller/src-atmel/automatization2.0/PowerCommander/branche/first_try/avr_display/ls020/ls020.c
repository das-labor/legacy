/*--------------------------------------
ls020 lib for avrgcc
Code from:

Juras-Projects 2009
LS020 LCD 8-bit palette demo program
Schematic diagram at http://www.juras-projects.org/eng/lcd.php
(S65 LCD Test Board)
Compiled with IAR for AVR
--------------------------------------*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "ls020.h"
#include "../config.h"


/*
 * Initialisierungswerte
 */
const char init_array_0[] PROGMEM =
{0xEF, 0x00, 0xEE, 0x04, 0x1B, 0x04, 0xFE, 0xFE,
0xFE, 0xFE, 0xEF, 0x90, 0x4A, 0x04, 0x7F, 0x3F, 
0xEE, 0x04, 0x43, 0x06};

/*
 * Initialisierungswerte
 */
const char init_array_1[] PROGMEM =
{0xEF, 0x90, 0x09, 0x83, 0x08, 0x00, 0x0B, 0xAF,
0x0A, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 
0xEF, 0x00, 0xEE, 0x0C, 0xEF, 0x90, 0x00, 0x80, 
0xEF, 0xB0, 0x49, 0x02, 0xEF, 0x00, 0x7F, 0x01,
0xE1, 0x81, 0xE2, 0x02, 0xE2, 0x76, 0xE1, 0x83,
0x80, 0x01, 0xEF, 0x90, 0x00, 0x00};

// Interface

static void ls020_wrcmd8(char cmd)
{
    CMD_PORT |= _BV(LCD_RS); // Set LCD_RS
    CMD_PORT &= ~_BV(LCD_CS); // select LCD
    SPDR = cmd;
    while (!(SPSR & _BV(SPIF)));
    CMD_PORT |= _BV(LCD_CS); // deselect LCD
}

static void ls020_wrcmd16(uint16_t cmd16)
{
    ls020_wrcmd8((cmd16 >> 8) & 0xFF);
    ls020_wrcmd8(cmd16 & 0xFF);
}

extern void ls020_wrdata8(char data)
{
    CMD_PORT &= ~_BV(LCD_RS); // Clear LCD_RS
    CMD_PORT &= ~_BV(LCD_CS); // select LCD
    SPDR = data;
    while (!(SPSR & _BV(SPIF)));
    CMD_PORT |= _BV(LCD_CS); // deselect LCD
}

// Init

extern void ls020_init_lcd(void)
{
    CMD_PORT &= ~_BV(LCD_RESET); // Pull LCD_RESET pin down
    _delay_ms(1);                // delay 1ms
    CMD_PORT |= _BV(LCD_RESET);  // Release LCD_RESET pin

    ls020_wrcmd16(0xFDFD);
    ls020_wrcmd16(0xFDFD);

    _delay_ms(68); // delay 68ms

    for (uint8_t i = 0; i < 20; i++)
    {
        ls020_wrcmd8(pgm_read_byte(init_array_0 + i));
    }
    _delay_ms(10); // delay 10ms

    for (uint8_t i = 0; i < 46; i++)
    {
        ls020_wrcmd8(pgm_read_byte(init_array_1 + i));
    }
}

//

extern void ls020_8bit_mode(char BGR)  // BGR=0 - disabled, BGR=1 - enabled.
{
    ls020_wrcmd16(0xE800 + (BGR&0x01) * 0x40);
}

extern void ls020_16bit_mode(void)
{
    ls020_wrcmd16(0xE80F);
}

// Color Modes

extern void ls020_8_color_mode(void)
{
    ls020_wrcmd16(0x0401);
    ls020_wrcmd16(0x0000);
}

extern void ls020_65k_color_mode(void)
{
    ls020_wrcmd16(0x0400);
    ls020_wrcmd16(0x0000);
}

//

extern void ls020_set_window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    ls020_wrcmd16(0x0500); // Set Direction
    ls020_wrcmd16(0x0A00 + x1);
    ls020_wrcmd16(0x0B00 + x2);
    ls020_wrcmd16(0x0800 + y1);
    ls020_wrcmd16(0x0900 + y2);
}

// Scrolling

extern void ls020_scroll(uint8_t offset)
{
    ls020_wrcmd16(0x1100 + offset);
}

