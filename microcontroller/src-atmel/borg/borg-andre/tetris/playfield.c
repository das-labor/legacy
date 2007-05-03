#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include "playfield.h"
#include "piece.h"


/****************************
 * construction/destruction *
 ****************************/

/* Function:         tetris_playfield_construct
 * Description:      constructs a playfield with the given diemensions
 * Argument nWidth:  width of playfield (4 <= n <= 16)
 * Argument nHeight: height of playfield (4 <= n <= 124)
 * Return value:     pointer to a newly created playfield
 */
tetris_playfield_t *tetris_playfield_construct(int8_t nWidth,
                                               int8_t nHeight)
{
	assert((nWidth >= 4) && (nWidth <= 16));
	assert((nHeight >= 4) && (nHeight <= 124));

	tetris_playfield_t *pPlayfield =
		(tetris_playfield_t*) malloc(sizeof(tetris_playfield_t));

	if (pPlayfield != NULL)
	{
		// allocating mem for dump array
		pPlayfield->dump = (uint16_t*) calloc(nHeight, sizeof(uint16_t));

		if (pPlayfield->dump != NULL)
		{
			// setting desired attributes
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
	return NULL;
}


/* Function:     tetris_playfield_destruct
 * Description:  destructs a playfield
 * Argument pPl: pointer to the playfield to be destructed
 * Return value: void
 */
void tetris_playfield_destruct(tetris_playfield_t *pPl)
{
	assert(pPl != NULL);

	// if memory for the dump array has been allocated, free it
	if (pPl->dump != NULL)
	{
		free(pPl->dump);
	}
	free(pPl);
}


/*******************************
 * playfield related functions *
 *******************************/

/* Function:     tetris_playfield_reset
 * Description:  resets playfield to begin a new game
 * Argument pPl: playfield to perform action on
 * Return value: void
 */
void tetris_playfield_reset(tetris_playfield_t *pPl)
{
	assert(pPl != NULL);

	pPl->pPiece = NULL;
	pPl->nColumn = 0;
	pPl->nRow = 0;
	pPl->nRowMask = 0;

	// clear dump if it has been allocated in memory
	if (pPl->dump != NULL)
	{
		memset(pPl->dump, 0, pPl->nHeight);
	}

	pPl->status = TETRIS_PFS_READY;
}


/* Function:            tetris_playfield_insertPiece
 * Description:         inserts a new piece
 * Argument pPl:        playfield to perform action on
 * Argument pPiece:     piece to be inserted
 * Argument ppOldPiece: [out] indirect pointer to former piece for deallocation
 * Return value:        void
 */
void tetris_playfield_insertPiece(tetris_playfield_t *pPl,
                                  tetris_piece_t *pPiece,
                                  tetris_piece_t** ppOldPiece)
{
	assert((pPl != NULL) && (pPiece != NULL) && (ppOldPiece != NULL));

	// a piece can only be inserted in state TETRIS_PFS_READY
	assert(pPl->status == TETRIS_PFS_READY);

	// row mask is now meaningless
	pPl->nRowMask = 0;

	// replace old piece
	*ppOldPiece = pPl->pPiece;
	pPl->pPiece = pPiece;

	// set horizontal start position (in the middle of the top line)
	pPl->nColumn = (pPl->nWidth - 2) / 2;

	// set vertical start position (first piece row with matter at pos. 1)
	uint16_t nPieceMap = tetris_piece_getBitmap(pPl->pPiece);
	uint16_t nElementMask = 0xF000;
	pPl->nRow = -3;
	while ((nPieceMap & nElementMask) == 0)
	{
		++pPl->nRow;
		nElementMask >>= 4;
	}
	if (pPl->nRow < 0)
	{
		++pPl->nRow;
	}

	// did we already collide with something?
	if (tetris_playfield_collision(pPl, pPl->nColumn, pPl->nRow) == 1)
	{
		// game over man, game over!!
		pPl->status = TETRIS_PFS_GAMEOVER;
	}
	else
	{
		// bring it on!
		pPl->status = TETRIS_PFS_HOVERING;
	}
}


/* Function:         tetris_playfield_collision
 * Description:      detects if piece collides with s.th. at a given position
 * Argument pPl:     playfield to perform action on
 * Argument nColumn: column where the piece should be moved
 * Argument nRow:    row where the piece should be moved
 * Return value:     1 for collision, 0 otherwise
 */
uint8_t tetris_playfield_collision(tetris_playfield_t *pPl,
                                   int8_t nColumn,
                                   int8_t nRow)
{
	assert(pPl != NULL);

	// only allow coordinates which are within sane ranges
	assert((nColumn >= -4) && (nColumn < pPl->nWidth));
	assert((nRow >= -4) && (nRow < pPl->nHeight));

	// The rows of a piece get compared with the background one by one
	// until either a collision occures or all rows are compared. Both the
	// piece row and the part of the playfield it covers are represented in
	// 4 bits which were singled out from their corresponding uint16_t
	// values and are aligned to LSB. In case where a piece overlaps with
	// either the left or the right border we "enhance" the playfield part
	// via bit shifting and set all bits representing the border to 1.
	// 
	// NOTE: LSB represents the left most position. 
	uint16_t nPieceMap = tetris_piece_getBitmap(pPl->pPiece);
	uint16_t nPlayfieldPart;
	uint16_t nPieceRowMap;

	// negative nRow values indicate that the piece hasn't fully entered the
	// playfield yet which requires special treatment if the piece overlaps
	// with either the left or the right border
	if (nRow < 0)
	{
		uint16_t nBorderMask = 0x0000;
		// piece overlaps with left border
		if (nColumn < 0)
		{
			nBorderMask = 0x1111 << (-nColumn - 1);
		}
		// piece overlaps with right border
		else if ((nColumn + 3) >= pPl->nWidth)
		{
			nBorderMask = 0x8888 >> ((nColumn + 3) - pPl->nWidth);
		}
		// return if piece collides with border
		if ((nPieceMap & nBorderMask) != 0)
		{
			return 1;
		}
	}

	// here we check the part which has already entered the playfield
	for (int8_t y = (nRow < 0) ? -nRow : 0; y < 4; ++y)
	{
		// current piece row overlaps with lower border
		if ((y + nRow) >= pPl->nHeight)
		{
			// all 4 bits represent the lower border
			nPlayfieldPart = 0x000F;
		}
		// piece overlaps with left border
		else if (nColumn < 0)
		{
			// clear all bits we are not interested in
			nPlayfieldPart = (pPl->dump[y + nRow] & (0x000F >> -nColumn));
			// add zeros to the left (the bits "behind" the left border)
			nPlayfieldPart <<= -nColumn;
			// set bits beyond left border to 1
			nPlayfieldPart |= 0x000F >> (4 + nColumn);
		}
		// piece overlaps with right border
		else if ((nColumn + 3) >= pPl->nWidth)
		{
			// align the bits we are interested in to LSB
			// (thereby clearing the rest)
			nPlayfieldPart = pPl->dump[y + nRow] >> nColumn;
			// set bits beyond right border to 1
			nPlayfieldPart |= 0xFFF8 >> (nColumn + 3 - pPl->nWidth);
		}
		// current row neither overlaps with left, right nor lower border
		else
		{
			// clear all bits we are not interested in and align the
			// remaing row to LSB
			nPlayfieldPart =
				(pPl->dump[y + nRow] & (0x000F << nColumn)) >> nColumn;
		}

		// clear all bits of the piece we are not interested in and
		// align the remaing row to LSB
		nPieceRowMap = (nPieceMap & (0x000F << (y << 2))) >> (y << 2);

		// finally check for a collision
		if ((nPlayfieldPart & nPieceRowMap) != 0)
		{
			return 1;
		}
	}

	// if we reach here, no collision was detected
	return 0;
}


/* Function:     tetris_playfield_advancePiece
 * Description:  lowers piece by one row or finally docks it
 * Argument pPl: playfield to perform action on
 * Return value: void
 */
void tetris_playfield_advancePiece(tetris_playfield_t *pPl)
{
	assert(pPl != NULL);

	// a piece can only be lowered if it is hovering
	assert (pPl->status == TETRIS_PFS_HOVERING);

	if (tetris_playfield_collision(pPl, pPl->nColumn, pPl->nRow + 1))
	{
		uint16_t nPiece = tetris_piece_getBitmap(pPl->pPiece);

		// Is the playfield filled up?
		if ((pPl->nRow < 0) && (nPiece & (0x0FFF >> ((3 + pPl->nRow) << 2))) != 0)
		{
			pPl->status = TETRIS_PFS_GAMEOVER;
		}
		else
		{
			// determine valid start point for dump index
			int8_t nStartRow = ((pPl->nRow + 3) < pPl->nHeight) ? 
				(pPl->nRow + 3) : pPl->nHeight - 1;
			for (int8_t i = nStartRow; i >= pPl->nRow; --i)
			{
				int8_t y = i - pPl->nRow;

				// clear all bits of the piece we are not interested in and
				// align the rest to LSB
				uint16_t nPieceMap = (nPiece & (0x000F << (y << 2))) >> (y << 2);
				// shift the remaining content to the current column
				if (pPl->nColumn >= 0)
				{
					nPieceMap <<= pPl->nColumn;
				}
				else
				{
					nPieceMap >>= -pPl->nColumn;
				}
				// embed piece in playfield
				pPl->dump[i] |= nPieceMap;
			}

			// the piece has finally been docked
			pPl->status = TETRIS_PFS_DOCKED;
		}
	}
	else
	{
		// since there is no collision the piece may continue its travel
		// to the ground...
		pPl->nRow++;
	}
}


/* Function:           tetris_playfield_movePiece
 * Description:        moves piece to the given direction
 * Argument pPl:       playfield to perform action on
 * Argument direction: direction (see tetris_playfield_direction_t)
 * Return value:       1 if piece could be moved, 0 otherwise 
 */
uint8_t tetris_playfield_movePiece(tetris_playfield_t *pPl,
                                   tetris_playfield_direction_t direction)
{
	assert(pPl != NULL);

	// a piece can only be moved if it is still hovering
	assert(pPl->status == TETRIS_PFS_HOVERING);

	int8_t nOffset = (direction == TETRIS_PFD_LEFT) ? -1 : 1;
	if (tetris_playfield_collision(pPl, pPl->nColumn + nOffset, pPl->nRow) == 0)
	{
		pPl->nColumn += nOffset;
		return 1;
	}

	return 0;
}


/* Function:     tetris_playfield_rotatePiece
 * Description:  rotates piece to the given direction
 * Argument pPl: playfield to perform action on
 * Argument r:   type of rotation (see tetris_piece_rotation_t)
 * Return value: 1 if piece could be rotated, 0 otherwise 
 */
uint8_t tetris_playfield_rotatePiece(tetris_playfield_t *pPl,
                                     tetris_piece_rotation_t rotation)
{
	assert(pPl != NULL);

	// a piece can only be rotation if it is still hovering
	assert(pPl->status == TETRIS_PFS_HOVERING);

	tetris_piece_rotate(pPl->pPiece, rotation);

	// does the rotated piece cause a collision?
	if (tetris_playfield_collision(pPl, pPl->nColumn, pPl->nRow) != 0)
	{
		// in that case we revert the rotation
		if (rotation == TETRIS_PC_ROT_CW)
		{
			tetris_piece_rotate(pPl->pPiece, TETRIS_PC_ROT_CCW);
		}
		else
		{
			tetris_piece_rotate(pPl->pPiece, TETRIS_PC_ROT_CW);
		}

		return 0;
	}

	return 1;
}


/* Function:     tetris_playfield_removeCompletedLines
 * Description:  removes completed lines (if any) and lowers the dump
 * Argument pPl: playfield to perform action on
 * Return value: void
 */
void tetris_playfield_removeCompleteLines(tetris_playfield_t *pPl)
{
	assert(pPl != NULL);

	// rows can only be removed if we are in state TETRIS_PFS_DOCKED
	assert(pPl->status == TETRIS_PFS_DOCKED);

	// bit mask of a full row
	uint16_t nFullRow = 0xFFFF >> (16 - pPl->nWidth);

	// return value
	uint8_t nRowMask = 0;

	// determine sane start and stop values for the dump' index
	int8_t nStartRow =
		((pPl->nRow + 3) >= pPl->nHeight) ? pPl->nHeight - 1 : pPl->nRow + 3;
	int8_t nStopRow = (pPl->nRow < 0) ? 0 : pPl->nRow;

	// dump index variables
	//   for incomplete rows, both variables will be decremented
	//   for complete rows, only i gets decremented
	int8_t nLowestRow = nStartRow;

	// this loop only considers rows which are affected by the piece 
	for (int8_t i = nStartRow; i >= nStopRow; --i)
	{
		// is current row a full row?
		if ((nFullRow & pPl->dump[i]) == nFullRow)
		{
			// set corresponding bit for the return value
			nRowMask |= 0x0008 >> (nStartRow - i);
		}
		else
		{
			// if nLowestRow and i differ, the dump has to be shifted
			if (i < nLowestRow)
			{
				pPl->dump[nLowestRow] = pPl->dump[i];
			}
			--nLowestRow;
		}
	}

	// if rows have been removed, this loop shifts the rest of the dump
	uint8_t nComplete = nLowestRow - nStopRow + 1;
	if (nComplete > 0)
	{
		for (int8_t i = nStopRow - 1; nLowestRow >= 0; --i)
		{
			// is the row we are copying from below the upper border?
			if (i >= 0)
			{
				// just copy from that row
				pPl->dump[nLowestRow] = pPl->dump[i];
			}
			else
			{
				// rows above the upper border are always empty
				pPl->dump[nLowestRow] = 0;
			}
			--nLowestRow;
		}
	}

	// ready to get the next piece
	pPl->status = TETRIS_PFS_READY;

	pPl->nRowMask = nRowMask;
}


/*****************
 * get functions *
 *****************/

/* Function:     tetris_playfield_getWidth
 * Description:  returns the width of the playfield
 * Argument pPl: the playfield we want information from
 * Return value: width of the playfield
 */
int8_t tetris_playfield_getWidth(tetris_playfield_t *pPl)
{
	assert(pPl != NULL);
	return pPl->nWidth;
}


/* Function:     tetris_playfield_getHeight
 * Description:  returns the height of the playfield
 * Argument pPl: the playfield we want information from
 * Return value: height of the playfield
 */
int8_t tetris_playfield_getHeight(tetris_playfield_t *pPl)
{
	assert(pPl != NULL);
	return pPl->nHeight;
}


/* Function:     tetris_playfield_getPiece
 * Description:  returns the currently falling piece
 * Argument pPl: the playfield we want information from
 * Return value: pointer to the currently falling piece
 */
tetris_piece_t *tetris_playfield_getPiece(tetris_playfield_t *pPl)
{
	assert(pPl != NULL);
	return pPl->pPiece;
}


/* Function:     tetris_playfield_getColumn
 * Description:  returns the column of the currently falling piece
 * Argument pPl: the playfield we want information from
 * Return value: column of the currently falling piece
 */
int8_t tetris_playfield_getColumn(tetris_playfield_t *pPl)
{
	assert(pPl != NULL);
	return pPl->nColumn;
}


/* Function:     tetris_playfield_getRow
 * Description:  returns the row of the currently falling piece
 * Argument pPl: the playfield we want information from
 * Return value: row of the currently falling piece
 */
int8_t tetris_playfield_getRow(tetris_playfield_t *pPl)
{
	assert(pPl != NULL);
	return pPl->nRow;
}


/* Function:     tetris_playfield_getRowMask
 * Description:  returns the row mask relative to nRow 
 * Argument pPl: the playfield we want information from
 * Return value: the first 4 bits indicate which lines (relative to nRow)
 *               have been removed if we are in status TETRIS_PFS_READY
 *               LSB is the highest line
 */
uint8_t tetris_playfield_getRowMask(tetris_playfield_t *pPl)
{
	assert(pPl != NULL);
	return pPl->nRowMask;
}


/* Function:     tetris_playfield_getStatus
 * Description:  returns the status of the playfield
 * Argument pPl: the playfield we want information from
 * Return value: status of the playfield (see tetris_playfield_status_t)
 */
tetris_playfield_status_t tetris_playfield_getStatus(tetris_playfield_t *pPl)
{
	assert(pPl != NULL);
	return pPl->status;
}


/* Function:      tetris_playfield_getDumpRow
 * Description:   returns the given row of the dump (as bitmap)
 * Argument pPl:  the playfield we want information from
 * Argument nRow: the number of the row (0 <= nRow < height of playfield)
 * Return value:  bitmap of the requested row (LSB is leftmost column)
 */
uint16_t tetris_playfield_getDumpRow(tetris_playfield_t *pPl,
                                     int8_t nRow)
{
	assert(pPl != NULL);
	assert((0 <= nRow) && (nRow < pPl->nHeight));
	return pPl->dump[nRow];
}

