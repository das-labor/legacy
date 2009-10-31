#ifndef TETRIS_PLAYFIELD_H_
#define TETRIS_PLAYFIELD_H_

#include <inttypes.h>
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
	TETRIS_PFS_READY,    // ready to get next piece
	TETRIS_PFS_HOVERING, // piece is still hovering
	TETRIS_PFS_GLIDING,  // piece is gliding on the dump
	TETRIS_PFS_DOCKED,   // piece has been docked
	TETRIS_PFS_GAMEOVER	 // playfield is filled up
}
tetris_playfield_status_t;


// tetris_playfield_t
typedef struct tetris_playfield_t
{
	int8_t nWidth;                    // width of playfield
	int8_t nHeight;                   // height of playfield
	tetris_piece_t *pPiece;           // currently falling piece
	int8_t nColumn;                   // horz. piece pos. (0 is left)
	int8_t nRow;                      // vert. piece pos. (0 is top)
	uint8_t nRowMask;                 // removed lines relative to nRow (bitmap)
	tetris_playfield_status_t status; // status
	uint16_t *dump;                   // playfield itself
}
tetris_playfield_t;


/****************************
 * construction/destruction *
 ****************************/

/* Function:         tetris_playfield_construct
 * Description:      constructs a playfield with the given diemensions
 * Argument nWidth:  width of playfield (4 <= n <= 16)
 * Argument nHeight: height of playfield (4 <= n <= 124)
 * Return value:     pointer to a newly created playfield
 */
tetris_playfield_t *tetris_playfield_construct(int8_t nWidth, int8_t nHeight);


/* Function:     tetris_playfield_destruct
 * Description:  destructs a playfield
 * Argument pPl: pointer to the playfield to be destructed
 * Return value: void
 */
void tetris_playfield_destruct(tetris_playfield_t *pPl);


/*******************************
 * playfield related functions *
 *******************************/

/* Function:     tetris_playfield_reset
 * Description:  resets playfield to begin a new game
 * Argument pPl: playfield to perform action on
 * Return value: void
 */
void tetris_playfield_reset(tetris_playfield_t *pPl);


/* Function:            tetris_playfield_insertPiece
 * Description:         inserts a new piece
 * Argument pPl:        playfield to perform action on
 * Argument pPiece:     piece to be inserted
 * Argument ppOldPiece: [out] indirect pointer to former piece for deallocation
 * Return value:        void
 */
void tetris_playfield_insertPiece(tetris_playfield_t *pPl,
                                  tetris_piece_t *pPiece,
                                  tetris_piece_t** ppOldPiece);


/* Function:         tetris_playfield_collision
 * Description:      detects if piece collides with s.th. at a given position
 * Argument pPl:     playfield to perform action on
 * Argument nColumn: column where the piece should be moved
 * Argument nRow:    row where the piece should be moved
 * Return value:     1 for collision, 0 otherwise
 */
uint8_t tetris_playfield_collision(tetris_playfield_t *pPl,
                                   int8_t nColumn,
                                   int8_t nRow);


/* Function:     tetris_playfield_advancePiece
 * Description:  lowers piece by one row or finally docks it
 * Argument pPl: playfield to perform action on
 * Return value: void
 */
void tetris_playfield_advancePiece(tetris_playfield_t *pPl);


/* Function:           tetris_playfield_movePiece
 * Description:        moves piece to the given direction
 * Argument pPl:       playfield to perform action on
 * Argument direction: direction (see tetris_playfield_direction_t)
 * Return value:       1 if piece could be moved, 0 otherwise
 */
uint8_t tetris_playfield_movePiece(tetris_playfield_t *pPl,
                                   tetris_playfield_direction_t direction);


/* Function:     tetris_playfield_rotatePiece
 * Description:  rotates piece to the given direction
 * Argument pPl: playfield to perform action on
 * Argument r:   type of rotation (see tetris_piece_rotation_t)
 * Return value: 1 if piece could be rotated, 0 otherwise
 */
uint8_t tetris_playfield_rotatePiece(tetris_playfield_t *pPl,
                                     tetris_piece_rotation_t rotation);


/* Function:     tetris_playfield_removeCompletedLines
 * Description:  removes completed lines (if any) and lowers the dump
 * Argument pPl: playfield to perform action on
 * Return value: void
 */
