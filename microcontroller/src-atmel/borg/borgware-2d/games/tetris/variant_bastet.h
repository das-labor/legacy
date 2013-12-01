/**
 * \addtogroup tetris
 * @{
 */

/**
 * @file variant_bastet.h
 * @brief Public interface of the "Bastard Tetris" module.
 * @details Based on the game "Bastet" (version 0.41) by Federico Poloni. His
 *          code isn't used as the Borg's Tetris framework is too different from
 *          that of "Petris" (on which Bastet is based), but the algorithms of
 *          this implementation are closely modeled after his ideas.
 * @author Christian Kroll
 * @see Federico Poloni's homepage: http://fph.altervista.org/prog/bastet.html
 */


#ifndef VARIANT_BASTET_H_
#define VARIANT_BASTET_H_

#include <stdint.h>
#include "bearing.h"
#include "piece.h"
#include "highscore.h"
#include "bucket.h"
#include "input.h"
#include "variants.h"


/***************
 * entry point *
 ***************/

/**
 * runs the Bastet game
 */
void tetris_bastet(void);


/*********
 * types *
 *********/

typedef struct tetris_bastet_scorepair_s
{
	tetris_piece_shape_t shape;
	int16_t nScore;
}
tetris_bastet_scorepair_t;


typedef struct tetris_bastet_variant_s
{
	uint16_t nScore;                          /**< score of the player */
	uint16_t nHighscore;                      /**< high score */
	uint16_t nHighscoreName;                  /**< champion's initials */
	uint8_t nLevel;                           /**< current level */
	uint16_t nLines;                          /**< number of completed lines */
	tetris_piece_t *pPreviewPiece;            /**< the piece for the preview */
	tetris_bucket_t *pBucket;                 /**< bucket to be examined */
	int16_t *pColScore;                       /**< score impact of each column*/
	int8_t *pStartingRow;                     /**< starting point for
	                                               collision detection */
	int8_t *pColHeights;                      /**< predicted column heights */
	tetris_bastet_scorepair_t nPieceScore[7]; /**< score for every piece */
}
tetris_bastet_variant_t;

tetris_variant_t const tetrisBastetVariant;


/****************************
 * construction/destruction *
 ****************************/

/**
 * constructs a bastet instance for a given bucket
 * @param pBucket the bucket to be observed
 * @return pointer to a newly created bastet instance
 */
void* tetris_bastet_construct(tetris_bucket_t *pBucket);


/**
 * destructs the given bastet instance
 * @param pVariantData the bastet instance to be destroyed
 */
void tetris_bastet_destruct(void *pVariantData);


/****************************
 * bastet related functions *
 ****************************/

/**
 * chooses a new worst possible piece
 * @param pVariantData the variant instance of interest
 * @return a tetris piece
 */
tetris_piece_t* tetris_bastet_choosePiece(void *pBastet);


/**
 * chooses a new (best possible) piece for the preview
 * @param pVariantData the variant instance of interest
 * @return a tetris piece
 */
tetris_piece_t* tetris_bastet_choosePreviewPiece(void *pBastet);


/**
 * add points which result from single step dropping
 * @param pVariantData the variant data object we want to modify
 */
void tetris_bastet_singleDrop(void *pVariantData);


/**
 * add points which result from a complete drop
 * @param pVariantData the variant data object we want to modify
 * @param nLines the number of rows involved
 */
void tetris_bastet_completeDrop(void *pVariantData,
                                uint8_t nLines);


/**
 * add points which result from removed rows
 * @param pVariantData the variant data object we want to modify
 * @param nRowMask bit mask of removed lines
 */
void tetris_bastet_removedLines(void *pVariantData,
                                uint8_t nRowMask);


/*********************
 * get/set functions *
 *********************/

/**
 * returns the current score
 * @param pVariantData variant data object we want information from
 * @return score
 */
uint16_t tetris_bastet_getScore(void *pVariantData);


/**
 * returns the current highscore
 * @param pVariantData variant data object we want information from
 * @return highscore
 */
uint16_t tetris_bastet_getHighscore(void *pVariantData);


/**
 * set highscore
 * @param pVariantData variant data object we want to modify
 * @param nHighscore highscore
 */
void tetris_bastet_setHighscore(void *pVariantData,
                                uint16_t nHighscore);


/**
 * returns the current highscore name
 * @param pVariantData variant data object we want information from
 * @return champion's name packed as uint16_t
 */
uint16_t tetris_bastet_getHighscoreName(void *pVariantData);


/**
 * set highscore name
 * @param pVariantData the variant data object we want to modify
 * @param nHighscoreName champion's name packed as uint16_t
 */
void tetris_bastet_setHighscoreName(void *pVariantData,
                                    uint16_t nHighscoreName);


/**
 * returns the current level
 * @param pVariantData variant data object we want information from
 * @return the level as uint8_t
 */
uint8_t tetris_bastet_getLevel(void *pVariantData);


/**
 * returns the number of completed lines
 * @param pVariantData the variant data object we want information from
 * @return number of completed lines
 */
uint16_t tetris_bastet_getLines(void *pVariantData);


/**
 * returns piece which was set via tetris_std_setPreviewPiece
 * @param pVariantData the variant data object we want information from
 * @return the piece intended to be the next one
 */
tetris_piece_t* tetris_bastet_getPreviewPiece(void *pVariantData);


/**
 * retrieves the variant's highscore index
 * @param pVariantData the variant data object we want information from
 */
tetris_highscore_index_t tetris_bastet_getHighscoreIndex(void *pVariantData);


/**
 * inform the Bastet instance about the player's last input
 * @param pVariantData the Bastet object we want to modify
 * @param inCmd the last issued command
 * @param bMoveOk 1 if the last move was possible, otherwise 0
 */
void tetris_bastet_setLastInput(void *pVariantData,
                                tetris_input_command_t inCmd,
                                uint8_t bMoveOk);


/**
 * returns the bearing which is requested by the Bastet instance (always 0)
 * @param pVariantData the variant data object we want information from
 * @return always TETRIS_BEARING_0 as we don't change the bearing in Bastet
 */
tetris_bearing_t tetris_bastet_getBearing(void *pVariantData);

#endif /*VARIANT_BASTET_H_*/

/*@}*/
