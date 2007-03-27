/* Borgtris
 * by: Christian Kroll
 * date: Tuesday, 2007/03/27
 */

#include <stdlib.h>
#include <inttypes.h>
#include "logic.h"
#include "piece.h"
#include "playfield.h"
#include "view.h"
#include "input.h"

void tetris ()
{
	uint8_t nLines = 0;
	uint8_t nLevel = 0;
	uint8_t nOldLevel = 0;
	uint8_t nRowMask = 0;
	int8_t nPieceRow = 0;
	uint32_t nPoints = 0;
	int8_t *pnWidth = (int8_t *)malloc(sizeof(int8_t));
	int8_t *pnHeight = (int8_t *)malloc(sizeof(int8_t));
	tetris_view_getDimensions(pnWidth, pnHeight);
	
    tetris_playfield_t *pPl = tetris_playfield_construct(*pnWidth, *pnHeight);
	tetris_input_t *pIn = tetris_input_construct();
    tetris_view_t *pView = tetris_view_construct(pPl);
    
    tetris_input_command_t cmd;
    tetris_piece_t *pPiece = NULL;
    tetris_piece_t *pOldPiece = NULL;
    tetris_piece_t *pNextPiece = pPiece =
    	tetris_piece_construct(rand() % 7, TETRIS_PC_ANGLE_0);
    
    while (tetris_playfield_getStatus(pPl) != TETRIS_PFS_GAMEOVER)
    {
    	switch (tetris_playfield_getStatus(pPl))
    	{
			case TETRIS_PFS_READY:
				pPiece = pNextPiece;
				pNextPiece =
					tetris_piece_construct(rand() % 7, TETRIS_PC_ANGLE_0);
				tetris_view_updateNextPiece(pView, pNextPiece);
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
						// if the game is still running, you get one point per line!
						if (tetris_playfield_getStatus(pPl) != TETRIS_PFS_GAMEOVER)
						{
							++nPoints;
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
						// if the game is still running, you get 2 points per line!
						if (tetris_playfield_getStatus(pPl) != TETRIS_PFS_GAMEOVER)
						{
							nPoints += (tetris_playfield_getRow(pPl) - nPieceRow) * 2;
						}
						break;
					case TETRIS_INCMD_NONE:
						// nothing to do
						break;
				}
				break;
				
			case TETRIS_PFS_DOCKED:
				nRowMask = tetris_playfield_removeCompleteLines(pPl);
				uint8_t nNewLines = tetris_logic_calculateLines(nRowMask);
				nLines += nNewLines;				
				
				// heighten the level every 10 lines
				if ((nRowMask != 0) && ((nLines / 10) < 10)) 
				{
					nLevel = nLines / 10;
					if (nLevel > nOldLevel)
					{ 
						tetris_input_setLevel(pIn, nLevel);
						nOldLevel = nLevel;
						tetris_view_updateLevel(pView);
					}
				}
				
				// points are calulated like the first Tetris version for
				// the Nintendo Game Boy
				switch (nNewLines)
				{
					case 1:
						nPoints += 40 * (nLevel + 1);
						break;
					case 2:
						nPoints += 100 * (nLevel + 1);
						break;
					case 3:
						nPoints += 300 * (nLevel + 1);
						break;
					case 4:
						nPoints += 1200 * (nLevel + 1);
						break;
				}
				break;
    	}
    	
    	tetris_view_updatePlayfield(pView);
    }

	free(pnWidth);
	free(pnHeight);
	tetris_input_destruct(pIn);
    tetris_view_destruct(pView);
    tetris_playfield_destruct(pPl);
    tetris_piece_destruct(pPiece);
    tetris_piece_destruct(pNextPiece);
}

/* Function:          tetris_logic_calculateLines
 * Description:       calculates no. of lines for the given row mask
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
