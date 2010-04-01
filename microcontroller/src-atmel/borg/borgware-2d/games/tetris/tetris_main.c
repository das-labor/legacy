#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

#include "tetris_main.h"
#include "variants.h"
#include "piece.h"
#include "playfield.h"
#include "view.h"
#include "input.h"
#include "highscore.h"


void tetris_main(const tetris_variant_t *const pVariantMethods)
{
	// get view dependent dimensions of the playfield
	int8_t nWidth;
	int8_t nHeight;
	tetris_view_getDimensions(&nWidth, &nHeight);

	// holds the current user command which should be processed
	tetris_input_command_t inCmd;

	// prepare data structures that drive the game...
	tetris_playfield_t *pPl = tetris_playfield_construct(nWidth, nHeight);
	void *pVariantData = pVariantMethods->construct(pPl);
	tetris_input_t *pIn = tetris_input_construct();
	tetris_view_t *pView = tetris_view_construct(pVariantMethods,
			pVariantData, pPl);

	// retrieve highscore
	tetris_highscore_index_t nHighscoreIndex =
			pVariantMethods->getHighscoreIndex(pVariantData);
	uint16_t nHighscore =
			tetris_highscore_retrieveHighscore(nHighscoreIndex);
	uint16_t nHighscoreName =
			tetris_highscore_retrieveHighscoreName(nHighscoreIndex);

	// the view only monitors the variant data and the playfield object for the
	// game status so we must put information like the next piece or the current
	// highscore to a place where the view can find it
	pVariantMethods->setHighscore(pVariantData, nHighscore);
	pVariantMethods->setHighscoreName(pVariantData, nHighscoreName);

	int8_t nPieceRow; // for determining skipped lines after a piece drop
	tetris_piece_t *pPiece = NULL; // initialize piece
	tetris_input_pace_t inPace; // pace flag

	// game loop, runs as long as the game is not over
	while (tetris_playfield_getStatus(pPl) != TETRIS_PFS_GAMEOVER)
	{
		// what we do strongly depends on the status of the playfield
		switch (tetris_playfield_getStatus(pPl))
		{
		// the playfield awaits a new piece
		case TETRIS_PFS_READY:
			pPiece = pVariantMethods->choosePiece(pVariantData);
			tetris_piece_t *pOldPiece;
			tetris_playfield_insertPiece(pPl, pPiece, &pOldPiece);
			// destruct old piece (if it exists) since we don't need it anymore
			if (pOldPiece != NULL)
			{
				tetris_piece_destruct(pOldPiece);
				pOldPiece = NULL;
			}
			break;

		// a piece is hovering and can be controlled by the player
		case TETRIS_PFS_HOVERING:
		case TETRIS_PFS_GLIDING:
			// if the piece is gliding the input module has to grant us
			// a minimum amount of time to move it
			if (tetris_playfield_getStatus(pPl) == TETRIS_PFS_GLIDING)
			{
				inPace = TETRIS_INPACE_GLIDING;
			}
			else
			{
				inPace = TETRIS_INPACE_HOVERING;
			}

			// ensure correct view mode if the game isn't paused
			if ((inCmd = tetris_input_getCommand(pIn, inPace))
					!= TETRIS_INCMD_PAUSE)
			{
				tetris_view_setViewMode(pView, TETRIS_VIMO_RUNNING);
			}

			// what we do depends on what the input module tells us
			switch (inCmd)
			{
			// game paused?
			case TETRIS_INCMD_PAUSE:
				// tell the view it should display the pause screen
				tetris_view_setViewMode(pView, TETRIS_VIMO_PAUSED);
				break;

			// the piece was pulled down by the almighty gravity
			case TETRIS_INCMD_GRAVITY:
				tetris_playfield_advancePiece(pPl);
				break;

			// the player has pulled down the piece herself/himself
			case TETRIS_INCMD_DOWN:
				tetris_playfield_advancePiece(pPl);
				// if the game still runs, reward the player with extra points
				if (tetris_playfield_getStatus(pPl) != TETRIS_PFS_GAMEOVER)
				{
					pVariantMethods->singleDrop(pVariantData, 1);
				}
				break;

			// player shifted the piece to the left
			case TETRIS_INCMD_LEFT:
				tetris_playfield_movePiece(pPl, TETRIS_PFD_LEFT);
				break;

			// player shifted the piece to the right
			case TETRIS_INCMD_RIGHT:
				tetris_playfield_movePiece(pPl, TETRIS_PFD_RIGHT);
				break;

			// player rotated the piece clockwise
			case TETRIS_INCMD_ROT_CW:
				tetris_playfield_rotatePiece(pPl, TETRIS_PC_ROT_CW);
				break;

			// player rotated the piece counter clockwise
			case TETRIS_INCMD_ROT_CCW:
				tetris_playfield_rotatePiece(pPl, TETRIS_PC_ROT_CCW);
				break;

			// the player decided to make an immediate drop
			case TETRIS_INCMD_DROP:
				nPieceRow = tetris_playfield_getRow(pPl);
				// emulate immediate drop
				while((tetris_playfield_getStatus(pPl) == TETRIS_PFS_GLIDING) ||
					(tetris_playfield_getStatus(pPl) == TETRIS_PFS_HOVERING))
				{
					tetris_playfield_advancePiece(pPl);
				}
				// if the game still runs, reward the player with extra points
				if (tetris_playfield_getStatus(pPl) != TETRIS_PFS_GAMEOVER)
				{
					pVariantMethods->completeDrop(pVariantData,
						tetris_playfield_getRow(pPl) - nPieceRow);
				}
				break;

			// avoid compiler warnings
			default:
				break;
			}

			pVariantMethods->setLastInput(pVariantData, inCmd);
			tetris_input_setOrientation(pIn,
					pVariantMethods->getOrientation(pVariantData));

			break;

		// the piece has irrevocably hit the ground
		case TETRIS_PFS_DOCKED:
			// avoid accidentally issued "down" commands
			tetris_input_resetDownKeyRepeat(pIn);

			// remove complete lines (if any)
			tetris_playfield_removeCompleteLines(pPl);

			// let the variant object decide how many points the player gets and
			// whether the level gets changed
			pVariantMethods->removedLines(pVariantData,
					tetris_playfield_getRowMask(pPl));
			tetris_input_setLevel(pIn, pVariantMethods->getLevel(pVariantData));
			break;

		// avoid compiler warnings
		default:
			break;
		}

		// the view updates its state every loop cycle to make changes visible
		tetris_view_update(pView);
	}

	// game is over and we provide the player with her/his results
	tetris_view_showResults(pView);

	// update highscore if it has been beaten
	uint16_t nScore = pVariantMethods->getScore(pVariantData);
	if (nScore > nHighscore)
	{
		nHighscore = nScore;
		nHighscoreName = tetris_highscore_inputName();
		tetris_highscore_saveHighscore(nHighscoreIndex, nHighscore);
		tetris_highscore_saveHighscoreName(nHighscoreIndex, nHighscoreName);
	}

	// cleanup
	tetris_view_destruct(pView);
	tetris_input_destruct(pIn);
	pVariantMethods->destruct(pVariantData);
	tetris_playfield_destruct(pPl);
	tetris_piece_destruct(pPiece);
}
