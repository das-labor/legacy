/**
 * \addtogroup tetris
 * @{
 */

/**
 * @file input.c
 * @brief Implementation of the input routines of Tetris.
 * @author Christian Kroll
 */


#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "../../compat/pgmspace.h"
#include "../../joystick/joystick.h"
#include "../../util.h"
#include "bearing.h"
#include "input.h"


/***********
 * defines *
 ***********/

#define WAIT(ms) wait(ms)
#define PM(value) pgm_read_byte(&value)

/** Number of milliseconds that each loop cycle waits. */
#define TETRIS_INPUT_TICKS 5

/**
 * Number of milliseconds the input is ignored after the pause key combination
 * has been pressed, since it is difficult to release all buttons
 * simultaneously.
 */
#define TETRIS_INPUT_PAUSE_TICKS 100

/**
 * Number of allowed loop cycles while in pause mode so that the game
 * automatically continues after five minutes.
 */
#define TETRIS_INPUT_PAUSE_CYCLES 60000

/** initial delay (in loop cycles) for key repeat */
#define TETRIS_INPUT_REPEAT_INITIALDELAY 35
/** delay (in loop cycles) for key repeat */
#define TETRIS_INPUT_REPEAT_DELAY 5

/** Number of loop cyles the left button is ignored */
#define TETRIS_INPUT_CHATTER_TICKS_LEFT    12
/** Number of loop cyles the right button is ignored */
#define TETRIS_INPUT_CHATTER_TICKS_RIGHT   12
/** Number of loop cyles the down button is ignored */
#define TETRIS_INPUT_CHATTER_TICKS_DOWN    12
/** Number of loop cyles the clockwise rotation button is ignored */
#define TETRIS_INPUT_CHATTER_TICKS_ROT_CW  24
/** Number of loop cyles the counter clockwise rotation button is ignored */
#define TETRIS_INPUT_CHATTER_TICKS_ROT_CCW 24
/** Number of loop cyles the drop button is ignored */
#define TETRIS_INPUT_CHATTER_TICKS_DROP    36

/** wait cycles per level (array of uint8_t) */
#define TETRIS_INPUT_LVL_CYCLES 200, 133, 100, 80, 66, 57, 50, 44, 40, 36, 33, \
	30, 28, 26, 25, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9


/***************************
 * non-interface functions *
 ***************************/

/**
 * sets an ignore counter to a command specific value if it is 0
 * @param pIn pointer to an input object
 * @param cmd the command whose counter should be set
 */
static void tetris_input_chatterProtect(tetris_input_t *pIn,
                                        tetris_input_command_t cmd)
{
	// never exceed the index
	assert(cmd < TETRIS_INCMD_NONE);

	// number of loop cycles a command is ignored after its button has been
	// released (every command has its own counter)
	static uint8_t const nInitialIgnoreValue[TETRIS_INCMD_NONE] PROGMEM =
	{
		TETRIS_INPUT_CHATTER_TICKS_LEFT,
		TETRIS_INPUT_CHATTER_TICKS_RIGHT,
		TETRIS_INPUT_CHATTER_TICKS_DOWN,
		TETRIS_INPUT_CHATTER_TICKS_ROT_CW,
		TETRIS_INPUT_CHATTER_TICKS_ROT_CCW,
		TETRIS_INPUT_CHATTER_TICKS_DROP,
		0, // TETRIS_INCMD_GRAVITY (irrelevant because it doesn't have a button)
		0  // TETRIS_INCMD_PAUSE (is a combination of ROT_CW and DOWN)
	};

	// setting ignore counter according to the predefined array
	if (pIn->nIgnoreCmdCounter[cmd] == 0)
	{
		// if the command isn't TETRIS_INCMD_PAUSE, setting the ignore counter
		// is straight forward
		if (cmd != TETRIS_INCMD_PAUSE)
		{
			pIn->nIgnoreCmdCounter[cmd] = PM(nInitialIgnoreValue[cmd]);
		}
		// TETRIS_INCMD_PAUSE is issued via a combination of the buttons for
		// TETRIS_INCMD_ROT_CW and TETRIS_INCMD_DOWN, so we must set their
		// ignore counters
		else
		{
			pIn->nIgnoreCmdCounter[TETRIS_INCMD_ROT_CW] =
				TETRIS_INPUT_CHATTER_TICKS_ROT_CW;
			pIn->nIgnoreCmdCounter[TETRIS_INCMD_DOWN] =
				TETRIS_INPUT_CHATTER_TICKS_DOWN;
		}
	}

	// The ignore counter of TETRIS_INCMD_PAUSE is either set to the counter
	// value of TETRIS_INCMD_ROT_CW or TETRIS_INCMD_DOWN (whichever is higher).
	if ((cmd == TETRIS_INCMD_ROT_CW) || (cmd == TETRIS_INCMD_DOWN))
	{
		// helper variables (which the compiler hopefully optimizes away)
		uint8_t const nRotCw = pIn->nIgnoreCmdCounter[TETRIS_INCMD_ROT_CW];
		uint8_t const nDown = pIn->nIgnoreCmdCounter[TETRIS_INCMD_DOWN];

		pIn->nIgnoreCmdCounter[TETRIS_INCMD_PAUSE] =
				nRotCw > nDown ? nRotCw : nDown;
	}
}


