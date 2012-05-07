/**
 * \addtogroup tetris
 * @{
 */

/**
 * @file tetris_main.h
 * @brief Public prototype of Tetris' main loop function.
 * @author Christian Kroll
 */

#ifndef TETRIS_MAIN_H_
#define TETRIS_MAIN_H_

#include "variants.h"


/**
 * runs the tetris game
 * @param pVariantMethods struct of function pointers for a game variant
 */
void tetris_main(tetris_variant_t const *const pVariantMethods);

#endif /* TETRIS_MAIN_H_ */

/*@}*/
