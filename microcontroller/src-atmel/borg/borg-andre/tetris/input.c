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
 * Description:  constructs an input structure for André's borg
 * Return value: pointer to a newly created input
 */
tetris_input_t *tetris_input_construct()
{
	tetris_input_t *pIn = (tetris_input_t *)malloc(sizeof(tetris_input_t));
	assert(pIn != NULL);

	waitForFire = 0;
		
	pIn->lastCmd = TETRIS_INCMD_NONE;
	pIn->nLevel = 0;
	pIn->nPass = 0;
	pIn->nRepeatCount = 0;
	
	return pIn;
}


/* Function:     tetris_input_destruct
 * Description:  destructs an input structure
 * Argument pIn: pointer to the input to be destructed
 * Return value: void
 */
void tetris_input_destruct(tetris_input_t *pIn)
{
	waitForFire = 1;
	assert(pIn != NULL);
	free(pIn);
}


/***************************
 * input related functions *
 ***************************/

/* Function:     tetris_input_getCommand
 * Description:  retrieves commands from joystick or loop interval
 * Argument pIn: pointer to input structure
 * Return value: void
 */
tetris_input_command_t tetris_input_getCommand(tetris_input_t *pIn)
{
	assert (pIn != NULL);
	
	static uint8_t nInterationsPerLevel[] = TETRIS_INPUT_ITERATIONS_PER_LEVEL;
	uint8_t nIterations = nInterationsPerLevel[pIn->nLevel];
	tetris_input_command_t cmd = TETRIS_INCMD_NONE;
	
	for (; pIn->nPass < nIterations; ++pIn->nPass)
	{
		if (JOYISFIRE)
		{
			if (pIn->lastCmd != TETRIS_INCMD_ROTATE_CLOCKWISE)
			{
				pIn->lastCmd = cmd = TETRIS_INCMD_ROTATE_CLOCKWISE;
				pIn->nRepeatCount = 0;
				++pIn->nPass;
			}
			else
			{
				cmd = TETRIS_INCMD_NONE;
			}
		}
		else if (JOYISDOWN) /* is really left */
		{
			if ((pIn->lastCmd != TETRIS_INCMD_LEFT) ||
				((pIn->lastCmd == TETRIS_INCMD_LEFT) &&
				(pIn->nRepeatCount >= TETRIS_INPUT_REPEATCOUNT)))
			{
				pIn->lastCmd = cmd = TETRIS_INCMD_LEFT;
				pIn->nRepeatCount = 0;
				++pIn->nPass;
			}
			else
			{
				cmd = TETRIS_INCMD_NONE;
				++pIn->nRepeatCount;
			}
		}
		else if (JOYISUP) /* is really right */
		{
			if ((pIn->lastCmd != TETRIS_INCMD_RIGHT) ||
				((pIn->lastCmd == TETRIS_INCMD_RIGHT) &&
				(pIn->nRepeatCount >= TETRIS_INPUT_REPEATCOUNT)))
			{
				pIn->lastCmd = cmd = TETRIS_INCMD_RIGHT;
				pIn->nRepeatCount = 0;
				++pIn->nPass;
			}
			else
			{
				cmd = TETRIS_INCMD_NONE;
				++pIn->nRepeatCount;
			}
		}
		else if (JOYISRIGHT) /* is really up */
		{
			if (pIn->lastCmd != TETRIS_INCMD_UP)
			{
				pIn->lastCmd = cmd = TETRIS_INCMD_UP;
				pIn->nRepeatCount = 0;
				pIn->nPass = 0;
			}
			else
			{
				cmd = TETRIS_INCMD_NONE;
			}
		}
		else if (JOYISLEFT) /* is really down */
		{
			if ((pIn->lastCmd != TETRIS_INCMD_DOWN) ||
				((pIn->lastCmd == TETRIS_INCMD_DOWN) &&
				(pIn->nRepeatCount >= TETRIS_INPUT_REPEATCOUNT)))
			{
				pIn->lastCmd = cmd = TETRIS_INCMD_DOWN;
				pIn->nRepeatCount = 0;
				pIn->nPass = 0;
			}
			else
			{
				cmd = TETRIS_INCMD_NONE;
				++pIn->nRepeatCount;
			}
		}
		else
		{
			pIn->lastCmd = cmd = TETRIS_INCMD_NONE;
			pIn->nRepeatCount = 0;
		}
		
		wait(TETRIS_INPUT_TICKS);
		if (cmd != TETRIS_INCMD_NONE)
			return cmd;
	}
	
	if (pIn->nPass >= nIterations)
	{
		pIn->nPass = 0;
	}

	return TETRIS_INCMD_DOWN;
}

/* Function:      tetris_input_setLevel
 * Description:   modifies time interval of input events
 * Argument pIn:  pointer to input structure
 * Argument nLvl: desired level (0 <= nLvl <= 9);
 * Return value:  void
 */
void tetris_input_setLevel(tetris_input_t *pIn,
                           uint8_t nLvl)
{
	assert(pIn != NULL);
	assert(nLvl <= 9);
	pIn->nLevel = nLvl;
}
