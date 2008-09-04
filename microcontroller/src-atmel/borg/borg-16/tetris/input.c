#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>
#include "../joystick.h"
#include "../util.h"
#include "input.h"

/* - the API simulator and the real API have different named wait functions
 * - the macro PM helps in reading values from PROGMEM on the AVR arch
 */
#ifdef __AVR__
	#include <avr/pgmspace.h>
	#define WAIT(ms) wait(ms)
	#define PM(value) pgm_read_word(&value)
#else
	#define PROGMEM
	#define WAIT(ms) myWait(ms)
	#define PM(value) (value)
#endif


/***********
 * defines *
 ***********/

// amount of milliseconds that each loop cycle waits
#define TETRIS_INPUT_TICKS 5

// amount of milliseconds the input is ignored after the pause combo has been
// pressed, since it is difficult to release all buttons simultaneously
#define TETRIS_INPUT_PAUSE_TICKS 100

// amount of allowed loop cycles while in pause mode so that the game
// automatically continues after a minute
#define TETRIS_INPUT_PAUSE_CYCLES 12000

// minimum of cycles in gliding mode
#define TETRIS_INPUT_GLIDE_CYCLES 75

// here you can adjust the delays (in loop cycles) for key repeat
#define TETRIS_INPUT_REPEAT_INITIALDELAY 40
#define TETRIS_INPUT_REPEAT_DELAY 10

// Here you can adjust the amount of loop cycles a command is ignored after
// its button has been released (to reduce joystick chatter)
#define TETRIS_INPUT_CHATTER_TICKS_ROT_CW  24
#define TETRIS_INPUT_CHATTER_TICKS_ROT_CCW 24
#define TETRIS_INPUT_CHATTER_TICKS_LEFT    12
#define TETRIS_INPUT_CHATTER_TICKS_RIGHT   12
#define TETRIS_INPUT_CHATTER_TICKS_DOWN    12
#define TETRIS_INPUT_CHATTER_TICKS_DROP    24


/***************************
 * non-interface functions *
 ***************************/

/* Function:     tetris_input_chatterProtect;
 * Description:  sets an ignore counter to a command specific value if it is 0
 * Argument pIn: pointer to an input object
 * Argument cmd: the command whose counter should be set
 * Return value: void
 */
void tetris_input_chatterProtect (tetris_input_t *pIn,
                                  tetris_input_command_t cmd)
{
	// never exceed the index
	assert(cmd < TETRIS_INCMD_NONE);

	// amount of loop cycles a command is ignored after its button has been
	// released (every command has its own counter)
	const static uint8_t nInitialIgnoreValue[TETRIS_INCMD_NONE] PROGMEM =
	{
		TETRIS_INPUT_CHATTER_TICKS_ROT_CW,
		TETRIS_INPUT_CHATTER_TICKS_ROT_CCW,
		TETRIS_INPUT_CHATTER_TICKS_LEFT,
		TETRIS_INPUT_CHATTER_TICKS_RIGHT,
		TETRIS_INPUT_CHATTER_TICKS_DOWN,
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
				PM(nInitialIgnoreValue[TETRIS_INCMD_ROT_CW]);
			pIn->nIgnoreCmdCounter[TETRIS_INCMD_DOWN] =
				PM(nInitialIgnoreValue[TETRIS_INCMD_DOWN]);
		}
	}

	// The ignore counter of TETRIS_INCMD_PAUSE is either set to the counter
	// value of TETRIS_INCMD_ROT_CW or TETRIS_INCMD_DOWN (whichever is higher).
	if ((cmd == TETRIS_INCMD_ROT_CW) || (cmd == TETRIS_INCMD_DOWN))
	{
		// helper variables (which the compiler hopefully optimizes away)
		uint8_t nRotCw = pIn->nIgnoreCmdCounter[TETRIS_INCMD_ROT_CW];
		uint8_t nDown = pIn->nIgnoreCmdCounter[TETRIS_INCMD_DOWN];

		pIn->nIgnoreCmdCounter[TETRIS_INCMD_PAUSE] =
				(nRotCw > nDown ? nRotCw : nDown);
	}
}


