#ifndef INPUT_H_
#define INPUT_H_

typedef enum tetris_input_command_t
{
	TETRIS_INCMD_NONE,
	TETRIS_INCMD_ROTATE_CLOCKWISE,
	TETRIS_INCMD_ROTATE_COUNTERCLOCKWISE,
	TETRIS_INCMD_LEFT,
	TETRIS_INCMD_RIGHT,
	TETRIS_INCMD_DOWN,
	TETRIS_INCMD_UP	
}
tetris_input_command_t;

tetris_input_command_t tetris_input_getCommand();

#endif /*INPUT_H_*/
