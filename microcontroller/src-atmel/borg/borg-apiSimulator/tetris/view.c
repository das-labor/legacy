#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include "../pixel.h"
#include "../util.h"
#include "view.h"


/****************************
 * construction/destruction *
 ****************************/

/* Function:     tetris_view_construct
 * Description:  constructs a view for André's borg
 * Argument pPl: pointer to the playfield which should be observed
 * Return value: pointer to a newly created view
 */
tetris_view_t *tetris_view_construct(tetris_playfield_t *pPl)
{
	// memory allocation
	assert(pPl != NULL);
	tetris_view_t *pView =
		(tetris_view_t*) malloc(sizeof(tetris_view_t));
	assert(pView != NULL);
	pView->pPl = pPl;
	
	// drawing playfield
	clear_screen(0);
	uint8_t x, y;
	for (y = 0; y < 16; ++y)
	{
		setpixel((pixel){4, y}, TETRIS_VIEW_COLORBORDER);
		setpixel((pixel){15, y}, TETRIS_VIEW_COLORBORDER);
	}
	for (y = 0; y < 5; ++y)
	{
		for (x = 0; x <= 3; ++x){
			setpixel((pixel){x, y}, TETRIS_VIEW_COLORBORDER);
			setpixel((pixel){x, y + 11}, TETRIS_VIEW_COLORBORDER);
		}
	}

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


/* Function:     tetris_view_updatePlayfield
 * Description:  informs a view about changes in the playfield
 * Argument pV:  pointer to the view which should be updated
 * Return value: void
 */
void tetris_view_updatePlayfield(tetris_view_t *pV)
{
	assert(pV != NULL);

	if (pV->pPl->nRow == -4)
	{
		return;
	}

	int8_t nPieceRow = tetris_playfield_getRow(pV->pPl);
	int8_t nStartRow =
		((nPieceRow + 3) < 16) ? (nPieceRow + 3) : 15;
	uint16_t nRowMap;
	uint16_t nElementMask;

	tetris_playfield_status_t status = tetris_playfield_getStatus(pV->pPl);

	int8_t nRow;
	for (nRow = nStartRow; nRow >= 0; --nRow)
	{	
		nRowMap = tetris_playfield_getDumpRow(pV->pPl, nRow);
		
		/* if a piece is hovering it needs to be drawn */
		if ((status == TETRIS_PFS_HOVERING) || (status == TETRIS_PFS_GAMEOVER))
		{
			if ((nRow >= nPieceRow))
			{
				int8_t y = nRow - nPieceRow;
				int8_t nColumn = tetris_playfield_getColumn(pV->pPl);
				uint16_t nPieceMap =
					tetris_piece_getBitfield(tetris_playfield_getPiece(pV->pPl));
				/* clear all bits of the piece we are not interested in and
				 * align the remaing row to LSB */
				nPieceMap = (nPieceMap & (0x000F << (y << 2))) >> (y << 2);
				/* shift remaining part to current column */
				if (nColumn >= 0)
				{
					nPieceMap <<= nColumn;
				}
				else
				{
					nPieceMap >>= -nColumn;
				}
				/* cut off unwanted stuff */
				nPieceMap &= 0x03ff;
				/* finally embed piece into the view */
				nRowMap |= nPieceMap;
			}
		}
		
		nElementMask = 0x0001;
		int8_t x;
		for (x = 0; x < 10; ++x)
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
}

/* Function:     tetris_view_updateNextPiece
 * Description:  informs a view about the next piece
 * Argument pV:  pointer to the view which should be updated
 * Argument pPc: pointer to the piece which should be drawn
 * Return value: void
 */
void tetris_view_updateNextPiece(tetris_view_t *pV,
                                  tetris_piece_t *pPc)
{
	uint8_t x;
	uint8_t y;
	uint8_t nColor;
	uint16_t nElementMask = 0x0001;
	uint16_t nPieceMap = tetris_piece_getBitfield(pPc);
	
	for (y = 0; y < 4; ++y)
	{
		for (x = 0; x < 4; ++x)
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
