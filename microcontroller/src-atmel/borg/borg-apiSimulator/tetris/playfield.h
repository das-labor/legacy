#ifndef TETRIS_PLAYFIELD_H_
#define TETRIS_PLAYFIELD_H_

#include <inttypes.h>
#include "../config.h"
#include "piece.h"

/*********
 * types *
 *********/
 
/* directions to which a piece can bie moved */
enum tetris_playfield_direction_t
	{TETRIS_PFD_LEFT, TETRIS_PFD_RIGHT, TETRIS_PFD_DOWN};

/* status of the playfield
 * - has the piece been docked?
 * - or is it still hovering?
 * - is the playfield filled up (game over)?
 */
enum tetris_playfield_status_t
	{TETRIS_PFS_HOVER, TETRIS_PFS_DOCKED, TETRIS_PFS_GAMEOVER};

/* tetris_playfield_t contains:
 * - the playfield itself
 * - the falling piece
 * - its position
 * - fill height
 * - if the piece has already touched a ground since
 *   the last advance (so it can be finally docked)   
 */
typedef struct {
	char playfield[10][NUM_ROWS];
	tetris_piece_t* p_piece;
	uint8_t x,y;
	uint8_t nHeight;
	uint8_t nHitGround;
} tetris_playfield_t;


/*****************************
 *  construction/destruction *
 *****************************/
tetris_playfield_t* tetris_playfield_construct();
void tetris_playfield_destruct(tetris_playfield_t* p_pl);


/********************************
 *  playfield related functions *
 ********************************/

/* Function:         tetris_playfield_insertPiece
 * Description:      Inserts a new piece 
 * Argument p_pl:    The playfield to perform action on
 * Argument p_piece: The type of piece
 * Return value:     void
 */
void tetris_playfield_insertPiece(tetris_playfield_t* p_pl, tetris_piece_t* p_piece);

/* Function:         tetris_playfield_advancePiece
 * Description:      Lowers piece by one row or finally docks it
 * Argument p_pl:    The playfield to perform action on
 * Return value:     Returns a value of type tetris_playfield_status_t (see above)
 */
enum tetris_playfield_status_t tetris_playfield_advancePiece(tetris_playfield_t* p_pl);

/* Function:         tetris_playfield_isAllowedDirection
 * Description:      Informs if the piece can be moved to the given direction
 * Argument p_pl:    The playfield to perform action on
 * Return value:     0 corresponds to false, anything other to true
 */
uint8_t tetris_playfield_isAllowedDirection(tetris_playfield_t* p_pl, enum tetris_playfield_direction_t direction);
uint8_t tetris_playfield_movePiece(tetris_playfield_t* p_pl, enum tetris_playfield_direction_t direction);
uint8_t tetris_playfield_dropPiece(tetris_playfield_t* p_pl);

uint8_t tetris_playfield_removeLine(tetris_playfield_t* p_pl, uint8_t nLineNumber);


#endif /*TETRIS_PLAYFIELD_H_*/
