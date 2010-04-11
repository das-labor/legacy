#ifndef BAST_H_
#define BAST_H_

#include <inttypes.h>
#include "variants.h"
#include "playfield.h"
#include "piece.h"
#include "orientation.h"
#include "input.h"

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

typedef struct tetris_bastet_scorepair_t
{
	tetris_piece_shape_t shape;
	int16_t nScore;
}
tetris_bastet_scorepair_t;


typedef struct tetris_bastet_variant_t
{
	uint16_t nScore;                           /** score of the player */
	uint16_t nHighscore;                       /** highscore */
	uint16_t nHighscoreName;                   /** champion's initials */
	uint8_t nLevel;                            /** current level */
	uint16_t nLines;                           /** number of completed lines */
	tetris_piece_t *pPreviewPiece;             /** the piece for the preview */
	tetris_playfield_t *pPlayfield;            /** playfield to be examined */
	int8_t *pActualColHeights;                 /** actual columns heights */
	int8_t *pColHeights;                       /** predicted column heights */
	tetris_bastet_scorepair_t nPieceScores[7]; /** score for every piece */
}
tetris_bastet_variant_t;

const tetris_variant_t tetrisBastetVariant;


/****************************
 * construction/destruction *
 ****************************/

/**
 * constructs a bastet instance for a given playfield
 * @param pPlayfield the playfield to be observed
 * @return pointer to a newly created bastet instance
 */
void* tetris_bastet_construct(tetris_playfield_t *pPl);


/**
 * destructs the given bastet instance
 * @param pVariantData the bastet instance to be destroyed
 */
void tetris_bastet_destruct(void *pVariantData);


/****************************
 * bastet related functions *
 ****************************/

/**
 * calculates a score for a piece at a given column
 * @param pBastet the bastet instance of interest
 * @param pPiece the piece to be tested
 * @param nColum the column where the piece should be dropped
 * @return score for the given move
 */
int16_t tetris_bastet_evaluateMove(tetris_bastet_variant_t *pBastet,
                                   tetris_piece_t *pPiece,
                                   int8_t nColumn);


/**
 * calculates the best possible score for every piece
 * @param pBastet the bastet instance of interest
 */
void tetris_bastet_evaluatePieces(tetris_bastet_variant_t *pBastet);


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
 * @param nLines the number of rows involved
 */
void tetris_bastet_singleDrop(void *pVariantData,
                              uint8_t nLines);


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


void tetris_bastet_setLastInput(void *pVariantData,
                                tetris_input_command_t inCmd);


tetris_orientation_t tetris_bastet_getOrientation(void *pVariantData);

#endif /* BAST_H_ */
