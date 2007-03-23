#include <inttypes.h>
#include "input.h"
#include "../joystick.h"
#include "../util.h"

tetris_input_command_t tetris_input_getCommand()
{
	tetris_input_command_t cmd = TETRIS_INCMD_NONE;
	static uint8_t i = 0;
	uint8_t nIterations = 10;
	uint8_t nTicks = 100;
	
	for (; i < nIterations; ++i)
	{
		if (JOYISFIRE)
		{
			cmd = TETRIS_INCMD_ROTATE_CLOCKWISE;
			++i;
		}
		else if (JOYISLEFT)
		{
			cmd = TETRIS_INCMD_LEFT;
			++i;
		}
		else if (JOYISRIGHT)
		{
			cmd = TETRIS_INCMD_RIGHT;
			++i;
		}
		else if (JOYISUP)
		{
			cmd = TETRIS_INCMD_UP;
			i = 0;
		}
		else if (JOYISDOWN)
		{
			cmd = TETRIS_INCMD_DOWN;
			i = 0;
		}
		myWait(nTicks);
		if (cmd != TETRIS_INCMD_NONE)
			return cmd;
	}
	
	if (i >= nIterations)
	{
		i = 0;
	}
	
	return TETRIS_INCMD_DOWN;
}
