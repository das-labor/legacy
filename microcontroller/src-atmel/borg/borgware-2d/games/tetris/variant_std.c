/**
 * \addtogroup tetris
 * @{
 */

/**
 * @file variant_std.c
 * @brief Implementation of the standard Tetris module.
 * @details The routines of this file provide selection of new pieces, line
 *          counting, score calculation, high score management, adjustment of
 *          the bucket's bearing and so on.
 * @author Christian Kroll
 */


#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "../../config.h"
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


/***********
 * defines *
 ***********/

#ifdef RANDOM_SUPPORT
	#define RANDOM8() random8()
#else
	#define RANDOM8() rand()
#endif


/***************
 * entry point *
 ***************/

#ifdef GAME_TETRIS
#ifdef MENU_SUPPORT
// Tetris icon, MSB is leftmost pixel
static uint8_t const tetris_icon[8] PROGMEM =
		{ 0x0f, 0x0f, 0xc3, 0xdb, 0xdb, 0xc3, 0xf0, 0xf0 };
game_descriptor_t tetris_game_descriptor
		__attribute__((section(".game_descriptors"))) =
{
	&tetris,
	tetris_icon,
};
#endif


void tetris(void)
{
	tetris_main(&tetrisStdVariant);
}
#endif

/****************************
 * construction/destruction *
 ****************************/

#ifdef GAME_TETRIS
tetris_variant_t const tetrisStdVariant =
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
	&tetris_std_getHighscoreIndex,
	&tetris_std_setLastInput,
	&tetris_std_getBearing
};
#endif


void *tetris_std_construct(tetris_bucket_t *pBucket)
{
	tetris_standard_variant_t *pStdVariant = (tetris_standard_variant_t *)
			malloc(sizeof(tetris_standard_variant_t));
	assert(pStdVariant != NULL);
	memset(pStdVariant, 0, sizeof(tetris_standard_variant_t));
	// don't begin with S and Z pieces according to official tetris guidelines
	pStdVariant->pPreviewPiece =
					tetris_piece_construct(RANDOM8() % 5, TETRIS_PC_ANGLE_0);

	return pStdVariant;
}


void tetris_std_destruct(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_standard_variant_t *pStdVariant =
			(tetris_standard_variant_t *)pVariantData;
	tetris_piece_destruct(pStdVariant->pPreviewPiece);
	free(pStdVariant);
}


/*****************************
 * variant related functions *
 *****************************/


tetris_piece_t* tetris_std_choosePiece(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_standard_variant_t *pStdVariant =
			(tetris_standard_variant_t *)pVariantData;
	tetris_piece_t *pPiece = pStdVariant->pPreviewPiece;
	pStdVariant->pPreviewPiece =
			tetris_piece_construct(RANDOM8() % 7, TETRIS_PC_ANGLE_0);
	return pPiece;
}


void tetris_std_singleDrop(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_standard_variant_t *pStdVariant =
			(tetris_standard_variant_t *)pVariantData;
	++pStdVariant->nScore;
}


void tetris_std_completeDrop(void *pVariantData,
                             uint8_t nLines)
{
	assert(pVariantData != 0);
	tetris_standard_variant_t *pStdVariant =
			(tetris_standard_variant_t *)pVariantData;
	pStdVariant->nScore += nLines * 2;
}


void tetris_std_removedLines(void *pVariantData,
                             uint8_t nRowMask)
{
	assert(pVariantData != 0);
	tetris_standard_variant_t *pStdVariant =
			(tetris_standard_variant_t *)pVariantData;
	uint8_t nLines = tetris_bucket_calculateLines(nRowMask);
	assert(nLines <= 4);
	pStdVariant->nLines += nLines;
	pStdVariant->nLevel = ((pStdVariant->nLines / 10) < TETRIS_INPUT_LEVELS) ?
		(pStdVariant->nLines / 10) : (TETRIS_INPUT_LEVELS - 1);

	static uint16_t const nScoreTable[] PROGMEM = {0, 50, 150, 250, 400};
	pStdVariant->nScore += pgm_read_word(&nScoreTable[nLines]);
}


/*****************
 * get functions *
 *****************/

uint16_t tetris_std_getScore(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_standard_variant_t *pStdVariant =
			(tetris_standard_variant_t *)pVariantData;
	return pStdVariant->nScore;
}


uint16_t tetris_std_getHighscore(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_standard_variant_t *pStdVariant =
			(tetris_standard_variant_t *)pVariantData;
	return pStdVariant->nHighscore;
}


void tetris_std_setHighscore(void *pVariantData,
                             uint16_t nHighscore)
{
	assert(pVariantData != 0);
	tetris_standard_variant_t *pStdVariant =
			(tetris_standard_variant_t *)pVariantData;
	pStdVariant->nHighscore = nHighscore;
}


uint16_t tetris_std_getHighscoreName(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_standard_variant_t *pStdVariant =
			(tetris_standard_variant_t *)pVariantData;
	return pStdVariant->nHighscoreName;
}


void tetris_std_setHighscoreName(void *pVariantData,
                                 uint16_t nHighscoreName)
{
	assert(pVariantData != 0);
	tetris_standard_variant_t *pStdVariant =
			(tetris_standard_variant_t *)pVariantData;
	pStdVariant->nHighscoreName = nHighscoreName;
}


uint8_t tetris_std_getLevel(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_standard_variant_t *pStdVariant =
			(tetris_standard_variant_t *)pVariantData;
	return pStdVariant->nLevel;
}


uint16_t tetris_std_getLines(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_standard_variant_t *pStdVariant =
			(tetris_standard_variant_t *)pVariantData;
	return pStdVariant->nLines;
}


tetris_piece_t* tetris_std_getPreviewPiece(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_standard_variant_t *pStdVariant =
			(tetris_standard_variant_t *)pVariantData;
	return pStdVariant->pPreviewPiece;
}


tetris_highscore_index_t tetris_std_getHighscoreIndex(void *pVariantData)
{
	return TETRIS_HISCORE_TETRIS;
}


void tetris_std_setLastInput(void *pVariantData,
                             tetris_input_command_t inCmd,
                             uint8_t bMoveOk)
{
}


tetris_bearing_t tetris_std_getBearing(void *pVariantData)
{
	assert (pVariantData != NULL);
	tetris_standard_variant_t *pStdVariant =
			(tetris_standard_variant_t *)pVariantData;

	return pStdVariant->nBearing;
}

/*@}*/
