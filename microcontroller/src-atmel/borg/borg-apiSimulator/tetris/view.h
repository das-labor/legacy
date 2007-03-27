#ifndef TETRIS_VIEW_H_
#define TETRIS_VIEW_H_

#include <inttypes.h>
#include "piece.h"
#include "playfield.h"

#define TETRIS_VIEW_BLINK_COUNT 2
#define TETRIS_VIEW_BLINK_DELAY 250

#define TETRIS_VIEW_COLORBORDER 1
#define TETRIS_VIEW_COLORPIECE 3
#define TETRIS_VIEW_COLORSPACE 0

typedef struct tetris_view_t
{
    tetris_playfield_t *pPl; /* associated playfield */
}
tetris_view_t;


/*****************************
 *  construction/destruction *
 *****************************/

/* Function:     tetris_view_construct
 * Description:  constructs a view for André's borg
 * Argument pPl: pointer to the playfield which should be observed
 * Return value: pointer to a newly created view
 */
tetris_view_t *tetris_view_construct(tetris_playfield_t *pPl);


/* Function:       tetris_view_destruct
 * Description:    destructs a view
 * Argument pView: pointer to the view to be destructed
 * Return value:   void
 */
void tetris_view_destruct(tetris_view_t *pView);


/***************************
 *  view related functions *
 ***************************/

/* Function:     tetris_view_getDimensions
 * Description:  destructs a view
 * Argument w:   [out] pointer to an int8_t to store the playfield width
 * Argument h:   [out] pointer to an int8_t to store the playfield height
 * Return value: void
 */
void tetris_view_getDimensions(int8_t *w,
                               int8_t *h);


/* Function:     tetris_view_updatePlayfield
 * Description:  informs a view about changes in the playfield
 * Argument pV:  pointer to the view which should be updated
 * Return value: void
 */
void tetris_view_updatePlayfield(tetris_view_t *pView);


/* Function:     tetris_view_updateNextPiece
 * Description:  informs a view about the next piece
 * Argument pV:  pointer to the view which should be updated
 * Argument pPc: pointer to the piece which should be drawn
 * Return value: void
 */
void tetris_view_updateNextPiece(tetris_view_t *pV,
                                 tetris_piece_t *pPc);


/* Function:     tetris_view_updateLevel
 * Description:  informs a view about entering a new level
 * Argument pV:  pointer to the view which should be updated
 * Return value: void
 */
void tetris_view_updateLevel(tetris_view_t *pV);


/* Function:         tetris_view_drawPlayfield
 * Description:      draws the playfield in the given color
 * Argument nColor:  the color for the border
 * Return value:     void
 */
void tetris_view_drawPlayfield(uint8_t nColor);

#endif /*TETRIS_VIEW_H_*/
