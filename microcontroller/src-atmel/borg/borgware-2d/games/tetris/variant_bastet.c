/**
 * \addtogroup tetris
 * @{
 */

/**
 * @file variant_bastet.c
 * @brief Implementation of the "Bastard Tetris" module.
 * @details Based on the game "Bastet" (version 0.41) by Federico Poloni. His
 *          code isn't used as the Borg's Tetris framework is too different from
 *          that of "Petris" (on which Bastet is based), but the algorithms of
 *          this implementation are closely modeled after his ideas.
 * @author Christian Kroll
 * @see Federico Poloni's homepage: http://fph.altervista.org/prog/bastet.html
 */


#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <limits.h>
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
#include "variant_bastet.h"


/***********
 * defines *
 ***********/

#define TETRIS_BASTET_HEIGHT_FACTOR 5

#ifdef RANDOM_SUPPORT
	#define RANDOM8() random8()
#else
	#define RANDOM8() (rand() % (UINT8_MAX + 1))
#endif


/***************************
 * non-interface functions *
 ***************************/

/**
 * Preprocess values like sane starting points for the collision detection or
 * the score impact of every unchanged column to speed up prediction routines.
 * @param pBastet bastet instance which should be preprocessed
 */
static void tetris_bastet_doPreprocessing(tetris_bastet_variant_t *pBastet)
{
	// retrieve sane start and stop values for the column and row indices
	int8_t const nWidth = tetris_bucket_getWidth(pBastet->pBucket);
	int8_t const nStartRow = tetris_bucket_getHeight(pBastet->pBucket) - 1;
	int8_t const nStopRow = tetris_bucket_getFirstTaintedRow(pBastet->pBucket);

	// clear old precalculated scores (last three elements are always 0)
	memset(pBastet->pColScore, 0, nWidth * sizeof(int16_t));
	// calculate the column heights of the actual bucket configuration
	// NOTE: in this loop, pColScore stores the actual column heights,
	//       later it will contain the "score impact" of every unchanged column
	for (int8_t y = nStartRow; y >= nStopRow; --y)
	{
		uint16_t const nDumpRow = tetris_bucket_getDumpRow(pBastet->pBucket, y);
		uint16_t nColMask = 0x0001;
		for (uint8_t x = 0; x < nWidth; ++x)
		{
			if ((nDumpRow & nColMask) != 0)
			{
				pBastet->pColScore[x] = nStartRow - y + 1;
			}
			nColMask <<= 1;
		}
	}

	// starting points for collision detection (to speedup things)
	// calculate the maxima of the 4-tuples from column -3 to -1
	pBastet->pStartingRow[0] = pBastet->pColScore[0];
	pBastet->pStartingRow[1] = pBastet->pColScore[0] > pBastet->pColScore[1] ?
			pBastet->pColScore[0] : pBastet->pColScore[1];
	pBastet->pStartingRow[2] = pBastet->pStartingRow[1] > pBastet->pColScore[2]?
			pBastet->pStartingRow[1] : pBastet->pColScore[2];
	// calculate the maxima of the 4-tuples from column 0 to width-1
	for (uint8_t i = 0; i < nWidth; ++i)
	{
		// casting from int16_t to int8_t is safe here, since at this point
		// pColScore only contains column heights which never exceed INT8_MAX-4
		int8_t t0 = pBastet->pColScore[i] > pBastet->pColScore[i + 1] ?
				pBastet->pColScore[i] : pBastet->pColScore[i + 1];
		int8_t t1 = pBastet->pColScore[i + 2] > pBastet->pColScore[i + 3] ?
				pBastet->pColScore[i + 2] : pBastet->pColScore[i + 3];
		pBastet->pStartingRow[i + 3] = t0 > t1 ? t0 : t1;
	}

	for (uint8_t i = nWidth + 3; i--;)
	{
		// normalize to bucket geometry
		pBastet->pStartingRow[i] = nStartRow - pBastet->pStartingRow[i];
		// finally calculate the score impact of every column
		pBastet->pColScore[i] *= TETRIS_BASTET_HEIGHT_FACTOR;
	}
}


/**
 * calculate the predicted column heights for a given column range
 * @param pBastet bastet instance whose column heights should be predicted
 * @param pPiece the piece to be tested
 * @param nColum the column where the piece should be dropped
 * @param nStartCol the first column of the range to be predicted
 * @param nStopCol the last column of the range to be predicted
 */
