#ifndef INPUT_H_
#define INPUT_H_

/***********
 * defines *
 ***********/

// amount of micro seconds that each loop cycle waits
#define TETRIS_INPUT_TICKS 10


// number of levels
#define TETRIS_INPUT_LEVELS 20
// amount of loop cycles for 10 levels until a piece will be moved down
#define TETRIS_INPUT_CYCLESPERLEVEL {100, 90, 81, 73, 66, 59, 53, 48, 43, 39, \
	35, 31, 28, 25, 23, 21, 19, 17, 15, 14}

// here you can adjust the delays (in loop cycles) for key repeat
#define TETRIS_INPUT_REPEAT_INITIALDELAY 20
#define TETRIS_INPUT_REPEAT_DELAY 5


/*********
 * types *
 *********/
 
typedef enum tetris_input_command_t
{
	TETRIS_INCMD_NONE,
	TETRIS_INCMD_ROTATE_CLOCKWISE,
	TETRIS_INCMD_ROTATE_COUNTERCLOCKWISE,
	TETRIS_INCMD_LEFT,
	TETRIS_INCMD_RIGHT,
	TETRIS_INCMD_DOWN,
	TETRIS_INCMD_DROP	
}
tetris_input_command_t;


typedef struct tetris_input_t
{
	uint8_t nLevel;                 // current level
	
	uint8_t nLoopCycles;            // tracks how many loop cycles have been
	                                // done since last forced piece movement
	                                
	int8_t nRepeatCount;            // tracks how many loop cycles have been
									// done since last reset of repeat counter
									 
	tetris_input_command_t cmdLast; // last command (important for key repeat)
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


/* Function:     tetris_input_queryJoystick
 * Description:  translates joystick movements to a tetris_input_command_t
 * Return value: void
 */
tetris_input_command_t tetris_input_queryJoystick();


/* Function:      tetris_input_setLevel
 * Description:   modifies time interval of input events
 * Argument pIn:  pointer to input structure
 * Argument nLvl: desired level (0 <= nLvl <= TETRIS_INPUT_LEVELS - 1)
 * Return value:  void
 */
void tetris_input_setLevel(tetris_input_t *pIn,
                           uint8_t nLvl);

#endif /*INPUT_H_*/