void tetris_playfield_removeCompleteLines(tetris_playfield_t *pPl);


/*****************
 * get functions *
 *****************/

/* Function:     tetris_playfield_getWidth
 * Description:  returns the width of the playfield
 * Argument pPl: the playfield we want information from
 * Return value: width of the playfield
 */
int8_t tetris_playfield_getWidth(tetris_playfield_t *pPl);


/* Function:     tetris_playfield_getHeight
 * Description:  returns the height of the playfield
 * Argument pPl: the playfield we want information from
 * Return value: height of the playfield
 */
int8_t tetris_playfield_getHeight(tetris_playfield_t *pPl);


/* Function:     tetris_playfield_getPiece
 * Description:  returns the currently falling piece
 * Argument pPl: the playfield we want information from
 * Return value: pointer to the currently falling piece
 */
tetris_piece_t *tetris_playfield_getPiece(tetris_playfield_t *pPl);


/* Function:     tetris_playfield_getColumn
 * Description:  returns the column of the currently falling piece
 * Argument pPl: the playfield we want information from
 * Return value: column of the currently falling piece
 */
int8_t tetris_playfield_getColumn(tetris_playfield_t *pPl);


/* Function:     tetris_playfield_getRow
 * Description:  returns the row of the currently falling piece
 * Argument pPl: the playfield we want information from
 * Return value: row of the currently falling piece
 */
int8_t tetris_playfield_getRow(tetris_playfield_t *pPl);


/* Function:     tetris_playfield_getRowMask
 * Description:  returns the row mask relative to nRow
 * Argument pPl: the playfield we want information from
 * Return value: the first 4 bits indicate which lines (relative to nRow)
 *               have been removed if we are in status TETRIS_PFS_READY
 */
uint8_t tetris_playfield_getRowMask(tetris_playfield_t *pPl);


/* Function:     tetris_playfield_getStatus
 * Description:  returns the status of the playfield
 * Argument pPl: the playfield we want information from
 * Return value: status of the playfield (see tetris_playfield_status_t)
 */
tetris_playfield_status_t tetris_playfield_getStatus(tetris_playfield_t *pPl);


/* Function:      tetris_playfield_getDumpRow
 * Description:   returns the given row of the dump (as bitmap)
 * Argument pPl:  the playfield we want information from
 * Argument nRow: the number of the row (0 <= nRow <= 124)
 * Return value:  bitmap of the requested row (LSB is leftmost column)
 */
uint16_t tetris_playfield_getDumpRow(tetris_playfield_t *pPl,
                                     int8_t nRow);


/* Function:         tetris_playfield_predictDeepestRow
 * Description:      returns the deepest possible row of a given piece
 * Argument pPl:     the playfield on which we want to test a piece
 * Argument pPiece:  the piece which should be tested
 * Argument nColumn: the column where the piece should be dropped
 * Return value:     the row of the piece (playfield compliant coordinates)
 */
int8_t tetris_playfield_predictDeepestRow(tetris_playfield_t *pPl,
                                          tetris_piece_t *pPiece,
                                          int8_t nColumn);


/* Function:         tetris_playfield_predictCompleteLines
 * Description:      predicts the number of complete lines for a piece at
 *                   a given column
 * Argument pPl:     the playfield on which we want to test a piece
 * Argument pPiece:  the piece which should be tested
 * Argument nColumn: the column where the piece should be dropped
 * Return value:     amount of complete lines
 */
int8_t tetris_playfield_predictCompleteLines(tetris_playfield_t *pPl,
                                             tetris_piece_t *pPiece,
                                             int8_t nColumn);


/* Function:         tetris_playfield_predictDumpRow
 * Description:      predicts the appearance of a playfield row for a piece
 *                   at a given column
 * Argument pPl:     the playfield on which we want to test a piece
 * Argument pPiece:  the piece which should be tested
 * Argument nColumn: the column where the piece should be dropped
 * Argument nRow:    the row of interest
 * Return value:     amount of complete lines
 */
uint16_t tetris_playfield_predictDumpRow(tetris_playfield_t *pPl,
                                         tetris_piece_t *pPiece,
                                         int8_t nColumn,
                                         int8_t nRow);

#endif /*TETRIS_PLAYFIELD_H_*/
