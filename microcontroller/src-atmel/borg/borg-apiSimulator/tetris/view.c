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
tetris_view_t* tetris_view_construct(tetris_playfield_t* pPl)
{
	// memory allocation
	assert(pPl != NULL);
	tetris_view_t* pView =
		(tetris_view_t*) malloc(sizeof(tetris_view_t));
	assert(pView != NULL);
	pView->pPl = pPl;
	
	// drawing playfield
	clear_screen(0);
	int i;
	for (i = 0; i < 16; i++)
	{
		setpixel((pixel){5, i}, colorBorder);
		setpixel((pixel){0, i}, colorBorder);
	}
	for (i = 1; i < 5; ++i)
	{
		setpixel((pixel){i,0}, colorBorder);
		setpixel((pixel){i,7}, colorBorder);
		setpixel((pixel){i,15}, colorBorder);
	}

	return pView;
}


/* Function:       tetris_view_destruct
 * Description:    destructs a view
 * Argument pView: pointer to the view which should be destructed
 * Return value:   void
 */
void tetris_view_destruct(tetris_view_t* pView)
{
	assert(pView != NULL);
	free(pView);
}


/***************************
 *  view related functions *
 ***************************/

/* Function:     tetris_view_updatePlayfield
 * Description:  destructs a view
 * Argument pV:  pointer to the view to be destructed
 * Return value: void
 */
void tetris_view_updatePlayfield(tetris_view_t* pV)
{
	assert(pV != NULL);
	
	int8_t nRow;
	int8_t nColumn;
	int8_t nPieceRow = tetris_playfield_getRow(pV->pPl) + 3;
	int8_t nStartRow =
		(nPieceRow < tetris_playfield_getHeight(pV->pPl)) ? nPieceRow + 3 : 15;
	uint16_t nRowBitmap;
	uint16_t nElementMask;
	unsigned char nColor;

	tetris_playfield_status_t status = tetris_playfield_getStatus(pV->pPl);

	for (nRow = nStartRow; nRow >= 0; --nRow)
	{	
		nRowBitmap = tetris_playfield_getDumpRow(pV->pPl, nRow);
		if ((status == TETRIS_PFS_HOVERING) || (status == TETRIS_PFS_GAMEOVER))
		{
			if ((nRow >= nStartRow) && (nRow <= nPieceRow))
			{
				int8_t y = nRow - nPieceRow;
				uint16_t nPiece =
					tetris_piece_getBitfield(tetris_playfield_getPiece(pV->pPl));
				/* clear all bits of the piece we are not interested in and
				 * align the remaing row to bit no. 0 */
				nPiece = (nPiece & (0x000F << (y << 2))) >> (y << 2);
				/* shift remaining part to current column */
				nPiece <<= tetris_playfield_getColumn(pV->pPl);
				/* cut off unwanted stuff */
				nPiece &= 0x03ff;
				/* finally embed piece into the view */
				nRowBitmap |= nPiece;
			}
		}
		
		nElementMask = 0x0001;
		for (nColumn = 0; nColumn < 10; ++nColumn)
		{
			if ((nRowBitmap & nElementMask) != 0)
			{
				nColor = colorPiece;
			}
			else
			{
				nColor = colorSpace;
			}
			setpixel((pixel){15-nColumn,nRow}, nColor);
			nElementMask <<= 1;
		}
	}
	
	wait(3000);
}

