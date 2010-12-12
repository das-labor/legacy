#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "../../compat/pgmspace.h"
#include "../../config.h"
#include "bucket.h"
#include "piece.h"


/***************************
 * non-interface functions *
 ***************************/

/**
 * determines if piece is either hovering or gliding
 * @param pBucket the bucket we want information from
 * @return TETRIS_BUS_HOVERING or TETRIS_BUS_GLIDING
 */
tetris_bucket_status_t tetris_bucket_hoverStatus(tetris_bucket_t* pBucket)
{
	assert(pBucket != NULL);

	// if the piece touches the dump we ensure that the status is "gliding"
	if (tetris_bucket_collision(pBucket, pBucket->nColumn, pBucket->nRow + 1))
	{
		return TETRIS_BUS_GLIDING;
	}
	// otherwise the status must be "hovering"
	else
	{
		return TETRIS_BUS_HOVERING;
	}
}


/****************************
 * construction/destruction *
 ****************************/

tetris_bucket_t *tetris_bucket_construct(int8_t nWidth,
                                         int8_t nHeight)
{
	assert((nWidth >= 4) && (nWidth <= 16));
	assert((nHeight >= 4) && (nHeight <= 124));

	tetris_bucket_t *pBucket =
			(tetris_bucket_t *)malloc(sizeof(tetris_bucket_t));

	if (pBucket != NULL)
	{
		// allocating memory for dump array
		pBucket->dump = (uint16_t*) calloc(nHeight, sizeof(uint16_t));

		if (pBucket->dump != NULL)
		{
			// setting requested attributes
			pBucket->nFirstTaintedRow = nHeight;
			pBucket->nWidth = nWidth;
			pBucket->nHeight = nHeight;
			// bit mask of a full row
			pBucket->nFullRow = 0xFFFF >> (16 - pBucket->nWidth);

			tetris_bucket_reset(pBucket);

			return pBucket;
		}
		else
		{
			free(pBucket);
			pBucket = NULL;
		}
	}
	return NULL;
}


void tetris_bucket_destruct(tetris_bucket_t *pBucket)
{
	assert(pBucket != NULL);

	// if memory for the dump array has been allocated, free it
	if (pBucket->dump != NULL)
	{
		free(pBucket->dump);
	}
	free(pBucket);
}


/*******************************
 * bucket related functions *
 *******************************/

uint8_t tetris_bucket_calculateLines(uint8_t nRowMask)
{
	uint8_t nMask = 0x0001;
	uint8_t nLines = 0;
	for (uint8_t i = 0; i < 4; ++i)
	{
		if ((nMask & nRowMask) != 0)
		{
			++nLines;
		}
		nMask <<= 1;
	}

	return nLines;
}


void tetris_bucket_reset(tetris_bucket_t *pBucket)
{
	assert(pBucket != NULL);

	pBucket->pPiece = NULL;
	pBucket->nColumn = 0;
	pBucket->nRow = 0;
	pBucket->nRowMask = 0;

	// clear dump if it has been allocated in memory
	if (pBucket->dump != NULL)
	{
		memset(pBucket->dump, 0, pBucket->nHeight * sizeof(uint16_t));
	}

	pBucket->status = TETRIS_BUS_READY;
}


int8_t tetris_bucket_getPieceStartPos(tetris_piece_t *pPiece)
{
	// set vertical start position (first piece row with matter at pos. 1)
	uint16_t nPieceMap = tetris_piece_getBitmap(pPiece);
	uint16_t nElementMask = 0xF000;
	int8_t nRow = -3;
	while ((nPieceMap & nElementMask) == 0)
	{
		++nRow;
		nElementMask >>= 4;
	}
	if (nRow < 0)
	{
		++nRow;
	}

	return nRow;
}


