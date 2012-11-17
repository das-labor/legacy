/**
 * \addtogroup tetris
 * @{
 */

/**
 * @file view.c
 * @brief Implementation of Tetris' graphical output routines.
 * @author Christian Kroll
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "../../config.h"
#include "../../pixel.h"
#include "../../util.h"
#include "../../scrolltext/scrolltext.h"
#include "bucket.h"
#include "piece.h"
#include "variants.h"
#include "view.h"


/***********
 * defines *
 ***********/

/** how often should the border blink (to indicate level up) */
#define TETRIS_VIEW_BORDER_BLINK_COUNT 2
/** amount of time (in ms) between border color changes */
#define TETRIS_VIEW_BORDER_BLINK_DELAY 100

/** how often should the lines blink when they get removed */
#define TETRIS_VIEW_LINE_BLINK_COUNT 3
/** amount of time (in ms) between line color changes */
#define TETRIS_VIEW_LINE_BLINK_DELAY 75

/** color of space */
#define TETRIS_VIEW_COLORSPACE   0
/** color of border */
#define TETRIS_VIEW_COLORBORDER  1
/** color of fading lines */
#define TETRIS_VIEW_COLORFADE    2
/** color of a piece */
#define TETRIS_VIEW_COLORPIECE   3
/** color of pause mode */
#define TETRIS_VIEW_COLORPAUSE   1
/** color of line counter */
#define TETRIS_VIEW_COLORCOUNTER 2


#ifdef GAME_TETRIS_FP
	#if NUM_ROWS < NUM_COLS
		#define VIEWCOLS NUM_ROWS
		#define VIEWROWS NUM_ROWS
	#elif NUM_ROWS > NUM_COLS
		#define VIEWCOLS NUM_COLS
		#define VIEWROWS NUM_COLS
	#else
		#define VIEWCOLS NUM_COLS
		#define VIEWROWS NUM_ROWS
	#endif
#else
	#define VIEWCOLS NUM_COLS
	#define VIEWROWS NUM_ROWS
#endif


#if VIEWROWS >= 20
	#define TETRIS_VIEW_YOFFSET_DUMP         ((VIEWROWS - 20) / 2)
	#define TETRIS_VIEW_HEIGHT_DUMP          20
#else
	#define TETRIS_VIEW_YOFFSET_DUMP         0
	#define TETRIS_VIEW_HEIGHT_DUMP          VIEWROWS
#endif


#if VIEWCOLS >= 16
	#define TETRIS_VIEW_XOFFSET_DUMP         (((VIEWCOLS - 16) / 2) + 1)
	#define TETRIS_VIEW_WIDTH_DUMP           10

	#if VIEWROWS >= 16
		#define TETRIS_VIEW_XOFFSET_COUNTER \
				(TETRIS_VIEW_XOFFSET_DUMP + TETRIS_VIEW_WIDTH_DUMP + 1)
		#define TETRIS_VIEW_YOFFSET_COUNT100 ((VIEWCOLS - 14) / 2)
		#define TETRIS_VIEW_YOFFSET_COUNT10  (TETRIS_VIEW_YOFFSET_COUNT100 + 2)
		#define TETRIS_VIEW_YOFFSET_COUNT1   (TETRIS_VIEW_YOFFSET_COUNT10 + 4)

		#define TETRIS_VIEW_XOFFSET_PREVIEW \
				(TETRIS_VIEW_XOFFSET_DUMP + TETRIS_VIEW_WIDTH_DUMP + 1)
		#define TETRIS_VIEW_YOFFSET_PREVIEW (TETRIS_VIEW_YOFFSET_COUNT1 + 4)
	#elif VIEWROWS < 16 && VIEWROWS >= 4
		#define TETRIS_VIEW_XOFFSET_PREVIEW \
				(TETRIS_VIEW_XOFFSET_DUMP + TETRIS_VIEW_WIDTH_DUMP + 1)
		#define TETRIS_VIEW_YOFFSET_PREVIEW  ((VIEWROWS - 4) / 2)
	#endif
#elif (VIEWCOLS < 16) && (VIEWCOLS >= 12)
	#define TETRIS_VIEW_XOFFSET_DUMP         ((VIEWCOLS - 10) / 2)
	#define TETRIS_VIEW_WIDTH_DUMP           10
#elif VIEWCOLS == 11
	#define TETRIS_VIEW_XOFFSET_DUMP         1
	#define TETRIS_VIEW_WIDTH_DUMP           10
