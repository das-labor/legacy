#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "piece.h"
#include "playfield.h"
#include "bast.h"


/***************************
 * non-interface functions *
 ***************************/

/* Function:         tetris_bastet_clearColHeights;
 * Description:      resets the array for the column heights
 * Argument pBastet: bastet instance whose array should be reset
 * Return value:     void
 */
void tetris_bastet_clearColHeights(tetris_bastet_t *pBastet)
{
	int8_t nWidth = tetris_playfield_getWidth(pBastet->pPlayfield);
	for (int i = 0; i < nWidth; ++i)
	{
		pBastet->pColHeights[i] = 0;
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
	tetris_bastet_clearColHeights(pBastet);

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
	int16_t nScore = -32000;

	// modify score based on complete lines
	int8_t nLines =	tetris_playfield_predictCompleteLines(pBastet->pPlayfield,
			pPiece, nColumn);
	nScore += 5000 * nLines;

	// go through every row and calculate column heights
	tetris_playfield_iterator_t iterator;
	int8_t nWidth = tetris_playfield_getWidth(pBastet->pPlayfield);
	int8_t nHeight = 1;
	uint16_t *pDump = tetris_playfield_predictBottomRow(&iterator,
			pBastet->pPlayfield, pPiece, nColumn);
	while (pDump != NULL)
	{
		for (int x = 0; x < nWidth; ++x)
		{
			if ((*pDump & (0x0001 << x)) != 0)
			{
				pBastet->pColHeights[x] = nHeight;
			}
		}
		pDump = tetris_playfield_predictNextRow(&iterator);
		++nHeight;
	}
	// modify score based on predicted column heights
	for (int x = 0; x < nWidth; ++x)
	{
		nScore -= 5 * pBastet->pColHeights[x];
	}

	// reset column height array for later use
	tetris_bastet_clearColHeights(pBastet);

	return nScore;
}