void tetris_bucket_insertPiece(tetris_bucket_t *pBucket,
                               tetris_piece_t *pPiece,
                               tetris_piece_t **ppOldPiece)
{
	assert((pBucket != NULL) && (pPiece != NULL) && (ppOldPiece != NULL));

	// a piece can only be inserted in state TETRIS_BUS_READY
	assert(pBucket->status == TETRIS_BUS_READY);

	// row mask is now meaningless
	pBucket->nRowMask = 0;

	// replace old piece
	*ppOldPiece = pBucket->pPiece;
	pBucket->pPiece = pPiece;

	// set horizontal start position (in the middle of the top line)
	pBucket->nColumn = (pBucket->nWidth - 2) / 2;

	// set vertical start position (first piece row with matter at pos. 1)
	pBucket->nRow = tetris_bucket_getPieceStartPos(pBucket->pPiece);

	// did we already collide with something?
	if (tetris_bucket_collision(pBucket, pBucket->nColumn, pBucket->nRow) == 1)
	{
		// game over man, game over!!
		pBucket->status = TETRIS_BUS_GAMEOVER;
	}
	else
	{
		// bring it on!
		pBucket->status = tetris_bucket_hoverStatus(pBucket);
	}
}


uint8_t tetris_bucket_collision(tetris_bucket_t *pBucket,
                                int8_t nColumn,
                                int8_t nRow)
{
	// A piece is represented by 16 bits (4 bits per row where the LSB marks the
	// left most position). The part of the bucket which is covered by the piece
	// is converted to this format (including the bucket borders) so that a
	// simple bitwise 'AND' tells us if the piece and the dump overlap.

	// only allow coordinates which are within sane ranges
	assert(pBucket != NULL);
	assert((nColumn > -4) && (nColumn < pBucket->nWidth));
	assert((nRow > -4) && (nRow < pBucket->nHeight));

	// left and right borders
	uint16_t const nPieceMap = tetris_piece_getBitmap(pBucket->pPiece);
	uint16_t nBucketPart = 0;
	if (nColumn < 0)
	{
		static uint16_t const nLeftPart[] PROGMEM = {0x7777, 0x3333, 0x1111};
		nBucketPart = pgm_read_word(&nLeftPart[nColumn + 3]);
	}
	else if (nColumn >= pBucket->nWidth - 3)
	{
		static uint16_t const nRightPart[] PROGMEM = {0xEEEE, 0xCCCC, 0x8888};
		nBucketPart = pgm_read_word(&nRightPart[pBucket->nWidth - nColumn - 1]);
	}
	// lower border
	if (nRow > pBucket->nHeight - 4)
	{
		nBucketPart |= 0xFFFF << ((pBucket->nHeight - nRow) * 4);
	}

	int8_t const nStop = (nRow + 3) < pBucket->nHeight ?
			nRow + 3 : pBucket->nHeight - 1;
	// mask those blocks which are not covered by the piece
	uint16_t nDumpMask = nColumn >= 0 ? 0x000F << nColumn : 0x000F >> -nColumn;
	// value for shifting blocks to the corresponding part of the piece
	int8_t nShift = -nColumn + (nRow < 0 ? 4 * -nRow : 0);
	for (int8_t y = nRow >= 0 ? nRow : 0; y <= nStop; ++y)
	{
		uint16_t nTemp = pBucket->dump[y] & nDumpMask;
		nBucketPart |= nShift >= 0 ? nTemp << nShift : nTemp >> -nShift;
		if ((nPieceMap & nBucketPart) != 0)
		{
			// collision
			return 1;
		}
		nShift += 4;
	}

	// if we reach here, no collision was detected
	return 0;
}


