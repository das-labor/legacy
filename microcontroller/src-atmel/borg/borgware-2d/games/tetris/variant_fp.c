#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "../../random/prng.h"
#include "../../compat/pgmspace.h"
#include "../../menu/menu.h"
#include "bearing.h"
#include "piece.h"
#include "highscore.h"
#include "bucket.h"
#include "input.h"
#include "variants.h"
#include "tetris_main.h"
#include "variant_std.h"
#include "variant_fp.h"


/***************
 * entry point *
 ***************/

#ifdef MENU_SUPPORT
// First Person Tetris icon, MSB is leftmost pixel
static uint8_t tetrisfp_icon[8] PROGMEM =
		{ 0xee, 0x89, 0xee, 0x88, 0x88, 0x20, 0x2c, 0x6c };
game_descriptor_t tetrisfp_game_descriptor
		__attribute__((section(".game_descriptors"))) =
{
	&tetris_fp,
	tetrisfp_icon,
};
#endif


void tetris_fp(void)
{
	tetris_main(&tetrisFpVariant);
}


/****************************
 * construction/destruction *
 ****************************/

tetris_variant_t const tetrisFpVariant =
{
	&tetris_std_construct,
	&tetris_std_destruct,
	&tetris_std_choosePiece,
	&tetris_std_singleDrop,
	&tetris_std_completeDrop,
	&tetris_std_removedLines,
	&tetris_std_getScore,
	&tetris_std_getHighscore,
	&tetris_std_setHighscore,
	&tetris_std_getHighscoreName,
	&tetris_std_setHighscoreName,
	&tetris_std_getLevel,
	&tetris_std_getLines,
	&tetris_std_getPreviewPiece,
	&tetris_fp_getHighscoreIndex,
	&tetris_fp_setLastInput,
	&tetris_std_getBearing
};


/*****************
 * get functions *
 *****************/

tetris_highscore_index_t tetris_fp_getHighscoreIndex(void *pVariantData)
{
	return TETRIS_HISCORE_FP;
}


void tetris_fp_setLastInput(void *pVariantData,
                            tetris_input_command_t inCmd)
{
	assert (pVariantData != NULL);
	tetris_standard_variant_t *pStdVariant =
			(tetris_standard_variant_t *)pVariantData;

	if (inCmd == TETRIS_INCMD_ROT_CW)
	{
		pStdVariant->nBearing += 1;
	}
	else if (inCmd == TETRIS_INCMD_ROT_CCW)
	{
		pStdVariant->nBearing += 3;
	}
	pStdVariant->nBearing %= 4;
}
