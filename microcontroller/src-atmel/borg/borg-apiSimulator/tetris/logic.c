/* Borgtris
 * by: Christian Kroll
 * date: Tuesday, 2007/03/27
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include "logic.h"
#include "piece.h"
#include "playfield.h"
#include "view.h"
#include "input.h"


/****************************
 * construction/destruction *
 ****************************/

/* Function:     tetris_logic_construct
 * Description:  constructs a logic object
 * Return value: pointer to a newly created logic object
 */
tetris_logic_t *tetris_logic_construct()
{
	tetris_logic_t *pLogic = (tetris_logic_t *)malloc(sizeof(tetris_logic_t));
	assert(pLogic != NULL);
	memset(pLogic, 0, sizeof(tetris_logic_t));
	return pLogic;
}


/* Function:     tetris_logic_destruct
 * Description:  destructs a logic object
 * Argument pIn: pointer to the logic object to be destructed
 * Return value: void
 */
void tetris_logic_destruct(tetris_logic_t *pLogic)
{
	assert(pLogic != 0);
	free(pLogic);
}


/***************************
 * logic related functions *
 ***************************/

void tetris ()
{	
	int8_t *pnWidth = (int8_t *)malloc(sizeof(int8_t));
	int8_t *pnHeight = (int8_t *)malloc(sizeof(int8_t));
	tetris_view_getDimensions(pnWidth, pnHeight);
	
	tetris_logic_t *pLogic = tetris_logic_construct();
	tetris_playfield_t *pPl = tetris_playfield_construct(*pnWidth, *pnHeight);
	tetris_input_t *pIn = tetris_input_construct();
	tetris_view_t *pView = tetris_view_construct(pLogic, pPl);
	
	static uint16_t nHighscore = 0;
	tetris_logic_setHighscore(pLogic, nHighscore);   
	
	int8_t nPieceRow;
	uint8_t nRowMask;
	
	tetris_input_command_t cmd;
	tetris_piece_t *pPiece = NULL;
	tetris_piece_t *pOldPiece = NULL;
	tetris_piece_t *pNextPiece = pPiece =
		tetris_piece_construct(rand() % 7, TETRIS_PC_ANGLE_0);
	tetris_logic_setPreviewPiece(pLogic, pNextPiece);
	
	while (tetris_playfield_getStatus(pPl) != TETRIS_PFS_GAMEOVER)
	{
		switch (tetris_playfield_getStatus(pPl))
		{
			case TETRIS_PFS_READY:
				pPiece = pNextPiece;
				pNextPiece =
					tetris_piece_construct(rand() % 7, TETRIS_PC_ANGLE_0);
				tetris_logic_setPreviewPiece(pLogic, pNextPiece);
				tetris_playfield_insertPiece(pPl, pPiece, &pOldPiece);
				if (pOldPiece != NULL)
				{
					tetris_piece_destruct(pOldPiece);
					pOldPiece = NULL;
				}
				break;
				
			case TETRIS_PFS_HOVERING:
				cmd = tetris_input_getCommand(pIn);
				switch (cmd)
				{
					case TETRIS_INCMD_DOWN:
						tetris_playfield_advancePiece(pPl);
						// if the game still runs, reward the player with extra points
						if (tetris_playfield_getStatus(pPl) != TETRIS_PFS_GAMEOVER)
						{
							tetris_logic_singleDrop(pLogic, 1);
						}
						break;
					case TETRIS_INCMD_LEFT:
						tetris_playfield_movePiece(pPl, TETRIS_PFD_LEFT);
						break;
					case TETRIS_INCMD_RIGHT:
						tetris_playfield_movePiece(pPl, TETRIS_PFD_RIGHT);
						break;
					case TETRIS_INCMD_ROTATE_CLOCKWISE:
						tetris_playfield_rotatePiece(pPl, TETRIS_PC_ROT_CLOCKWISE);
						break;
					case TETRIS_INCMD_ROTATE_COUNTERCLOCKWISE:
						tetris_playfield_rotatePiece(pPl, TETRIS_PC_ROT_COUNTERCLOCKWISE);
						break;
					case TETRIS_INCMD_DROP:
						nPieceRow = tetris_playfield_getRow(pPl);
						// emulate immediate drop
						while (tetris_playfield_getStatus(pPl) == TETRIS_PFS_HOVERING)
						{
							tetris_playfield_advancePiece(pPl);
						}
						// if the game still runs, reward the player with extra points
						if (tetris_playfield_getStatus(pPl) != TETRIS_PFS_GAMEOVER)
						{
							tetris_logic_completeDrop(pLogic, tetris_playfield_getRow(pPl) - nPieceRow);	
						}
						break;
					case TETRIS_INCMD_NONE:
						// nothing to do
						break;
				}
				break;
				
			case TETRIS_PFS_DOCKED:
				tetris_playfield_removeCompleteLines(pPl);
				nRowMask = tetris_playfield_getRowMask(pPl);				
				tetris_logic_removedLines(pLogic, nRowMask);			
				tetris_input_setLevel(pIn, tetris_logic_getLevel(pLogic));

				break;
		}
		
		tetris_view_update(pView);
	}
	
	tetris_view_showResults(pView);
	uint16_t nScore = tetris_logic_getScore(pView->pLogic);
	if (nScore > nHighscore)
	{
		nHighscore = nScore;
	}

	free(pnWidth);
	free(pnHeight);
	tetris_view_destruct(pView);
	tetris_input_destruct(pIn);
	tetris_playfield_destruct(pPl);
	tetris_logic_destruct(pLogic);
	tetris_piece_destruct(pPiece);
	tetris_piece_destruct(pNextPiece);
}

