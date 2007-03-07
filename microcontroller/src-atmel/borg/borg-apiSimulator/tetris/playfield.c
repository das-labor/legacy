#include <stdlib.h>
#include "playfield.h"

/*****************************
 *  construction/destruction *
 *****************************/
tetris_playfield_t* tetris_playfield_construct(int8_t nWidth,
                                               int8_t nHeight)
{
	if (nWidth <= 16)
	{
		tetris_playfield_t* pPlayfield =
			(tetris_playfield_t*) malloc(sizeof(tetris_playfield_t));
			
		if (pPlayfield != NULL)
		{
			pPlayfield->contents =
				(uint16_t*) malloc(sizeof(uint16_t) * nHeight);
				
			if (pPlayfield->contents != NULL)
			{
				pPlayfield->nWidth = nWidth;
				pPlayfield->nHeight = nHeight;
				tetris_playfield_reset(pPlayfield);

				return pPlayfield;
			}
			else
			{
				free(pPlayfield);
				pPlayfield = NULL;
			}
		}
	}
	return NULL;
}

void tetris_playfield_destruct(tetris_playfield_t* pPl)
{
	if (pPl != NULL)
	{
		if (pPl->contents != NULL)
		{
			free(pPl->contents);
		}
		free(pPl);
	}
}


/********************************
 *  playfield related functions *
 ********************************/

/* Function:           tetris_playfield_reset
 * Description:        resets playfield to begin a new game
 * Argument pPl:       playfield to perform action on
 * Return value:       void
 */
void tetris_playfield_reset(tetris_playfield_t* pPl)
{
	if (pPl != NULL)
	{
		pPl->pPiece = NULL;
		pPl->nColumn = 0;
		pPl->nRow = 0;

		/* clear contents */
		if (pPl->contents != NULL)
		{
			int i;
			for (i = 0; i < pPl->nHeight; ++i)
			{
				pPl->contents[i] = 0;
			}
		}

		pPl->status = TETRIS_PFS_READY;
	}
}


/* Function:            tetris_playfield_insertPiece
 * Description:         inserts a new piece
 * Argument pPl:        playfield to perform action on
 * Argument pPiece:     piece to be inserted
 * Argument ppOldPiece: indirect pointer to former piece for deallocation
 * Return value:        0 corresponds to false, anything other to true
 */
uint8_t tetris_playfield_insertPiece(tetris_playfield_t* pPl,
                                     tetris_piece_t* pPiece,
                                     tetris_piece_t** ppOldPiece)
{
	/* a piece can only be inserted in state TETRIS_PFS_READY */
	if (pPl->status == TETRIS_PFS_READY)
	{
		/* replace old piece */
		*ppOldPiece = pPl->pPiece;
		pPl->pPiece = pPiece;

		/* set start position (in the middle of the top line) */
		pPl->nColumn = (pPl->nWidth - 2) / 2;
		pPl->nRow = -tetris_piece_lastMatterRow(pPiece) - 1;

		pPl->status = TETRIS_PFS_HOVERING;

		/* OK */
		return 1;
	}
	else
	{
		/* wrong state */
		*ppOldPiece = NULL;
		return 0;
	}
}


/* Function:         collision
 * Description:      detects if piece collides with s.th. at a given position
 * Argument pPl:     playfield to perform action on
 * Argument nColumn: column where the piece should be moved
 * Argument nRow:    row where the piece should be moved
 * Return value:     1 for collision, 0 otherwise
 */
uint8_t tetris_playfield_collision(tetris_playfield_t* pPl,
                                   int8_t nColumn,
                                   int8_t nRow)
{
	/* only allow coordinates which are within sane ranges */
	if ((nColumn < pPl->nWidth) && (nRow < pPl->nHeight))
	{
		/* The rows of a piece get compared with the background one by one
		 * until either a collision occures or all rows are compared. Both the
		 * piece row and the part of the playfield it covers are represented in
		 * 4 bits which were singled out from their corresponding uint16_t
		 * values and are aligned to LSB. In case where a piece overlaps with
		 * either the left or the right border we "enhance" the playfield part
		 * via bit shifting and set all bits representing the border to 1.
		 * 
		 * NOTE: LSB represents the left most position. 
		 */
		uint16_t nPiece = tetris_piece_getBitfield(pPl->pPiece);
		uint16_t nPlayfieldPart;
		uint16_t nPieceRow;
		int8_t y;

		/* start at a piece row which has actually entered the playfield */
		for (y = (nRow < 0) ? -nRow : 0; y < 4; ++y)
		{
			/* current piece row overlaps with lower border */
			if ((y + nRow) >= pPl->nHeight)
			{
				/* all 4 bits represent the lower border */
				nPlayfieldPart = 0x000F;
			}
			/* piece overlaps with left border */
			else if (nColumn < 0)
			{
				/* clear all bits we are not interested in */
				nPlayfieldPart =
					(pPl->contents[y + nRow] & (0x000F >> -nColumn));
				/* add zeros to the left (the bits "behind" the left border) */
				nPlayfieldPart <<= -nColumn;
				/* set bit representing left border to 1 */
				nPlayfieldPart |= 0x0008 >> (4 + nColumn);
			}
			/* piece overlaps with right border */
			else if ((nColumn + 3) >= pPl->nWidth)
			{
				/* align the bits we are interested in to LSB
				 * (thereby clearing the rest) */
				nPlayfieldPart = pPl->contents[y + nRow] >> nColumn;
				/* set bit representing right border to 1 */
				nPlayfieldPart |= 0x0008 >> (nColumn + 3 - pPl->nWidth);
			}
			/* current row neither overlaps with left, right nor lower border */
			else
			{
				/* clear all bits we are not interested in and align the
				 * remaing row to bit no. 0 */
				nPlayfieldPart =
					(pPl->contents[y + nRow] & (0x000F << nColumn)) >> nColumn;
			}

			/* clear all bits of the piece we are not interested in and
			 * align the remaing row to bit no. 0 */
			nPieceRow = (nPiece & (0x000F << (y << 2))) >> (y << 2);

			if ((nPlayfieldPart & nPieceRow) != 0)
			{
				return 1;
			}
		}
	}
	else
	{
		/* faulty coordinates will be treated as collisions */
		return 1;
	}

	/* if we reach here, no collision was detected */
	return 0;
}


