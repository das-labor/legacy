#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "../random/prng.h"
#include "../util.h"
#include "../autoconf.h"
#include "../pixel.h"
#include "bitmapscroller.h"


/**
 * This structure merely contains the meta data of the bitmap. The actual
 * bitmap data is delivered by a user provided function.
 */
typedef struct bitmap_t
{
	unsigned char nWidth;          /**< Width of the bitmap. */
	unsigned char nHeight;         /**< Height of the bitmap. */
	unsigned char nBitPlanes;      /**< Number of bit planes. */
	bitmap_getChunk_t fpGetChunk;  /**< Bitmap chunk retrieving function. */
	unsigned int nFrame;           /**< Current frame number. */
	unsigned char nViewportWidth;  /**< Width of the displayed content. */
	unsigned char nViewportHeight; /**< Height of the displayed content. */
	unsigned char nXDomain;        /**< Last valid x-coordinate for viewport. */
	unsigned char nYDomain;        /**< Last valid y-coordinate for viewport. */
	unsigned char nChunkDomain;    /**< Last valid chunk for viewport. */
	unsigned char nChunkCount;     /**< Amount of horiz. chunks of the bitmap.*/
}
bitmap_t;


/**
 * This function generates an eight-by-one pixel chunk for a given pair of pixel
 * coordinates and a borg plane index. The resulting chunk can be copied
 * directly to the borg's frame buffer. It utilizes the user provided "getChunk"
 * function which in contrast only accepts chunk based coordinates (resulting in
 * chunks whose pixel based x-coordinates are always a multiple of eight) and
 * uses bit planes which defer from the plane scheme used by the borg's frame
 * buffer.
 * @param pBitmap The bitmap of interest.
 * @param nBorgPlane The nunmber of the borg plane of interest.
 * @param x x-coordinate of the bitmap
 * @param y y-coordinate of the bitmap
 * @return The bitmap chunk packed into an unsigned char.
 */
static unsigned char bitmap_getAlignedChunk(bitmap_t const *const pBitmap,
                                            unsigned char const nBorgPlane,
                                            unsigned char const x,
                                            unsigned char const y)
{
	assert(x <= pBitmap->nChunkDomain);
	assert(y < pBitmap->nHeight);

	unsigned char nChunk = 0xFF;
	unsigned char nMask = 1;
	unsigned char const nAlignment = x % 8;
	unsigned char const x_8 = x / 8;

	// we have to go through every bit plane
	for (unsigned char i = 0; i < pBitmap->nBitPlanes; ++i)
	{
		// generate chunk
		unsigned char nPlaneChunk;
		nPlaneChunk = pBitmap->fpGetChunk(i, x_8, y, pBitmap->nFrame)
				<< nAlignment;
		// in case of misalignment we combine two chunks with chunk based
		// coordinates into one chunk which aligns to the given x-coordinate
		if (nAlignment != 0)
		{
			nPlaneChunk |= pBitmap->fpGetChunk(i, x_8 + 1, y, pBitmap->nFrame)
					>> (8 - nAlignment);
		}

		// convert the bit plane information into the borg plane format
		nChunk &= ((nBorgPlane + 1) & nMask) == 0 ? ~nPlaneChunk : nPlaneChunk;
		nMask <<= 1;
	}

	return nChunk;
}


/**
 * This function actually draws the bitmap onto the screen. The viewport is the
 * part of the bitmap which can be seen on the display.
 * @param pBitmap The bitmap which shall be shown.
 * @param nX The x-coordinate of the bitmap which shall be displayed at the top
 *           left of the viewport.
 * @param nY The y-coordinate of the bitmap which shall be displayed at the top
 *           left of the viewport.
 */
static void bitmap_drawViewport(bitmap_t const *const pBitmap,
                                unsigned char const nX,
                                unsigned char const nY)
{
	assert(nX <= pBitmap->nXDomain);
	assert(nY <= pBitmap->nYDomain);

	for (unsigned char y = 0; y < pBitmap->nViewportHeight; ++y)
	{
		for (unsigned char x = 0; x < pBitmap->nViewportWidth; x += 8)
		{
			// determine correct column within frame buffer
			unsigned char const nCol = pBitmap->nChunkCount - 1 - (x / 8);

			for (unsigned char p = NUMPLANE; p--;)
			{
				// retrieve a chunk which can be copied into the frame buffer
				unsigned char nChunk;
#if ((NUM_COLS % 8) == 0)
				// borg widths which are a multiple of 8 allow for a straight
				// forward chunk retrieval
				nChunk = bitmap_getAlignedChunk(pBitmap, p, nX + x, nY + y);
#else
				// in case the borg width is not a multiple of 8 some shifting
				// is required to cover those bits who really affect the display
				if ((x + nX) > (8 - NUM_COLS % 8))
				{
					nChunk = bitmap_getAlignedChunk(pBitmap, p,
							nX + x - (8 - NUM_COLS % 8), nY + y);
				}
				else
				{
					nChunk = bitmap_getAlignedChunk(pBitmap, p,
							nX, nY + y) >> (8 - NUM_COLS % 8);
				}
#endif
				// if a bit in plane p + 1 is set, the corresponding bit in p
				// has to be set as well
				if (p < (NUMPLANE - 1))
				{
					nChunk |= pixmap[p + 1][y][nCol];
				}
				// copy chunk into corresponding frame buffer plane
				pixmap[p][y][nCol] = nChunk;
			}
		}
	}
}


