/**
 * \addtogroup tetris
 * @{
 */

/**
 * @file bucket.h
 * @brief Public interface definitions of Tetris' game logic module.
 * @author Christian Kroll
 */


#ifndef BUCKET_H_
#define BUCKET_H_

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "../../config.h"
#include "piece.h"


/***********
 * defines *
 ***********/

#define TETRIS_BUCKET_INVALID -4
#define TETRIS_BUCKET_MAX_COLUMNS 16
#define TETRIS_BUCKET_MAX_ROWS (INT8_MAX - 4)


/*********
 * types *
 *********/

// directions to which a piece can be moved
enum tetris_bucket_direction_e
{
	TETRIS_BUD_LEFT  = -1,
	TETRIS_BUD_RIGHT = 1
};
#ifdef NDEBUG
	typedef int8_t tetris_bucket_direction_t;
#else
	typedef enum tetris_bucket_direction_e tetris_bucket_direction_t;
#endif


// status of the bucket
enum tetris_bucket_status_e
{
	TETRIS_BUS_HOVERING = 0, /** piece is hovering */
	TETRIS_BUS_GLIDING  = 1, /** piece is gliding on the dump */
	TETRIS_BUS_DOCKED   = 2, /** piece has been docked */
	TETRIS_BUS_READY    = 3, /** ready to get next piece */
	TETRIS_BUS_GAMEOVER	= 4  /** bucket is filled up */
};
#ifdef NDEBUG
	typedef uint8_t tetris_bucket_status_t;
#else
	typedef enum tetris_bucket_status_e tetris_bucket_status_t;
#endif


// tetris_bucket_t
typedef struct tetris_bucket_s
{
	int8_t nWidth;                  /** width of bucket */
	int8_t nHeight;                 /** height of bucket */
	tetris_piece_t *pPiece;         /** currently falling piece */
	int8_t nColumn;                 /** horz. piece pos. (0 is left) */
	int8_t nRow;                    /** vert. piece pos. (0 is top) */
	uint8_t nRowMask;               /** removed lines relative to nRow */
	tetris_bucket_status_t status;  /** status of the bucket */
	int8_t nFirstTaintedRow;        /** top most row which has matter */
	uint16_t nFullRow;              /** value of a full row */
	uint16_t *dump;                 /** bucket itself */
}
tetris_bucket_t;


// iterator for predicted dump rows
typedef struct tetris_bucket_iterator_s
{
	tetris_bucket_t *pBucket; /** bucket to be examined */
	uint16_t nPieceMap;       /** piece bitmap */
	int8_t nShift;            /** helper variable for shifting piece bitmaps */
	int8_t nCurrentRow;       /** the actual row in the bucket */
	int8_t nPieceTopRow;      /** the highest row index of the piece */
	int8_t nPieceBottomRow;   /** the lowest row index of the piece */
	int8_t nStopRow;          /** the last row to be examined */
	uint16_t nRowBuffer;      /** buffer for returned row */
}
tetris_bucket_iterator_t;


/****************************
 * construction/destruction *
 ****************************/

/**
 * constructs a bucket with the given dimensions
 * @param nWidth width of bucket (4 <= n <= 16)
 * @param nHeight height of bucket (4 <= n <= TETRIS_BUCKET_MAX_COLUMNS)
 * @return pointer to a newly created bucket
 */
tetris_bucket_t *tetris_bucket_construct(int8_t nWidth,
                                         int8_t nHeight);


/**
 * destructs a bucket
 * @param pBucket pointer to the bucket to be destructed
 */
inline static void tetris_bucket_destruct(tetris_bucket_t *pBucket)
{
	assert(pBucket != NULL);
	assert(pBucket->dump != NULL);
	free(pBucket->dump);
	free(pBucket);
}


/*******************************
 * bucket related functions *
 *******************************/

/**
 * calculates number of lines for the given row mask
 * @param nRowMask row mask from which the no. of lines will be calculated
 * @return number of lines of the row mask
 */
inline static uint8_t tetris_bucket_calculateLines(uint8_t nRowMask)
{
	uint8_t nLines = 0;
	if (nRowMask & 0x01)
	{
		++nLines;
	}
	if (nRowMask & 0x02)
	{
		++nLines;
	}
	if (nRowMask & 0x04)
	{
		++nLines;
	}
	if (nRowMask & 0x08)
	{
		++nLines;
	}
	return nLines;
}


/**
 * resets bucket to begin a new game
 * @param pBucket bucket to perform action on
 */
void tetris_bucket_reset(tetris_bucket_t *pBucket);


/**
 * inserts a new piece
 * @param pBucket bucket to perform action on
 * @param pPiece piece to be inserted
 * @return pointer to former piece for deallocation
 */
tetris_piece_t *tetris_bucket_insertPiece(tetris_bucket_t *pBucket,
                                          tetris_piece_t *pPiece);


/**
 * lowers piece by one row or finally docks it
 * @param pBucket bucket to perform action on
 */
void tetris_bucket_advancePiece(tetris_bucket_t *pBucket);


/**
 * moves piece to the given direction
 * @param pBucket bucket to perform action on
 * @param direction direction (see tetris_bucket_direction_t)
 * @return 1 if piece could be moved, 0 otherwise
 */
uint8_t tetris_bucket_movePiece(tetris_bucket_t *pBucket,
                                tetris_bucket_direction_t direction);


/**
 * rotates piece to the given direction
 * @param pBucket bucket to perform action on
 * @param r type of rotation (see tetris_piece_rotation_t)
 * @return 1 if piece could be rotated, 0 otherwise
 */