#else
	#define TETRIS_VIEW_XOFFSET_DUMP         0
	#define TETRIS_VIEW_WIDTH_DUMP           VIEWCOLS
#endif



/***************************
 * non-interface functions *
 ***************************/

/**
 * setpixel replacement which may transform the pixel coordinates
 * @param nBearing bearing of the view
 * @param x x-coordinate of the pixel
 * @param y y-coordinate of the pixel
 * @param nColor Color of the pixel
 */
static void tetris_view_setpixel(tetris_bearing_t nBearing,
                                 uint8_t x,
                                 uint8_t y,
                                 uint8_t nColor)
{
	x = VIEWCOLS - 1 - x;

	pixel px;
	switch (nBearing)
	{
	default:
	case TETRIS_BEARING_0:
		px = (pixel){x, y};
		break;
	case TETRIS_BEARING_90:
		px = (pixel){y, VIEWCOLS - 1 - x};
		break;
	case TETRIS_BEARING_180:
		px = (pixel){VIEWCOLS - 1 - x, VIEWROWS - 1 - y};
		break;
	case TETRIS_BEARING_270:
		px = (pixel){VIEWROWS - 1 - y, x};
		break;
	}
	setpixel(px, nColor);
}


/**
 * draws a horizontal line
 * @param nBearing bearing of the view
 * @param x1 first x-coordinate of the line
 * @param x2 second x-coordinate of the line
 * @param y y-coordinate of the line
 * @param nColor Color of the line
 */
inline static void tetris_view_drawHLine(tetris_bearing_t nBearing,
                                         uint8_t x1,
                                         uint8_t x2,
                                         uint8_t y,
                                         uint8_t nColor)
{
	assert(x1 <= x2);

	for (uint8_t x = x1; x <= x2; ++x)
	{
		tetris_view_setpixel(nBearing, x, y, nColor);
	}
}


/**
 * draws a vertical line
 * @param nBearing bearing of the view
 * @param x x-coordinate of the line
 * @param y1 first y-coordinate of the line
 * @param y2 second y-coordinate of the line
 * @param nColor Color of the line
 */
inline static void tetris_view_drawVLine(tetris_bearing_t nBearing,
                                         uint8_t x,
                                         uint8_t y1,
                                         uint8_t y2,
                                         uint8_t nColor)
{
	assert(y1 <= y2);

	for (uint8_t y = y1; y <= y2; ++y)
	{
		tetris_view_setpixel(nBearing, x, y, nColor);
	}
}


/**
 * helper function to dim the piece color if game is paused
 * @param pV pointer to the view whose pause status is of interest
 */
inline static uint8_t tetris_view_getPieceColor(tetris_view_t *pV)
{
	if (pV->modeCurrent == TETRIS_VIMO_RUNNING)
	{
		return TETRIS_VIEW_COLORPIECE;
	}
	else
	{
		return TETRIS_VIEW_COLORPAUSE;
	}
}


/**
 * redraws the dump and the falling piece (if necessary)
 * @param pV pointer to the view on which the dump should be drawn
 */
static void tetris_view_drawDump(tetris_view_t *pV)
{
	assert(pV->pBucket != NULL);
	if (tetris_bucket_getRow(pV->pBucket) <= -4)
	{
		return;
	}
	tetris_bearing_t const nBearing =
			pV->pVariantMethods->getBearing(pV->pVariant);

	for (int8_t nRow = TETRIS_VIEW_HEIGHT_DUMP - 1; nRow >= 0; --nRow)
	{
		uint16_t nRowMap = tetris_bucket_getDumpRow(pV->pBucket, nRow);

		// if a piece is hovering or gliding it needs to be drawn
		int8_t nPieceRow = tetris_bucket_getRow(pV->pBucket);
		tetris_bucket_status_t status = tetris_bucket_getStatus(pV->pBucket);
		if (((status == TETRIS_BUS_HOVERING) || (status == TETRIS_BUS_GLIDING)
				|| (status == TETRIS_BUS_GAMEOVER)) && (nRow >= nPieceRow)
				&& (nRow <= nPieceRow + 3))
		{
			int8_t nColumn = tetris_bucket_getColumn(pV->pBucket);
			uint16_t nPieceMap =
					tetris_piece_getBitmap(tetris_bucket_getPiece(pV->pBucket));
			// clear all bits of the piece we are not interested in and
			// align the remaining row to LSB
			uint8_t y = (uint8_t)(nRow - nPieceRow);
			nPieceMap = (nPieceMap & (0x000Fu << (y * 4u))) >> (y * 4u);
			// shift remaining part to current column and embed piece into view
			nRowMap |= nColumn >= 0 ?
					nPieceMap << nColumn : nPieceMap >> -nColumn;
		}

		uint16_t nElementMask = 0x0001;
		for (uint8_t x = 0; x < TETRIS_VIEW_WIDTH_DUMP; ++x)
		{
			unsigned char nColor = (nRowMap & nElementMask) ?
					tetris_view_getPieceColor(pV) : TETRIS_VIEW_COLORSPACE;
			tetris_view_setpixel(nBearing, TETRIS_VIEW_XOFFSET_DUMP + x,
					TETRIS_VIEW_YOFFSET_DUMP + (uint8_t)nRow, nColor);
			nElementMask <<= 1;
		}
	}
}