/**
 * remaps tetris commands according to current bearing of the bucket
 * @param nBearing bearing of the bucket
 * @param nCmd command which has to be mapped
 * @return mapped tetris command
 * @see tetris_input_command_t
 */
static
tetris_input_command_t tetris_input_mapCommand(tetris_bearing_t nBearing,
                                               tetris_input_command_t nCmd)
{
	return (nCmd < TETRIS_INCMD_ROT_CCW) ? (nCmd - nBearing + 4u) % 4u : nCmd;
}


/**
 * translates joystick movements into tetris commands
 * @return interpreted joystick command
 * @see tetris_input_command_t
 */
static tetris_input_command_t tetris_input_queryJoystick(tetris_input_t *pIn)
{
	// map port input to a tetris command
	tetris_input_command_t cmdJoystick;
	if (JOYISFIRE)
	{
		cmdJoystick = TETRIS_INCMD_DROP;
	}
	else if (JOYISLEFT)
	{
		cmdJoystick = TETRIS_INCMD_LEFT;
	}
	else if (JOYISRIGHT)
	{
		cmdJoystick = TETRIS_INCMD_RIGHT;
	}
	else if (JOYISUP && JOYISDOWN)
	{
		cmdJoystick = TETRIS_INCMD_PAUSE;
		WAIT(TETRIS_INPUT_PAUSE_TICKS);
	}
	else if (JOYISDOWN)
	{
		cmdJoystick = TETRIS_INCMD_DOWN;
	}
	else if (JOYISUP)
	{
		cmdJoystick = TETRIS_INCMD_ROT_CW;
	}
	else
	{
		cmdJoystick = TETRIS_INCMD_NONE;
	}

	// decrement all ignore counters
	for (uint8_t nIgnIndex = 0; nIgnIndex < TETRIS_INCMD_NONE; ++nIgnIndex)
	{
		if (pIn->nIgnoreCmdCounter[nIgnIndex] != 0)
		{
			--pIn->nIgnoreCmdCounter[nIgnIndex];
		}
	}

	// chatter protection
	if (cmdJoystick < TETRIS_INCMD_NONE)
	{
		if (pIn->nIgnoreCmdCounter[cmdJoystick] == 0)
		{
			tetris_input_chatterProtect(pIn, cmdJoystick);
		}
		else if (cmdJoystick != pIn->cmdRawLast)
		{
			cmdJoystick = TETRIS_INCMD_NONE;
		}
	}

	// memorize current command (for detecting prolonged key presses)
	pIn->cmdRawLast = cmdJoystick;

	// remap command according to current bearing
	tetris_input_command_t cmdReturn =
			tetris_input_mapCommand(pIn->nBearing, cmdJoystick);

	return cmdReturn;
}


/****************************
 * construction/destruction *
 ****************************/

tetris_input_t *tetris_input_construct(void)
{
	tetris_input_t *pIn = (tetris_input_t *)malloc(sizeof(tetris_input_t));
	assert(pIn != NULL);

	pIn->cmdRawLast = pIn->cmdLast = TETRIS_INCMD_NONE;
	pIn->nBearing = TETRIS_BEARING_0;
	tetris_input_setLevel(pIn, 0);
	pIn->nLoopCycles = 0;
	pIn->nRepeatCount = -TETRIS_INPUT_REPEAT_INITIALDELAY;
	pIn->nPauseCount = 0;
	memset(pIn->nIgnoreCmdCounter, 0, TETRIS_INCMD_NONE);

	return pIn;
}


/***************************
 * input related functions *
 ***************************/

