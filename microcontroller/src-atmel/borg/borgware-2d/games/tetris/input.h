#ifndef INPUT_H_
#define INPUT_H_

#include <inttypes.h>
#include "orientation.h"

/**
 * \defgroup TetrisInputDefinesPublic Input: Public constants
 */
/*@{*/

/***********
 * defines *
 ***********/

/** number of levels */
#define TETRIS_INPUT_LEVELS 30

/*@}*/


/**
 * \defgroup TetrisInputTypes Input: Data types
 */
/*@{*/

/*********
 * types *
 *********/

/**
 * allowed input values
 */
typedef enum tetris_input_command_t
{
	TETRIS_INCMD_LEFT,    /**< move piece left */
	TETRIS_INCMD_RIGHT,   /**< move piece right */
	TETRIS_INCMD_DOWN,    /**< lower piece by one row */
	TETRIS_INCMD_ROT_CW,  /**< rotate clockwise */
	TETRIS_INCMD_ROT_CCW, /**< rotate counter clockwise */
	TETRIS_INCMD_DROP,    /**< move piece to the ground immediately */
	TETRIS_INCMD_GRAVITY, /**< piece gets pulled by gravity */
	TETRIS_INCMD_PAUSE,   /**< pause the game */
	TETRIS_INCMD_NONE     /**< idle (must alway be the last one) */
}
tetris_input_command_t;


/**
 * values which influence the gravity time limit for a piece
 */
typedef enum tetris_input_pace_t
{
	TETRIS_INPACE_HOVERING, /**< normal falling pace */
	TETRIS_INPACE_GLIDING   /**< guarantees a minimum docking time to avoid
	                             accidentally docked pieces in higher levels */
}
tetris_input_pace_t;


/**
 * data structure for the input module
 */
typedef struct tetris_input_t
{
	/**
	 *  current level (determines falling speed)
	 */
	uint8_t nLevel;

	/**
	 * Amount of loop cycles between forced piece movements. This value gets
	 * set via the tetris_input_setLevel() function.
	 */
	uint8_t nMaxCycles;

	/**
	 * This counter keeps track of the number of loop cycles which have been
	 * done since the last forced piece movement. It gets reset if it either
	 * reaches a well defined value (causing a gravity command to be issued)
	 * or the player has moved down the piece herself/himself.
	 */
	uint8_t nLoopCycles;

	/**
	 * Amount of loop cycles in which the same command has been issued
	 * consecutively. It gets reset if either the current command differs from
	 * the last one or a well-defined value has been reached (thereby
	 * regulating the pace of the key repeat as commands are only processed
	 * if that value is reached).
	 */
	int8_t nRepeatCount;

	/**
	 * Keeps track of the number of loop cycles which have been run while in
	 * pause mode. As soon as a well defined value is reached, the game
	 * continues (in case someone paused the game and forgot to resume it).
	 */
	uint16_t nPauseCount;


	/**
	 * last real command (important for key repeat and chatter protection)
	 */
	tetris_input_command_t cmdRawLast;


	/**
	 * last mapped command (important for key repeat)
	 */
	tetris_input_command_t cmdLast;


	/**
	 * Every command has its own counter. A command is ignored as long as its
	 * counter is unequal to 0. A counter gets set to a specific value (or 0)
	 * if the button of the corresponding command has been released by the
	 * player. All counters get decremented by one every loop cycle until they
	 * are zero. This is used to work against joystick chatter. Look at the
	 * TETRIS_INPUT_CHATTER_TICKS_... constants in input.c for the initial
	 * values of these counters.
	 */
	uint8_t nIgnoreCmdCounter[TETRIS_INCMD_NONE];

	/**
	 * orientation of the direction mapping
	 */
	tetris_orientation_t nOrientation;
}
tetris_input_t;

/*@}*/


/**
 * \defgroup TetrisInputRelated Input: Interface functions
 */
/*@{*/

/****************************
 * construction/destruction *
 ****************************/

/**
 * constructs an input object for André's borg
 * @return pointer to a newly created input object
 */
tetris_input_t *tetris_input_construct(void);


/**
 * destructs an input object
 * @param pIn pointer to the input object which should be destructed
 */
void tetris_input_destruct(tetris_input_t *pIn);


/***************************
 * input related functions *
 ***************************/

/**
 * retrieves commands from joystick or loop interval
 * @param pIn pointer to an input object
 * @param nPace falling pace
 * @return see definition of tetris_input_command_t
 * @see definition of tetris_input_pace_t
 */
tetris_input_command_t tetris_input_getCommand(tetris_input_t *pIn,
                                               tetris_input_pace_t nPace);


/**
 * modifies time interval of input events
 * @param pIn pointer to an input object
 * @param nLvl desired level (0 <= nLvl <= TETRIS_INPUT_LEVELS - 1)
 */
void tetris_input_setLevel(tetris_input_t *pIn,
                           uint8_t nLvl);


/**
 * resets the key repeat count for the down key
 * @param pIn pointer to an input object
 */
void tetris_input_resetDownKeyRepeat(tetris_input_t *pIn);


/**
 * set the orientation of the direction control mapping
 * @param pIn pointer to an input object
 * @param nOrient desired orientation
 */
void tetris_input_setOrientation(tetris_input_t *pIn,
                                 tetris_orientation_t nOrient);

/*@}*/

#endif /*INPUT_H_*/
