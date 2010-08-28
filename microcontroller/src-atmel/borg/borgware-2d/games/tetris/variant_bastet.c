#include <stdlib.h>
#include <string.h>
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
#include "variant_bastet.h"


/***********
 * defines *
 ***********/

#define TETRIS_BASTET_HEIGHT_FACTOR 5


/***************************
 * non-interface functions *
 ***************************/

/**
 * calculate the score impact of every column (without any prediction)
 * @param pBastet bastet instance whose column heights should be calculated
 */
void tetris_bastet_calcActualColumnsScoreImpact(tetris_bastet_variant_t *pBastet)
{
	// retrieve sane start and stop values for the column and row indices
	int8_t nWidth = tetris_bucket_getWidth(pBastet->pBucket);
	int8_t nStartRow = tetris_bucket_getHeight(pBastet->pBucket) - 1;
	int8_t nStopRow = tetris_bucket_getFirstMatterRow(pBastet->pBucket);

	// calculate the column heights of the actual bucket configuration
	for (int8_t y = nStartRow; y >= nStopRow; --y)
	{
		uint16_t nDumpRow = tetris_bucket_getDumpRow(pBastet->pBucket, y);
		uint16_t nColMask = 0x0001;
		for (int8_t x = 0; x < nWidth; ++x)
		{
			if ((nDumpRow & nColMask) != 0)
			{
				pBastet->pActualColScoreImpact[x] = nStartRow - y + 1;
			}
			nColMask <<= 1;
		}
	}
	// calculate the score impact of every column
	for (int x = 0; x < nWidth; ++x)
	{
		pBastet->pActualColScoreImpact[x] *= TETRIS_BASTET_HEIGHT_FACTOR;
	}
}


/**
 * calculate the predicted column heights for a given column range
 * @param pBastet bastet instance whose column heights should be predicted
 * @param pPiece the piece to be tested
 * @param nColum the column where the piece should be dropped
 * @param nStartCol the first column of the range to be predicted
 * @param nStopCol the last column of the range to be predicted
 * @return 0 if dropped piece would cause an overflow, 1 if prediction succeeds
 */
uint8_t tetris_bastet_calcPredictedColHeights(tetris_bastet_variant_t *pBastet,
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
	if (pDump == NULL)
	{
		// an immediately returned NULL is caused by a full dump -> low score
		return 0;
	}
	while (pDump != NULL)
	{
		uint16_t nColMask = 0x0001 << nStartCol;
		for (int x = nStartCol; x <= nStopCol; ++x)
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
	return 1;
}


/**
 * compare function for quick sorting the pieces by score
 * @param pa the first value to compare
 * @param pb the second value to compare
 */
int tetris_bastet_qsortCompare(void const *pa,
                               void const *pb)
{
	tetris_bastet_scorepair_t *pScorePairA = (tetris_bastet_scorepair_t *)pa;
	tetris_bastet_scorepair_t *pScorePairB = (tetris_bastet_scorepair_t *)pb;
	if (pScorePairA->nScore == pScorePairB->nScore)
	{
		return 0;
	}
	else if (pScorePairA->nScore < pScorePairB->nScore)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}


/***************
 * entry point *
 ***************/

#ifdef MENU_SUPPORT
// Bastet icon, MSB is leftmost pixel
static uint8_t bastet_icon[8] PROGMEM =
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
	pBastet->pActualColScoreImpact = (int8_t*) calloc(nWidth, sizeof(int8_t));
	pBastet->pColHeights = (int8_t*) calloc(nWidth, sizeof(int8_t));

	return pBastet;
}


void tetris_bastet_destruct(void *pVariantData)
{
	assert(pVariantData != 0);
	tetris_bastet_variant_t *pBastetVariant =
			(tetris_bastet_variant_t *)pVariantData;
	if (pBastetVariant->pActualColScoreImpact != NULL)
	{
		free(pBastetVariant->pActualColScoreImpact);
	}
	if (pBastetVariant->pColHeights != NULL)
	{
		free(pBastetVariant->pColHeights);
	}
	if (pBastetVariant->pPreviewPiece != NULL)
	{
		tetris_piece_destruct(pBastetVariant->pPreviewPiece);
	}

	free(pBastetVariant);
}


/****************************
 * bastet related functions *
 ****************************/

int16_t tetris_bastet_evaluateMove(tetris_bastet_variant_t *pBastet,
                                   tetris_piece_t *pPiece,
                                   int8_t nColumn)
{
	// initial score of the given piece
	int16_t nScore = -32000;

	// the row where the given piece collides
	int8_t nDeepestRow = tetris_bucket_predictDeepestRow(pBastet->pBucket,
			pPiece, nColumn);

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

	// predicted column heights
	if (!tetris_bastet_calcPredictedColHeights(pBastet, pPiece, nDeepestRow,
			nColumn, nStartCol, nStopCol))
	{
		// in case the prediction fails we return the lowest possible score
		return -32766;
	}

	// modify score based on predicted column heights
	for (int x = 0; x < nWidth; ++x)
	{
		if ((x >= nStartCol) && (x <= nStopCol))
		{
			nScore -= TETRIS_BASTET_HEIGHT_FACTOR * pBastet->pColHeights[x];
		}
		else
		{
			nScore -= pBastet->pActualColScoreImpact[x];
		}
	}

	return nScore;
}


void tetris_bastet_evaluatePieces(tetris_bastet_variant_t *pBastet)
{
	// precache actual column heights
	tetris_bastet_calcActualColumnsScoreImpact(pBastet);
	int8_t nWidth = tetris_bucket_getWidth(pBastet->pBucket);
	tetris_piece_t *pPiece = tetris_piece_construct(TETRIS_PC_LINE,
			TETRIS_PC_ANGLE_0);
	for (int8_t nBlock = TETRIS_PC_LINE; nBlock <= TETRIS_PC_Z; ++nBlock)
	{
		int16_t nMaxScore = -32768;
		tetris_piece_setShape(pPiece, nBlock);
		int8_t nAngleCount = tetris_piece_getAngleCount(pPiece);
		for (int8_t nAngle = TETRIS_PC_ANGLE_0; nAngle < nAngleCount; ++nAngle)
		{
			tetris_piece_setAngle(pPiece, nAngle);
			tetris_piece_rotate(pPiece, TETRIS_PC_ROT_CW);
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
		pBastet->nPieceScore[i].nScore += random8() % 5 - 2;
	}

	// sort pieces by their score in ascending order
	qsort(pBastet->nPieceScore, 7, sizeof(tetris_bastet_scorepair_t),
		&tetris_bastet_qsortCompare);

	// new "preview" piece (AKA "won't give you this one")
	if (pBastet->pPreviewPiece != NULL)
	{
		tetris_piece_destruct(pBastet->pPreviewPiece);
	}
	pBastet->pPreviewPiece =
			tetris_piece_construct(pBastet->nPieceScore[6].shape,
					TETRIS_PC_ANGLE_0);

	tetris_piece_t *pPiece = NULL;
	uint8_t const nPercent[4] = {75, 92, 98, 100};
	uint8_t nRnd = rand() % 100;
	for (uint8_t i = 0; i < 4; ++i)
	{
		if (nRnd < nPercent[i])
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


void tetris_bastet_singleDrop(void *pVariantData,
                              uint8_t nLines)
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
                                tetris_input_command_t inCmd)
{
	return;
}


tetris_bearing_t tetris_bastet_getBearing(void *pVariantData)
{
	return TETRIS_BEARING_0;
}