void tetris_bucket_advancePiece(tetris_bucket_t *pBucket)
{
	assert(pBucket != NULL);

	// a piece can only be lowered if it is hovering or gliding
	assert ((pBucket->status == TETRIS_BUS_HOVERING) ||
		(pBucket->status == TETRIS_BUS_GLIDING));

	if (tetris_bucket_collision(pBucket, pBucket->nColumn, pBucket->nRow + 1))
	{
		uint16_t nPiece = tetris_piece_getBitmap(pBucket->pPiece);

		// Is the bucket filled up?
		if ((pBucket->nRow < 0) &&
				(nPiece & (0x0FFF >> ((3 + pBucket->nRow) << 2))) != 0)
		{
			pBucket->status = TETRIS_BUS_GAMEOVER;
		}
		else
		{
			// determine valid start point for dump index
			int8_t nStartRow = ((pBucket->nRow + 3) < pBucket->nHeight) ?
				(pBucket->nRow + 3) : pBucket->nHeight - 1;
			for (int8_t i = nStartRow; i >= pBucket->nRow; --i)
			{
				int8_t y = i - pBucket->nRow;

				// clear all bits of the piece we are not interested in and
				// align the rest to LSB
				uint16_t nPieceMap = (nPiece & (0x000F << (y << 2))) >> (y << 2);
				// shift the remaining content to the current column
				if (pBucket->nColumn >= 0)
				{
					nPieceMap <<= pBucket->nColumn;
				}
				else
				{
					nPieceMap >>= -pBucket->nColumn;
				}
				// embed piece in bucket
				pBucket->dump[i] |= nPieceMap;
			}

			// update value for the highest row with matter
			int8_t nPieceRow = pBucket->nRow;
			uint16_t nMask = 0x000F;
			for (int i = 0; i < 4; ++i, nMask <<= 4)
			{
				if ((nMask & nPiece) != 0)
				{
					nPieceRow += i;
					break;
				}
			}
			pBucket->nFirstTaintedRow = (pBucket->nFirstTaintedRow > nPieceRow) ?
					nPieceRow : pBucket->nFirstTaintedRow;

			// the piece has finally been docked
			pBucket->status = TETRIS_BUS_DOCKED;
		}
	}
	else
	{
		// since there is no collision the piece may continue its travel
		// to the ground...
		pBucket->nRow++;

		// are we gliding?
		pBucket->status = tetris_bucket_hoverStatus(pBucket);
	}
}


uint8_t tetris_bucket_movePiece(tetris_bucket_t *pBucket,
                                tetris_bucket_direction_t direction)
{
	assert(pBucket != NULL);

	// a piece can only be moved if it is still hovering or gliding
	assert((pBucket->status == TETRIS_BUS_HOVERING) ||
		(pBucket->status == TETRIS_BUS_GLIDING));

	int8_t nOffset = (direction == TETRIS_BUD_LEFT) ? -1 : 1;
	if (tetris_bucket_collision(pBucket, pBucket->nColumn + nOffset,
			pBucket->nRow) == 0)
	{
		pBucket->nColumn += nOffset;

		// are we gliding?
		pBucket->status = tetris_bucket_hoverStatus(pBucket);
		return 1;
	}

	return 0;
}


uint8_t tetris_bucket_rotatePiece(tetris_bucket_t *pBucket,
                                  tetris_piece_rotation_t rotation)
{
	assert(pBucket != NULL);

	// a piece can only be rotation if it is still hovering or gliding
	assert((pBucket->status == TETRIS_BUS_HOVERING) ||
			(pBucket->status == TETRIS_BUS_GLIDING));

	tetris_piece_rotate(pBucket->pPiece, rotation);

	// does the rotated piece cause a collision?
	if (tetris_bucket_collision(pBucket, pBucket->nColumn, pBucket->nRow) != 0)
	{
		// in that case we revert the rotation
		if (rotation == TETRIS_PC_ROT_CW)
		{
			tetris_piece_rotate(pBucket->pPiece, TETRIS_PC_ROT_CCW);
		}
		else
		{
			tetris_piece_rotate(pBucket->pPiece, TETRIS_PC_ROT_CW);
		}

		return 0;
	}

	// are we gliding?
	pBucket->status = tetris_bucket_hoverStatus(pBucket);

	return 1;
}


