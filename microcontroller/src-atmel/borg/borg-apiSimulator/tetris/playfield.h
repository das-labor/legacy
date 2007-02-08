#ifndef TETRIS_PLAYFIELD_H_
#define TETRIS_PLAYFIELD_H_

#include <inttypes.h>
#include "../config.h"
#include "piece.h"

/***********
 * defines *
 ***********/

#define TETRIS_PF_WIDTH 10

/*********
 * types *
 *********/
 
/* directions to which a piece can be moved */
enum tetris_playfield_direction_t {
	TETRIS_PFD_LEFT,
	TETRIS_PFD_RIGHT,
	TETRIS_PFD_DOWN
};


/* status of the playfield */
enum tetris_playfield_status_t {
	TETRIS_PFS_HOVERING, /* piece is still hovering */
	TETRIS_PFS_DOCKED,   /* piece has been docked */
	TETRIS_PFS_GAMEOVER	 /* playfield is filled up */
};


/* tetris_playfield_t */
typedef struct {
	uint8_t playfield[TETRIS_PF_WIDTH][NUM_ROWS]; /* playfield itself */
	enum tetris_playfield_status_t status;        /* status */
	tetris_piece_t* p_piece;                      /* currently falling piece */
	int8_t x,y;                                   /* its position (0,0 is bottom left) */
	uint8_t nHeight;                              /* fill height */
} tetris_playfield_t;


/*****************************
 *  construction/destruction *
 *****************************/
tetris_playfield_t* tetris_playfield_construct();
void tetris_playfield_destruct(tetris_playfield_t* p_pl);


/********************************
 *  playfield related functions *
 ********************************/

/* Function:           tetris_playfield_insertPiece
 * Description:        Inserts a new piece
 * Argument p_pl:      The playfield to perform action on
 * Argument p_piece:   The piece to be inserted
 * Return value:       0 corresponds to false, anything other to true
 */
uint8_t tetris_playfield_insertPiece(tetris_playfield_t* p_pl, tetris_piece_t* p_piece);


/* Function:           tetris_playfield_rotatePiece
 * Description:        rotates falling piece to the given direction
 * Argument p_pl:      The playfield to perform action on
 * Argument r:         The direction of the rotation (see also tetris_piece_rotation_t above)
 * Return value:       0 corresponds to false, anything other to true
 */
uint8_t tetris_playfield_rotatePiece(tetris_playfield_t* p_pl, enum tetris_piece_rotation_t r);


/* Function:           tetris_playfield_advancePiece
 * Description:        Lowers piece by one row or finally docks it
 * Argument p_pl:      The playfield to perform action on
 * Return value:       Returns a value of enum tetris_playfield_status_t (see above)
 */
enum tetris_playfield_status_t tetris_playfield_advancePiece(tetris_playfield_t* p_pl);


/* Function:           tetris_playfield_isAllowedDirection
 * Description:        Informs if piece can be moved to given direction
 * Argument p_pl:      The playfield to perform action on
 * Argument direction: Direction (see tetris_playfield_direction_t)
 * Return value:       0 corresponds to false, anything other to true
 */
uint8_t tetris_playfield_isAllowedDirection(tetris_playfield_t* p_pl, enum tetris_playfield_direction_t direction);


/* Function:           tetris_playfield_movePiece
 * Description:        Moves piece to the given direction
 * Argument p_pl:      The playfield to perform action on
 * Argument direction: Direction (see tetris_playfield_direction_t)
 * Return value:       void
 */
void tetris_playfield_movePiece(tetris_playfield_t* p_pl, enum tetris_playfield_direction_t direction);


/* Function:           tetris_playfield_dropPiece
 * Description:        Drops piece immediately
 * Argument p_pl:      The playfield to perform action on
 * Return value:       Returns a value of enum tetris_playfield_status_t (see above)
 */
enum tetris_playfield_status_t tetris_playfield_dropPiece(tetris_playfield_t* p_pl);


/* Function:           tetris_playfield_removeCompleteLines
 * Description:        Removes completed lines (if any) and lowers the dump correlatively
 * Argument p_pl:      The playfield to perform action on
 * Return value:       Returns number of removed lines (0-4)
 */
uint8_t tetris_playfield_removeCompletedLines(tetris_playfield_t* p_pl);


/* Function:           tetris_playfield_reset
 * Description:        Resets playfield to begin a new game;
 * Argument p_pl:      The playfield to perform action on
 * Return value:       void
 */
void tetris_playfield_reset(tetris_playfield_t* p_pl);


#endif /*TETRIS_PLAYFIELD_H_*/
