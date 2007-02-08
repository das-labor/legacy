#ifndef TETRIS_PIECE_H_
#define TETRIS_PIECE_H_

#include <inttypes.h>


/*********
 * types *
 *********/

enum tetris_piece_type_t {
	TETRIS_PC_LINE,
	TETRIS_PC_T,
	TETRIS_PC_SQUARE,
	TETRIS_PC_L,
	TETRIS_PC_LBACK,
	TETRIS_PC_S,
	TETRIS_PC_Z
};


enum tetris_piece_angle_t {
	TETRIS_PC_ANGLE_0,
	TETRIS_PC_ANGLE_90,
	TETRIS_PC_ANGLE_180,
	TETRIS_PC_ANGLE_270
};


enum tetris_piece_rotation_t {
	TETRIS_PC_ROT_CLOCKWISE,
	TETRIS_PC_ROT_COUNTERCLOCKWISE
};


typedef struct {
	enum tetris_piece_type_t type;  // holds the bitmaps of the 4 angels of the piece
	enum tetris_piece_angle_t angle; // specifies one of 4 angels (in steps of 90 degrees)
} tetris_piece_t;


/*****************************
 *  construction/destruction *
 *****************************/

tetris_piece_t* tetris_piece_construct (enum tetris_piece_type_t t, enum tetris_piece_angle_t a);
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
 * Argument p_pc:   The piece to rotate
 * Argument r:      Type of rotation (see tetris_piece_rotation_t above)
 * Return value:    void
 */
void tetris_piece_rotate(tetris_piece_t* p_pc, enum tetris_piece_rotation_t r);


/* Function:        tetris_piece_lastSolidMatterRow
 * Description:     Determines the last row which contains solid matter (counting from 0)
 * Argument p_pc:   The piece to rotate
 * Return value:    The last row containing solid matter
 */
uint8_t tetris_piece_lastSolidMatterRow(tetris_piece_t* p_pc);

#endif /*TETRIS_PIECE_H_*/
