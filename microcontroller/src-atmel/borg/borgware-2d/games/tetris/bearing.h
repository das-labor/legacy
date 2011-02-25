#ifndef BEARING_H_
#define BEARING_H_

enum tetris_bearing
{
	TETRIS_BEARING_0,
	TETRIS_BEARING_90,
	TETRIS_BEARING_180,
	TETRIS_BEARING_270
};
#ifdef NDEBUG
	typedef uint8_t tetris_bearing_t;
#else
	typedef enum tetris_bearing tetris_bearing_t;
#endif

#endif /* BEARING_H_ */
