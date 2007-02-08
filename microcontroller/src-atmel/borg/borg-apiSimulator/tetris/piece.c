#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "piece.h"

/*****************************
 *  construction/destruction *
 *****************************/

tetris_piece_t* tetris_piece_construct (enum tetris_piece_type_t t, enum tetris_piece_angle_t a) {
    tetris_piece_t* p_piece = (tetris_piece_t*) malloc (sizeof(tetris_piece_t));
    if (p_piece != NULL) {
    	p_piece->type = t;
    	p_piece->angle = a;
    }

    return p_piece;
}

void tetris_piece_destruct(tetris_piece_t* p_pc) {
    if (p_pc != 0)
        free(p_pc);
}


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
uint8_t tetris_piece_solidMatter(tetris_piece_t* p_pc, uint8_t x, uint8_t y) {
    static uint16_t pc_line[]   = {0x2222, 0x00F0, 0x2222, 0x00F0};
    static uint16_t pc_t[]      = {0x0262, 0x0270, 0x0230, 0x0072};
    static uint16_t pc_square[] = {0x0066, 0x0066, 0x0066, 0x0066};
    static uint16_t pc_l[]      = {0x0443, 0x0074, 0x0622, 0x0130};
    static uint16_t pc_lback[]  = {0x0226, 0x0430, 0x0322, 0x0C60};
    static uint16_t pc_s[]      = {0x0462, 0x0360, 0x0462, 0x0360};
    static uint16_t pc_z[]      = {0x0264, 0x0C60, 0x0264, 0x0C60};

    uint16_t piece;
    switch (p_pc->type) {
    case TETRIS_PC_LINE:
        piece = pc_line[p_pc->angle];
        break;
    case TETRIS_PC_T:
        piece = pc_t[p_pc->angle];
        break;
    case TETRIS_PC_SQUARE:
        piece = pc_square[p_pc->angle];
        break;
    case TETRIS_PC_L:
        piece = pc_l[p_pc->angle];
        break;
    case TETRIS_PC_LBACK:
        piece = pc_lback[p_pc->angle];
        break;
    case TETRIS_PC_S:
        piece = pc_s[p_pc->angle];
        break;
    case TETRIS_PC_Z:
    default:
        piece = pc_z[p_pc->angle];
        break;
    }
     
    return ((((y << 2) | x) & piece) > 0) ? 1 : 0;
}


/* Function:       tetris_piece_rotate
 * Description:    Rotates a piece
 * Argument p_pc:  The piece to rotate
 * Argument r:     Type of rotation (see tetris_piece_rotation_t above)
 * Return value:   void
 */
void tetris_piece_rotate(tetris_piece_t* p_pc, enum tetris_piece_rotation_t r) {
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
	int i;
	int sum = 0;
	
	for (i = 0; i < 3; ++i) {
		sum += tetris_piece_solidMatter(p_pc, i, 3);
	}
	
	return (sum > 0) ? 3 : 2;
}
