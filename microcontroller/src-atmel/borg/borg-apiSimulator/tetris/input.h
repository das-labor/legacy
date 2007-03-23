#ifndef INPUT_H_
#define INPUT_H_

#define TETRIS_INPUT_REPEATCOUNT 10
#define TETRIS_INPUT_TICKS 10
#define TETRIS_INPUT_ITERATIONS_PER_LEVEL {100, 90, 80, 70, 60, 50, 40, 30, 20, 10}

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

typedef struct tetris_input_t
{
	uint8_t nLevel;
	uint8_t nPass;
	uint8_t nRepeatCount;
	tetris_input_command_t lastCmd;
}
tetris_input_t;


/****************************
 * construction/destruction *
 ****************************/

/* Function:     tetris_input_construct
 * Description:  constructs an input structure for André's borg
 * Return value: pointer to a newly created input
 */
tetris_input_t *tetris_input_construct();


/* Function:     tetris_input_destruct
 * Description:  destructs an input structure
 * Argument pIn: pointer to the input to be destructed
 * Return value: void
 */
void tetris_input_destruct(tetris_input_t *pIn);


/***************************
 * input related functions *
 ***************************/

/* Function:     tetris_input_getCommand
 * Description:  retrieves commands from joystick or loop interval
 * Argument pIn: pointer to input structure
 * Return value: void
 */
tetris_input_command_t tetris_input_getCommand(tetris_input_t *pIn);

/* Function:      tetris_input_setLevel
 * Description:   modifies time interval of input events
 * Argument pIn:  pointer to input structure
 * Argument nLvl: desired level (0 <= nLvl <= 9);
 * Return value:  void
 */
void tetris_input_setLevel(tetris_input_t *pIn,
                                             uint8_t nLvl);

#endif /*INPUT_H_*/
