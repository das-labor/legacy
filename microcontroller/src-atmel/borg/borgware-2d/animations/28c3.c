#include <stdint.h>
#include <assert.h>

#include "../compat/pgmspace.h"
#include "bitmapscroller.h"
#include "28c3.h"


static uint8_t const a28c3Bitmap[2][30][9] PROGMEM =
	{{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x10, 0x00},
	  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x0e, 0x00},
	  {0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0xf0, 0x08, 0x80},
	  {0x00, 0x00, 0x00, 0x00, 0x00, 0x6f, 0xe0, 0x07, 0xc0},
	  {0x00, 0x00, 0x00, 0x00, 0x01, 0x7e, 0x40, 0x00, 0xfe},
	  {0x00, 0x00, 0x00, 0x0f, 0x05, 0xfa, 0x00, 0x01, 0xf8},
	  {0x00, 0x00, 0x01, 0x80, 0xd7, 0xef, 0x38, 0x0b, 0xf0},
	  {0x00, 0x00, 0x08, 0x0f, 0xf9, 0xb3, 0xf8, 0x2f, 0xf0},
	  {0x00, 0x00, 0x14, 0xf7, 0xf9, 0xff, 0xfe, 0x3f, 0xf0},
	  {0x00, 0x00, 0x30, 0x7f, 0xfa, 0x7f, 0xff, 0xff, 0xc0},
	  {0x00, 0x01, 0xcf, 0xff, 0xfd, 0x7f, 0xff, 0xff, 0x80},
	  {0x00, 0x04, 0x7f, 0xff, 0xf8, 0xff, 0xff, 0xff, 0x40},
	  {0x00, 0x03, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xfe, 0x80},
	  {0x00, 0x05, 0xff, 0xbf, 0x87, 0xff, 0xff, 0xfd, 0x00},
	  {0x00, 0x02, 0xce, 0x62, 0x3f, 0xff, 0xff, 0xf8, 0x00},
	  {0x00, 0x00, 0x41, 0x7f, 0xff, 0xff, 0xff, 0xe8, 0x00},
	  {0x00, 0x00, 0x02, 0xe1, 0xff, 0xff, 0xff, 0xc0, 0x00},
	  {0x00, 0x00, 0x05, 0xa3, 0xff, 0xff, 0xfe, 0xc0, 0x00},
	  {0x00, 0x00, 0x1a, 0x00, 0xff, 0xff, 0xf2, 0x00, 0x00},
	  {0x00, 0x00, 0x67, 0x41, 0xff, 0xfe, 0x60, 0x00, 0x00},
	  {0x00, 0x01, 0x01, 0xf8, 0xe0, 0x1c, 0x00, 0x00, 0x00},
	  {0x00, 0x18, 0x1f, 0xeb, 0xff, 0xc0, 0x00, 0x00, 0x00},
	  {0x01, 0xc0, 0xe0, 0xf1, 0xff, 0xa0, 0x00, 0x00, 0x00},
	  {0x0e, 0x18, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00},
	  {0x33, 0x80, 0x00, 0x17, 0xff, 0x00, 0x00, 0x00, 0x00},
	  {0x70, 0x00, 0x00, 0x5f, 0xfa, 0x00, 0x00, 0x00, 0x00},
	  {0x00, 0x00, 0x01, 0x7f, 0xe8, 0x00, 0x00, 0x00, 0x00},
	  {0x00, 0x00, 0x01, 0xff, 0xa0, 0x00, 0x00, 0x00, 0x00},
	  {0x00, 0x00, 0x00, 0x7c, 0x80, 0x00, 0x00, 0x00, 0x00},
	  {0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00}},

	 {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00},
	  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00},
	  {0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x07, 0x00},
	  {0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00},
	  {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00},
	  {0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x03, 0xfc},
	  {0x00, 0x00, 0x00, 0x7f, 0x0f, 0xf0, 0xf0, 0x07, 0xf8},
	  {0x00, 0x00, 0x07, 0xf0, 0x07, 0xcf, 0xfc, 0x1f, 0xf0},
	  {0x00, 0x00, 0x0f, 0x08, 0x00, 0xff, 0xff, 0xff, 0xe0},
	  {0x00, 0x00, 0x0f, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xe0},
	  {0x00, 0x00, 0x3f, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xc0},
	  {0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80},
	  {0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00},
	  {0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00},
	  {0x00, 0x01, 0xf0, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0x00},
	  {0x00, 0x00, 0x00, 0xf3, 0xff, 0xff, 0xff, 0xf0, 0x00},
	  {0x00, 0x00, 0x01, 0xf3, 0xff, 0xff, 0xff, 0xe0, 0x00},
	  {0x00, 0x00, 0x02, 0x41, 0xff, 0xff, 0xff, 0x00, 0x00},
	  {0x00, 0x00, 0x04, 0x01, 0xff, 0xff, 0xfc, 0x00, 0x00},
	  {0x00, 0x00, 0x18, 0x80, 0xff, 0xff, 0x80, 0x00, 0x00},
	  {0x00, 0x00, 0xff, 0xc1, 0xff, 0xe0, 0x00, 0x00, 0x00},
	  {0x00, 0x07, 0xe0, 0xf1, 0xff, 0xe0, 0x00, 0x00, 0x00},
	  {0x00, 0x3f, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00},
	  {0x01, 0xe0, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 0x00},
	  {0x0c, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x00, 0x00, 0x00},
	  {0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00},
	  {0x00, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00},
	  {0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00},
	  {0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00},
	  {0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00}}};


static uint8_t const nMargin = 5;


static uint8_t logo_28c3_getChunk(unsigned char const nBitPlane,
                                  unsigned char const nChunkX,
                                  unsigned char const nChunkY,
                                  unsigned int const nFrame)
{
	assert(nBitPlane < 2);
	assert(nChunkX < 9);
	assert(nChunkY < (nMargin + 30 + nMargin));

	if (nChunkY < nMargin || nChunkY >= (nMargin + 30))
	{
		return 0;
	}
	return pgm_read_byte(&a28c3Bitmap[nBitPlane][nChunkY - nMargin][nChunkX]);
}


void logo_28c3()
{
	// width 72, height 30+x, 2 bitplanes (4 colors), 600 frames à 50ms (20 fps)
	bitmap_scroll(72, nMargin + 30 + nMargin, 2, 600, 50, logo_28c3_getChunk);
}