static void tetris_bastet_predictColHeights(tetris_bastet_variant_t *pBastet,
                                            tetris_piece_t *pPiece,
                                            int8_t nDeepestRow,
                                            int8_t nColumn,
                                            int8_t nStartCol,
                                            int8_t nStopCol)
{
	// go through every row and calculate column heights
	tetris_bucket_iterator_t iterator;
	int8_t nHeight = 1;
	uint16_t *pDump = tetris_bucket_predictBottomRow(&iterator,
			pBastet->pBucket, pPiece, nDeepestRow, nColumn);
	while (pDump != NULL)
	{
		uint16_t nColMask = 0x0001 << nStartCol;
		for (int8_t x = nStartCol; x <= nStopCol; ++x)
		{
			if ((*pDump & nColMask) != 0)
			{
				pBastet->pColHeights[x] = nHeight;
			}
			nColMask <<= 1;
		}
		pDump = tetris_bucket_predictNextRow(&iterator);
		++nHeight;
	}
}


/**
 * sorts the evaluated pieces by score in ascending order (via bubble sort)
 * @param pBastet the Bastet instance whose evaluated pieces should be sorted
 */
static void tetris_bastet_sortPieces(tetris_bastet_variant_t *pBastet)
{
	for (uint8_t i = 7; i--;)
	{
		for (uint8_t j = 0; j < i; ++j)
		{
			if (pBastet->nPieceScore[j].nScore >
				pBastet->nPieceScore[j + 1].nScore)
			{
				tetris_bastet_scorepair_t tmp = pBastet->nPieceScore[j];
				pBastet->nPieceScore[j] = pBastet->nPieceScore[j + 1];
				pBastet->nPieceScore[j + 1] = tmp;
			}
		}
	}
}


/**
 * calculates a score for a piece at a given column
 * @param pBastet the bastet instance of interest
 * @param pPiece the piece to be tested
 * @param nColum the column where the piece should be dropped
 * @return score for the given move
 */
static int16_t tetris_bastet_evaluateMove(tetris_bastet_variant_t *pBastet,
                                          tetris_piece_t *pPiece,
                                          int8_t nColumn)
{
	// initial score of the given piece
	int16_t nScore = -32000;

	// the row where the given piece collides
	int8_t nDeepestRow = tetris_bucket_predictDeepestRow(pBastet->pBucket,
			pPiece, pBastet->pStartingRow[nColumn + 3], nColumn);

	// in case the prediction fails we return the lowest possible score
	if (nDeepestRow <= TETRIS_BUCKET_INVALID)
	{
		return -32766;
	}

	// modify score based on complete lines
	int8_t nLines =	tetris_bucket_predictCompleteLines(pBastet->pBucket,
			pPiece, nDeepestRow, nColumn);
	nScore += 5000 * nLines;

	// determine a sane range of columns whose heights we want to predict
	int8_t nWidth = tetris_bucket_getWidth(pBastet->pBucket);
	int8_t nStartCol, nStopCol;
	// if lines have been removed, we need to recalculate all column heights
	if (nLines != 0)
	{
		nStartCol = 0;
		nStopCol = nWidth - 1;
	}
	// if no lines were removed, we only need to recalculate a few columns
	else
	{
		nStartCol = (nColumn < 0) ? 0 : nColumn;
		nStopCol = (nColumn + 3) < nWidth ? nColumn + 3 : nWidth - 1;
	}

	// predict column heights of this move
	tetris_bastet_predictColHeights(pBastet, pPiece, nDeepestRow, nColumn,
			nStartCol, nStopCol);

	// modify score based on predicted column heights
	for (int8_t x = nWidth; x--;)
	{
		if ((x >= nStartCol) && (x <= nStopCol))
		{
			nScore -= TETRIS_BASTET_HEIGHT_FACTOR * pBastet->pColHeights[x];
		}
		else
		{
			nScore -= pBastet->pColScore[x];
		}
	}

	return nScore;
}


/**
 * calculates the best possible score for every piece
 * @param pBastet the bastet instance of interest
 */
