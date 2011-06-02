#ifndef VARIANT_FP_H_
#define VARIANT_FP_H_

#include <stdint.h>
#include "highscore.h"
#include "variants.h"
#include "input.h"


/***************
 * entry point *
 ***************/

/**
 * runs the First Person Tetris game
 */
void tetris_fp(void);


tetris_variant_t const tetrisFpVariant;


/*********************
 * get/set functions *
 *********************/


/**
 * retrieves the variant's highscore index
 * @param pVariantData the variant data object we want information from
 */
tetris_highscore_index_t tetris_fp_getHighscoreIndex(void *pVariantData);


/**
 * inform the First Person Tetris instance about the player's last input
 * @param pVariantData the First Person Tetris data object we want to modify
 * @param inCmd the last issued command
 * @param bMoveOk 1 if the last move was possible, otherwise 0
 */
void tetris_fp_setLastInput(void *pVariantData,
                            tetris_input_command_t inCmd,
                            uint8_t bMoveOk);

#endif /*VARIANT_FP_H_*/
