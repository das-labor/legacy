#ifndef BAST_H_
#define BAST_H_

#include <inttypes.h>
#include "playfield.h"
#include "piece.h"

/*********
 * types *
 *********/

typedef struct tetris_bastet_t
{
	tetris_playfield_t *pPlayfield; // the playfield to be examined
	int8_t *pColHeights;            // array of calculated column heights
}
tetris_bastet_t;


/****************************
 * construction/destruction *
 ****************************/

/* Function:            tetris_bastet_construct
 * Description:         constructs a bastet instance for a given playfield
 * Argument pPlayfield: the playfield to be observed
 * Return value:        pointer to a newly created bastet instance
 */
tetris_bastet_t* tetris_bastet_construct(tetris_playfield_t *pPl);


/* Function:         tetris_bastet_destruct
 * Description:      destructs the given bastet instance
 * Argument pBastet: the bastet instance to be destroyed
 * Return value:     void
 */
void tetris_bastet_destruct(tetris_bastet_t *pBastet);


/****************************
 * bastet related functions *
 ****************************/

/* Function:         tetris_bastet_construct
 * Description:      calculates a score for a piece at a given column
 * Argument pBastet: the bastet instance of interest
 * Argument pPiece:  the piece to be tested
 * Argument pnColum: the column where the piece should be dropped
 * Return value:     score for the given move
 */
int16_t tetris_bastet_evalPos(tetris_bastet_t *pBastet,
                              tetris_piece_t *pPiece,
                              int8_t nColumn);

#endif /* BAST_H_ */