tetris_input_command_t tetris_input_getCommand(tetris_input_t *pIn,
                                               tetris_input_pace_t nPace)
{
	assert (pIn != NULL);

	// this variable both serves as the return value and as a flag for not
	// leaving the function as long as its value is TETRIS_INCMD_NONE
	tetris_input_command_t cmdReturn = TETRIS_INCMD_NONE;

	// if the piece is gliding we grant the player a reasonable amount of time
	// to make the game more controllable at higher falling speeds
	uint8_t nMaxCycles = pIn->nMaxCycles > nPace ? pIn->nMaxCycles : nPace;

	while (pIn->nLoopCycles < nMaxCycles)
	{
		// holds the (mapped) command value of the joystick
		tetris_input_command_t cmdJoystick = tetris_input_queryJoystick(pIn);

		switch (cmdJoystick)
		{
		case TETRIS_INCMD_LEFT:
		case TETRIS_INCMD_RIGHT:
		case TETRIS_INCMD_DOWN:
			// only react if either the current command differs from the last
			// one or enough loop cycles have been run on the same command (for
			// key repeat)
			if (pIn->cmdLast != cmdJoystick ||
					pIn->nRepeatCount++ >= TETRIS_INPUT_REPEAT_DELAY)
			{
				// reset repeat counter
				if (pIn->cmdLast != cmdJoystick)
				{
					// different command: we set an extra initial delay
					pIn->nRepeatCount = -TETRIS_INPUT_REPEAT_INITIALDELAY;
				}
				else
				{
					// same command: there's no extra initial delay
					pIn->nRepeatCount = 0;
				}

				// update cmdLast and return value
				pIn->cmdLast = cmdReturn = cmdJoystick;
			}
			break;

		case TETRIS_INCMD_DROP:
		case TETRIS_INCMD_ROT_CW:
		case TETRIS_INCMD_ROT_CCW:
			// no key repeat here
			if (pIn->cmdLast != cmdJoystick)
			{
				pIn->cmdLast = cmdReturn = cmdJoystick;
			}
			break;

		case TETRIS_INCMD_PAUSE:
			// if this is an initial pause command, make sure that the logic
			// module is informed about it
			if (pIn->cmdLast != TETRIS_INCMD_PAUSE)
			{
				pIn->cmdLast = cmdReturn = cmdJoystick;
				pIn->nPauseCount = 0;
			}
			break;

		case TETRIS_INCMD_NONE:
			// If the game is paused (last command was TETRIS_INCMD_PAUSE) we
			// ensure that the variable which holds that last command isn't
			// touched. We use this as a flag so that the loop cycle counter
			// doesn't get incremented. We count the number of pause cycles,
			// though. If enough cycles have been run, we enforce the
			// continuation of the game.
			if ((pIn->cmdLast != TETRIS_INCMD_PAUSE) ||
				(++pIn->nPauseCount > TETRIS_INPUT_PAUSE_CYCLES))
			{
					pIn->cmdLast = TETRIS_INCMD_NONE;
			}

			// reset repeat counter
			pIn->nRepeatCount = -TETRIS_INPUT_REPEAT_INITIALDELAY;
			break;

		default:
			break;
		}

		// reset automatic falling if the player has dropped a piece
		if (cmdReturn == TETRIS_INCMD_DOWN || cmdReturn == TETRIS_INCMD_DROP)
		{
			pIn->nLoopCycles = 0;
		}
		// otherwise ensure automatic falling (unless the game is running)
		else if (pIn->cmdLast != TETRIS_INCMD_PAUSE)
		{
			++pIn->nLoopCycles;
		}

		WAIT(TETRIS_INPUT_TICKS);
		if (cmdReturn != TETRIS_INCMD_NONE)
		{
			return cmdReturn;
		}
	}

	// since we have left the loop we reset the cycle counter
	pIn->nLoopCycles = 0;

	return TETRIS_INCMD_GRAVITY;
}


void tetris_input_setLevel(tetris_input_t *pIn,
                           uint8_t nLvl)
{
	assert(pIn != NULL);
	assert(nLvl <= TETRIS_INPUT_LEVELS - 1);

	static uint8_t const nCycles[] PROGMEM = {TETRIS_INPUT_LVL_CYCLES};
	pIn->nMaxCycles = PM(nCycles[nLvl]);
}


void tetris_input_resetDownKeyRepeat(tetris_input_t *pIn)
{
	assert(pIn != NULL);
	if (pIn->cmdLast == TETRIS_INCMD_DOWN)
	{
		pIn->nRepeatCount = -TETRIS_INPUT_REPEAT_INITIALDELAY;
	}
}


void tetris_input_setBearing(tetris_input_t *pIn,
                             tetris_bearing_t nBearing)
{
	pIn->nBearing = nBearing;

	// avoid weird key repeating effects because the currently pressed button
	// changes its meaning as soon as the bearing changes
	pIn->cmdLast = tetris_input_mapCommand(pIn->nBearing, pIn->cmdRawLast);
}

/*@}*/