uint8_t tetris_bucket_rotatePiece(tetris_bucket_t *pBucket,
                                  tetris_piece_rotation_t rotation);


/**
 * removes completed lines (if any) and lowers the dump
 * @param pBucket bucket to perform action on
 */
void tetris_bucket_removeCompleteLines(tetris_bucket_t *pBucket);


/*****************
 * get functions *
 *****************/

/**
 * returns the width of the bucket
 * @param pBucket the bucket we want information from
 * @return width of the bucket
 */
inline static int8_t tetris_bucket_getWidth(tetris_bucket_t *pBucket)
{
	assert(pBucket != NULL);
	return pBucket->nWidth;
}


/**
 * returns the height of the bucket
 * @param pBucket the bucket we want information from
 * @return height of the bucket
 */
inline static int8_t tetris_bucket_getHeight(tetris_bucket_t *pBucket)
{
	assert(pBucket != NULL);
	return pBucket->nHeight;
}


/**
 * returns the currently falling piece
 * @param pBucket the bucket we want information from
 * @return pointer to the currently falling piece
 */
inline static tetris_piece_t *tetris_bucket_getPiece(tetris_bucket_t *pBucket)
{
	assert(pBucket != NULL);
	return pBucket->pPiece;
}


/**
 * returns the column of the currently falling piece
 * @param pBucket the bucket we want information from
 * @return column of the currently falling piece
 */
inline static int8_t tetris_bucket_getColumn(tetris_bucket_t *pBucket)
{
	assert(pBucket != NULL);
	return pBucket->nColumn;
}


/**
 * returns the row of the currently falling piece
 * @param pBucket the bucket we want information from
 * @return row of the currently falling piece
 */
inline static int8_t tetris_bucket_getRow(tetris_bucket_t *pBucket)
{
	assert(pBucket != NULL);
	return pBucket->nRow;
}


/**
 * returns the row of the currently falling piece
 * @param pBucket the bucket we want information from
 * @return highest row with matter
 */
inline static int8_t tetris_bucket_getFirstTaintedRow(tetris_bucket_t *pBucket)
{
	assert(pBucket != NULL);
	return pBucket->nFirstTaintedRow;
}


/**
 * returns the row mask relative to nRow
 * @param pBucket the bucket we want information from
 * @return bit mask of removed lines (relative to current position)
 */
inline static uint8_t tetris_bucket_getRowMask(tetris_bucket_t *pBucket)
{
	assert(pBucket != NULL);
	return pBucket->nRowMask;
}


/**
 * returns the status of the bucket
 * @param pBucket the bucket we want information from
 * @return status of the bucket (see tetris_bucket_status_t)
 */
inline static tetris_bucket_status_t tetris_bucket_getStatus(tetris_bucket_t *p)
{
	assert(p != NULL);
	return p->status;
}


/**
 * returns the given row of the dump (as bitmap)
 * @param pBucket the bucket we want information from
 * @param nRow the number of the row (0 <= nRow <= TETRIS_BUCKET_MAX_COLUMNS)
 * @return bitmap of the requested row (LSB is leftmost column)
 */
inline static uint16_t tetris_bucket_getDumpRow(tetris_bucket_t *pBucket,
                                                int8_t nRow)
{
	assert(pBucket != NULL);
	assert((0 <= nRow) && (nRow < pBucket->nHeight));
	return pBucket->dump[nRow];
}


#ifdef GAME_BASTET

/**
 * returns the deepest possible row for a given piece
 * @param pBucket the bucket on which we want to test a piece
 * @param pPiece the piece which should be tested
 * @param nStartRow the row where the collision detection should start
 * @param nColumn the column where the piece should be dropped
 * @return the row of the piece (bucket compliant coordinates)
 */
int8_t tetris_bucket_predictDeepestRow(tetris_bucket_t *pBucket,
                                       tetris_piece_t *pPiece,
                                       int8_t nStartRow,
                                       int8_t nColumn);


/**
 * predicts the number of complete lines for a piece at a given column
 * @param pBucket the bucket on which we want to test a piece
 * @param pPiece the piece which should be tested
 * @param nRow the row where the given piece collides
 * @param nColumn the column where the piece should be dropped
 * @return number of complete lines
 */
int8_t tetris_bucket_predictCompleteLines(tetris_bucket_t *pBucket,
                                          tetris_piece_t *pPiece,
                                          int8_t nRow,
                                          int8_t nColumn);


/**
 * predicts appearance of the bottom row and initializes an iterator structure
 * @param pIt a pointer to an iterator which should be initialized
 * @param pBucket the bucket on which we want to test a piece
 * @param pPiece the piece which should be tested
 * @param nRow the row where the given piece collides
 * @param nColumn the column where the piece should be dropped
 * @return appearance of the bottom row of the predicted dump (bit mask)
 */
uint16_t *tetris_bucket_predictBottomRow(tetris_bucket_iterator_t *pIt,
                                         tetris_bucket_t *pBucket,
                                         tetris_piece_t *pPiece,
                                         int8_t nRow,
                                         int8_t nColumn);


/**
 * predicts appearance of the next row (via iterator) of the bucket
 * @param pIt a pointer to a dump iterator
 * @return appearance of next predicted row (or NULL -> no next line)
 */
uint16_t *tetris_bucket_predictNextRow(tetris_bucket_iterator_t *pIt);

#endif /* GAME_BASTET */

#endif /*BUCKET_H_*/

/*@}*/
