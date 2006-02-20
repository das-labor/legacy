#include "invaders.h"

void initGame(Game* gm, unsigned char level, unsigned char l, unsigned char r) { 
	gm->level = level;
	unsigned char np;
	for(np = 0; np < NUMPLANE; ++np) {
		gm->guards[np][1] = l;
		gm->guards[np][0] = r;
	}

}


void initPlayer(Player* pl) {
	pl->points 	= 0;
	pl->lives 	= START_LIVES;	
}



void initLevel(Game* gm, Invaders* iv) {
	int np, i;
	switch(gm->level) {
		case 0: for(np = 0; np < NUMPLANE; ++np) {
					for(i = 0; i < INVADER_ROWS; ++i) {
						if(np >= 1 ) {
							iv->invaders[np][i] = 0x00;
							continue;
						}
						iv->invaders[np][i] = 0xFF;
					}
				}
				iv->pos.x = 4;
				iv->pos.y = 3;
				iv->clear_r = 0;
				iv->clear_l = 0;
				iv->depth = INVADER_ROWS;
				iv->speed = MIN_SPEED-1;
				iv->direction = 1;
				iv->isEdged = 0;
				for(i = 0; i < 8; ++i) iv->colums_hit[i]=0;

			break;
			
		case 1: for(np = 0; np < NUMPLANE; ++np) {
					for(i = 0; i < INVADER_ROWS; ++i) {
						if((i > INVADER_ROWS/2) && np == NUMPLANE-1) {
							iv->invaders[np][i] = 0x00;
							continue;
						}
						iv->invaders[np][i] = 0xFF;
					}
				}
				iv->pos.x = 4;
				iv->pos.y = 3;
				iv->clear_r = 0;
				iv->clear_l = 0;
				iv->depth = INVADER_ROWS;
				iv->speed = MIN_SPEED-1;
				iv->direction = 1;
				iv->isEdged = 0;
				for(i = 0; i < 8; ++i) iv->colums_hit[i]=0;

			break;
		case 2: for(np = 0; np < NUMPLANE; ++np) {
					for(i = 0; i < INVADER_ROWS; ++i) {
						iv->invaders[np][i] = 0xFF;
					}
				}
				iv->pos.x = 4;
				iv->pos.y = 3;
				iv->clear_r = 0;
				iv->clear_l = 0;
				iv->depth = INVADER_ROWS;
				iv->speed = MIN_SPEED-1;
				iv->direction = 1;
				iv->isEdged = 0;
				for(i = 0; i < 8; ++i) iv->colums_hit[i]=0;
			break;
			
	}
	
}

