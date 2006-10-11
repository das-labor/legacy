#ifndef TETRIS_PIECE_H_
#define TETRIS_PIECE_H_

#include <inttypes.h>

typedef uint8_t tetris_piece_t[4][4];

enum tetris_piece_type_t
	{TETRIS_PT_LINE, TETRIS_PT_T, TETRIS_PT_SQUARE, TETRIS_PT_L, TETRIS_PT_LBACK, TETRIS_PT_S, TETRIS_PT_SBACK};
enum tetris_piece_rotation_t {TETRIS_PR_CLOCKWISE, TETRIS_PR_COUNTERCLOCKWISE};

// construction/destruction
tetris_piece_t* tetris_piece_construct (enum tetris_piece_type_t t);
void tetris_piece_destruct(tetris_piece_t* p);

// actions
void tetris_piece_rotate(tetris_piece_t* p_src, tetris_piece_t* p_dst, enum tetris_piece_rotation_t r);

#endif /*TETRIS_PIECE_H_*/
