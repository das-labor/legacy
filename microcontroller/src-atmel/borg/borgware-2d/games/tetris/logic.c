/* Borgtris
 * by: Christian Kroll
 * date: Tuesday, 2007/09/16
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

#include "../../autoconf.h"
#include "../../compat/eeprom.h"
#include "../../compat/pgmspace.h"
#include "../../menu/menu.h"
#include "../../random/prng.h"
#include "../../pixel.h"

#include "logic.h"
#include "piece.h"
#include "playfield.h"
#include "view.h"
#include "input.h"
#include "highscore.h"

#define NUMHIGHSCORES 3

#ifdef GAME_BASTET
#include "bast.h"
#endif

#ifdef GAME_TETRIS_FP
#include "tetrisfp.h"
#endif

#ifdef EEMEM
/***********************
 * Highscore in EEPROM *
 ***********************/

uint16_t tetris_logic_nHighscore[NUMHIGHSCORES] EEMEM;
uint16_t tetris_logic_nHighscoreName[NUMHIGHSCORES] EEMEM;
#endif

// Tetris icon, MSB is leftmost pixel
void tetris();

#ifdef MENU_SUPPORT
static uint8_t tetris_icon[8] PROGMEM =
	{ 0x0f, 0x0f, 0xc3, 0xdb, 0xdb, 0xc3, 0xf0, 0xf0 };
game_descriptor_t tetris_game_descriptor
	__attribute__((section(".game_descriptors"))) =
{
	&tetris,
	tetris_icon,
};


#ifdef GAME_TETRIS_FP
// Bastet icon, MSB is leftmost pixel
static uint8_t tetrisfp_icon[8] PROGMEM =
	{ 0xee, 0x89, 0xee, 0x88, 0x88, 0x20, 0x2c, 0x6c };
game_descriptor_t tetrisfp_game_descriptor
	__attribute__((section(".game_descriptors"))) =
{
	&tetris_fp,
	tetrisfp_icon,
};
#endif

#ifdef GAME_BASTET
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


#endif /*MENU_SUPPORT*/

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
	uint8_t nMask = 0x0001;
	uint8_t nLines = 0;
	for (uint8_t i = 0; i < 4; ++i)
	{
		if ((nMask & nRowMask) != 0)
		{
			++nLines;
		}
		nMask <<= 1;
	}

	return nLines;
}


/* Function:                 tetris_logic_retrieveHighscore
 * Description:              retrieves the highscore from storate
 * Argument nHighscoreIndex: highscore index (for different game variants)
 * Return value:             the highscore
 */
uint16_t tetris_logic_retrieveHighscore(uint8_t nHighscoreIndex)
{
#ifdef EEMEM
	uint16_t nHighscore = 0;
	nHighscore = eeprom_read_word(&tetris_logic_nHighscore[nHighscoreIndex]);

	// a score of 65535 is most likely caused by uninitialized EEPROM addresses
	if (nHighscore == 65535)
	{
		nHighscore = 0;
	}

	return nHighscore;
#else
	return 0;
#endif
}


/* Function:                 tetris_logic_saveHighscore
 * Description:              saves the highscore into the storage
 * Argument nHighscoreIndex: highscore index (for different game variants)
 * Argument nHighscoreName:  the highscore
 * Return value:             void
 */
void tetris_logic_saveHighscore(uint8_t nHighscoreIndex, uint16_t nHighscore)
{
#ifdef EEMEM
	if (nHighscore > tetris_logic_retrieveHighscore(nHighscoreIndex))
	{
		eeprom_write_word(&tetris_logic_nHighscore[nHighscoreIndex],
				nHighscore);
	}
#endif
}


/* Function:                 tetris_logic_retrieveHighscoreName
 * Description:              retrieves the initials of the champion from storage
 * Argument nHighscoreIndex: highscore index (for different game variants)
 * Return value:             the initials of the champion packed into a uint16_t
 */
uint16_t tetris_logic_retrieveHighscoreName(uint8_t nHighscoreIndex)
{
#ifdef EEMEM
	uint16_t nHighscoreName = 0;
	nHighscoreName =
			eeprom_read_word(&tetris_logic_nHighscoreName[nHighscoreIndex]);

	// a score of 65535 is most likely caused by uninitialized EEPROM addresses
	if (nHighscoreName == 65535)
	{
		nHighscoreName = 0;
	}

	return nHighscoreName;
#else
	return 0;
#endif
}