#ifdef TETRIS_VIEW_XOFFSET_PREVIEW
/**
 * redraws the preview window
 * @param pV pointer to the view on which the piece should be drawn
 * @param pPc pointer to the piece for the preview window (may be NULL)
 */
static void tetris_view_drawPreviewPiece(tetris_view_t *pV,
                                         tetris_piece_t *pPc)
{
	tetris_bearing_t nBearing =
			pV->pVariantMethods->getBearing(pV->pVariant);

	if (pPc != NULL)
	{
		uint8_t nColor;
		uint16_t nElementMask = 0x0001;
		uint16_t nPieceMap;
		if (pV->modeCurrent == TETRIS_VIMO_RUNNING)
		{
			nPieceMap = tetris_piece_getBitmap(pPc);
		}
		else
		{
			// an iconized "P"
			nPieceMap = 0x26a6;
		}

		for (uint8_t y = 0; y < 4; ++y)
		{
			for (uint8_t x = 0; x < 4; ++x)
			{
				if ((nPieceMap & nElementMask) != 0)
				{
					nColor = TETRIS_VIEW_COLORPIECE;
				}
				else
				{
					nColor = TETRIS_VIEW_COLORSPACE;
				}
				tetris_view_setpixel(nBearing,
						TETRIS_VIEW_XOFFSET_PREVIEW + x,
						TETRIS_VIEW_YOFFSET_PREVIEW + y,
						nColor);
				nElementMask <<= 1;
			}
		}
	}
	else
	{
		for (uint8_t y = 0; y < 4; ++y)
		{
			for (uint8_t x = 0; x < 4; ++x)
			{
				tetris_view_setpixel(nBearing,
						TETRIS_VIEW_XOFFSET_PREVIEW + x,
						TETRIS_VIEW_YOFFSET_PREVIEW + y,
						TETRIS_VIEW_COLORSPACE);
			}
		}
	}
}
#endif

/**
 * draws borders in the given color
 * @param pV pointer to the view on which the borders should be drawn
 * @param nColor the color for the border
 */
