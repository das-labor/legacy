

/*
 *
 * arial8
 *
 * created with FontCreator
 * written by F. Maximilian Thiele
 *
 * http://www.apetech.de/fontCreator
 * me@apetech.de
 *
 * File Name           : arial8.h
 * Date                : 23.12.2007
 * Font size in bytes  : 2870
 * Font width          : 10
 * Font height         : 8
 * Font first char     : 32
 * Font last char      : 128
 * Font used chars     : 96
 *
 * The font data are defined as
 *
 * struct _FONT_ {
 *     uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
 *     uint8_t    font_Width_in_Pixel_for_fixed_drawing;
 *     uint8_t    font_Height_in_Pixel_for_all_characters;
 *     unit8_t    font_First_Char;
 *     uint8_t    font_Char_Count;
 *
 *     uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
 *                  // for each character the separate width in pixels,
 *                  // characters < 128 have an implicit virtual right empty row
 *
 *     uint8_t    font_data[];
 *                  // bit field of all characters
 */

#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef ARIAL8_H
#define ARIAL8_H

#define ARIAL8_WIDTH 10
#define ARIAL8_HEIGHT 8

static uint8_t arial8[] PROGMEM = {
    0x0B, 0x36, // size
    0x0A, // width
    0x08, // height
    0x20, // first char
    0x60, // char count
    
    // char widths
    0x03, 0x01, 0x03, 0x05, 0x04, 0x07, 0x05, 0x03, 0x02, 0x02, 
    0x05, 0x03, 0x01, 0x03, 0x01, 0x03, 0x04, 0x02, 0x04, 0x04, 
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x01, 0x01, 0x04, 0x04, 
    0x04, 0x04, 0x08, 0x05, 0x04, 0x05, 0x04, 0x04, 0x04, 0x06, 
    0x04, 0x01, 0x03, 0x04, 0x03, 0x05, 0x04, 0x06, 0x04, 0x06, 
    0x04, 0x05, 0x05, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x02, 
    0x03, 0x02, 0x03, 0x04, 0x01, 0x04, 0x03, 0x04, 0x04, 0x04, 
    0x02, 0x04, 0x03, 0x01, 0x02, 0x03, 0x02, 0x05, 0x03, 0x04, 
    0x03, 0x04, 0x02, 0x04, 0x03, 0x03, 0x04, 0x05, 0x04, 0x04, 
    0x04, 0x02, 0x01, 0x02, 0x04, 0x04, 
    
    // font data
    0x00, 0x00, 0x00, // 32
    0xBF, // 33
    0x03, 0x00, 0x03, // 34
    0x24, 0x7E, 0x24, 0x7E, 0x24, // 35
    0x39, 0x44, 0x44, 0x39, // 36
    0x82, 0x45, 0x32, 0x0C, 0x12, 0x29, 0x10, // 37
    0x30, 0x4A, 0x55, 0x2A, 0x50, // 38
    0x02, 0x05, 0x02, // 39
    0x7E, 0x81, // 40
    0x81, 0x7E, // 41
    0x22, 0x14, 0x3E, 0x14, 0x22, // 42
    0x08, 0x3E, 0x08, // 43
    0xE0, // 44
    0x08, 0x08, 0x08, // 45
    0x20, // 46
    0x70, 0x0C, 0x07, // 47
    0x3E, 0x41, 0x41, 0x3E, // 48
    0x02, 0x7F, // 49
    0x62, 0x51, 0x49, 0x46, // 50
    0x22, 0x49, 0x49, 0x36, // 51
    0x18, 0x14, 0x7F, 0x10, // 52
    0x27, 0x49, 0x49, 0x31, // 53
    0x3E, 0x49, 0x49, 0x30, // 54
    0x01, 0x71, 0x0D, 0x03, // 55
    0x36, 0x49, 0x49, 0x36, // 56
    0x06, 0x49, 0x49, 0x3E, // 57
    0x24, // 58
    0x64, // 59
    0x08, 0x14, 0x22, 0x22, // 60
    0x14, 0x14, 0x14, 0x14, // 61
    0x22, 0x22, 0x14, 0x08, // 62
    0x02, 0x01, 0x59, 0x06, // 63
    0x3C, 0x42, 0xBE, 0xA5, 0xA5, 0xBD, 0x22, 0x1C, // 64
    0x78, 0x16, 0x11, 0x16, 0x78, // 65
    0x7F, 0x49, 0x49, 0x36, // 66
    0x3E, 0x41, 0x41, 0x41, 0x22, // 67
    0x7F, 0x41, 0x41, 0x3E, // 68
    0x7F, 0x49, 0x49, 0x41, // 69
    0x7F, 0x09, 0x01, 0x01, // 70
    0x3C, 0x42, 0x41, 0x49, 0x49, 0x3A, // 71
    0x7F, 0x08, 0x08, 0x7F, // 72
    0x7F, // 73
    0x30, 0x40, 0x7F, // 74
    0x7F, 0x0C, 0x12, 0x61, // 75
    0x7F, 0x40, 0x40, // 76
    0x7F, 0x02, 0x1C, 0x02, 0x7F, // 77
    0x7F, 0x06, 0x18, 0x7F, // 78
    0x1C, 0x22, 0x41, 0x41, 0x22, 0x1C, // 79
    0x7F, 0x09, 0x09, 0x06, // 80
    0x1C, 0x22, 0x49, 0x51, 0x22, 0x5C, // 81
    0x7F, 0x09, 0x09, 0x76, // 82
    0x32, 0x45, 0x49, 0x49, 0x32, // 83
    0x01, 0x01, 0x7F, 0x01, 0x01, // 84
    0x7F, 0x40, 0x40, 0x7F, // 85
    0x03, 0x1C, 0x60, 0x1C, 0x03, // 86
    0x7F, 0x60, 0x38, 0x60, 0x7F, // 87
    0x61, 0x12, 0x0C, 0x12, 0x61, // 88
    0x01, 0x06, 0x78, 0x06, 0x01, // 89
    0x61, 0x51, 0x4D, 0x43, 0x41, // 90
    0xFF, 0x81, // 91
    0x02, 0x05, 0x02, // 92
    0x81, 0xFF, // 93
    0x06, 0x01, 0x06, // 94
    0x80, 0x80, 0x80, 0x80, // 95
    0x01, // 96
    0x34, 0x54, 0x54, 0x7C, // 97
    0x7F, 0x44, 0x7C, // 98
    0x38, 0x44, 0x44, 0x44, // 99
    0x38, 0x44, 0x44, 0x7F, // 100
    0x38, 0x54, 0x54, 0x4C, // 101
    0x7E, 0x09, // 102
    0x1C, 0x52, 0x52, 0x7E, // 103
    0x7F, 0x04, 0x7C, // 104
    0x79, // 105
    0x40, 0x3C, // 106
    0x7E, 0x18, 0x24, // 107
    0x3F, 0x40, // 108
    0x7C, 0x04, 0x7C, 0x04, 0x7C, // 109
    0x7C, 0x04, 0x7C, // 110
    0x38, 0x44, 0x44, 0x38, // 111
    0x7C, 0x24, 0x3C, // 112
    0x18, 0x24, 0x24, 0x7C, // 113
    0x7C, 0x04, // 114
    0x08, 0x54, 0x54, 0x20, // 115
    0x04, 0x7E, 0x04, // 116
    0x7C, 0x40, 0x7C, // 117
    0x1C, 0x60, 0x60, 0x1C, // 118
    0x1C, 0x30, 0x08, 0x30, 0x1C, // 119
    0x24, 0x18, 0x18, 0x24, // 120
    0x0C, 0x50, 0x50, 0x7C, // 121
    0x64, 0x54, 0x4C, 0x44, // 122
    0x08, 0xF7, // 123
    0xFF, // 124
    0x81, 0x7E, // 125
    0x08, 0x08, 0x08, 0x08, // 126
    0x3E, 0x22, 0x22, 0x3E // 127
    
};

#endif