void tetris_bucket_removeCompleteLines(tetris_bucket_t *pBucket)
{
	assert(pBucket != NULL);

	// rows can only be removed if we are in state TETRIS_BUS_DOCKED
	assert(pBucket->status == TETRIS_BUS_DOCKED);

	// bit mask (only 4 bits) that tells us if the n-th row after the
	// current nRow is complete (n-th bit set to 1, LSB represents nRow itself)
	pBucket->nRowMask = 0;

	// only consider rows which are affected by the piece (from low to high)
	//   for incomplete rows, both i and nShiftIndex will be decremented
	//   for complete rows, only i gets decremented
	int8_t nLowestRow = (pBucket->nRow + 3) < pBucket->nHeight ?
			pBucket->nRow + 3 : pBucket->nHeight - 1;
	int8_t nShiftIndex = nLowestRow;
	for (int8_t i = nLowestRow; i >= pBucket->nFirstTaintedRow; --i)
	{
		// is current row a full row?
		if ((pBucket->nFullRow & pBucket->dump[i]) == pBucket->nFullRow)
		{
			// set corresponding bit for the row mask
			pBucket->nRowMask |= 0x01 <<  (i - pBucket->nRow);
		}
		else
		{
			// if nShiftIndex and i differ, the dump has to be shifted
			if (i < nShiftIndex)
			{
				pBucket->dump[nShiftIndex] = pBucket->dump[i];
			}
			// if there were no completed lines within the range covered by the
			// piece, we don't need to look for those any further
			else if ((nLowestRow - i) >= 3)
			{
				break;
			}
			--nShiftIndex;
		}
	}
	// any completed rows removed?
	if (pBucket->nRowMask != 0)
	{
		// clear space from which the rows have been shifted away
		for (int8_t i = nShiftIndex; i >= pBucket->nFirstTaintedRow; --i)
		{
			pBucket->dump[i] = 0;
		}
		pBucket->nFirstTaintedRow = nShiftIndex + 1;
	}

	// ready to get the next piece
	pBucket->status = TETRIS_BUS_READY;
}


#ifdef GAME_BASTET

int8_t tetris_bucket_predictDeepestRow(tetris_bucket_t *pBucket,
                                       tetris_piece_t *pPiece,
                                       int8_t nStartingRow,
                                       int8_t nColumn)
{
	assert(pBucket != NULL);
	assert(pPiece != NULL);
	assert(nStartingRow >= -1 && nStartingRow < pBucket->nHeight);
	assert(nColumn >= -3 && nColumn < pBucket->nWidth);

	// exchange current piece of the bucket (to use its collision detection)
	tetris_piece_t *pActualPiece  = pBucket->pPiece;
	pBucket->pPiece = pPiece;

	// determine empty rows of the bottom of piece which may overlap the dump
	uint16_t nMap = tetris_piece_getBitmap(pPiece);
	int8_t nOffset = 0;
	if ((nMap & 0xF000) != 0)
		nOffset = 3;
	else if ((nMap & 0xFF00) != 0)
		nOffset = 2;
	else if ((nMap & 0xFFF0) != 0)
		nOffset = 1;
	int8_t nRow = nStartingRow - nOffset;

	// check if the piece collides with the left or the right wall
	if ((nRow < -3) || (((nColumn < 0) || (nColumn >= pBucket->nWidth - 3)) &&
			tetris_bucket_collision(pBucket, nColumn, nRow)))
	{
		nRow = TETRIS_BUCKET_INVALIDROW;
	}
	// determine deepest row
	else
	{
		while (!tetris_bucket_collision(pBucket, nColumn, nRow + 1))
		{
			++nRow;
		}
		if ((nRow < 0) && (((nRow + 4) * 4) << nMap))
		{
				nRow = TETRIS_BUCKET_INVALIDROW;
		}
	}

	// restore actual bucket piece
	pBucket->pPiece = pActualPiece;

	return nRow;
}


