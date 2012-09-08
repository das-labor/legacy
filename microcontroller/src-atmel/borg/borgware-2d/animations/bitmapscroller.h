/**
 * \addtogroup bitmap
 * @{
 */

/**
 * @file bitmapscroller.h
 * @brief Public interface for the Borg's bitmap scroller.
 * @author Christian Kroll
 */

#ifndef BITMAP_SCROLLER_H_
#define BITMAP_SCROLLER_H_

#include <stdint.h>


/**
 * This type definition describes a function which is supposed to provide an
 * eight-by-one pixel chunk of a bitmap for a given bit plane. The x-coordinates
 * are based on chunks, not on pixels (i.e. nChunkX=2 has to be interpreted as
 * a pixel based x-coordinate of 16). This way it is easy to provide the bitmap
 * via a simple lookup in an uint8_t typed array. (0,0) is considered as the top
 * left coordinate.
 * @param nBitPlane Number of the desired bit plane.
 * @param nChunkX x-coordinate of the chunk.
 * @param nChunkY y-coordinate of the chunk.
 * @param nFrame The current frame number (in case you want to animate sth.).
 * @return an eight-by-one chunk of the bitmap packed into an uint8_t typed
 */
typedef uint8_t (*bitmap_getChunk_t)(unsigned char const nBitPlane,
                                     unsigned char const nChunkX,
                                     unsigned char const nChunkY,
                                     unsigned int const nFrame);


void bitmap_scroll(unsigned char const nWidth,
                   unsigned char const nHeight,
                   unsigned char const nBitPlanes,
                   unsigned int const nTickCount,
                   int const nTick,
                   unsigned char const nFrameTickDivider,
                   unsigned char const nMovementTickDivider,
                   bitmap_getChunk_t fpGetChunk);

#endif /* BITMAP_SCROLLER_H_ */

/*@}*/
