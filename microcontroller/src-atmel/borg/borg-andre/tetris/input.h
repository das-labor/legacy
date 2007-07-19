#ifndef INPUT_H_
#define INPUT_H_

#include <inttypes.h>


/***********
 * defines *
 ***********/

// number of levels
#define TETRIS_INPUT_LEVELS 20


/*********
 * types *
 *********/

typedef enum tetris_input_command_t
{
	TETRIS_INCMD_ROT_CW,  // rotate clockwise
	TETRIS_INCMD_ROT_CCW, // rotate counter clockwise
	TETRIS_INCMD_LEFT,    // move piece left
	TETRIS_INCMD_RIGHT,   // move piece right
	TETRIS_INCMD_DOWN,    // lower piece by one row
	TETRIS_INCMD_DROP,    // move piece to the ground immediately  
	TETRIS_INCMD_GRAVITY, // piece gets pulled by gravity
	TETRIS_INCMD_IGNORE,  // padding byte, because avr-gcc can't handle uneven
	                      // array sizes in PROGMEM
	TETRIS_INCMD_NONE     // idle (must alway be the last one)
}
tetris_input_command_t;


typedef struct tetris_input_t
{
	// current level (determines falling speed)
	uint8_t nLevel;

	// This counter tracks how many loop cycles have been done since the last
	// forced piece movement. It gets reset if it either reaches a well
	// defined value (causing a gravity command to be issued) or the player has
	// moved down the piece herself/himself.
	uint8_t nLoopCycles;
	
	// Amount of loop cycles in which the same command has been issued
	// consecutively. It gets reset if either the current command differs from
	// the last one or a well-defined value has been reached (thereby
	// regulating the pace of the key repeat as commands are only processed
	// if that value is reached).
	int8_t nRepeatCount;
	
	// last command (important for key repeat)
	tetris_input_command_t cmdLast;
	
	// Every command has its own counter. A command is ignored as long as its
	// counter is unequal to 0. A counter gets set to a command specific
	// value (or 0) if its corresponding command was issued directly before the
	// player has released the button. All counters get decremented every loop
	// cycle until they are zero. This is used to work against the chattering
	// of some joysticks. Look at the function tetris_input_chatterCheck() in
	// input.c for the default initial values of these counters.
	uint8_t nIgnoreCmdCounter[TETRIS_INCMD_NONE];
}
tetris_input_t;

/****************************
 * construction/destruction *
 ****************************/

/* Function:     tetris_input_construct
 * Description:  constructs an input object for André's borg
 * Return value: pointer to a newly created input object
 */
tetris_input_t *tetris_input_construct();


/* Function:     tetris_input_destruct
 * Description:  destructs an input object
 * Argument pIn: pointer to the input object which should be destructed
 * Return value: void
 */
void tetris_input_destruct(tetris_input_t *pIn);


/***************************
 * input related functions *
 ***************************/

/* Function:     tetris_input_getCommand
 * Description:  retrieves commands from joystick or loop interval
 * Argument pIn: pointer to an input object
 * Return value: see definition of tetris_input_command_t
 */
tetris_input_command_t tetris_input_getCommand(tetris_input_t *pIn);


/* Function:      tetris_input_setLevel
 * Description:   modifies time interval of input events
 * Argument pIn:  pointer to an input object
 * Argument nLvl: desired level (0 <= nLvl <= TETRIS_INPUT_LEVELS - 1)
 * Return value:  void
 */
void tetris_input_setLevel(tetris_input_t *pIn,
                           uint8_t nLvl);


#endif /*INPUT_H_*/

