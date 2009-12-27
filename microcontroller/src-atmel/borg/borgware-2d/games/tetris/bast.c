#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "../../random/prng.h"
#include "piece.h"
#include "playfield.h"
#include "bast.h"


/***************************
 * non-interface functions *
 ***************************/

/* Function:         tetris_bastet_clearColHeights;
 * Description:      resets the array for the column heights
 * Argument pBastet: bastet instance whose array should be reset
 * Argument nStart:  start index
 * Argument nStop:   stop index
 * Return value:     void
 */
void tetris_bastet_clearColHeights(tetris_bastet_t *pBastet,
                                   int8_t nStart,
                                   int8_t nStop)
{
	for (int i = nStart; i <= nStop; ++i)
	{
		pBastet->pColHeights[i] = 0;
	}
}


/* Function:     tetris_bastet_qsortCompare
 * Description:  compare function for quick sorting the pieces by score
 * Argument pa:  the first value to compare
 * Argument pb:  the second value to compare
 * Return value: void
 */
int tetris_bastet_qsortCompare(const void *pa, const void *pb)
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


/****************************
 * construction/destruction *
 ****************************/

/* Function:            tetris_bastet_construct
 * Description:         constructs a bastet instance for a given playfield
 * Argument pPlayfield: the playfield to be observed
 * Return value:        pointer to a newly created bastet instance
 */
tetris_bastet_t* tetris_bastet_construct(tetris_playfield_t *pPl)
{
	tetris_bastet_t *pBastet =
		(tetris_bastet_t *) malloc(sizeof(tetris_bastet_t));

	pBastet->pPlayfield = pPl;

	int8_t nWidth = tetris_playfield_getWidth(pBastet->pPlayfield);
	pBastet->pColHeights = (int8_t*) calloc(nWidth, sizeof(int8_t));
	tetris_bastet_clearColHeights(pBastet, 0, nWidth - 1);

	return pBastet;
}


/* Function:         tetris_bastet_destruct
 * Description:      destructs the given bastet instance
 * Argument pBastet: the bastet instance to be destroyed
 * Return value:     void
 */
void tetris_bastet_destruct(tetris_bastet_t *pBastet)
{
	if (pBastet->pColHeights != NULL)
	{
		free(pBastet->pColHeights);
	}

	free(pBastet);
}


/****************************
 * bastet related functions *
 ****************************/

/* Function:         tetris_bastet_construct
 * Description:      calculates a score for a piece at a given column
 * Argument pBastet: the bastet instance of interest
 * Argument pPiece:  the piece to be tested
 * Argument pnColum: the column where the piece should be dropped
 * Return value:     score for the given move
 */
int16_t tetris_bastet_evalPos(tetris_bastet_t *pBastet,
                              tetris_piece_t *pPiece,
                              int8_t nColumn)
{
	// the row where the given piece collides
	int8_t nDeepestRow = tetris_playfield_predictDeepestRow(pBastet->pPlayfield,
			pPiece, nColumn);

	// initial score of the given piece
	int16_t nScore = -32000;

	// modify score based on complete lines
	int8_t nLines =	tetris_playfield_predictCompleteLines(pBastet->pPlayfield,
			pPiece, nDeepestRow, nColumn);
	nScore += 5000 * nLines;

	// determine sane start and stop columns whose heights we want to calculate
	int8_t nWidth = tetris_playfield_getWidth(pBastet->pPlayfield);
	int8_t nStartCol = ((nColumn - 1) < 0) ? 0 : nColumn - 1;
	int8_t nStopCol;
	// do we start at the left most position?
	if (nColumn <= -3)
	{
		// in case we start at the left most position we calculate all heights
		nStopCol = nWidth - 1;
		// reset all column heights to zero
		tetris_bastet_clearColHeights(pBastet, 0 , nWidth);
	}
	// if not, only calculate columns which are affected by the piece
	else
	{
		nStopCol = (nColumn + 3) < nWidth ? nColumn + 3 : nWidth - 1;
		// clear affected column heights to prevent miscalculations
		tetris_bastet_clearColHeights(pBastet, nStartCol, nStopCol);
	}

	// go through every row and calculate column heights
	tetris_playfield_iterator_t iterator;
	int8_t nHeight = 1;
	uint16_t *pDump = tetris_playfield_predictBottomRow(&iterator,
			pBastet->pPlayfield, pPiece, nDeepestRow, nColumn);
	if (pDump == NULL)
	{
		// an immediately returned NULL is caused by a full dump -> low score
		return -32766;
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
		pDump = tetris_playfield_predictNextRow(&iterator);
		++nHeight;
	}
	// modify score based on predicted column heights
	for (int x = 0; x < nWidth; ++x)
	{
		nScore -= 5 * pBastet->pColHeights[x];
	}

	return nScore;
}


