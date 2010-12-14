#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "../../compat/pgmspace.h"
#include "piece.h"


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
	assert((pPc->angle < 4) && (pPc->shape < 7));

	// Lookup table:
	// A value in an array represents a piece in a specific angle (rotating
	// clockwise from index 0).
	static uint16_t const piece[][4] PROGMEM =
		{{0x0F00, 0x2222, 0x0F00, 0x2222},  // LINE
		 {0x4E00, 0x4640, 0x0E40, 0x4C40},  // T
		 {0x0660, 0x0660, 0x0660, 0x0660},  // SQUARE
		 {0x2E00, 0x88C0, 0x0E80, 0xC440},  // L
		 {0x8E00, 0x6440, 0x0E20, 0x44C0},  // LBACK
		 {0x6C00, 0x4620, 0x6C00, 0x4620},  // S
		 {0xC600, 0x4C80, 0xC600, 0x4C80}}; // Z

	return pgm_read_word(&piece[pPc->shape][pPc->angle]);
}


void tetris_piece_rotate(tetris_piece_t *pPc,
                         tetris_piece_rotation_t nRotation)
{
	assert(pPc != NULL);
	assert(nRotation < 2);

	// we just rotate through the available angles in the given direction and
	// wrap around (via modulo) where appropriate
	pPc->angle = (pPc->angle + ((nRotation == TETRIS_PC_ROT_CW) ? 1 : 3)) % 4;
}


int8_t tetris_piece_getAngleCount(tetris_piece_t *pPc)
{
	assert(pPc != NULL);

	static int8_t const angleCounts[] PROGMEM = {2, 4, 1, 4, 4, 2, 2};

	return pgm_read_byte(&angleCounts[pPc->shape]);
}
