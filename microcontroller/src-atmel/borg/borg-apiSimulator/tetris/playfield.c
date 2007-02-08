#include <stdlib.h>
#include <string.h>
#include "playfield.h"

/*****************************
 *  construction/destruction *
 *****************************/
tetris_playfield_t* tetris_playfield_construct() {
	tetris_playfield_t* p_playfield = (tetris_playfield_t*) malloc(sizeof(tetris_playfield_t));
	tetris_playfield_reset(p_playfield);
	return p_playfield;
}

void tetris_playfield_destruct(tetris_playfield_t* p_pl) {
	if (p_pl != 0)
		free(p_pl);
}


/********************************
 *  playfield related functions *
 ********************************/

/* Function:           tetris_playfield_insertPiece
 * Description:        Inserts a new piece
 * Argument p_pl:      The playfield to perform action on
 * Argument p_piece:   The type of piece
 * Return value:       0 corresponds to false, anything other to true
 */
uint8_t tetris_playfield_insertPiece(tetris_playfield_t* p_pl, tetris_piece_t* p_piece) {
	/* a piece can only be inserted if the playfield is in state TETRIS_PFS_DOCKED */
	if (p_pl->status == TETRIS_PFS_DOCKED) {
		/* replace old piece */
		p_pl->p_piece = p_piece;
		
		/* set start position (in the middle of the top line) */
		p_pl->x = (TETRIS_PF_WIDTH - 2) / 2;
		int8_t row = tetris_piece_lastSolidMatterRow(p_piece);
		if (row > -1) {
			p_pl->y = -row;
		} else {
			p_pl->y = 0;
		}
		
		/* OK */
		return 1;
	} else {
		/* wrong state */
		return 0;
	}
}


/* Function:           tetris_playfield_rotatePiece
 * Description:        rotates falling piece to the given direction
 * Argument p_pl:      The playfield to perform action on
 * Argument r:         The direction of the rotation (see also tetris_piece_rotation_t in piece.h)
 * Return value:       0 corresponds to false, anything other to true
 */
uint8_t tetris_playfield_rotatePiece(tetris_playfield_t* p_pl, enum tetris_piece_rotation_t r) {
	return 0;
}


/* Function:           tetris_playfield_advancePiece
 * Description:        Lowers piece by one row or finally docks it
 * Argument p_pl:      The playfield to perform action on
 * Return value:       Returns a value of enum tetris_playfield_status_t (see above)
 */
enum tetris_playfield_status_t tetris_playfield_advancePiece(tetris_playfield_t* p_pl) {
	return TETRIS_PFS_DOCKED;
}


/* Function:           tetris_playfield_isAllowedDirection
 * Description:        Informs if piece can be moved to given direction
 * Argument p_pl:      The playfield to perform action on
 * Argument direction: Direction (see tetris_playfield_direction_t)
 * Return value:       0 corresponds to false, anything other to true
 */
uint8_t tetris_playfield_isAllowedDirection(tetris_playfield_t* p_pl, enum tetris_playfield_direction_t direction) {
	return 0;
}


/* Function:           tetris_playfield_movePiece
 * Description:        Moves piece to the given direction
 * Argument p_pl:      The playfield to perform action on
 * Argument direction: Direction (see tetris_playfield_direction_t)
 * Return value:       void
 */
void tetris_playfield_movePiece(tetris_playfield_t* p_pl, enum tetris_playfield_direction_t direction) {
}


/* Function:           tetris_playfield_dropPiece
 * Description:        Drops piece immediately
 * Argument p_pl:      The playfield to perform action on
 * Return value:       Returns a value of enum tetris_playfield_status_t (see above)
 */
enum tetris_playfield_status_t tetris_playfield_dropPiece(tetris_playfield_t* p_pl) {
	return TETRIS_PFS_DOCKED;
}


/* Function:           tetris_playfield_removeCompletedLines
 * Description:        Removes completed lines (if any) and lowers the dump correlatively
 * Argument p_pl:      The playfield to perform action on
 * Return value:       Returns number of removed lines (0-4)
 */
uint8_t tetris_playfield_removeCompleteLines(tetris_playfield_t* p_pl) {
	return 0;
}


/* Function:           tetris_playfield_reset
 * Description:        Resets playfield to begin a new game;
 * Argument p_pl:      The playfield to perform action on
 * Return value:       void
 */
void tetris_playfield_reset(tetris_playfield_t* p_pl) {
	if (p_pl != NULL) {
		memset(p_pl, 0, sizeof(tetris_playfield_t));
	}
}