int8_t tetris_bucket_predictCompleteLines(tetris_bucket_t *pBucket,
                                          tetris_piece_t *pPiece,
                                          int8_t nRow,
                                          int8_t nColumn)
{
	assert(pBucket != NULL);
	assert(pPiece != NULL);
	assert(nRow > -4 && nRow < pBucket->nHeight);
	assert(nColumn > -4 && nColumn < pBucket->nWidth);

	// initialization
	int8_t nCompleteRows = 0;
	uint16_t nPieceMap = tetris_piece_getBitmap(pPiece);
	int8_t nStartRow = nRow;
	int8_t const nStopRow = (nRow + 3) >= pBucket->nHeight ?
			pBucket->nHeight - 1 : nRow + 3;
	if (nRow < 0)
	{
		nPieceMap >>= -nRow * 4;
		nStartRow = 0;
	}

	for (int8_t y = nStartRow; y <= nStopRow; ++y)
	{
		uint16_t nTemp = nPieceMap & 0x000F;
		nTemp = nColumn >= 0 ? nTemp << nColumn : nTemp >> -nColumn;
		if ((pBucket->dump[y] ^ nTemp) == pBucket->nFullRow)
		{
			++nCompleteRows;
		}
		nPieceMap >>= 4;
	}

	return nCompleteRows;
}


uint16_t* tetris_bucket_predictBottomRow(tetris_bucket_iterator_t *pIt,
                                         tetris_bucket_t *pBucket,
                                         tetris_piece_t *pPiece,
                                         int8_t nRow,
                                         int8_t nColumn)
{
	pIt->pBucket = pBucket;
	pIt->nCurrentRow = pBucket->nHeight - 1;
	pIt->nRowBuffer = 0;

	// determine sane start and stop values for the piece's row indices
	pIt->nPieceHighestRow = nRow;
	pIt->nPieceLowestRow = ((pIt->nPieceHighestRow + 3) < pBucket->nHeight) ?
			(pIt->nPieceHighestRow + 3) : pBucket->nHeight - 1;

	// prepare piece bitmap for faster detection of complete lines
	pIt->nPieceMap = tetris_piece_getBitmap(pPiece);
	if ((nRow + 3) >= pBucket->nHeight)
	{
		pIt->nPieceMap <<= (nRow + 4 - pBucket->nHeight) * 4;
	}
	pIt->nShift = nColumn - 12;

	// don't return any trailing rows which are empty, so we look for a stop row
	pIt->nStopRow = pBucket->nFirstTaintedRow < nRow ?
			pBucket->nFirstTaintedRow : nRow;
	pIt->nStopRow = pIt->nStopRow < 0 ? 0 : pIt->nStopRow;

	return tetris_bucket_predictNextRow(pIt);
}


uint16_t* tetris_bucket_predictNextRow(tetris_bucket_iterator_t *pIt)
{
	if ((pIt->nPieceHighestRow > -4) && (pIt->nCurrentRow >= pIt->nStopRow))
	{
		uint16_t nTemp = 0;
		// embed piece if it is there
		if ((pIt->nCurrentRow <= pIt->nPieceLowestRow) &&
				(pIt->nCurrentRow >= pIt->nPieceHighestRow))
		{
			uint16_t nTemp = pIt->nPieceMap & 0xF000;
			nTemp |= pIt->nShift >= 0 ?
					nTemp << pIt->nShift : nTemp >> -pIt->nShift;
			pIt->nPieceMap <<= 4;
		}
		pIt->nCurrentRow = pIt->pBucket->dump[pIt->nCurrentRow--] | nTemp;
		// don't return full (and therefore removed) rows
		if (pIt->nRowBuffer == pIt->pBucket->nFullRow)
		{
			// recursively determine next (?) row instead
			return tetris_bucket_predictNextRow(pIt);
		}
		// row isn't full
		else
		{
			return &pIt->nRowBuffer;
		}
	}
	else
	{
		return NULL;
	}
}

#endif /* GAME_BASTET */
