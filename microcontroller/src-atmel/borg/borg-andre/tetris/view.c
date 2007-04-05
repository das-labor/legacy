#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include "../pixel.h"
#include "../util.h"
#include "../scrolltext.h"
#include "view.h"

/* the API simulator and the real API have different named wait functions */ 
#ifdef __AVR__
	#define WAIT(ms) wait(ms)
#else
	#define WAIT(ms) myWait(ms)
#endif

/****************************
 * construction/destruction *
 ****************************/

/* Function:     tetris_view_construct
 * Description:  constructs a view for André's borg
 * Argument pPl: pointer to logic object which should be observed
 * Argument pPl: pointer to playfield which should be observed
 * Return value: pointer to a newly created view
 */
tetris_view_t *tetris_view_construct(tetris_logic_t *pLogic,
                                     tetris_playfield_t *pPl)
{
	// memory allocation
	assert((pLogic != NULL) && (pPl != NULL));
	tetris_view_t *pView =
		(tetris_view_t *) malloc(sizeof(tetris_view_t));
	assert(pView != NULL);

	// init
	memset(pView, 0, sizeof(tetris_view_t));
	pView->pLogic = pLogic;
	pView->pPl = pPl;

	// drawing some first stuff
	clear_screen(0);
	tetris_view_drawBorders(TETRIS_VIEW_COLORBORDER);

	return pView;
}


/* Function:       tetris_view_destruct
 * Description:    destructs a view
 * Argument pView: pointer to the view which should be destructed
 * Return value:   void
 */
void tetris_view_destruct(tetris_view_t *pView)
{
	assert(pView != NULL);
	free(pView);
}


/***************************
 *  view related functions *
 ***************************/

/* Function:     tetris_view_getDimensions
 * Description:  destructs a view
 * Argument w:   [out] pointer to an int8_t to store the playfield width
 * Argument h:   [out] pointer to an int8_t to store the playfield height
 * Return value: void
 */
void tetris_view_getDimensions(int8_t *w,
                               int8_t *h)
{
	assert((w != NULL) && (h != NULL));
	*w = 10;
	*h = 16;
}


/* Function:     tetris_view_update
 * Description:  informs a view about changes in the game
 * Argument pV:  pointer to the view which should be updated
 * Return value: void
 */
void tetris_view_update(tetris_view_t *pV)
{
	assert(pV != NULL);

	if (tetris_playfield_getRow(pV->pPl) <= -4)
	{
		return;
	}

	int8_t nPieceRow = tetris_playfield_getRow(pV->pPl);
	int8_t nStartRow =
		((nPieceRow + 3) < 16) ? (nPieceRow + 3) : 15;
	uint16_t nRowMap;
	uint16_t nElementMask;

	tetris_playfield_status_t status = tetris_playfield_getStatus(pV->pPl);

	for (int8_t nRow = nStartRow; nRow >= 0; --nRow)
	{	
		nRowMap = tetris_playfield_getDumpRow(pV->pPl, nRow);
		
		// if a piece is hovering it needs to be drawn
		if ((status == TETRIS_PFS_HOVERING) || (status == TETRIS_PFS_GAMEOVER))
		{
			if ((nRow >= nPieceRow))
			{
				int8_t y = nRow - nPieceRow;
				int8_t nColumn = tetris_playfield_getColumn(pV->pPl);
				uint16_t nPieceMap =
					tetris_piece_getBitmap(tetris_playfield_getPiece(pV->pPl));
				// clear all bits of the piece we are not interested in and
				// align the remaing row to LSB
				nPieceMap = (nPieceMap & (0x000F << (y << 2))) >> (y << 2);
				// shift remaining part to current column
				if (nColumn >= 0)
				{
					nPieceMap <<= nColumn;
				}
				else
				{
					nPieceMap >>= -nColumn;
				}
				// cut off unwanted stuff
				nPieceMap &= 0x03ff;
				// finally embed piece into the view
				nRowMap |= nPieceMap;
			}
		}
		
		nElementMask = 0x0001;

		for (int8_t x = 0; x < 10; ++x)
		{
			unsigned char nColor;
			if ((nRowMap & nElementMask) != 0)
			{
				nColor = TETRIS_VIEW_COLORPIECE;
			}
			else
			{
				nColor = TETRIS_VIEW_COLORSPACE;
			}
			setpixel((pixel){14-x,nRow}, nColor);
			nElementMask <<= 1;
		}
	}
	
	// draw preview piece
	tetris_view_drawPreviewPiece(tetris_logic_getPreviewPiece(pV->pLogic));
	
	// visual feedback to inform about a level change
	uint8_t nLevel = tetris_logic_getLevel(pV->pLogic);
	if (nLevel != pV->nOldLevel)
	{
		tetris_view_blinkBorders();
		pV->nOldLevel = nLevel;
	}
}


/* Function:     tetris_view_showResults
 * Description:  shows results after game
 * Argument pV:  pointer to the view which should show the reults
 * Return value: void
 */
void tetris_view_showResults(tetris_view_t *pV)
{
	char pszResults[48];
	uint16_t nScore = tetris_logic_getScore(pV->pLogic);
	uint16_t nHighscore = tetris_logic_getHighscore(pV->pLogic);
	uint8_t nLines = tetris_logic_getLines(pV->pLogic);
	
	if (nScore <= nHighscore)
	{
		snprintf(pszResults, 48 * sizeof(char),
			"</#Lines %u    Score %u    Highscore %u",
			nLines, nScore, nHighscore);
	}
	else
	{
		snprintf(pszResults, 48 * sizeof(char),
			"</#Lines %u    New Highscore %u", nLines, nScore);
	}
	
	scrolltext(pszResults);
}


/***************************
 * non-interface functions *
 ***************************/

/* Function:      tetris_view_drawPreviewPiece
 * Description:   redraws the preview window
 * Argmument pPc: pointer to the piece for the preview window (may be NULL)
 * Return value:  void
 */
void tetris_view_drawPreviewPiece(tetris_piece_t *pPc)
{
	if (pPc != NULL)
	{
		uint8_t nColor;
		uint16_t nElementMask = 0x0001;
		uint16_t nPieceMap = tetris_piece_getBitmap(pPc);
		
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
				setpixel((pixel) {3 - x, y + 6}, nColor);
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
				setpixel((pixel) {3 - x, y + 6}, TETRIS_VIEW_COLORSPACE);
			}
		}
	}
}


/* Function:         tetris_view_drawBorders
 * Description:      draws borders in the given color
 * Argument nColor:  the color for the border
 * Return value:     void
 */
void tetris_view_drawBorders(uint8_t nColor)
{
	// drawing playfield
	uint8_t x, y;
	for (y = 0; y < 16; ++y)
	{
		setpixel((pixel){4, y}, nColor);
		setpixel((pixel){15, y}, nColor);
	}
	for (y = 0; y < 5; ++y)
	{
		for (x = 0; x <= 3; ++x){
			setpixel((pixel){x, y}, nColor);
			setpixel((pixel){x, y + 11}, nColor);
		}
	}
}


/* Function:     tetris_view_blinkBorders
 * Description:  makes the borders blink to notify player of a level change
 * Return value: void
 */
void tetris_view_blinkBorders()
{
	for (uint8_t i = 0; i < TETRIS_VIEW_BLINK_COUNT; ++i)
	{
		tetris_view_drawBorders(TETRIS_VIEW_COLORPIECE);
		WAIT(TETRIS_VIEW_BLINK_DELAY);
		tetris_view_drawBorders(TETRIS_VIEW_COLORBORDER);
		WAIT(TETRIS_VIEW_BLINK_DELAY);
	}
}
