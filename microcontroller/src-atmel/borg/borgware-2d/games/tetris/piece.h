#ifndef TETRIS_PIECE_H_
#define TETRIS_PIECE_H_

#include <stdint.h>

/**
 * \defgroup TetrisPieceTypes Piece: Data types
 */
/*@{*/

/*********
 * types *
 *********/

/** shape attributes for a piece */
typedef enum tetris_piece_shape_t
{
	TETRIS_PC_LINE,   /**< the I shaped brick */
	TETRIS_PC_T,      /**< the T shaped brick */
	TETRIS_PC_SQUARE, /**< the square shaped brick */
	TETRIS_PC_L,      /**< the L shaped brick */
	TETRIS_PC_LBACK,  /**< the reverse L shaped brick */
	TETRIS_PC_S,      /**< the S shaped brick */
	TETRIS_PC_Z       /**< the Z shaped brick */
}
tetris_piece_shape_t;


/** possible angles for a brick */
typedef enum tetris_piece_angle_t
{
	TETRIS_PC_ANGLE_0,   /**< standard angle */
	TETRIS_PC_ANGLE_90,  /**< rotated by 90 degrees */
	TETRIS_PC_ANGLE_180, /**< rotated by 180 degrees */
	TETRIS_PC_ANGLE_270  /**< rotated by 270 degrees */
}
tetris_piece_angle_t;


/** rotation attributes */
typedef enum tetris_piece_rotation_t
{
	TETRIS_PC_ROT_CW, /**< clockwise rotation */
	TETRIS_PC_ROT_CCW /**< counter clockwise rotation */
}
tetris_piece_rotation_t;

/**
 * describes the attributes of a piece
 * @see tetris_piece_shape_t
 * @see tetris_piece_angle_t
 */
typedef struct tetris_piece_t
{
	tetris_piece_shape_t shape; /**< specifies the shape of the piece */
	tetris_piece_angle_t angle; /**< specifies one of 4 angels */
}
tetris_piece_t;

/*@}*/


/**
 * \defgroup TetrisPieceRelated Piece: Interface functions
 */
/*@{*/

/*****************************
 *  construction/destruction *
 *****************************/

/**
 * constructs a piece with the given attributes
 * @param s shape of the piece (see tetris_piece_shape_t)
 * @param a its angle (see tetris_piece_angel_t)
 * @return pointer to a newly created piece
 */
tetris_piece_t *tetris_piece_construct(tetris_piece_shape_t s,
                                       tetris_piece_angle_t a);


/**
 * destructs a piece
 * @param pPc pointer to the piece to be destructed
 */
 void tetris_piece_destruct(tetris_piece_t *pPc);


/***************************
 * piece related functions *
 ***************************/

/**
 * returns bitfield representation of the piece
 * @param pPc piece from which the bitfield shuld be retrieved
 * @return bitfield representation of the piece
 */
uint16_t tetris_piece_getBitmap(tetris_piece_t *pPc);


/**
 * rotates a piece
 * @param pPc piece to rotate
 * @param nRotation type of rotation (see tetris_piece_rotation_t)
 */
void tetris_piece_rotate(tetris_piece_t *pPc,
                         tetris_piece_rotation_t nRotation);


/**
 * changes the shape of a piece
 * @param pPc piece to change
 * @param shape the shape of interest
 */
inline static void tetris_piece_setShape(tetris_piece_t *pPc,
                                         tetris_piece_shape_t shape)
{
	assert(pPc != NULL);
	assert((shape >= 0) && (shape <= TETRIS_PC_Z));

	pPc->shape = shape;
}


/**
 * changes the angle of a piece
 * @param pPc piece to change
 * @param angle the angle of interest
 */
inline static void tetris_piece_setAngle(tetris_piece_t *pPc,
                                         tetris_piece_angle_t angle)
{
	assert(pPc != NULL);
	assert((angle >= TETRIS_PC_ANGLE_0) && (angle <= TETRIS_PC_ANGLE_270));

	pPc->angle = angle;
}


/**
 * returns the number of different angles
 * @param pPc piece whose angle count we want to know
 * @return number of different angles
 */
int8_t tetris_piece_getAngleCount(tetris_piece_t *pPc);

/*@}*/

#endif /*TETRIS_PIECE_H_*/
