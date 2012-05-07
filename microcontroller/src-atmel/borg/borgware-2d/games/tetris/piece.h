/**
 * \addtogroup tetris
 * @{
 */

/**
 * @file piece.h
 * @brief Public interface definitions of the piece module.
 * @author Christian Kroll
 */


#ifndef TETRIS_PIECE_H_
#define TETRIS_PIECE_H_

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>


/*********
 * types *
 *********/

/** shape attributes for a piece */
enum tetris_piece_shape_e
{
	TETRIS_PC_LINE,   /**< the I shaped brick */
	TETRIS_PC_T,      /**< the T shaped brick */
	TETRIS_PC_SQUARE, /**< the square shaped brick */
	TETRIS_PC_L,      /**< the L shaped brick */
	TETRIS_PC_LBACK,  /**< the reverse L shaped brick */
	TETRIS_PC_S,      /**< the S shaped brick */
	TETRIS_PC_Z       /**< the Z shaped brick */
};
#ifdef NDEBUG
	typedef uint8_t tetris_piece_shape_t;
#else
	typedef enum tetris_piece_shape_e tetris_piece_shape_t;
#endif


/** possible angles for a brick */
enum tetris_piece_angle_e
{
	TETRIS_PC_ANGLE_0,   /**< standard angle */
	TETRIS_PC_ANGLE_90,  /**< rotated by 90 degrees */
	TETRIS_PC_ANGLE_180, /**< rotated by 180 degrees */
	TETRIS_PC_ANGLE_270  /**< rotated by 270 degrees */
};
#ifdef NDEBUG
	typedef uint8_t tetris_piece_angle_t;
#else
	typedef enum tetris_piece_angle_e tetris_piece_angle_t;
#endif

/** rotation attributes */
enum tetris_piece_rotation_e
{
	TETRIS_PC_ROT_CW  = 1, /**< clockwise rotation */
	TETRIS_PC_ROT_CCW = 3  /**< counter clockwise rotation */
};
#ifdef NDEBUG
	typedef uint8_t tetris_piece_rotation_t;
#else
	typedef enum tetris_piece_rotation_e tetris_piece_rotation_t;
#endif

/**
 * describes the attributes of a piece
 * @see tetris_piece_shape_t
 * @see tetris_piece_angle_t
 */
typedef struct tetris_piece_s
{
	tetris_piece_shape_t shape; /**< specifies the shape of the piece */
	tetris_piece_angle_t angle; /**< specifies one of 4 angels */
}
tetris_piece_t;


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
inline static void tetris_piece_destruct(tetris_piece_t *pPc)
{
	assert(pPc != NULL);
	free(pPc);
}


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
inline static void tetris_piece_rotate(tetris_piece_t *pPc,
                                       tetris_piece_rotation_t nRotation)
{
	assert(pPc != NULL);
	assert(nRotation == TETRIS_PC_ROT_CW || nRotation == TETRIS_PC_ROT_CCW);

	// we just rotate through the available angles in the given direction and
	// wrap around (via modulo) where appropriate
	pPc->angle = (pPc->angle + nRotation) % 4;
}


/**
 * changes the shape of a piece
 * @param pPc piece to change
 * @param shape the shape of interest
 */
inline static void tetris_piece_setShape(tetris_piece_t *pPc,
                                         tetris_piece_shape_t shape)
{
	assert(pPc != NULL);
	assert(shape <= TETRIS_PC_Z);

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
	assert(angle <= TETRIS_PC_ANGLE_270);

	pPc->angle = angle;
}


/**
 * returns the number of different angles
 * @param pPc piece whose angle count we want to know
 * @return number of different angles
 */
uint8_t tetris_piece_getAngleCount(tetris_piece_t *pPc);


/**
 * returns the index of the first filled row of a piece
 * @param nBitmap the bitmap of the piece of interest
 * @return index of the first filled row
 */
inline static int8_t tetris_piece_getTopRow(uint16_t const nBitmap)
{
	if (!(nBitmap & 0x0FFF))
	{
		return 3; // first three rows can be skipped
	}
	else if (!(nBitmap & 0x00FF))
	{
		return 2; // first two rows can be skipped
	}
	else if (!(nBitmap & 0x000F))
	{
		return 1; // first row can be skipped
	}
	return 0;     // no row can be skipped
}


/**
 * returns the offset to the last filled row of a piece
 * @param nBitmap the bitmap of the piece of interest
 * @return offset to the last filled row
 */
inline static int8_t tetris_piece_getBottomOffset(uint16_t const nBitmap)
{
	if (nBitmap > 0x0FFF)
	{
		return 3; // piece spans over 4 rows
	}
	else if (nBitmap > 0x00FF)
	{
		return 2; // last row of the piece is empty
	}
	else if (nBitmap > 0x000F)
	{
		return 1; // last two rows of the piece are empty
	}
	return 0;     // last three rows of the piece are empty
}

#endif /*TETRIS_PIECE_H_*/

/*@}*/
