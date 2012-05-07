/**
 * \addtogroup tetris
 * @{
 */

/**
 * @file view.c
 * @brief Public interface definitions of Tetris' graphical output routines.
 * @author Christian Kroll
 */


#ifndef TETRIS_VIEW_H_
#define TETRIS_VIEW_H_

#include <stdint.h>
#include "bearing.h"
#include "bucket.h"
#include "variants.h"


/*********
 * types *
 *********/

/** presentation modes */
enum tetris_view_mode_e
{
	TETRIS_VIMO_PAUSED,
	TETRIS_VIMO_RUNNING
};
#ifdef NDEBUG
	typedef uint8_t tetris_view_mode_t;
#else
	typedef enum tetris_view_mode_e tetris_view_mode_t;
#endif


/** data structure that drives the view module */
typedef struct tetris_view_s
{
	tetris_variant_t const *pVariantMethods; /** variant function pointers */
	void *pVariant;                          /** associated variant object */
	tetris_bucket_t *pBucket;                /** associated bucket */
	tetris_view_mode_t modeCurrent;          /** current presentation mode */
	tetris_view_mode_t modeOld;              /** old presentation mode */
	uint8_t nOldLevel;                       /** for detecting level changes */
	tetris_bearing_t nBearing;               /** bearing of the bucket */
}
tetris_view_t;


/*****************************
 *  construction/destruction *
 *****************************/

/**
 * constructs a view for André's borg
 * @param pVarMethods associated variant method pointers
 * @param pVariantData pointer to variant data object which should be observed
 * @param pBucket pointer to bucket which should be observed
 * @return pointer to a newly created view
 */
tetris_view_t *tetris_view_construct(tetris_variant_t const *const pVarMethods,
                                     void *pVariantData,
                                     tetris_bucket_t *pBucket);


/**
 * destructs a view
 * @param pView: pointer to the view to be destructed
 */
inline static void tetris_view_destruct(tetris_view_t *pView)
{
	assert(pView != NULL);
	free(pView);
}


/***************************
 *  view related functions *
 ***************************/

/**
 * destructs a view
 * @param w pointer to an int8_t to store the bucket width
 * @param h pointer to an int8_t to store the bucket height
 */
void tetris_view_getDimensions(int8_t *w,
                               int8_t *h);


/**
 * sets the view mode (pause or running)
 * @param pV  pointer to the view whose mode should be set
 * @param vm  see definition of tetris_view_mode_t
 */
inline static void tetris_view_setViewMode(tetris_view_t *const pV,
                                           tetris_view_mode_t const vm)
{
	pV->modeOld = pV->modeCurrent;
	pV->modeCurrent = vm;
}


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