static void tetris_view_drawBorders(tetris_view_t *pV,
                                    uint8_t nColor)
{
	tetris_bearing_t nBearing =
			pV->pVariantMethods->getBearing(pV->pVariant);

#if TETRIS_VIEW_YOFFSET_DUMP != 0
	// fill upper space if required
	for (uint8_t y = 0; y < TETRIS_VIEW_YOFFSET_DUMP; ++y)
	{
		tetris_view_drawHLine(nBearing, 0, VIEWCOLS - 1, y, nColor);
	}
#endif

#if VIEWROWS > TETRIS_VIEW_HEIGHT_DUMP
	// fill lower space if required
	uint8_t y = TETRIS_VIEW_YOFFSET_DUMP + TETRIS_VIEW_HEIGHT_DUMP;
	for (; y < VIEWROWS; ++y)
	{
		tetris_view_drawHLine(nBearing, 0, VIEWCOLS - 1, y, nColor);
	}
#endif

#if	TETRIS_VIEW_XOFFSET_DUMP != 0
	// fill left space if required
	for (uint8_t x = 0; x < TETRIS_VIEW_XOFFSET_DUMP; ++x)
	{
		tetris_view_drawVLine(nBearing, x, TETRIS_VIEW_YOFFSET_DUMP,
				TETRIS_VIEW_YOFFSET_DUMP + TETRIS_VIEW_HEIGHT_DUMP - 1, nColor);
	}
#endif

#if VIEWCOLS > 16
	// fill right space if required
	uint8_t x = TETRIS_VIEW_XOFFSET_DUMP + TETRIS_VIEW_WIDTH_DUMP + 5;
	for (; x < VIEWCOLS; ++x)
	{
		tetris_view_drawVLine(nBearing, x, TETRIS_VIEW_YOFFSET_DUMP,
				TETRIS_VIEW_YOFFSET_DUMP + TETRIS_VIEW_HEIGHT_DUMP - 1, nColor);
	}
#endif


#ifdef TETRIS_VIEW_XOFFSET_COUNTER
	tetris_view_drawVLine(nBearing, TETRIS_VIEW_XOFFSET_COUNTER - 1,
			TETRIS_VIEW_YOFFSET_DUMP,
			TETRIS_VIEW_YOFFSET_DUMP + TETRIS_VIEW_HEIGHT_DUMP - 1, nColor);

	for (uint8_t x = TETRIS_VIEW_XOFFSET_COUNTER;
			x < TETRIS_VIEW_XOFFSET_COUNTER + 3; ++x)
	{
		tetris_view_drawVLine(nBearing, x, TETRIS_VIEW_YOFFSET_DUMP,
				TETRIS_VIEW_YOFFSET_COUNT100 - 1, nColor);
		tetris_view_drawVLine(nBearing, x, TETRIS_VIEW_YOFFSET_PREVIEW + 4,
				TETRIS_VIEW_YOFFSET_DUMP + TETRIS_VIEW_HEIGHT_DUMP - 1, nColor);
	}

	tetris_view_drawVLine(nBearing, TETRIS_VIEW_XOFFSET_COUNTER + 3,
			TETRIS_VIEW_YOFFSET_DUMP, TETRIS_VIEW_YOFFSET_COUNT1 + 3, nColor);

	tetris_view_drawVLine(nBearing, TETRIS_VIEW_XOFFSET_COUNTER + 3,
			TETRIS_VIEW_YOFFSET_PREVIEW + 4,
			TETRIS_VIEW_YOFFSET_DUMP + TETRIS_VIEW_HEIGHT_DUMP - 1, nColor);

	tetris_view_drawHLine(nBearing, TETRIS_VIEW_XOFFSET_COUNTER,
			TETRIS_VIEW_XOFFSET_COUNTER + 3, TETRIS_VIEW_YOFFSET_COUNT100 + 1,
			nColor);

	tetris_view_drawHLine(nBearing, TETRIS_VIEW_XOFFSET_COUNTER,
			TETRIS_VIEW_XOFFSET_COUNTER + 3, TETRIS_VIEW_YOFFSET_COUNT10 + 3,
			nColor);

	tetris_view_drawHLine(nBearing, TETRIS_VIEW_XOFFSET_COUNTER,
			TETRIS_VIEW_XOFFSET_COUNTER + 3, TETRIS_VIEW_YOFFSET_COUNT1 + 3,
			nColor);
#elif defined TETRIS_VIEW_XOFFSET_PREVIEW
	tetris_view_drawVLine(nBearing, TETRIS_VIEW_XOFFSET_PREVIEW - 1,
			TETRIS_VIEW_YOFFSET_DUMP,
			TETRIS_VIEW_YOFFSET_DUMP + TETRIS_VIEW_HEIGHT_DUMP - 1, nColor);

	for (uint8_t x = TETRIS_VIEW_XOFFSET_PREVIEW;
			x < TETRIS_VIEW_XOFFSET_PREVIEW + 4; ++x)
	{
		tetris_view_drawVLine(nBearing, x, TETRIS_VIEW_YOFFSET_DUMP,
				TETRIS_VIEW_YOFFSET_PREVIEW - 1, nColor);
		tetris_view_drawVLine(nBearing, x, TETRIS_VIEW_YOFFSET_PREVIEW + 4,
				TETRIS_VIEW_YOFFSET_DUMP + TETRIS_VIEW_HEIGHT_DUMP - 1, nColor);
	}
#elif TETRIS_VIEW_WIDTH_DUMP < VIEWCOLS
	for (uint8_t x = TETRIS_VIEW_XOFFSET_DUMP + TETRIS_VIEW_WIDTH_DUMP;
			x < VIEWCOLS; ++x)
	{
		tetris_view_drawVLine(nBearing, x, TETRIS_VIEW_YOFFSET_DUMP,
				TETRIS_VIEW_YOFFSET_DUMP + TETRIS_VIEW_HEIGHT_DUMP - 1, nColor);
	}
#endif
}


