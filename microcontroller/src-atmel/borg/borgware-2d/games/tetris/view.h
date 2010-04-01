#ifndef TETRIS_VIEW_H_
#define TETRIS_VIEW_H_

#include <inttypes.h>
#include "variants.h"
#include "piece.h"
#include "playfield.h"


/**
 * \defgroup TetrisViewTypes View: Data types
 */
/*@{*/

/*********
 * types *
 *********/

/** presentation modes */
typedef enum tetris_view_mode_t
{
	TETRIS_VIMO_PAUSED,
	TETRIS_VIMO_RUNNING
}
tetris_view_mode_t;


/** data structure that drives the view module */
typedef struct tetris_view_t
{
  const tetris_variant_t *pVariantMethods; /** variant function pointers */
  void *pVariant;                          /** associated variant object */
  tetris_playfield_t *pPl;                 /** associated playfield */
  tetris_view_mode_t modeCurrent;          /** current presentation mode */
  tetris_view_mode_t modeOld;              /** old presentation mode */
  uint8_t nOldLevel;                       /** for detecting level changes */
  tetris_orientation_t nOrient;            /** orientation for the playfield */
}
tetris_view_t;

/*@}*/


/**
 * \defgroup TetrisInterface View: Interface functions
 */
/*@{*/

/*****************************
 *  construction/destruction *
 *****************************/

/**
 * constructs a view for André's borg
 * @param pVarMethods associated variant method pointers
 * @param pVariantData pointer to variant data object which should be observed
 * @param pPl pointer to playfield which should be observed
 * @return pointer to a newly created view
 */
tetris_view_t *tetris_view_construct(const tetris_variant_t *const pVarMethods,
                                     void *pVariantData,
                                     tetris_playfield_t *pPl);


/**
 * destructs a view
 * @param pView: pointer to the view to be destructed
 */
void tetris_view_destruct(tetris_view_t *pView);


/***************************
 *  view related functions *
 ***************************/

/**
 * destructs a view
 * @param w pointer to an int8_t to store the playfield width
 * @param h pointer to an int8_t to store the playfield height
 */
void tetris_view_getDimensions(int8_t *w,
                               int8_t *h);


/**
 * sets the view mode (pause or running)
 * @param pV  pointer to the view whose mode should be set
 * @param vm  see definition of tetris_view_mode_t
 */
void tetris_view_setViewMode(tetris_view_t *pV, tetris_view_mode_t vm);


/**
 * informs a view about changes in the game
 * @param pV pointer to the view which should be updated
 */
void tetris_view_update(tetris_view_t *pV);


/**
 * shows results after game
 * @param pV pointer to the view which should show the results
 */
void tetris_view_showResults(tetris_view_t *pV);


#endif /*TETRIS_VIEW_H_*/

/*@}*/
