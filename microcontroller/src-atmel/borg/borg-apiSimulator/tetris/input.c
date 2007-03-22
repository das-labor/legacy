#include <inttypes.h>
#include "input.h"
#include "../joystick.h"
#include "../util.h"

tetris_input_command_t tetris_input_getCommand()
{
	tetris_input_command_t cmd = TETRIS_INCMD_NONE;
	uint8_t i;
	uint8_t nIterations = 50;
	uint8_t nTicks = 20;
	
	for (i = 0; i < nIterations; ++i)
	{
		if (JOYISFIRE)
			cmd = TETRIS_INCMD_ROTATE_CLOCKWISE;
		if (JOYISLEFT)
			cmd = TETRIS_INCMD_LEFT;
		if (JOYISRIGHT)
			cmd = TETRIS_INCMD_RIGHT;
		if (JOYISDOWN)
			cmd = TETRIS_INCMD_DOWN;
		if (JOYISUP)
			cmd = TETRIS_INCMD_UP;
		myWait(nTicks);
		if (cmd != TETRIS_INCMD_NONE)
			return cmd;
	}
	
	return TETRIS_INCMD_DOWN;
}
