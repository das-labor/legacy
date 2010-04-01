#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include "piece.h"

#ifdef __AVR__
	#include <avr/pgmspace.h>
#else
	#define PROGMEM
#endif


/*****************************
 *  construction/destruction *
 *****************************/

tetris_piece_t *tetris_piece_construct(tetris_piece_shape_t s,
                                       tetris_piece_angle_t a)
{
	tetris_piece_t *p_piece = (tetris_piece_t*) malloc (sizeof(tetris_piece_t));
	assert(p_piece != NULL);

	p_piece->shape = s;
	p_piece->angle = a;

	return p_piece;
}

void tetris_piece_destruct(tetris_piece_t *pPc)
{
	assert(pPc != NULL);
	free(pPc);
}


/****************************
 *  piece related functions *
 ****************************/

uint16_t tetris_piece_getBitmap(tetris_piece_t *pPc)
{
	assert(pPc != NULL);

	// Lookup table:
	// A value in an array represents a piece in a specific angle (rotating
	// clockwise from index 0).
	static const uint16_t piece[][4] PROGMEM =
		{{0x0F00, 0x2222, 0x0F00, 0x2222},  // LINE
		 {0x4E00, 0x4640, 0x0E40, 0x4C40},  // T
		 {0x0660, 0x0660, 0x0660, 0x0660},  // SQUARE
		 {0x2E00, 0x88C0, 0x0E80, 0xC440},  // L
		 {0x8E00, 0x6440, 0x0E20, 0x44C0},  // LBACK
		 {0x6C00, 0x4620, 0x6C00, 0x4620},  // S
		 {0xC600, 0x4C80, 0xC600, 0x4C80}}; // Z

	#ifdef __AVR__
		return pgm_read_word(&piece[pPc->shape][pPc->angle]);
	#else
		return piece[pPc->shape][pPc->angle];
	#endif
}


void tetris_piece_rotate(tetris_piece_t *pPc,
                         tetris_piece_rotation_t r)
{
	assert(pPc != NULL);

	// we just rotate through the available angles in the given direction and
	// make wrap arounds where appropriate
	switch (r)
	{
	case TETRIS_PC_ROT_CW:
		if (pPc->angle == TETRIS_PC_ANGLE_270)
		{
			pPc->angle = TETRIS_PC_ANGLE_0;
		}
		else
		{
			++pPc->angle;
		}
		break;
	case TETRIS_PC_ROT_CCW:
		if (pPc->angle == TETRIS_PC_ANGLE_0)
		{
			pPc->angle = TETRIS_PC_ANGLE_270;
		}
		else
		{
			--pPc->angle;
		}
		break;
	}
}


void tetris_piece_setShape(tetris_piece_t *pPc,
                           tetris_piece_shape_t shape)
{
	assert(pPc != NULL);
	assert((shape >= 0) && (shape <= TETRIS_PC_Z));

	pPc->shape = shape;
}


void tetris_piece_setAngle(tetris_piece_t *pPc,
                           tetris_piece_angle_t angle)
{
	assert(pPc != NULL);
	assert((angle >= TETRIS_PC_ANGLE_0) && (angle <= TETRIS_PC_ANGLE_270));

	pPc->angle = angle;
}


int8_t tetris_piece_getAngleCount(tetris_piece_t *pPc)
{
	assert(pPc != NULL);

	static const int8_t angleCounts[] PROGMEM = {2, 4, 1, 4, 4, 2, 2};

#ifdef __AVR__
	return pgm_read_word(&angleCounts[pPc->shape]);
#else
	return angleCounts[pPc->shape];
#endif
}
