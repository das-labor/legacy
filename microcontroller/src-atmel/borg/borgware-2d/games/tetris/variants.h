#ifndef VARIANTS_H_
#define VARIANTS_H_

#include <stdint.h>
#include "bearing.h"
#include "piece.h"
#include "highscore.h"
#include "bucket.h"
#include "input.h"

typedef struct tetris_variant
{
	/**
	 * constructs a variant data object
	 * @param pBucket related bucket object
	 * @return pointer to a newly created variant data object
	 */
	void* (*construct)(tetris_bucket_t *pBucket);


	/**
	 * destructs a variant data object
	 * @param pVariantData pointer to a logic object to be destructed
	 */
	void (*destruct)(void *pVariantData);


	/**
	 * chooses a new piece
	 * @param pVariantData the variant instance of interest
	 * @return a tetris piece
	 */
	tetris_piece_t* (*choosePiece)(void *pVariantData);


	/**
	 * add points which result from single step dropping
	 * @param pVariantData the variant data object we want to modify
	 */
	void (*singleDrop)(void *pVariantData);

	/**
	 * add points which result from a complete drop
	 * @param pVariantData the variant data object we want to modify
	 * @param nLines the number of rows involved
	 */
	void (*completeDrop)(void *pVariantData,
	                     uint8_t nLines);


	/**
	 * add points which result from removed rows
	 * @param pVariantData the variant data object we want to modify
	 * @param nRowMask bit mask of removed lines
	 */
	void (*removedLines)(void *pVariantData,
	                     uint8_t nRowMask);


	/**
	 * returns the current score
	 * @param pVariantData variant data object we want information from
	 * @return score
	 */
	uint16_t (*getScore)(void *pVariantData);


	/**
	 * returns the current highscore
	 * @param pVariantData variant data object we want information from
	 * @return highscore
	 */
	uint16_t (*getHighscore)(void *pVariantData);


	/**
	 * set highscore
	 * @param pVariantData variant data object we want to modify
	 * @param nHighscore highscore
	 */
	void (*setHighscore)(void *pVariantData,
	                     uint16_t nHighscore);


	/**
	 * returns the current highscore name
	 * @param pVariantData variant data object we want information from
	 * @return champion's name packed as uint16_t
	 */
	uint16_t (*getHighscoreName)(void *pVariantData);


	/**
	 * set highscore name
	 * @param pVariantData the variant data object we want to modify
	 * @param nHighscoreName champion's name packed as uint16_t
	 */
	void (*setHighscoreName)(void *pVariantData,
	                         uint16_t nHighscoreName);


	/**
	 * returns the current level
	 * @param pVariantData variant data object we want information from
	 * @return the level as uint8_t
	 */
	uint8_t (*getLevel)(void *pVariantData);


	/**
	 * returns the number of completed lines
	 * @param pVariantData the variant data object we want information from
	 * @return number of completed lines
	 */
	uint16_t (*getLines)(void *pVariantData);


	/**
	 * returns piece which was set via tetris_std_setPreviewPiece
	 * @param pVariantData the variant data object we want information from
	 * @return the piece intended to be the next one
	 */
	tetris_piece_t* (*getPreviewPiece)(void *pVariantData);


	/**
	 * retrieves the variant's highscore index
	 * @param pVariantData the variant data object we want information from
	 */
	tetris_highscore_index_t (*getHighscoreIndex)(void *pVariantData);


	/**
	 * inform the variant about the player's last input
	 * @param pVariantData the variant data object we want to modify
	 * @param inCmd the last issued command
	 */
	void (*setLastInput)(void *pVariantData,
			             tetris_input_command_t inCmd);

	/**
	 * retrieves the variant's preferred bearing of the bucket
	 * @param pVariantData the variant data object we want information from
	 */
	tetris_bearing_t (*getBearing)(void *pVariantData);
}
tetris_variant_t;

#endif /*VARIANTS_H_*/
