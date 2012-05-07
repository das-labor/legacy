/**
 * \addtogroup tetris
 * @{
 */

/**
 * @file bearing.h
 * @brief Public interface for denoting the current bearing of the bucket.
 * @author Christian Kroll
 */


#ifndef BEARING_H_
#define BEARING_H_

#include <stdint.h>

/**
 * Denotes the bearing of the bucket.
 */
enum tetris_bearing_e
{
	TETRIS_BEARING_0,  //!< TETRIS_BEARING_0
	TETRIS_BEARING_90, //!< TETRIS_BEARING_90
	TETRIS_BEARING_180,//!< TETRIS_BEARING_180
	TETRIS_BEARING_270 //!< TETRIS_BEARING_270
};
#ifdef NDEBUG
	typedef uint8_t tetris_bearing_t;
#else
	typedef enum tetris_bearing_e tetris_bearing_t;
#endif


#endif /* BEARING_H_ */

/*@}*/
