#ifndef TETRIS_VIEW_H_
#define TETRIS_VIEW_H_

#include <inttypes.h>
#include "logic.h"
#include "piece.h"
#include "playfield.h"


/*********
 * types *
 *********/

typedef struct tetris_view_t
{
	tetris_logic_t *pLogic;  // associated logic object
	tetris_playfield_t *pPl; // associated playfield
	uint8_t nOldLevel;       // helper variable to recognize level changes
}
tetris_view_t;


/*****************************
 *  construction/destruction *
 *****************************/

/* Function:     tetris_view_construct
 * Description:  constructs a view for André's borg
 * Argument pPl: pointer to logic object which should be observed
 * Argument pPl: pointer to playfield which should be observed
 * Return value: pointer to a newly created view
 */
tetris_view_t *tetris_view_construct(tetris_logic_t *pLogic,
                                     tetris_playfield_t *pPl);


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


/* Function:     tetris_view_update
 * Description:  informs a view about changes in the game
 * Argument pV:  pointer to the view which should be updated
 * Return value: void
 */
void tetris_view_update(tetris_view_t *pV);


/* Function:     tetris_view_showResults
 * Description:  shows results after game
 * Argument pV:  pointer to the view which should show the reults
 * Return value: void
 */
void tetris_view_showResults(tetris_view_t *pV);


#endif /*TETRIS_VIEW_H_*/

