/**
 * \addtogroup tetris
 * @{
 */

/**
 * @file variant_fp.h
 * @brief Public interface of the "First Person Tetris" module.
 * @details Originally implemented by Michael Holzt. However, his implementation
 *          did not quite fit into this framework anymore as it was heavily
 *          modified to improve modularization. Therefore, his code got replaced
 *          completely. Sorry about that, but thanks for the original
 *          implementation nonetheless!
 * @author Christian Kroll
 * @see variant_std.h
 */

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
 * retrieves the variant's high score index
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

/*@}*/