static void tetris_bastet_evaluatePieces(tetris_bastet_variant_t *pBastet)
{
	// precache actual column heights
	tetris_bastet_doPreprocessing(pBastet);
	int8_t nWidth = tetris_bucket_getWidth(pBastet->pBucket);
	tetris_piece_t *pPiece = tetris_piece_construct(TETRIS_PC_LINE,
			TETRIS_PC_ANGLE_0);
	for (uint8_t nBlock = TETRIS_PC_LINE; nBlock <= TETRIS_PC_Z; ++nBlock)
	{
		int16_t nMaxScore = -32768;
		tetris_piece_setShape(pPiece, nBlock);
		int8_t nAngleCount = tetris_piece_getAngleCount(pPiece);
		for (uint8_t nAngle = TETRIS_PC_ANGLE_0; nAngle < nAngleCount; ++nAngle)
		{
			tetris_piece_setAngle(pPiece, nAngle);
			for (int8_t nCol = -3; nCol < nWidth; ++nCol)
			{
				int16_t nScore = tetris_bastet_evaluateMove(pBastet,
						pPiece, nCol);
				nMaxScore = nMaxScore > nScore ? nMaxScore : nScore;
			}
		}
		pBastet->nPieceScore[nBlock].shape = nBlock;
		pBastet->nPieceScore[nBlock].nScore = nMaxScore;
	}
	tetris_piece_destruct(pPiece);
}


/***************
 * entry point *
 ***************/

#ifdef MENU_SUPPORT
// Bastet icon, MSB is leftmost pixel
static uint8_t const bastet_icon[8] PROGMEM =
		{ 0x81, 0xc3, 0xff, 0x99, 0xff, 0xff, 0x66, 0x3c };
game_descriptor_t bastet_game_descriptor
		__attribute__((section(".game_descriptors"))) =
{
	&tetris_bastet,
	bastet_icon,
};
#endif


void tetris_bastet(void)
{
	tetris_main(&tetrisBastetVariant);
}


/****************************
 * construction/destruction *
 ****************************/

tetris_variant_t const tetrisBastetVariant =
{
	&tetris_bastet_construct,
	&tetris_bastet_destruct,
	&tetris_bastet_choosePiece,
	&tetris_bastet_singleDrop,
	&tetris_bastet_completeDrop,
	&tetris_bastet_removedLines,
	&tetris_bastet_getScore,
	&tetris_bastet_getHighscore,
	&tetris_bastet_setHighscore,
	&tetris_bastet_getHighscoreName,
	&tetris_bastet_setHighscoreName,
	&tetris_bastet_getLevel,
	&tetris_bastet_getLines,
	&tetris_bastet_getPreviewPiece,
	&tetris_bastet_getHighscoreIndex,
	&tetris_bastet_setLastInput,
	&tetris_bastet_getBearing
};


void *tetris_bastet_construct(tetris_bucket_t *pBucket)
{
	tetris_bastet_variant_t *pBastet =
		(tetris_bastet_variant_t *) malloc(sizeof(tetris_bastet_variant_t));
	memset(pBastet, 0, sizeof(tetris_bastet_variant_t));

	pBastet->pBucket = pBucket;

	int8_t nWidth = tetris_bucket_getWidth(pBastet->pBucket);
	pBastet->pColScore = (int16_t*) calloc(nWidth + 3, sizeof(int16_t));
	pBastet->pStartingRow = (int8_t*) calloc(nWidth + 3, sizeof(int8_t));
	pBastet->pColHeights = (int8_t*) calloc(nWidth, sizeof(int8_t));

	return pBastet;
}


void tetris_bastet_destruct(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_bastet_variant_t *pBastetVariant =
			(tetris_bastet_variant_t *)pVariantData;
	free(pBastetVariant->pColScore);
	free(pBastetVariant->pColHeights);
	if (pBastetVariant->pPreviewPiece != NULL)
	{
		tetris_piece_destruct(pBastetVariant->pPreviewPiece);
	}

	free(pBastetVariant);
}


/****************************
 * bastet related functions *
 ****************************/

