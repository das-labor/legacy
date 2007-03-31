#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include "input.h"
#include "../joystick.h"
#include "../util.h"

/*****************************
 *  construction/destruction *
 *****************************/

/* Function:     tetris_input_construct
 * Description:  constructs an input object for André's borg
 * Return value: pointer to a newly created input
 */
tetris_input_t *tetris_input_construct()
{
	tetris_input_t *pIn = (tetris_input_t *)malloc(sizeof(tetris_input_t));
	assert(pIn != NULL);
	
	pIn->cmdLast = TETRIS_INCMD_NONE;
	pIn->nLevel = 0;
	pIn->nLoopCycles = 0;
	pIn->nRepeatCount = -TETRIS_INPUT_REPEAT_INITIALDELAY;
	
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

/* Function:     tetris_input_getCommand
 * Description:  retrieves commands from joystick or loop interval
 * Argument pIn: pointer to input object
 * Return value: void
 */
tetris_input_command_t tetris_input_getCommand(tetris_input_t *pIn)
{
	assert (pIn != NULL);

	static uint8_t nCyclesPerLevel[] = TETRIS_INPUT_CYCLESPERLEVEL;
	uint8_t nMaxCycles = nCyclesPerLevel[pIn->nLevel];
	tetris_input_command_t cmdJoystick = TETRIS_INCMD_NONE;
	tetris_input_command_t cmdReturn = TETRIS_INCMD_NONE;

	for (; pIn->nLoopCycles < nMaxCycles; ++pIn->nLoopCycles)
	{
		cmdJoystick = tetris_input_queryJoystick();

		switch (cmdJoystick)
		{
			case TETRIS_INCMD_LEFT:
			case TETRIS_INCMD_RIGHT:
			case TETRIS_INCMD_DOWN:
				// only react if either the current command differs from the last
				// or enough loop cycles have been run on the same command
				// (for key repeat)
				if ((pIn->cmdLast != cmdJoystick) || ((pIn->cmdLast == cmdJoystick)
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

					if (cmdJoystick == TETRIS_INCMD_DOWN)
					{
						// suppress autom. falling if down key is pressed anyway
						pIn->nLoopCycles = 0;
					}
					else
					{
						// right or left moves must not prevent autom. falling
						++pIn->nLoopCycles;
					}

					// update cmdLast and return value
					pIn->cmdLast = cmdReturn = cmdJoystick;
				}
				else
				{
					// if not enough loop cylces have been run we increment the
					// repeat counter and ensure that we continue the loop
					++pIn->nRepeatCount;
					cmdReturn = TETRIS_INCMD_NONE;
				}
				break;

			case TETRIS_INCMD_DROP:
			case TETRIS_INCMD_ROTATE_CLOCKWISE:
			case TETRIS_INCMD_ROTATE_COUNTERCLOCKWISE:
				// no key repeat here
				if (pIn->cmdLast != cmdJoystick)
				{
					pIn->nRepeatCount =  -TETRIS_INPUT_REPEAT_INITIALDELAY;
					if (cmdJoystick == TETRIS_INCMD_DROP)
					{
						// reset autom. falling if player has dropped the piece
						pIn->nLoopCycles = 0;
					}
					else
					{
						// rotations must not prevent autom. falling
						++pIn->nLoopCycles;
					}

					pIn->cmdLast = cmdReturn = cmdJoystick;
				}
				else
				{
					cmdReturn = TETRIS_INCMD_NONE;
				}
				break;
				
			case TETRIS_INCMD_NONE:
				pIn->cmdLast = cmdReturn = TETRIS_INCMD_NONE;
				pIn->nRepeatCount =  -TETRIS_INPUT_REPEAT_INITIALDELAY;
				break;
		}
		
		myWait(TETRIS_INPUT_TICKS);
		if (cmdReturn != TETRIS_INCMD_NONE)
			return cmdReturn;
	}
	
	if (pIn->nLoopCycles >= nMaxCycles)
	{
		pIn->nLoopCycles = 0;

		// in higher levels the key repeat may actually be slower than the
		// falling speed, so if we reach here before we have run enough loop
		// cycles for down key repeat, we reset the repeat counter to ensure
		// smooth falling movements
		if (pIn->cmdLast == TETRIS_INCMD_DOWN)
		{
			pIn->nRepeatCount =  -TETRIS_INPUT_REPEAT_INITIALDELAY;
		}
	}

	return TETRIS_INCMD_DOWN;
}


/* Function:      tetris_input_setLevel
 * Description:   modifies time interval of input events
 * Argument pIn:  pointer to input structure
 * Argument nLvl: desired level (0 <= nLvl <= TETRIS_INPUT_LEVELS - 1)
 * Return value:  void
 */
void tetris_input_setLevel(tetris_input_t *pIn,
                           uint8_t nLvl)
{
	assert(pIn != NULL);
	assert(nLvl <= TETRIS_INPUT_LEVELS - 1);
	pIn->nLevel = nLvl;
}


/***************************
 * non-interface functions *
 ***************************/

/* Function:     tetris_input_queryJoystick
 * Description:  translates joystick movements into tetris_input_command_t
 * Return value: void
 */
tetris_input_command_t tetris_input_queryJoystick()
{
	if (JOYISFIRE)
	{
		return TETRIS_INCMD_DROP;
	}
	else if (JOYISLEFT)
	{
		return TETRIS_INCMD_LEFT;
	}
	else if (JOYISRIGHT)
	{
		return TETRIS_INCMD_RIGHT;
	}
	else if (JOYISUP)
	{
		return TETRIS_INCMD_ROTATE_CLOCKWISE;
	}
	else if (JOYISDOWN)
	{
		return TETRIS_INCMD_DOWN;
	}
	else
	{
		return TETRIS_INCMD_NONE;
	}
}
