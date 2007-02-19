#include <inttypes.h>
#include <stdlib.h>
#include "piece.h"

/*****************************
 *  construction/destruction *
 *****************************/

/* Function:        tetris_piece_construct
 * Description:     constructs a piece with the given attributes
 * Argument s:      shape of the piece (see tetris_piece_shape_t)
 * Argument a:      its angle (see tetris_piece_angel_t)
 * Return value:    pointer to a newly created piece
 */
tetris_piece_t* tetris_piece_construct(tetris_piece_shape_t s,
                                       tetris_piece_angle_t a)
{
    tetris_piece_t* p_piece =
    	(tetris_piece_t*) malloc (sizeof(tetris_piece_t));
    	
    if (p_piece != NULL)
    {
        p_piece->shape = s;
        p_piece->angle = a;
    }

    return p_piece;
}

/* Function:        tetris_piece_destruct
 * Description:     destructs a piece
 * Argument pPc:    pointer to the piece to be destructed
 * Return value:    void
 */
void tetris_piece_destruct(tetris_piece_t* pPc)
{
    if (pPc != NULL)
        free(pPc);
}


/********************************
 *  piece related functions *
 ********************************/

/* Function:        tetris_piece_getBitfield
 * Description:     returns bitfield representation of the piece 
 * Argument pPc:    piece from which the bitfield shuld be retrieved
 * Return value:    bitfield representation of the piece
 *                  - nth nibble is nth row of the piece (from upper left)
 *                  - the LSB of a nibble represents the left side of the row
 */
uint16_t tetris_piece_getBitfield(tetris_piece_t* pPc)
{
    /* Lookup table:
     * A value in an array represents a piece in a specific angle (rotating
     * clockwise from index 0).
     */
    static uint16_t piece[][4] = {{0x2222, 0x0F00, 0x2222, 0x0F00}, /* LINE */
                                  {0x0E40, 0x4640, 0x4E00, 0x4C40}, /* T */
                                  {0x6600, 0x6600, 0x6600, 0x6600}, /* SQUARE */
                                  {0x6220, 0x1700, 0x4460, 0x0E80}, /* LBACK */
                                  {0x6440, 0x0E20, 0x44C0, 0x8E00}, /* L */
                                  {0x4620, 0x6C00, 0x4620, 0x6C00}, /* Z */
                                  {0x2640, 0x6300, 0x2640, 0x6300}};/* S */

    return piece[pPc->shape][pPc->angle];
}


/* Function:       tetris_piece_rotate
 * Description:    rotates a piece
 * Argument pPc:   piece to rotate
 * Argument r:     type of rotation (see tetris_piece_rotation_t)
 * Return value:   void
 */
void tetris_piece_rotate(tetris_piece_t* pPc,
                         tetris_piece_rotation_t r)
{
    /* we just rotate through the available angles in the given direction and
     * make wrap arounds where appropriate
     */
    switch (r)
    {
    case TETRIS_PC_ROT_CLOCKWISE:
        if (pPc->angle == TETRIS_PC_ANGLE_270)
        {
            pPc->angle = TETRIS_PC_ANGLE_0;
        }
        else
        {
            ++pPc->angle;
        }
        break;
    case TETRIS_PC_ROT_COUNTERCLOCKWISE:
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


/* Function:        tetris_piece_lastMatterRow
 * Description:     determines last row of a piece which contains matter
 * Argument pPc:    piece to rotate
 * Return value:    no. of last row containing matter (counting from 0)
 */
uint8_t tetris_piece_lastMatterRow(tetris_piece_t* pPc)
{
    /* either row no. 2 or no. 3 is the last one which contains matter
     * so if the last nibble isn't 0, row no. 3 must be the last one
     */
    if ((0xF000 & tetris_piece_getBitfield(pPc)) != 0)
    {
        return 3;
    }
    else
    {
        return 2;
    }
}