/* Function:     tetris_input_queryJoystick
 * Description:  translates joystick movements into tetris_input_command_t
 * Argument pIn: pointer to an input object
 * Return value: see definition of tetris_input_command_t
 */
tetris_input_command_t tetris_input_queryJoystick()
{
	tetris_input_command_t cmdReturn;

	if (JOYISFIRE)
	{
		cmdReturn = TETRIS_INCMD_DROP;
	}
	else if (JOYISLEFT)
	{
		cmdReturn = TETRIS_INCMD_LEFT;
	}
	else if (JOYISRIGHT)
	{
		cmdReturn = TETRIS_INCMD_RIGHT;
	}
	else if (JOYISUP && JOYISDOWN)
	{
		cmdReturn = TETRIS_INCMD_PAUSE;
		WAIT(TETRIS_INPUT_PAUSE_TICKS);
	}
	else if (JOYISDOWN)
	{
		cmdReturn = TETRIS_INCMD_DOWN;
	}
	else if (JOYISUP)
	{
		cmdReturn = TETRIS_INCMD_ROT_CW;
	}
	else
	{
		cmdReturn = TETRIS_INCMD_NONE;
	}

	return cmdReturn;
}


/*****************************
 *  construction/destruction *
 *****************************/

/* Function:     tetris_input_construct
 * Description:  constructs an input object for André's borg
 * Return value: pointer to a newly created input object
 */
tetris_input_t *tetris_input_construct()
{
	tetris_input_t *pIn = (tetris_input_t *)malloc(sizeof(tetris_input_t));
	assert(pIn != NULL);

	pIn->cmdLast = TETRIS_INCMD_NONE;
	pIn->nLevel = 0xFF;
	tetris_input_setLevel(pIn, 0);
	pIn->nLoopCycles = 0;
	pIn->nRepeatCount = -TETRIS_INPUT_REPEAT_INITIALDELAY;
	pIn->nPauseCount = 0;
	memset(pIn->nIgnoreCmdCounter, 0, TETRIS_INCMD_NONE);

	return pIn;
}


/* Function:     tetris_input_destruct
 * Description:  destructs an input structure
 * Argument pIn: pointer to the input object which should to be destructed
 * Return value: void
 */
void tetris_input_destruct(tetris_input_t *pIn)
{
	assert(pIn != NULL);
	free(pIn);
}


/***************************
 * input related functions *
 ***************************/

/* Function:       retris_input_getCommand
 * Description:    retrieves commands from joystick or loop interval
 * Argument pIn:   pointer to an input object
 * Argument nPace: falling pace (see definition of tetris_input_pace_t)
 * Return value:   see definition of tetris_input_command_t
 */
