#ifndef TETRIS_VIEW_H_
#define TETRIS_VIEW_H_

#include <inttypes.h>
#include "piece.h"
#include "playfield.h"


#define colorBorder 1
#define colorPiece 3
#define colorSpace 0

typedef struct tetris_view_t
{
    tetris_playfield_t* pPl; // specifies the shape of the piece
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
tetris_view_t* tetris_view_construct(tetris_playfield_t* pPl);


/* Function:       tetris_view_destruct
 * Description:    destructs a view
 * Argument pView: pointer to the view to be destructed
 * Return value:   void
 */
void tetris_view_destruct(tetris_view_t* pView);


/***************************
 *  view related functions *
 ***************************/

/* Function:       tetris_view_updatePlayfield
 * Description:    destructs a view
 * Argument pView: pointer to the view which should be destructed
 * Return value:   void
 */
void tetris_view_updatePlayfield(tetris_view_t* pView);


#endif /*TETRIS_VIEW_H_*/
