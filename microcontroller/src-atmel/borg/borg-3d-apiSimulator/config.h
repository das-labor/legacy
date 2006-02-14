/** This file is used to enter the configuration values of a program.
 *  The idee is to use the same libary files for differnt programs. Each
 *  of it has his own config.h. And in this file are the different parameters
 *  defined.
 */

#define NUM_PLANES 8
#define PLANEBYTES 8
#define NUM_LEVELS 3

/* These Numbers are deprecated and schould be replaced, 
 * because they were confusing.
 */
#define NUM_ROWS 8
#define NUM_COLS 8
/* and should be replaced throug the following macros.
 */
#define LEN_X 8
#define LEN_Y 8
#define LEN_Z 8

/** This defines how many Streams in matrix are used.
 *  FIXME: Better define directly above the animation-program. */
#define STREAMER_NUM 30

// FIXME: Here should be the uart parametration too. And what about can and the canadress.