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
	
	pIn->lastCmd = TETRIS_INCMD_NONE;
	pIn->nLevel = 0;
	pIn->nPass = 0;
	
	return pIn;
}


/* Function:     tetris_input_destruct
 * Description:  destructs an input structure
 * Argument pIn: pointer to the input to be destructed
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
				++pIn->nPass;
			}
			else
			{
				cmd = TETRIS_INCMD_NONE;
			}
		}
		else if (JOYISLEFT)
		{
			if (pIn->lastCmd != TETRIS_INCMD_LEFT)
			{
				pIn->lastCmd = cmd = TETRIS_INCMD_LEFT;
				++pIn->nPass;
			}
			else
			{
				cmd = TETRIS_INCMD_NONE;
			}
		}
		else if (JOYISRIGHT)
		{
			if (pIn->lastCmd != TETRIS_INCMD_RIGHT)
			{
				pIn->lastCmd = cmd = TETRIS_INCMD_RIGHT;
				++pIn->nPass;
			}
			else
			{
				cmd = TETRIS_INCMD_NONE;
			}
		}
		else if (JOYISUP)
		{
			if (pIn->lastCmd != TETRIS_INCMD_UP)
			{
				pIn->lastCmd = cmd = TETRIS_INCMD_UP;
				pIn->nPass = 0;
			}
			else
			{
				cmd = TETRIS_INCMD_NONE;
			}
		}
		else if (JOYISDOWN)
		{
			if (pIn->lastCmd != TETRIS_INCMD_DOWN)
			{
				pIn->lastCmd = cmd = TETRIS_INCMD_DOWN;
				pIn->nPass = 0;
			}
			else
			{
				cmd = TETRIS_INCMD_NONE;
			}
		}
		else
		{
			pIn->lastCmd = cmd = TETRIS_INCMD_NONE;
		}
		
		myWait(TETRIS_INPUT_TICKS);
		if (cmd != TETRIS_INCMD_NONE)
			return cmd;
	}
	
	if (pIn->nPass >= nIterations)
	{
		pIn->nPass = 0;
	}

	return TETRIS_INCMD_DOWN;
}
