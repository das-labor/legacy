#ifndef BORG_FONTS_H
#define BORG_FONTS_H

#include <inttypes.h>


struct FONT_DEF 
{
   unsigned char store_width;            /* glyph storage width in bytes */
   unsigned char glyph_height;           /* glyph height for storage */
   const uint16_t *glyph_table;     /* font table start address in memory */
   const unsigned char *width_table;     /* variable width table start adress */
   unsigned char glyph_beg;              /* start ascii offset in table */
   unsigned char glyph_end;              /* end ascii offset in table */
   unsigned char glyph_def;              /* code for undefined glyph code */
};

extern struct FONT_DEF font[];

#endif // BORG_FONTS_H
