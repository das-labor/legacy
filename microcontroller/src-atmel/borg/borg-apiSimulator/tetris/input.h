#ifndef INPUT_H_
#define INPUT_H_

#include <inttypes.h>


/***********
 * defines *
 ***********/

// number of levels
#define TETRIS_INPUT_LEVELS 30


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
	TETRIS_INCMD_PAUSE,   // pause the game
	TETRIS_INCMD_NONE     // idle (must alway be the last one)
}
tetris_input_command_t;


typedef enum tetris_input_pace_t
{
	TETRIS_INPACE_HOVERING, // normal falling pace
	TETRIS_INPACE_GLIDING   /* guarantees a minimum docking time to avoid that
	                           pieces are docked immediately if they hit something
	                           in higher levels */
}
tetris_input_pace_t;


typedef struct tetris_input_t
{
	// current level (determines falling speed)
	uint8_t nLevel;

	// Amount of loop cycles between forced piece movements. This value gets
	// set via the tetris_input_setLevel() function.
	uint8_t nMaxCycles;

	// This counter keeps track of the number of loop cycles which have been
	// done since the last forced piece movement. It gets reset if it either
	// reaches a well defined value (causing a gravity command to be issued)
	// or the player has moved down the piece herself/himself.
	uint8_t nLoopCycles;

	// Amount of loop cycles in which the same command has been issued
	// consecutively. It gets reset if either the current command differs from
	// the last one or a well-defined value has been reached (thereby
	// regulating the pace of the key repeat as commands are only processed
	// if that value is reached).
	int8_t nRepeatCount;

	// Keeps track of the number of loop cycles which have been run while in
	// pause mode. As soon as a well defined value is reached, the game
	// continues (in case someone paused the game and forgot to resume it).
	uint16_t nPauseCount;


	// last command (important for key repeat)
	tetris_input_command_t cmdLast;

	// Every command has its own counter. A command is ignored as long as its
	// counter is unequal to 0. A counter gets set to a specific value (or 0)
	// if the button of the corresponding command has been released by the
	// player. All counters get decremented by one every loop cycle until they
	// are zero. This is used to work against joystick chatter. Look at the
	// TETRIS_INPUT_CHATTER_TICKS_... constants in input.c for the initial
	// values of these counters.
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

/* Function:       retris_input_getCommand
 * Description:    retrieves commands from joystick or loop interval
 * Argument pIn:   pointer to an input object
 * Argument nPace: falling pace (see definition of tetris_input_pace_t)
 * Return value:   see definition of tetris_input_command_t
 */
tetris_input_command_t tetris_input_getCommand(tetris_input_t *pIn,
                                               tetris_input_pace_t nPace);


/* Function:      tetris_input_setLevel
 * Description:   modifies time interval of input events
 * Argument pIn:  pointer to an input object
 * Argument nLvl: desired level (0 <= nLvl <= TETRIS_INPUT_LEVELS - 1)
 * Return value:  void
 */
void tetris_input_setLevel(tetris_input_t *pIn,
                           uint8_t nLvl);


/* Function:      tetris_input_resetDownKeyRepeat
 * Description:   resets the key repeat count for the down key
 * Argument pIn:  pointer to an input object
 * Return value:  void
 */
void tetris_input_resetDownKeyRepeat(tetris_input_t *pIn);

#endif /*INPUT_H_*/
