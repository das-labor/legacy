#ifndef TETRIS_PIECE_H_
#define TETRIS_PIECE_H_

#include <inttypes.h>


/*********
 * types *
 *********/

enum tetris_piece_type_t {
	TETRIS_PT_LINE,
	TETRIS_PT_T,
	TETRIS_PT_SQUARE,
	TETRIS_PT_L,
	TETRIS_PT_LBACK,
	TETRIS_PT_S,
	TETRIS_PT_Z,
	TETRIS_PT_DUMMY    /* empty piece which acts as a buffer for rotations */
};


enum tetris_piece_rotation_t {
	TETRIS_PR_CLOCKWISE,
	TETRIS_PR_COUNTERCLOCKWISE
};

typedef uint8_t tetris_piece_t[4][4];


/*****************************
 *  construction/destruction *
 *****************************/

tetris_piece_t* tetris_piece_construct (enum tetris_piece_type_t t);
void tetris_piece_destruct(tetris_piece_t* p);


/********************************
 *  piece related functions *
 ********************************/

/* Function:        tetris_piece_solidMatter
 * Description:     Determines if the piece consists of solid matter at the given position 
 * Argument p_pc:   The piece to examine
 * Argument x:      The x coordinate
 * Argument y:      The y coordinate
 * Return value:    0 corresponds to no matter, everything > 0 to solid matter
 */
uint8_t tetris_piece_solidMatter(tetris_piece_t* p_pc, uint8_t x, uint8_t y);


/* Function:        tetris_piece_rotate
 * Description:     Rotates a piece
 * Argument p_src:  The piece to rotate
 * Argument p_dst:  The piece where the rotation will be stored 
 * Argument r:      Type of rotation (see tetris_piece_rotation_t above)
 * Return value:    void
 */
void tetris_piece_rotate(tetris_piece_t* p_src, tetris_piece_t* p_dst, enum tetris_piece_rotation_t r);


/* Function:        tetris_piece_lastSolidMatterRow
 * Description:     Determines the last row which contains solid matter (counting from 0)
 * Argument p_pc:   The piece to rotate
 * Return value:    The last row containing solid matter or -1 if no matter was found
 */
int8_t tetris_piece_lastSolidMatterRow(tetris_piece_t* p_pc);

#endif /*TETRIS_PIECE_H_*/