tetris_piece_t* tetris_bastet_choosePiece(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_bastet_variant_t *pBastet =
			(tetris_bastet_variant_t *)pVariantData;

	// determine the best score for every piece
	tetris_bastet_evaluatePieces(pBastet);

	// perturb score (-2 to +2) to avoid stupid tie handling
	for (uint8_t i = 0; i < 7; ++i)
	{
		pBastet->nPieceScore[i].nScore += RANDOM8() % 5 - 2;
	}

	// sort pieces by their score in ascending order
	tetris_bastet_sortPieces(pBastet);

	// new "preview" piece (AKA "won't give you this one")
	if (pBastet->pPreviewPiece != NULL)
	{
		tetris_piece_destruct(pBastet->pPreviewPiece);
	}
	pBastet->pPreviewPiece =
			tetris_piece_construct(pBastet->nPieceScore[6].shape,
					TETRIS_PC_ANGLE_0);

	tetris_piece_t *pPiece = NULL;
	uint8_t const nPercent[4] = {191, 235, 250, 255};
	uint8_t const nRnd = RANDOM8();
	for (uint8_t i = 0; i < 4; ++i)
	{
		if (nRnd <= nPercent[i])
		{
			// circumvent a trick where the line piece consecutively gets the
			// lowest score although it removes a line every time
			if ((pBastet->nPieceScore[i].shape == TETRIS_PC_LINE) &&
					(pBastet->nPieceScore[i].nScore >= -28000))
			{
				i += ((i == 0) ? 1 : -1);
			}

			pPiece = tetris_piece_construct(pBastet->nPieceScore[i].shape,
						TETRIS_PC_ANGLE_0);
			break;
		}
	}
	return pPiece;
}


void tetris_bastet_singleDrop(void *pVariantData)
{
	return;
}


void tetris_bastet_completeDrop(void *pVariantData,
                                uint8_t nLines)
{
	return;
}


void tetris_bastet_removedLines(void *pVariantData,
                                uint8_t nRowMask)
{
	assert(pVariantData != 0);
	tetris_bastet_variant_t *pBastet =
			(tetris_bastet_variant_t *)pVariantData;
	uint8_t nLines = tetris_bucket_calculateLines(nRowMask);

	pBastet->nLines += nLines;
	pBastet->nLevel = ((pBastet->nLines / 10) < TETRIS_INPUT_LEVELS) ?
		(pBastet->nLines / 10) : (TETRIS_INPUT_LEVELS - 1);

	pBastet->nScore += nLines;
	return;
}


/*****************
 * get functions *
 *****************/

uint16_t tetris_bastet_getScore(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_bastet_variant_t *pBastetVariant =
			(tetris_bastet_variant_t *)pVariantData;
	return pBastetVariant->nScore;
}


uint16_t tetris_bastet_getHighscore(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_bastet_variant_t *pBastetVariant =
			(tetris_bastet_variant_t *)pVariantData;
	return pBastetVariant->nHighscore;
}


void tetris_bastet_setHighscore(void *pVariantData,
                                uint16_t nHighscore)
{
	assert(pVariantData != 0);
	tetris_bastet_variant_t *pBastetVariant =
			(tetris_bastet_variant_t *)pVariantData;
	pBastetVariant->nHighscore = nHighscore;
}


uint16_t tetris_bastet_getHighscoreName(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_bastet_variant_t *pBastetVariant =
			(tetris_bastet_variant_t *)pVariantData;
	return pBastetVariant->nHighscoreName;
}


void tetris_bastet_setHighscoreName(void *pVariantData,
                                    uint16_t nHighscoreName)
{
	assert(pVariantData != 0);
	tetris_bastet_variant_t *pBastetVariant =
			(tetris_bastet_variant_t *)pVariantData;
	pBastetVariant->nHighscoreName = nHighscoreName;
}


uint8_t tetris_bastet_getLevel(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_bastet_variant_t *pBastet =
			(tetris_bastet_variant_t *)pVariantData;
	return pBastet->nLevel;
}


uint16_t tetris_bastet_getLines(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_bastet_variant_t *pBastet =
			(tetris_bastet_variant_t *)pVariantData;
	return pBastet->nLines;
}


tetris_piece_t* tetris_bastet_getPreviewPiece(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_bastet_variant_t *pBastetVariant =
			(tetris_bastet_variant_t *)pVariantData;
	return pBastetVariant->pPreviewPiece;
}


tetris_highscore_index_t tetris_bastet_getHighscoreIndex(void *pVariantData)
{
	return TETRIS_HISCORE_BASTET;
}


void tetris_bastet_setLastInput(void *pVariantData,
                                tetris_input_command_t inCmd,
                                uint8_t bMoveOk)
{
	return;
}


tetris_bearing_t tetris_bastet_getBearing(void *pVariantData)
{
	return TETRIS_BEARING_0;
}

/*@}*/
