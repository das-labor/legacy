#ifndef TETRIS_PIECE_H_
#define TETRIS_PIECE_H_

#include <inttypes.h>


/*********
 * types *
 *********/

typedef enum tetris_piece_shape_t
{
	TETRIS_PC_LINE,
	TETRIS_PC_T,
	TETRIS_PC_SQUARE,
	TETRIS_PC_L,
	TETRIS_PC_LBACK,
	TETRIS_PC_S,
	TETRIS_PC_Z
}
tetris_piece_shape_t;


typedef enum tetris_piece_angle_t
{
	TETRIS_PC_ANGLE_0,
	TETRIS_PC_ANGLE_90,
	TETRIS_PC_ANGLE_180,
	TETRIS_PC_ANGLE_270
}
tetris_piece_angle_t;


typedef enum tetris_piece_rotation_t
{
	TETRIS_PC_ROT_CW, // clockwise rotation
	TETRIS_PC_ROT_CCW // counter clockwise rotation
}
tetris_piece_rotation_t;


typedef struct tetris_piece_t
{
	tetris_piece_shape_t shape; // specifies the shape of the piece
	tetris_piece_angle_t angle; // specifies one of 4 angels
}
tetris_piece_t;


/*****************************
 *  construction/destruction *
 *****************************/

/* Function:     tetris_piece_construct
 * Description:  constructs a piece with the given attributes
 * Argument s:   shape of the piece (see tetris_piece_shape_t)
 * Argument a:   its angle (see tetris_piece_angel_t)
 * Return value: pointer to a newly created piece
 */
tetris_piece_t *tetris_piece_construct(tetris_piece_shape_t s,
                                       tetris_piece_angle_t a);


/* Function:     tetris_piece_destruct
 * Description:  destructs a piece
 * Argument pPc: pointer to the piece to be destructed
 * Return value: void
 */
 void tetris_piece_destruct(tetris_piece_t *pPc);


/****************************
 *  piece related functions *
 ****************************/

/* Function:     tetris_piece_getBitmap
 * Description:  returns bitfield representation of the piece 
 * Argument pPc: piece from which the bitfield shuld be retrieved
 * Return value: bitfield representation of the piece
 *               - nth nibble is nth row of the piece (from upper left)
 *               - the LSB of a nibble represents the left side of a row
 */
uint16_t tetris_piece_getBitmap(tetris_piece_t *pPc);


/* Function:     tetris_piece_rotate
 * Description:  rotates a piece
 * Argument pPc: piece to rotate
 * Argument r:   type of rotation (see tetris_piece_rotation_t)
 * Return value: void
 */
void tetris_piece_rotate(tetris_piece_t *pPc,
                         tetris_piece_rotation_t r);


#endif /*TETRIS_PIECE_H_*/

