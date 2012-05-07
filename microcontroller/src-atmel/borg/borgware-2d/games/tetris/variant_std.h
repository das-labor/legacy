/**
 * \addtogroup tetris
 * @{
 */

/**
 * @file variant_std.h
 * @brief Public interface of the standard Tetris module.
 * @author Christian Kroll
 */


#ifndef VARIANT_STD_H_
#define VARIANT_STD_H_

#include <stdint.h>
#include "piece.h"
#include "bearing.h"
#include "input.h"
#include "variants.h"


/***************
 * entry point *
 ***************/

#ifdef GAME_TETRIS
/**
 * runs the tetris game
 */
void tetris(void);

#endif


/*********
 * types *
 *********/

typedef struct tetris_standard_variant_s
{
	uint16_t nScore;               /** score of the player */
	uint16_t nHighscore;           /** highscore */
	uint16_t nHighscoreName;       /** champion's initials */
	uint8_t nLevel;                /** current level */
	uint16_t nLines;               /** number of completed lines */
	tetris_piece_t *pPreviewPiece; /** the piece intended to be the next one */
	tetris_bearing_t nBearing;     /** bearing of the bucket */
}
tetris_standard_variant_t;


tetris_variant_t const tetrisStdVariant;


/****************************
 * construction/destruction *
 ****************************/

/**
 * constructs a variant data object
 * @param pBucket related bucket object
 * @return pointer to a newly created variant data object
 */
void *tetris_std_construct(tetris_bucket_t *pBucket);


/**
 * destructs a variant data object
 * @param pVariantData pointer to a variant data object to be destructed
 */
void tetris_std_destruct(void *pVariantData);


/*****************************
 * variant related functions *
 *****************************/

/**
 * chooses a new piece
 * @param pVariantData the variant instance of interest
 * @return a tetris piece
 */
tetris_piece_t* tetris_std_choosePiece(void *pVariantData);


/**
 * add points which result from single step dropping
 * @param pVariantData the variant data object we want to modify
 */
void tetris_std_singleDrop(void *pVariantData);


/**
 * add points which result from a complete drop
 * @param pVariantData the variant data object we want to modify
 * @param nLines the number of rows involved
 */
void tetris_std_completeDrop(void *pVariantData,
                             uint8_t nLines);


/**
 * add points which result from removed rows
 * @param pVariantData the variant data object we want to modify
 * @param nRowMask bit mask of removed lines
 */
void tetris_std_removedLines(void *pVariantData,
                             uint8_t nRowMask);


/*********************
 * get/set functions *
 *********************/

/**
 * returns the current score
 * @param pVariantData variant data object we want information from
 * @return score
 */
uint16_t tetris_std_getScore(void *pVariantData);


/**
 * returns the current highscore
 * @param pVariantData variant data object we want information from
 * @return highscore
 */
uint16_t tetris_std_getHighscore(void *pVariantData);


/**
 * set highscore
 * @param pVariantData variant data object we want to modify
 * @param nHighscore highscore
 */
void tetris_std_setHighscore(void *pVariantData,
                             uint16_t nHighscore);


/**
 * returns the current highscore name
 * @param pVariantData variant data object we want information from
 * @return champion's name packed as uint16_t
 */
uint16_t tetris_std_getHighscoreName(void *pVariantData);


/**
 * set highscore name
 * @param pVariantData the variant data object we want to modify
 * @param nHighscoreName champion's name packed as uint16_t
 */
void tetris_std_setHighscoreName(void *pVariantData,
                                 uint16_t nHighscoreName);


/**
 * returns the current level
 * @param pVariantData variant data object we want information from
 * @return the level as uint8_t
 */
uint8_t tetris_std_getLevel(void *pVariantData);


/**
 * returns the number of completed lines
 * @param pVariantData the variant data object we want information from
 * @return number of completed lines
 */
uint16_t tetris_std_getLines(void *pVariantData);


/**
 * returns piece which was set via tetris_std_setPreviewPiece
 * @param pVariantData the variant data object we want information from
 * @return the piece intended to be the next one
 */
tetris_piece_t* tetris_std_getPreviewPiece(void *pVariantData);


/**
 * retrieves the variant's highscore index
 * @param pVariantData the variant data object we want information from
 */
tetris_highscore_index_t tetris_std_getHighscoreIndex(void *pVariantData);


/**
 * inform the Tetris instance about the player's last input
 * @param pVariantData the Tetris data object we want to modify
 * @param inCmd the last issued command
 * @param bMoveOk 1 if the last move was possible, otherwise 0
 */
void tetris_std_setLastInput(void *pVariantData,
                             tetris_input_command_t inCmd,
                             uint8_t bMoveOk);


/**
 * returns the bearing which is requested by the Tetris instance
 * @param pVariantData the variant data object we want information from
 * @return always TETRIS_BEARING_0 as we don't change the bearing in Bastet
 */
tetris_bearing_t tetris_std_getBearing(void *pVariantData);

#endif /*VARIANT_STD_H_*/

/*@}*/