tetris_input_command_t tetris_input_getCommand(tetris_input_t *pIn,
                                               tetris_input_pace_t nPace)
{
	assert (pIn != NULL);

	// holds the translated command value of the joystick
	tetris_input_command_t cmdJoystick = TETRIS_INCMD_NONE;

	// this variable both serves as the return value and as a flag for not
	// leaving the function as long as its value is TETRIS_INCMD_NONE
	tetris_input_command_t cmdReturn = TETRIS_INCMD_NONE;

	uint8_t nMaxCycles;

	// if the piece is gliding we grant the player a reasonable amount of time
	// to make the game more controllable at higher falling speeds
	if ((nPace == TETRIS_INPACE_GLIDING) &&
			(pIn->nMaxCycles < TETRIS_INPUT_GLIDE_CYCLES))
	{
		nMaxCycles = TETRIS_INPUT_GLIDE_CYCLES;
	}
	else
	{
		nMaxCycles = pIn->nMaxCycles;
	}

	while (pIn->nLoopCycles < nMaxCycles)
	{
		cmdJoystick = tetris_input_queryJoystick();

		// only obey current command if it is not considered as chattering
		if (((cmdJoystick < TETRIS_INCMD_NONE) ?
			pIn->nIgnoreCmdCounter[cmdJoystick] : 0) == 0)
		{
			switch (cmdJoystick)
			{
			case TETRIS_INCMD_LEFT:
			case TETRIS_INCMD_RIGHT:
			case TETRIS_INCMD_DOWN:
				// only react if either the current command differs from the
				// last one or enough loop cycles have been run on the same
				// command (for key repeat)
				if ((pIn->cmdLast != cmdJoystick)
					|| ((pIn->cmdLast == cmdJoystick)
					&& (pIn->nRepeatCount >= TETRIS_INPUT_REPEAT_DELAY)))
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
				else
				{
					// if not enough loop cycles have been run we increment the
					// repeat counter, ensure that we continue the loop and
					// keep the key repeat functioning
					++pIn->nRepeatCount;
					cmdReturn = TETRIS_INCMD_NONE;
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
				else
				{
					// if we reach here the command is ignored
					cmdReturn = TETRIS_INCMD_NONE;
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
				// consecutive pause commands should not cause the loop to leave
				else
				{
					cmdReturn = TETRIS_INCMD_NONE;
				}
				break;

			case TETRIS_INCMD_NONE:
				// chatter protection
				if (pIn->cmdLast != TETRIS_INCMD_NONE)
				{
					tetris_input_chatterProtect(pIn, pIn->cmdLast);
				}

				// If the game is paused (last command was TETRIS_INCMD_PAUSE)
				// we ensure that the variable which holds that last command
				// isn't touched. We use this as a flag so that the loop cycle
				// counter doesn't get incremented.
				// We count the number of pause cycles, though. If enough pause
				// cycles have been run, we enforce the continuation of the game.
				if ((pIn->cmdLast != TETRIS_INCMD_PAUSE) ||
					(++pIn->nPauseCount > TETRIS_INPUT_PAUSE_CYCLES))
				{
						pIn->cmdLast = TETRIS_INCMD_NONE;
				}

				// reset repeat counter
				pIn->nRepeatCount = -TETRIS_INPUT_REPEAT_INITIALDELAY;

				// using cmdReturn as a flag for not leaving the loop
				cmdReturn = TETRIS_INCMD_NONE;
				break;

			default:
				break;
			}
		}
		// current command is considered as chattering
		else
		{
			pIn->cmdLast = cmdReturn = TETRIS_INCMD_NONE;
		}

		// decrement all ignore counters
		for (int nIgnoreIndex = 0; nIgnoreIndex < TETRIS_INCMD_NONE; ++nIgnoreIndex)
		{
			if (pIn->nIgnoreCmdCounter[nIgnoreIndex] != 0)
			{
				--pIn->nIgnoreCmdCounter[nIgnoreIndex];
			}
		}

		// reset automatic falling if the player has dropped a piece
		if ((cmdReturn == TETRIS_INCMD_DOWN)
			|| (cmdReturn == TETRIS_INCMD_DROP))
		{
			pIn->nLoopCycles = 0;
		}
		// otherwise ensure automatic falling (unless the game is running)
		else if ((cmdReturn != TETRIS_INCMD_PAUSE) &&
				!((cmdReturn == TETRIS_INCMD_NONE) &&
						(pIn->cmdLast == TETRIS_INCMD_PAUSE)))
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


/* Function:      tetris_input_setLevel
 * Description:   modifies time interval of input events
 * Argument pIn:  pointer to an input object
 * Argument nLvl: desired level (0 <= nLvl <= TETRIS_INPUT_LEVELS - 1)
 * Return value:  void
 */
void tetris_input_setLevel(tetris_input_t *pIn,
                           uint8_t nLvl)
{
	assert(pIn != NULL);
	assert(nLvl <= TETRIS_INPUT_LEVELS - 1);
	if (pIn->nLevel != nLvl)
	{
		pIn->nLevel = nLvl;
		pIn->nMaxCycles = 400 / (nLvl + 2);
	}
}
