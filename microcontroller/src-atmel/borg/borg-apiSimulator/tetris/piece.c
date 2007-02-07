#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "piece.h"

/*****************************
 *  construction/destruction *
 *****************************/

tetris_piece_t* tetris_piece_construct (enum tetris_piece_type_t t) {
    static tetris_piece_t pc_line = {
                                        {0,0,1,0},
                                        {0,0,1,0},
                                        {0,0,1,0},
                                        {0,0,1,0}
                                    };

    static tetris_piece_t pc_t = {
                                     {0,0,0,0},
                                     {0,0,1,0},
                                     {0,1,1,1},
                                     {0,0,0,0}
                                 };

    static tetris_piece_t pc_square = {
                                          {0,0,0,0},
                                          {0,1,1,0},
                                          {0,1,1,0},
                                          {0,0,0,0}
                                      };

    static tetris_piece_t pc_l = {
                                     {0,0,1,0},
                                     {0,0,1,0},
                                     {0,0,1,1},
                                     {0,0,0,0}
                                 };

    static tetris_piece_t pc_lback = {
                                         {0,0,1,0},
                                         {0,0,1,0},
                                         {0,1,1,0},
                                         {0,0,0,0}
                                     };

    static tetris_piece_t pc_s = {
                                     {0,0,0,0},
                                     {0,0,1,1},
                                     {0,1,1,0},
                                     {0,0,0,0}
                                 };

    static tetris_piece_t pc_z = {
                                     {0,0,0,0},
                                     {0,1,1,0},
                                     {0,0,1,1},
                                     {0,0,0,0}
                                 };

    tetris_piece_t* p_piece = (tetris_piece_t*) malloc (sizeof(tetris_piece_t));

    switch (t) {
    case TETRIS_PT_LINE:
        memcpy(p_piece, &pc_line, sizeof(tetris_piece_t));
        break;
    case TETRIS_PT_T:
        memcpy(p_piece, &pc_t, sizeof(tetris_piece_t));
        break;
    case TETRIS_PT_SQUARE:
        memcpy(p_piece, &pc_square, sizeof(tetris_piece_t));
        break;
    case TETRIS_PT_L:
        memcpy(p_piece, &pc_l, sizeof(tetris_piece_t));
        break;
    case TETRIS_PT_LBACK:
        memcpy(p_piece, &pc_lback, sizeof(tetris_piece_t));
        break;
    case TETRIS_PT_S:
        memcpy(p_piece, &pc_s, sizeof(tetris_piece_t));
        break;
    case TETRIS_PT_Z:
        memcpy(p_piece, &pc_z, sizeof(tetris_piece_t));
        break;
    case TETRIS_PT_DUMMY:
    default:
        memset(p_piece, 0, sizeof(tetris_piece_t));
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
	if ((x < 3) && (y < 3)) {
		return *p_pc[x][y];
	}
	
	return 0;
}


/* Function:        tetris_piece_rotate
 * Description:     Rotates a piece
 * Argument p_src:  The piece to rotate
 * Argument p_dst:  The piece where the rotation will be stored 
 * Argument r:      Type of rotation (see tetris_piece_rotation_t above)
 * Return value:    void
 */
void tetris_piece_rotate(tetris_piece_t* p_pc_src, tetris_piece_t* p_pc_dst, enum tetris_piece_rotation_t r) {
    uint8_t y = 0;
    uint8_t x = 0;

    if ((p_pc_src != NULL) && (p_pc_dst != NULL)) {
        for (y = 0; y < 4; ++y) {
            for (x = 0; x < 4; x++) {
                if (r == TETRIS_PR_CLOCKWISE)
                    *p_pc_dst[x][y] = *p_pc_src[y][3-x];
                else
                    *p_pc_dst[x][y] = *p_pc_src[3-y][x];
            }
        }
    }
}


/* Function:        tetris_piece_lastSolidMatterRow
 * Description:     Determines the last row which contains solid matter (counting from 0)
 * Argument p_pc:   The piece to rotate
 * Return value:    The last row containing solid matter or -1 if no matter was found
 */
int8_t tetris_piece_lastSolidMatterRow(tetris_piece_t* p_pc) {
	uint8_t row_sum = 0;
	int8_t offset;
	for (offset = 3; (row_sum == 0) && (offset >= 0); --offset) {
		row_sum = *p_pc[0][offset] + *p_pc[1][offset] + *p_pc[2][offset] + *p_pc[3][offset];
	}

	return offset;
}
