#ifndef TETRIS_PLAYFIELD_H_
#define TETRIS_PLAYFIELD_H_

#include <inttypes.h>
#include "../../autoconf.h"
#include "piece.h"


/*********
 * types *
 *********/

// directions to which a piece can be moved
typedef enum tetris_playfield_direction_t
{
	TETRIS_PFD_LEFT,
	TETRIS_PFD_RIGHT
}
tetris_playfield_direction_t;


// status of the playfield
typedef enum tetris_playfield_status_t
{
	TETRIS_PFS_READY,    /** ready to get next piece */
	TETRIS_PFS_HOVERING, /** piece is still hovering */
	TETRIS_PFS_GLIDING,  /** piece is gliding on the dump */
	TETRIS_PFS_DOCKED,   /** piece has been docked */
	TETRIS_PFS_GAMEOVER	 /** playfield is filled up */
}
tetris_playfield_status_t;


// tetris_playfield_t
typedef struct tetris_playfield_t
{
	int8_t nWidth;                    /** width of playfield */
	int8_t nHeight;                   /** height of playfield */
	tetris_piece_t *pPiece;           /** currently falling piece */
	int8_t nColumn;                   /** horz. piece pos. (0 is left) */
	int8_t nRow;                      /** vert. piece pos. (0 is top) */
	uint8_t nRowMask;                 /** removed lines relative to nRow */
	tetris_playfield_status_t status; /** status */
	int8_t nFirstMatterRow;           /** first row from top which has matter */
	uint16_t *dump;                   /** playfield itself */
}
tetris_playfield_t;


// iterator for predicted dump rows
typedef struct tetris_playfield_iterator_t
{
	tetris_playfield_t *pPlayfield; /** playfield to be examined */
	tetris_piece_t *pPiece;         /** piece which should be tested */
	int8_t nColumn;                 /** column where piece should be dropped */
	uint16_t nFullRow;              /** value of a full row */
	int8_t nCurrentRow;             /** the actual row in the playfield */
	int8_t nPieceHighestRow;        /** the highest row index of the piece */
	int8_t nPieceLowestRow;         /** the lowest row index of the piece */
	int8_t nStopRow;                /** the last row to be examined */
	uint16_t nRowBuffer;            /** internal buffer for returned rows */
}
tetris_playfield_iterator_t;


/****************************
 * construction/destruction *
 ****************************/

/**
 * constructs a playfield with the given diemensions
 * @param nWidth width of playfield (4 <= n <= 16)
 * @param nHeight height of playfield (4 <= n <= 124)
 * @return pointer to a newly created playfield
 */
tetris_playfield_t *tetris_playfield_construct(int8_t nWidth, int8_t nHeight);


/**
 * destructs a playfield
 * @param pPl pointer to the playfield to be destructed
 */
void tetris_playfield_destruct(tetris_playfield_t *pPl);


/*******************************
 * playfield related functions *
 *******************************/

/**
 * calculates number of lines for the given row mask
 * @param nRowMask row mask from which the no. of lines will be calculated
 * @return number of lines of the row mask
 */
uint8_t tetris_playfield_calculateLines(uint8_t nRowMask);


/**
 * resets playfield to begin a new game
 * @param pPl playfield to perform action on
 */
void tetris_playfield_reset(tetris_playfield_t *pPl);


/**
 * inserts a new piece
 * @param pPl playfield to perform action on
 * @param pPiece piece to be inserted
 * @param ppOldPiece [out] indirect pointer to former piece for deallocation
 */
void tetris_playfield_insertPiece(tetris_playfield_t *pPl,
                                  tetris_piece_t *pPiece,
                                  tetris_piece_t** ppOldPiece);


/**
 * detects if piece collides with s.th. at a given position
 * @param pPl playfield to perform action on
 * @param nColumn column where the piece should be moved
 * @param nRow row where the piece should be moved
 * @return 1 for collision, 0 otherwise
 */
uint8_t tetris_playfield_collision(tetris_playfield_t *pPl,
                                   int8_t nColumn,
                                   int8_t nRow);


/**
 * lowers piece by one row or finally docks it
 * @param pPl playfield to perform action on
 */
void tetris_playfield_advancePiece(tetris_playfield_t *pPl);


/**
 * moves piece to the given direction
 * @param pPl playfield to perform action on
 * @param direction direction (see tetris_playfield_direction_t)
 * @return 1 if piece could be moved, 0 otherwise
 */
uint8_t tetris_playfield_movePiece(tetris_playfield_t *pPl,
                                   tetris_playfield_direction_t direction);