/**
 * lets the borders blink to notify player of a level change
 * @param pV pointer to the view whose borders should blink
 */
static void tetris_view_blinkBorders(tetris_view_t *pV)
{
	for (uint8_t i = TETRIS_VIEW_BORDER_BLINK_COUNT * 2; i--;)
	{
		tetris_view_drawBorders(pV, (i & 0x01) ?
				TETRIS_VIEW_COLORBORDER : TETRIS_VIEW_COLORPIECE);
		wait(TETRIS_VIEW_BORDER_BLINK_DELAY);
	}
}


/**
 * lets complete lines blink to emphasize their removal
 * @param pV pointer to the view whose complete lines should blink
 */
static void tetris_view_blinkLines(tetris_view_t *pV)
{

	// reduce necessity of pointer arithmetic
	int8_t nRow = tetris_bucket_getRow(pV->pBucket);

	tetris_bearing_t nBearing =
			pV->pVariantMethods->getBearing(pV->pVariant);

	// don't try to draw below the border
	int8_t nDeepestRowOffset = ((nRow + 3) < TETRIS_VIEW_HEIGHT_DUMP ?
			3 : TETRIS_VIEW_HEIGHT_DUMP - (nRow + 1));

	// this loop controls how often the lines should blink
	for (uint8_t i = 0; i < TETRIS_VIEW_LINE_BLINK_COUNT; ++i)
	{
		// this loop determines the color of the line to be drawn
		for (uint8_t nColIdx = 0; nColIdx < 2; ++nColIdx)
		{
			// iterate through the possibly complete lines
			for (uint8_t j = 0, nMask = 0x01; j <= nDeepestRowOffset; ++j)
			{
				// is current line a complete line?
				if ((tetris_bucket_getRowMask(pV->pBucket) & nMask) != 0)
				{
					// draw line in current color
					int8_t y = nRow + j;
					for (int8_t x = tetris_bucket_getWidth(pV->pBucket); x--;)
					{

						uint8_t nColor = (nColIdx == 0 ? TETRIS_VIEW_COLORFADE
								: TETRIS_VIEW_COLORPIECE);
						tetris_view_setpixel(nBearing,
								TETRIS_VIEW_XOFFSET_DUMP + (uint8_t)x,
								TETRIS_VIEW_YOFFSET_DUMP + (uint8_t)y,
								nColor);
					}
				}
				nMask <<= 1;
			}
			// wait a few ms to make the blink effect visible
			wait(TETRIS_VIEW_LINE_BLINK_DELAY);
		}
	}
}


#ifdef TETRIS_VIEW_XOFFSET_COUNTER
/**
 * draws counter of completed rows (0-399)
 * @param pV pointer to the view
 */
static void tetris_view_drawLineCounter(tetris_view_t *pV)
{

	tetris_bearing_t nBearing =
			pV->pVariantMethods->getBearing(pV->pVariant);

	// get number of completed lines
	uint16_t nLines = pV->pVariantMethods->getLines(pV->pVariant);

	// get decimal places
	uint8_t nOnes = nLines % 10;
	uint8_t nTens = (nLines / 10) % 10;
	uint8_t nHundreds = (nLines / 100) % 10;

	// draws the decimal places as 3x3 squares with 9 pixels
	for (uint8_t i = 0, x = 0, y = 0; i < 9; ++i)
	{
		// pick drawing color for the ones
		uint8_t nOnesPen = nOnes > i ?
			TETRIS_VIEW_COLORCOUNTER : TETRIS_VIEW_COLORSPACE;
		tetris_view_setpixel(nBearing, TETRIS_VIEW_XOFFSET_COUNTER + x,
				TETRIS_VIEW_YOFFSET_COUNT1 + y, nOnesPen);

		// pick drawing color for the tens
		uint8_t nTensPen = nTens > i ?
			TETRIS_VIEW_COLORCOUNTER : TETRIS_VIEW_COLORSPACE;
		tetris_view_setpixel(nBearing, TETRIS_VIEW_XOFFSET_COUNTER + x,
				TETRIS_VIEW_YOFFSET_COUNT10 + y, nTensPen);

		// a maximum of 399 lines can be displayed
		if (i < 3)
		{
			// pick drawing color for the hundreds
			uint8_t nHundredsPen = nHundreds > i ?
				TETRIS_VIEW_COLORCOUNTER : TETRIS_VIEW_COLORSPACE;
			tetris_view_setpixel(nBearing, TETRIS_VIEW_XOFFSET_COUNTER + x,
					TETRIS_VIEW_YOFFSET_COUNT100 + y, nHundredsPen);

		}

		// wrap lines if required
		if ((++x % 3) == 0)
		{
			++y;
			x = 0;
		}
	}
}
#endif


