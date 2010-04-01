#ifndef VARIANT_FP_H_
#define VARIANT_FP_H_

#include <inttypes.h>
#include "variant_std.h"
#include "variants.h"
#include "highscore.h"
#include "piece.h"
#include "orientation.h"
#include "input.h"


/***************
 * entry point *
 ***************/

/**
 * runs the First Person Tetris game
 */
void tetris_fp(void);


const tetris_variant_t tetrisFpVariant;


/*********************
 * get/set functions *
 *********************/


/**
 * retrieves the variant's highscore index
 * @param pVariantData the variant data object we want information from
 */
tetris_highscore_index_t tetris_fp_getHighscoreIndex(void *pVariantData);


void tetris_fp_setLastInput(void *pVariantData,
                            tetris_input_command_t inCmd);

#endif /*VARIANT_FP_H_*/