/* Function:                 tetris_logic_saveHighscoreName
 * Description:              saves the initials of the champion
 * Argument nHighscoreIndex: highscore index (for different game variants)
 * Argument nHighscoreName:  the initials of the champion packed into a uint16_t
 * Return value:             void
 */
void tetris_logic_saveHighscoreName(uint8_t nHighscoreIndex,
                                    uint16_t nHighscoreName)
{
#ifdef EEMEM
	eeprom_write_word(&tetris_logic_nHighscoreName[nHighscoreIndex],
			nHighscoreName);
#endif
}


/****************************
 * construction/destruction *
 ****************************/

/* Function:         tetris_logic_construct
 * Description:      constructs a logic object
 * Argument nBastet: 0 for normal tetris, 1 for bastet
 * Return value:     pointer to a newly created logic object
 */
tetris_logic_t *tetris_logic_construct(uint8_t nBastet)
{
	tetris_logic_t *pLogic = (tetris_logic_t *) malloc(sizeof(tetris_logic_t));
	assert(pLogic != NULL);
	memset(pLogic, 0, sizeof(tetris_logic_t));
	pLogic->nBastet = nBastet;
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

/* Function:     tetris
 * Description:  runs the tetris game
 * Return value: void
 */
void tetris()
{
	tetris_main(0);
}


/* Function:     tetris_bastet
 * Description:  runs the bastet game
 * Return value: void
 */
void tetris_bastet()
{
	tetris_main(1);
}


/* Function:     tetris_fp
 * Description:  runs the tetris first person game
 * Return value: void
 */
void tetris_fp()
{
	tetris_main(2);
}


/* Function:         tetris_main
 * Description:      runs the tetris game
 * Argument nMode:   0 for normal Tetris, 1 for Bastet
 *                   2 for first person tetris
 * Return value:     void
 */
void tetris_main(int8_t nMode)
{
	// get view dependent dimensions of the playfield
	int8_t nWidth;
	int8_t nHeight;
	tetris_view_getDimensions(&nWidth, &nHeight);

	// holds the current user command which should be processed
	tetris_input_command_t inCmd;

#ifdef GAME_TETRIS_FP
	tetris_screendir = 0;
#endif

	// prepare data structures that drive the game...
	tetris_logic_t *pLogic = tetris_logic_construct((nMode==1));
	tetris_playfield_t *pPl = tetris_playfield_construct(nWidth, nHeight);
	tetris_input_t *pIn = tetris_input_construct();
	tetris_view_t *pView = tetris_view_construct(pLogic, pPl, (nMode==2));
#ifdef GAME_BASTET
	tetris_bastet_t *pBastet;
#endif

	// runtime variable
	int8_t nPieceRow;

	// retrieve highscore
	static uint16_t nHighscore = 0;
	static uint16_t nHighscoreName = 0;
#ifndef GAME_BASTET
	if (nHighscore == 0)
	{
#endif
		nHighscore = tetris_logic_retrieveHighscore(nMode);
		nHighscoreName = tetris_logic_retrieveHighscoreName(nMode);
#ifndef GAME_BASTET
	}
#endif

	// initialize current and next piece
	tetris_piece_t *pPiece;
	tetris_piece_t *pNextPiece;
#ifdef GAME_BASTET
	if (nMode == 1)
	{
		pBastet = tetris_bastet_construct(pPl);
		pNextPiece = pPiece = NULL;
	}
	else
	{
#endif
		pNextPiece = pPiece =
				tetris_piece_construct(random8() % 7, TETRIS_PC_ANGLE_0);
		tetris_logic_setPreviewPiece(pLogic, pNextPiece);
#ifdef GAME_BASTET
	}
#endif

	// the view only monitors the logic and the playfield object for the game
	// status so we must put information like the next piece or the current
	// highscore to a place where the view can find it
	tetris_logic_setHighscore(pLogic, nHighscore);
	tetris_logic_setHighscoreName(pLogic, nHighscoreName);

	// pace flag
	tetris_input_pace_t inPace;

	// game loop, runs as long as the game is not over
	while (tetris_playfield_getStatus(pPl) != TETRIS_PFS_GAMEOVER)
	{
		// what we do strongly depends on the status of the playfield
		switch (tetris_playfield_getStatus(pPl))
		{
		// the playfield awaits a new piece
		case TETRIS_PFS_READY:
#ifdef GAME_BASTET
			if (nMode == 1)
			{
				if (pPiece != NULL)
				{
					tetris_piece_destruct(pPiece);
				}
				if (pNextPiece != NULL)
				{
					tetris_piece_destruct(pNextPiece);
				}
				pPiece = tetris_bastet_choosePiece(pBastet);
				pNextPiece = tetris_bastet_choosePreviewPiece(pBastet);
				tetris_piece_t *pOldPiece;
				tetris_playfield_insertPiece(pPl, pPiece, &pOldPiece);
				tetris_logic_setPreviewPiece(pLogic, pNextPiece);
			}
			else
			{
#endif
				// make preview piece the current piece and create a new
				// preview piece
				pPiece = pNextPiece;
				pNextPiece = tetris_piece_construct(random8() % 7,
					TETRIS_PC_ANGLE_0);
				tetris_logic_setPreviewPiece(pLogic, pNextPiece);

				// insert new piece into playfield
				tetris_piece_t *pOldPiece;
				tetris_playfield_insertPiece(pPl, pPiece, &pOldPiece);

				// destruct old piece (if it exists) since we don't need it
				// anymore
				if (pOldPiece != NULL)
				{
					tetris_piece_destruct(pOldPiece);
					pOldPiece = NULL;
				}
#ifdef GAME_BASTET
			}
#endif
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
			if ((inCmd = tetris_input_getCommand(pIn, inPace, (nMode==2)))
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
					tetris_logic_singleDrop(pLogic, 1);
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
#ifdef GAME_TETRIS_FP
				if (nMode == 2) {
				  tetris_view_rotate(); 
				  tetris_playfield_rotatePiece(pPl, TETRIS_PC_ROT_CCW);
				} else
				{
#endif
					tetris_playfield_rotatePiece(pPl, TETRIS_PC_ROT_CW);
#ifdef GAME_TETRIS_FP
				}
#endif
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
					tetris_logic_completeDrop(pLogic,
						tetris_playfield_getRow(pPl) - nPieceRow);
				}
				break;

			// avoid compiler warnings
			default:
				break;
			}
			break;

		// the piece has irrevocably hit the ground
		case TETRIS_PFS_DOCKED:
			// avoid accidentally issued "down" commands
			tetris_input_resetDownKeyRepeat(pIn);

			// remove complete lines (if any)
			tetris_playfield_removeCompleteLines(pPl);

			// let the logic object decide how many points the player gets
			// and whether the level gets changed
			tetris_logic_removedLines(pLogic, tetris_playfield_getRowMask(pPl));
			tetris_input_setLevel(pIn, tetris_logic_getLevel(pLogic));
			break;

		// avoid compiler warnings
		default:
			break;
		}

		// the view updates it state every loop cycle to make changes visible
		tetris_view_update(pView);
	}

	// game is over and we provide the player with her/his results
	tetris_view_showResults(pView);

	// update highscore if it has been beaten
	uint16_t nScore = tetris_logic_getScore(pLogic);
	if (nScore > nHighscore)
	{
		nHighscore = nScore;
		nHighscoreName = tetris_highscore_inputName();
		tetris_logic_saveHighscore(nMode, nHighscore);
		tetris_logic_saveHighscoreName(nMode, nHighscoreName);
	}

	// clean up
#ifdef GAME_BASTET
	if (nMode == 1)
	{
		tetris_bastet_destruct(pBastet);
	}
#endif
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
#ifdef GAME_BASTET
	if (pLogic->nBastet) return;
#endif
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
#ifdef GAME_BASTET
	if (pLogic->nBastet) return;
#endif
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

#ifdef GAME_BASTET
	if (pLogic->nBastet)
	{
		pLogic->nScore += nLines;
		return;
	}
#endif
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


/* Function:        tetris_logic_getHighscoreName
 * Description:     returns the current highscore name
 * Argument pLogic: the logic object we want information from
 * Return value:    the highscore name packed as uint16_t
 */
uint16_t tetris_logic_getHighscoreName(tetris_logic_t *pLogic)
{
	assert(pLogic != NULL);
	return pLogic->nHighscoreName;
}



/* Function:                 tetris_logic_setHighscoreName
 * Description:              set highscore name
 * Argument pLogic:          the logic object we want to modify
 * Argmument nHighscoreName: highscore name
 */
void tetris_logic_setHighscoreName(tetris_logic_t *pLogic,
                               uint16_t nHighscoreName)
{
	assert(pLogic != NULL);
	pLogic->nHighscoreName = nHighscoreName;
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
 * Return value:    number of completed lines as uint16_t
 */
uint16_t tetris_logic_getLines(tetris_logic_t *pLogic)
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