/**
 * unpacks the champion's initials from the uint16_t packed form
 * @param nHighscoreName the champion's initials packed into a uint16_t
 * @param pszName pointer to an array of char for the unpacked initials
 */
static void tetris_view_formatHighscoreName(uint16_t nHighscoreName,
                                            char *pszName)
{
	for (uint8_t i = 3; i--; nHighscoreName >>= 5)
	{
		if ((pszName[i] = (nHighscoreName & 0x1F) + 65) == '_')
		{
			pszName[i] = ' ';
		}
	}
	pszName[3] = '\0';
}


/****************************
 * construction/destruction *
 ****************************/

tetris_view_t *tetris_view_construct(tetris_variant_t const *const pVarMethods,
                                     void *pVariantData,
                                     tetris_bucket_t *pBucket)
{
	// memory allocation
	assert((pVariantData != NULL) && (pBucket != NULL));
	tetris_view_t *pView =
		(tetris_view_t *) malloc(sizeof(tetris_view_t));
	assert(pView != NULL);

	// init
	memset(pView, 0, sizeof(tetris_view_t));
	pView->pVariantMethods = pVarMethods;
	pView->pVariant = pVariantData;
	pView->pBucket = pBucket;
	pView->modeCurrent = pView->modeOld = TETRIS_VIMO_RUNNING;

	// drawing some first stuff
	clear_screen(0);
	tetris_view_drawBorders(pView, TETRIS_VIEW_COLORBORDER);

	return pView;
}


/***************************
 *  view related functions *
 ***************************/

void tetris_view_getDimensions(int8_t *w,
                               int8_t *h)
{
	assert((w != NULL) && (h != NULL));
	*w = TETRIS_VIEW_WIDTH_DUMP;
	*h = TETRIS_VIEW_HEIGHT_DUMP;
}


void tetris_view_update(tetris_view_t *pV)
{
	assert(pV != NULL);

	tetris_view_drawBorders(pV, TETRIS_VIEW_COLORBORDER);

#ifdef TETRIS_VIEW_XOFFSET_PREVIEW
	// draw preview piece
	tetris_view_drawPreviewPiece(pV,
			pV->pVariantMethods->getPreviewPiece(pV->pVariant));
#endif

	// let complete lines blink (if there are any)
	if (tetris_bucket_getRowMask(pV->pBucket) != 0)
	{
		tetris_view_blinkLines(pV);
	}

#ifdef TETRIS_VIEW_XOFFSET_COUNTER
	// update line counter
	tetris_view_drawLineCounter(pV);
#endif

	// draw dump
	tetris_view_drawDump(pV);

	// visual feedback to inform about a level change
	uint8_t nLevel = pV->pVariantMethods->getLevel(pV->pVariant);
	if (nLevel != pV->nOldLevel)
	{
		tetris_view_blinkBorders(pV);
		pV->nOldLevel = nLevel;
	}
}


void tetris_view_showResults(tetris_view_t *pV)
{
#ifdef SCROLLTEXT_SUPPORT
	char pszResults[55], pszHighscoreName[4];
	uint16_t nScore = pV->pVariantMethods->getScore(pV->pVariant);
	uint16_t nHighscore = pV->pVariantMethods->getHighscore(pV->pVariant);
	uint16_t nLines = pV->pVariantMethods->getLines(pV->pVariant);
	uint16_t nHighscoreName =
			pV->pVariantMethods->getHighscoreName(pV->pVariant);
	tetris_view_formatHighscoreName(nHighscoreName, pszHighscoreName);

	if (nScore <= nHighscore)
	{
		snprintf(pszResults, sizeof(pszResults),
			"</#Lines %u    Score %u    Highscore %u (%s)",
			nLines, nScore, nHighscore, pszHighscoreName);
	}
	else
	{
		snprintf(pszResults, sizeof(pszResults),
			"</#Lines %u    New Highscore %u", nLines, nScore);
	}
	scrolltext(pszResults);
#endif
}

/*@}*/
