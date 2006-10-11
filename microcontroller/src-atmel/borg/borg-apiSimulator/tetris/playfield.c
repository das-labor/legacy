#include <stdlib.h>
#include "playfield.h"

tetris_playfield_t* tetris_playfield_construct() {
	tetris_playfield_t* p_playfield = (tetris_playfield_t*) malloc(sizeof(tetris_playfield_t));
	return p_playfield;
}

void tetris_playfield_destruct(tetris_playfield_t* p_pl) {
	if (p_pl != 0)
		free(p_pl);
}