/* Function:         tetris_bastet_minimax
 * Description:      calculates the best possible score for every piece
 * Argument pBastet: the bastet instance of interest
 * Return value:     void
 */
void tetris_bastet_minimax(tetris_bastet_t *pBastet)
{
	int8_t nWidth = tetris_playfield_getWidth(pBastet->pPlayfield);
	tetris_piece_t *pPiece = tetris_piece_construct(TETRIS_PC_LINE,
			TETRIS_PC_ANGLE_0);
	for (int8_t nBlock = TETRIS_PC_LINE; nBlock <= TETRIS_PC_Z; ++nBlock)
	{
		int16_t nMaxScore = -32768;
		tetris_piece_changeShape(pPiece, nBlock);
		int8_t nAngleCount = tetris_piece_angleCount(pPiece);
		for (int8_t nAngle = TETRIS_PC_ANGLE_0; nAngle < nAngleCount; ++nAngle)
		{
			tetris_piece_changeAngle(pPiece, nAngle);
			for (int8_t nCol = -3; nCol < nWidth; ++nCol)
			{
				int16_t nScore = tetris_bastet_evalPos(pBastet, pPiece, nCol);
				nMaxScore = nMaxScore > nScore ? nMaxScore : nScore;
			}
		}
		pBastet->nPieceScores[nBlock].shape = nBlock;
		pBastet->nPieceScores[nBlock].nScore = nMaxScore;
	}
	tetris_piece_destruct(pPiece);
}


/* Function:         tetris_bastet_choosePiece
 * Description:      calculates the worst possible piece
 * Argument pBastet: the bastet instance of interest
 * Return value:     the worst possible piece
 */
tetris_piece_t* tetris_bastet_choosePiece(tetris_bastet_t *pBastet)
{
	const uint8_t nPercent[7] = {75, 92, 98, 100, 100, 100, 100};
	tetris_bastet_minimax(pBastet);

	// perturb score (-2 to +2) to avoid stupid tie handling
	for (uint8_t i = 0; i < 7; ++i)
	{
		pBastet->nPieceScores[i].nScore += random8() % 5 - 2;
	}

	qsort(pBastet->nPieceScores, 7, sizeof(tetris_bastet_scorepair_t),
		&tetris_bastet_qsortCompare);

	uint8_t nRnd = rand() % 100;
	for (uint8_t i = 0; i < 7; i++)
	{
		if (nRnd < nPercent[i])
		{
			return tetris_piece_construct(pBastet->nPieceScores[i].shape,
					TETRIS_PC_ANGLE_0);
		}
	}

	//should not arrive here
	return tetris_piece_construct(pBastet->nPieceScores[0].shape,
			TETRIS_PC_ANGLE_0);
}


/* Function:         tetris_bastet_choosePreviewPiece
 * Description:      returns the best possible piece
 *                   (run tetris_bastet_choosePiece first!)
 * Argument pBastet: the bastet instance of interest
 * Return value:     the worst possible piece
 */
tetris_piece_t* tetris_bastet_choosePreviewPiece(tetris_bastet_t *pBastet)
{
	return tetris_piece_construct(pBastet->nPieceScores[6].shape,
			TETRIS_PC_ANGLE_0);
}
