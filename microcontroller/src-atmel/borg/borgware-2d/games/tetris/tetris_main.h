#ifndef TETRIS_MAIN_H_
#define TETRIS_MAIN_H_

#include "variants.h"


/**
 * runs the tetris game
 * @param pVariantMethods struct of function pointers for a game variant
 */
void tetris_main(tetris_variant_t const *const pVariantMethods);


#endif /* TETRIS_MAIN_H_ */