/* Function:        tetris_logic_singleDrop
 * Description:     add points which result from single step dropping
 * Argument pLogic: the logic object we want to modify
 * Argument nLines: the number of rows involved
 * Return value:    void
 */
void tetris_logic_singleDrop(tetris_logic_t *pLogic,
                             uint8_t nLines)
{
	assert(pLogic != 0);
	pLogic->nScore += nLines;
}


/* Function:        tetris_logic_completeDrop
 * Description:     add points which result from a complete drop
 * Argument pLogic: the logic object we want to modify
 * Argument nLines: the number of rows involved
 * Return value:    void
 */
void tetris_logic_completeDrop(tetris_logic_t *pLogic,
                               uint8_t nLines)
{
	assert(pLogic != 0);
	pLogic->nScore += nLines * 2;
}


/* Function:          tetris_logic_removedLines
 * Description:       add points which result from removed rows
 * Argument pLogic:   the logic object we want to modify
 * Argument nRowMask: see tetris_playfield_completeLines
 * Return value:      void
 */
void tetris_logic_removedLines(tetris_logic_t *pLogic,
                               uint8_t nRowMask)
{
	assert(pLogic != 0);
	uint8_t nLines = tetris_logic_calculateLines(nRowMask);
	pLogic->nLines += nLines;
	pLogic->nLevel = ((pLogic->nLines / 10) < TETRIS_INPUT_LEVELS) ?
		(pLogic->nLines / 10) : (TETRIS_INPUT_LEVELS - 1);

	switch (nLines)
	{
		case 1:
			pLogic->nScore += 50;
			break;
		case 2:
			pLogic->nScore += 150;
			break;
		case 3:
			pLogic->nScore += 250;
			break;
		case 4:
			pLogic->nScore += 400;
			break;
	}
}


/*****************
 * get functions *
 *****************/
 
/* Function:        tetris_logic_getScore
 * Description:     returns the current score
 * Argument pLogic: the logic object we want information from
 * Return value:    the score as uint16_t
 */
uint16_t tetris_logic_getScore(tetris_logic_t *pLogic)
{
	assert(pLogic != NULL);
	return pLogic->nScore;
}


/* Function:        tetris_logic_getHighscore
 * Description:     returns the current highscore
 * Argument pLogic: the logic object we want information from
 * Return value:    the highscore as uint16_t
 */

uint16_t tetris_logic_getHighscore(tetris_logic_t *pLogic)
{
	assert(pLogic != NULL);
	return pLogic->nHighscore;
}

/* Function:             tetris_logic_setHighscore
 * Description:          set highscore
 * Argument pLogic:      the logic object we want to modify
 * Argmument nHighscore: highscore
 */
void tetris_logic_setHighscore(tetris_logic_t *pLogic,
                               uint16_t nHighscore)
{
	assert(pLogic != NULL);
	pLogic->nHighscore = nHighscore;
}


/* Function:        tetris_logic_getLevel
 * Description:     returns the current level
 * Argument pLogic: the logic object we want information from
 * Return value:    the level as uint8_t
 */
uint8_t tetris_logic_getLevel(tetris_logic_t *pLogic)
{
	assert(pLogic != NULL);
	return pLogic->nLevel;
}


/* Function:        tetris_logic_getLines
 * Description:     returns the number of completed lines
 * Argument pLogic: the logic object we want information from
 * Return value:    number of completed lines as uint8_t
 */
uint8_t tetris_logic_getLines(tetris_logic_t *pLogic)
{
	assert(pLogic != NULL);
	return pLogic->nLines;
}


/* Function:        tetris_logic_setPreviewPiece
 * Description:     help for the view to determine the preview piece
 * Argument pLogic: the logic object we want to modify
 * Argument pPiece: pointer to piece intended to be the next one (may be NULL)
 * Return value:    void
 */
void tetris_logic_setPreviewPiece(tetris_logic_t *pLogic,
                                  tetris_piece_t *pPiece)
{
	assert(pLogic != NULL);
	pLogic->pPreviewPiece = pPiece;
}


/* Function:        tetris_logic_getPreviewPiece
 * Description:     returns piece which was set via tetris_logic_setPreviewPiece
 * Argument pLogic: the logic object we want information from
 * Return value:    the piece intended to be the next one (may be NULL)
 */
tetris_piece_t* tetris_logic_getPreviewPiece(tetris_logic_t *pLogic)
{
	assert(pLogic != NULL);
	return pLogic->pPreviewPiece;
}


/***************************
 * non-interface functions *
 ***************************/

/* Function:          tetris_logic_calculateLines
 * Description:       calculates number of lines for the given row mask
 * Argument nRowMask: row mask from which the no. of lines will be calculated
 * Return value:      number of lines of the row mask
 */
uint8_t tetris_logic_calculateLines(uint8_t nRowMask)
{
	uint8_t i;
	uint8_t nMask = 0x0001;
	uint8_t nLines = 0;
	for (i = 0; i < 4; ++i)
	{
		if ((nMask & nRowMask) != 0)
		{
			++nLines;
		}
		nMask <<= 1;
	}
	
	return nLines;
}