/* Function:     tetris_playfield_advancePiece
 * Description:  lowers piece by one row or finally docks it
 * Argument pPl: playfield to perform action on
 * Return value: playfield status (see tetris_playfield_status_t)
 */
tetris_playfield_status_t tetris_playfield_advancePiece(tetris_playfield_t* pPl)
{
	if (pPl->status == TETRIS_PFS_HOVERING)
	{
		if (tetris_playfield_collision(pPl, pPl->nColumn, pPl->nRow + 1))
		{
			uint16_t nPiece = tetris_piece_getBitfield(pPl->pPiece);
			uint16_t nPieceRow;
			int8_t y;

			/* Is the playfield filled up? */
			if ((pPl->nRow < 0) && (nPiece & (0x0FFF >> (3 + pPl->nRow))) != 0)
			{
				pPl->status = TETRIS_PFS_GAMEOVER;
			}
			else
			{
				/* the expression (y + pPl->nRow) has to be a valid index for
				 * the contents array */
				for (y = (pPl->nRow < 0) ? -(pPl->nRow) : 0;
					(y < 4) && ((y + pPl->nRow) <= pPl->nHeight); ++y)
				{
					/* clear all bits of the piece we are not interested in */
					nPieceRow = nPiece & (0x000F << (y << 2));
					/* shift the remaining content to the current column */
					nPieceRow <<= pPl->nColumn - (y << 2);
					/* achor piece in playfield */
					pPl->contents[y + pPl->nRow] |= nPieceRow;
				}

				/* the piece has finally been docked */
				pPl->nColumn = 0;
				pPl->nRow = 0;
				pPl->status = TETRIS_PFS_DOCKED;
			}
		}
		else
		{
			/* since there is no collision the piece may continue its travel
			 * to the ground... */
			pPl->nRow++;
		}
	}
	return pPl->status;
}


/* Function:           tetris_playfield_movePiece
 * Description:        moves piece to the given direction
 * Argument pPl:       playfield to perform action on
 * Argument direction: direction (see tetris_playfield_direction_t)
 * Return value:       1 if piece could be moved, 0 otherwise 
 */
uint8_t tetris_playfield_movePiece(tetris_playfield_t* pPl,
                                   tetris_playfield_direction_t direction)
{
	if (pPl != NULL)
	{
		int8_t nOffset = (direction == TETRIS_PFD_LEFT) ? -1 : 1;
		if (tetris_playfield_collision(pPl, pPl->nColumn + nOffset, pPl->nRow) == 0)
		{
			pPl->nColumn += nOffset;
			return 1;
		}
	}

	return 0;
}


/* Function:     tetris_playfield_removeCompletedLines
 * Description:  removes completed lines (if any) and lowers the dump
 * Argument pPl: playfield to perform action on
 * Return value: number of removed lines (0-4)
 */
uint8_t tetris_playfield_removeCompleteLines(tetris_playfield_t* pPl)
{
	uint8_t nRowMask = 0;
	if (pPl->status == TETRIS_PFS_DOCKED)
	{
		uint16_t nFullRow = 0xFFFF >> (16 - pPl->nWidth);
		
		int8_t nStartRow =
			((pPl->nRow + 3) >= pPl->nHeight) ? pPl->nHeight - 1 : pPl->nRow + 3;
		int8_t nStopRow = (pPl->nRow < 0) ? 0 : pPl->nRow;
			
		int8_t nLowestRow = nStartRow;
		int8_t i;
		
		for (i = nStartRow; i >= nStopRow; --i)
		{
			if ((nFullRow & pPl->contents[i]) == nFullRow)
			{
				nRowMask |= 0x0008 >> (nStartRow - i);
			}
			else
			{
				if (i < nLowestRow)
				{
					pPl->contents[nLowestRow] = pPl->contents[i];
				}
				--nLowestRow;
			}
		}
		
		// still work in progress
		uint8_t nComplete = nLowestRow - nStopRow + 1;
		if (nComplete > 0)
		{
			for (i = nStopRow; nLowestRow >= 0; --i)
			{
				pPl->contents[nLowestRow] = pPl->contents[i];
				--nLowestRow;
			}
			/* tetris_playfield_shiftDown(pPl, nLowestRow, nComplete,
				nLowestRow + 1); */
		}
	}
	return nRowMask;
}


/* Function:            shiftDown
 * Description:         shifts rows by a step size within a given window of rows 
 * Argument pPl:        playfield to perform action on
 * Argument nLowestRow: the lowest row to begin with
 * Argument nSteps:     how many steps per shift should be done 
 * Argument nWindow:    the window within the shifts should be performed
 * Return value:        void
 */
void tetris_playfield_shiftDown(tetris_playfield_t* pPl,
                                uint8_t nLowestRow,
                                uint8_t nSteps,
                                uint8_t nWindow)
{
	int8_t i = 0;
	for (i = nLowestRow; i > (nLowestRow - nWindow); --i)
	{
		if ((i - nSteps) > (nLowestRow - nWindow))
		{
			pPl->contents[i] = pPl->contents[i - nSteps];
		}
		else
		{
			pPl->contents[i] = 0;
		}
	}
}