/**
 * This functions calculates randomly chosen offsets to move the viewport
 * accross the bitmap.
 * @param pBitmap The bitmap of interest.
 * @param x The current x-coordinate of the viewport.
 * @param y The current y-coordinate of the viewport.
 * @param pdx Pointer to a variable which shall hold the horizontal offset.
 * @param pdy Pointer to a variable which shall hold the vertical offset.
 */
static void bitmap_recalculateVector(bitmap_t const *const pBitmap,
                                     unsigned char const x,
                                     unsigned char const y,
                                     signed char *const pdx,
                                     signed char *const pdy)
{
	if (((x + *pdx) > (pBitmap->nXDomain)) || ((x + *pdx) < 0))
	{
		*pdx = random8() % 2u * (x < (pBitmap->nXDomain / 2) ? 1 : -1);
	}
	if (((y + *pdy) > (pBitmap->nYDomain)) || ((y + *pdy) < 0))
	{
		*pdy = random8() % 2u * (y < (pBitmap->nYDomain / 2) ? 1 : -1);
	}
	if (*pdx == 0 && *pdy == 0)
	{
		*pdx = (x < (pBitmap->nXDomain / 2) ? 1 : -1);
		*pdy = (y < (pBitmap->nYDomain / 2) ? 1 : -1);
	}
}


/**
 * This function scrolls through a bitmap.
 * @param nWidth Width of the bitmap.
 * @param nHeight Height of bitmap.
 * @param nBitPlanes Amount of bit planes.
 * @param nTickCount How many ticks the animation will last.
 * @param nTick Time quantum in milliseconds.
 * @param nFrameTickDivider Amount of ticks between frame changes.
 * @param nMovementTickDiver Amount of ticks between movement changes.
 * @param fpGetChunk Function that returns an eight-by-one chunk of a bitmap.
 */
void bitmap_scroll(unsigned char const nWidth,
                   unsigned char const nHeight,
                   unsigned char const nBitPlanes,
                   unsigned int const nTickCount,
                   unsigned int const nTick,
                   unsigned char const nFrameTickDivider,
                   unsigned char const nMovementTickDivider,
                   bitmap_getChunk_t fpGetChunk)
{
	assert((nBitPlanes > 0) && (nBitPlanes <= 8));
	assert(nFrameTickDivider > 0);
	assert(nFrameTickDivider > 0);

	bitmap_t bitmap;

	// user provided values
	bitmap.nWidth = nWidth;
	bitmap.nHeight = nHeight;
	bitmap.nBitPlanes = nBitPlanes;
	bitmap.fpGetChunk = fpGetChunk;

	// calculate domains for coordinates and chunk indices
	bitmap.nViewportWidth = (NUM_COLS > nWidth) ? nWidth : NUM_COLS;
	bitmap.nViewportHeight = (NUM_ROWS > nHeight) ? nHeight : NUM_ROWS;
	bitmap.nXDomain = nWidth - bitmap.nViewportWidth;
	bitmap.nYDomain = nHeight - bitmap.nViewportHeight;
	bitmap.nChunkDomain = nWidth - 8;
	bitmap.nChunkCount = (((bitmap.nViewportWidth - 1) / 8) + 1);

	// initial starting point
	bitmap.nFrame = 0;
	unsigned char x = bitmap.nXDomain > 0 ? random8() % bitmap.nXDomain : 0;
	unsigned char y = bitmap.nYDomain > 0 ? random8() % bitmap.nYDomain : 0;
	signed char dx = 0;
	signed char dy = 0;

	for (unsigned int i = 0; i < nTickCount; ++i)
	{
		bitmap_drawViewport(&bitmap, x, y);
		if ((i % nFrameTickDivider) == 0)
		{
			++bitmap.nFrame;
		}
		if ((i % nMovementTickDivider) == 0)
		{
			bitmap_recalculateVector(&bitmap, x, y, &dx, &dy);
			x += bitmap.nWidth > bitmap.nViewportWidth ? dx : 0;
			y += bitmap.nHeight > bitmap.nViewportHeight ? dy : 0;
		}
		wait(nTick);
	}
}
