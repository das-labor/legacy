#ifndef TETRIS_MAIN_H_
#define TETRIS_MAIN_H_

#include "variants.h"


/**
 * runs the tetris game
 * @param pVariantMethods struct of function pointers for a game variant
 */
void tetris_main(const tetris_variant_t *const pVariantMethods);


#endif /* TETRIS_MAIN_H_ */
