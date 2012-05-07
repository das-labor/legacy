/**
 * \defgroup tetris Tetris, the popular puzzle game.
 * @{
 */

/**
 * @file tetris_main.c
 * @brief Main loop of the Tetris module.
 * @author Christian Kroll
 */

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "bearing.h"
#include "piece.h"
#include "highscore.h"
#include "bucket.h"
#include "input.h"
#include "variants.h"
#include "view.h"
#include "tetris_main.h"

void tetris_main(tetris_variant_t const *const pVariantMethods)
{
	// get view dependent dimensions of the bucket
	int8_t nWidth, nHeight;
	tetris_view_getDimensions(&nWidth, &nHeight);

	// holds the current user command which should be processed
	tetris_input_command_t inCmd;

	// prepare data structures that drive the game...
	tetris_bucket_t *pBucket = tetris_bucket_construct(nWidth, nHeight);
	void *pVariantData = pVariantMethods->construct(pBucket);
	tetris_input_t *pIn = tetris_input_construct();
	tetris_view_t *pView = tetris_view_construct(pVariantMethods,
			pVariantData, pBucket);

	// retrieve highscore
	tetris_highscore_index_t nHighscoreIndex =
			pVariantMethods->getHighscoreIndex(pVariantData);
	uint16_t nHighscore =
			tetris_highscore_retrieveHighScore(nHighscoreIndex);
	uint16_t nHighscoreName =
			tetris_highscore_retrieveHighScoreName(nHighscoreIndex);

	// the view only monitors the variant data and the bucket object for the
	// game status so we must put information like the next piece or the current
	// highscore to a place where the view can find it
	pVariantMethods->setHighscore(pVariantData, nHighscore);
	pVariantMethods->setHighscoreName(pVariantData, nHighscoreName);

	int8_t nPieceRow; // for determining skipped lines after a piece drop
	tetris_piece_t *pPiece = NULL; // initialize piece
	tetris_input_pace_t inPace; // pace flag

	// game loop, runs as long as the game is not over
	while (tetris_bucket_getStatus(pBucket) != TETRIS_BUS_GAMEOVER)
	{
		// what we do strongly depends on the status of the bucket
		switch (tetris_bucket_getStatus(pBucket))
		{
		// the bucket awaits a new piece
		case TETRIS_BUS_READY:
			pPiece = pVariantMethods->choosePiece(pVariantData);
			// destruct old piece (if it exists) since we don't need it anymore
			tetris_piece_t *pOld;
			if ((pOld = tetris_bucket_insertPiece(pBucket, pPiece)) != NULL)
			{
				tetris_piece_destruct(pOld);
			}
			break;

		// a piece is hovering and can be controlled by the player
		case TETRIS_BUS_HOVERING:
		case TETRIS_BUS_GLIDING:
			// if the piece is gliding the input module has to grant us
			// a minimum amount of time to move it
			if (tetris_bucket_getStatus(pBucket) == TETRIS_BUS_GLIDING)
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

			// helper variable which tells us whether the last move was possible
			uint8_t bMoveOk = 1;

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
				tetris_bucket_advancePiece(pBucket);
				break;

			// the player has pulled down the piece herself/himself
			case TETRIS_INCMD_DOWN:
				tetris_bucket_advancePiece(pBucket);
				// if the game still runs, reward the player with extra points
				if (tetris_bucket_getStatus(pBucket) != TETRIS_BUS_GAMEOVER)
				{
					pVariantMethods->singleDrop(pVariantData);
				}
				break;

			// player shifted the piece to the left
			case TETRIS_INCMD_LEFT:
				bMoveOk = tetris_bucket_movePiece(pBucket, TETRIS_BUD_LEFT);
				break;

			// player shifted the piece to the right
			case TETRIS_INCMD_RIGHT:
				bMoveOk = tetris_bucket_movePiece(pBucket, TETRIS_BUD_RIGHT);
				break;

			// player rotated the piece clockwise
			case TETRIS_INCMD_ROT_CW:
				bMoveOk = tetris_bucket_rotatePiece(pBucket, TETRIS_PC_ROT_CW);
				break;

			// player rotated the piece counter clockwise
			case TETRIS_INCMD_ROT_CCW:
				bMoveOk = tetris_bucket_rotatePiece(pBucket, TETRIS_PC_ROT_CCW);
				break;

			// the player decided to make an immediate drop
			case TETRIS_INCMD_DROP:
				nPieceRow = tetris_bucket_getRow(pBucket);
				// emulate immediate drop
				while(tetris_bucket_getStatus(pBucket) == TETRIS_BUS_GLIDING ||
					tetris_bucket_getStatus(pBucket) == TETRIS_BUS_HOVERING)
				{
					tetris_bucket_advancePiece(pBucket);
				}
				// if the game still runs, reward the player with extra points
				if (tetris_bucket_getStatus(pBucket) != TETRIS_BUS_GAMEOVER)
				{
					pVariantMethods->completeDrop(pVariantData,
						tetris_bucket_getRow(pBucket) - nPieceRow);
				}
				break;

			// avoid compiler warnings
			default:
				break;
			}

			// inform variant object about the user's last move
			pVariantMethods->setLastInput(pVariantData, inCmd, bMoveOk);

			// inform the input module about the requested bearing of the
			// variant object
			tetris_input_setBearing(pIn,
					pVariantMethods->getBearing(pVariantData));

			break;

		// the piece has irrevocably hit the ground
		case TETRIS_BUS_DOCKED:
			// avoid accidentally issued "down" commands
			tetris_input_resetDownKeyRepeat(pIn);

			// remove complete lines (if any)
			tetris_bucket_removeCompleteLines(pBucket);

			// let the variant object decide how many points the player gets and
			// whether the level gets changed
			pVariantMethods->removedLines(pVariantData,
					tetris_bucket_getRowMask(pBucket));
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
		tetris_highscore_saveHighScore(nHighscoreIndex, nHighscore);
		tetris_highscore_saveHighScoreName(nHighscoreIndex, nHighscoreName);
	}

	// cleanup
	tetris_view_destruct(pView);
	tetris_input_destruct(pIn);
	pVariantMethods->destruct(pVariantData);
	tetris_bucket_destruct(pBucket);
	tetris_piece_destruct(pPiece);
}

/*@}*/