/**
 * rotates piece to the given direction
 * @param pPl playfield to perform action on
 * @param r type of rotation (see tetris_piece_rotation_t)
 * @return 1 if piece could be rotated, 0 otherwise
 */
uint8_t tetris_playfield_rotatePiece(tetris_playfield_t *pPl,
                                     tetris_piece_rotation_t rotation);


/**
 * removes completed lines (if any) and lowers the dump
 * @param pPl playfield to perform action on
 */
void tetris_playfield_removeCompleteLines(tetris_playfield_t *pPl);


/*****************
 * get functions *
 *****************/

/**
 * returns the width of the playfield
 * @param pPl the playfield we want information from
 * @return width of the playfield
 */
int8_t tetris_playfield_getWidth(tetris_playfield_t *pPl);


/**
 * returns the height of the playfield
 * @param pPl the playfield we want information from
 * @return height of the playfield
 */
int8_t tetris_playfield_getHeight(tetris_playfield_t *pPl);


/**
 * returns the currently falling piece
 * @param pPl the playfield we want information from
 * @return pointer to the currently falling piece
 */
tetris_piece_t *tetris_playfield_getPiece(tetris_playfield_t *pPl);


/**
 * returns the column of the currently falling piece
 * @param pPl the playfield we want information from
 * @return column of the currently falling piece
 */
int8_t tetris_playfield_getColumn(tetris_playfield_t *pPl);


/**
 * returns the row of the currently falling piece
 * @param pPl the playfield we want information from
 * @return row of the currently falling piece
 */
int8_t tetris_playfield_getRow(tetris_playfield_t *pPl);


/**
 * returns the row of the currently falling piece
 * @param pPl the playfield we want information from
 * @return highest row with matter
 */
int8_t tetris_playfield_getFirstMatterRow(tetris_playfield_t *pPl);


/**
 * returns the row mask relative to nRow
 * @param pPl the playfield we want information from
 * @return bit mask of removed lines (relative to current position)
 */
uint8_t tetris_playfield_getRowMask(tetris_playfield_t *pPl);


/**
 * returns the status of the playfield
 * @param pPl the playfield we want information from
 * @return status of the playfield (see tetris_playfield_status_t)
 */
tetris_playfield_status_t tetris_playfield_getStatus(tetris_playfield_t *pPl);


/**
 * returns the given row of the dump (as bitmap)
 * @param pPl the playfield we want information from
 * @param nRow the number of the row (0 <= nRow <= 124)
 * @return bitmap of the requested row (LSB is leftmost column)
 */
uint16_t tetris_playfield_getDumpRow(tetris_playfield_t *pPl,
                                     int8_t nRow);


#ifdef GAME_BASTET

/**
 * returns the deepest possible row for a given piece
 * @param pPl the playfield on which we want to test a piece
 * @param pPiece the piece which should be tested
 * @param nColumn the column where the piece should be dropped
 * @return the row of the piece (playfield compliant coordinates)
 */
int8_t tetris_playfield_predictDeepestRow(tetris_playfield_t *pPl,
                                          tetris_piece_t *pPiece,
                                          int8_t nColumn);


/**
 * predicts the number of complete lines for a piece at a given column
 * @param pPl the playfield on which we want to test a piece
 * @param pPiece the piece which should be tested
 * @param nRow the row where the given piece collides
 * @param nColumn the column where the piece should be dropped
 * @return amount of complete lines
 */
int8_t tetris_playfield_predictCompleteLines(tetris_playfield_t *pPl,
                                             tetris_piece_t *pPiece,
                                             int8_t nRow,
                                             int8_t nColumn);


/**
 * predicts appearance of the bottom row and initializes an iterator structure
 * @param pIt a pointer to an iterator which should be initialized
 * @param pPl the playfield on which we want to test a piece
 * @param pPiece the piece which should be tested
 * @param nRow the row where the given piece collides
 * @param nColumn the column where the piece should be dropped
 * @return appearance of the predicted dump row at the bottom as bit mask
 */
uint16_t* tetris_playfield_predictBottomRow(tetris_playfield_iterator_t *pIt,
                                            tetris_playfield_t *pPl,
                                            tetris_piece_t *pPiece,
                                            int8_t nRow,
                                            int8_t nColumn);


/**
 * predicts appearance of the next row of the playfield (for a given iterator)
 * @param pIt a pointer to a dump iterator
 * @return appearance of next predicted row (or NULL -> no next line)
 */
uint16_t* tetris_playfield_predictNextRow(tetris_playfield_iterator_t *pIt);

#endif /* GAME_BASTET */

#endif /*TETRIS_PLAYFIELD_H_*/
