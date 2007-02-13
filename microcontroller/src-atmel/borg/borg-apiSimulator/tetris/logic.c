#include "logic.h"

#include <stdio.h>
#include "piece.h"
#include "playfield.h"

/*
int main (int argc, char* argv[]) {
	
	tetris_playfield_t* p_pf;
	tetris_piece_t* p_piece;
	
	p_pf = tetris_playfield_construct(10, 16);
	p_piece = tetris_piece_construct(TETRIS_PC_LINE, TETRIS_PC_ANGLE_90);
	
	tetris_playfield_insertPiece(p_pf, p_piece);

	int i;
	for (i = -3; i < 10; ++i)
	{
		uint8_t nCollision = tetris_playfield_collisionDetect(p_pf, p_piece, 0, i);
		printf("column: %d   collision: %d\n", i, nCollision);
	}
	
	tetris_playfield_destruct (p_pf);
	tetris_piece_destruct (p_piece);
	
	return 0;
}
*/
