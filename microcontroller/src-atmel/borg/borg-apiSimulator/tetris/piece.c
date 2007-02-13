#include <inttypes.h>
#include <stdlib.h>
#include "piece.h"

/*****************************
 *  construction/destruction *
 *****************************/

/* Function:        tetris_piece_construct
 * Description:     Constructs a piece with the given attributes
 * Argument s:      The shape of the piece (see defintion of tetris_piece_shape_t in piece.h)
 * Argument a:      Its angle (see defintion of tetris_piece_angel_t in piece.h)
 * Return value:    Pointer to a newly created piece
 */
tetris_piece_t* tetris_piece_construct (enum tetris_piece_shape_t s, enum tetris_piece_angle_t a) {
    tetris_piece_t* p_piece = (tetris_piece_t*) malloc (sizeof(tetris_piece_t));
    if (p_piece != NULL) {
    	p_piece->shape = s;
    	p_piece->angle = a;
    }

    return p_piece;
}

/* Function:        tetris_piece_destruct
 * Description:     Destructs a piece
 * Argument p:      The pointer of the piece to be destructed
 * Return value:    void
 */
void tetris_piece_destruct(tetris_piece_t* p_pc) {
    if (p_pc != NULL)
        free(p_pc);
}


/********************************
 *  piece related functions *
 ********************************/

/* Function:        tetris_piece_getBitfield
 * Description:     Returns the bitfield representation of the piece 
 * Argument p_pc:   The piece from which the bitfield shuld be retrieved
 * Return value:    Returns the bitfield representation of the piece
 */
uint16_t tetris_piece_getBitfield(tetris_piece_t* p_pc) {
	/* lookup table:
	 * a value in an array represents a piece in a specific angle (rotating clockwise from index 0)
	 * a nibble at position n in such a piece corresponds to a bitmap of its row number n
	 */
    static uint16_t piece[][4] = {{0x2222, 0x00F0, 0x2222, 0x00F0},  /* TETRIS_PC_LINE */
                                  {0x0262, 0x0270, 0x0230, 0x0072},  /* TETRIS_PC_T */
                                  {0x0066, 0x0066, 0x0066, 0x0066},  /* TETRIS_PC_SQUARE */
                                  {0x0443, 0x0074, 0x0622, 0x0130},  /* TETRIS_PC_LBACK */
                                  {0x0226, 0x0430, 0x0322, 0x0C60},  /* TETRIS_PC_L */
                                  {0x0462, 0x0360, 0x0462, 0x0360},  /* TETRIS_PC_Z */
                                  {0x0264, 0x0C60, 0x0264, 0x0C60}}; /* TETRIS_PC_S */

    /* x and y are mapped to the corresponding bit in the piece value and
     * a "1" is returned if the bit is set, a "0" otherwise 
     */ 
    return piece[p_pc->shape][p_pc->angle];
}


/* Function:       tetris_piece_rotate
 * Description:    Rotates a piece
 * Argument p_pc:  The piece to rotate
 * Argument r:     Type of rotation (see tetris_piece_rotation_t above)
 * Return value:   void
 */
void tetris_piece_rotate(tetris_piece_t* p_pc, enum tetris_piece_rotation_t r) {
	/* we just rotate through the available angles in the given direction and
	 * make wrap arounds where appropriate
	 */
	switch (r) {
	case TETRIS_PC_ROT_CLOCKWISE:
	    if (p_pc->angle == TETRIS_PC_ANGLE_270) {
	    	p_pc->angle = TETRIS_PC_ANGLE_0;
	    } else {
	    	++p_pc->angle;
	    }
		break;
	case TETRIS_PC_ROT_COUNTERCLOCKWISE:
	    if (p_pc->angle == TETRIS_PC_ANGLE_0) {
	    	p_pc->angle = TETRIS_PC_ANGLE_270;
	    } else {
	    	--p_pc->angle;
	    }
		break;
	}
}


/* Function:        tetris_piece_lastSolidMatterRow
 * Description:     Determines the last row which contains solid matter (counting from 0)
 * Argument p_pc:   The piece to rotate
 * Return value:    The last row containing solid matter
 */
uint8_t tetris_piece_lastSolidMatterRow(tetris_piece_t* p_pc) {
	/* either row no. 2 or no. 3 is the last one which contains matter 
	 * so if the nibble of row 3 isn't 0, it must be the last row
	 */	
	if ((0x000F & tetris_piece_getBitfield(p_pc)) != 0) {
		return 3;
	} else {
		return 2;
	}
}
