#include "invaders.h"

void draw(Game* gm, Player* pl, Invaders* iv, Cannon* cn, 
			pixel ishots[MAX_SHOTS], pixel* shot ) {
	clear_screen(0);
	int np, i;
	
	/*DRAW CANNON*/
	if(cn->pos >= 1 ) {
		setpixel( (pixel){cn->pos-1, 15},  	pl->lives);
	}
	if(cn->pos < 16) {
		setpixel( (pixel){cn->pos, 15}, 	pl->lives);
		setpixel( (pixel){cn->pos, 14}, 	pl->lives);
	}
	if(cn->pos < 15) {
		setpixel( (pixel){cn->pos+1, 15}, 	pl->lives);
	}

	
	/* DRAW GUARDS */
	for(np = 0; np < NUMPLANE; np++) {
		pixmap[np][GUARD_LINE][0] |= gm->guards[np][0];
		pixmap[np][GUARD_LINE][1] |= gm->guards[np][1];
	}	

	/* DRAW INVADERS */
	
	//int i, np;
	if(iv->pos.x < 0) {

		for(i = 0; i < iv->depth; ++i) {
			for(np = 0; np < NUMPLANE; np++) {
				pixmap[np][iv->pos.y +i][0] |= 
								(iv->invaders[np][i] >> (iv->pos.x*(-1)));
			}
		}	

	}else if( iv->pos.x< 8) {
		for(i = 0; i < INVADER_ROWS; ++i) {
			for(np = 0; np < NUMPLANE; np++) {
				pixmap[np][iv->pos.y+i][0] |= 
								(iv->invaders[np][i] << iv->pos.x);
				pixmap[np][iv->pos.y+i][1] |= 
								(iv->invaders[np][i] >> (8 - iv->pos.x));
			}
		}	

	} else if(iv->pos.x== 8) {
		for(i = 0; i < INVADER_ROWS; ++i) {
			for(np = 0; np < NUMPLANE; np++) {
				pixmap[np][iv->pos.y+i][1] |= 
											iv->invaders[np][i];
			}
		}
		
	} else if(iv->pos.x>8) {
		for(i = 0; i < INVADER_ROWS; ++i) {
	
			for(np = 0; np < NUMPLANE; np++) {
				pixmap[np][iv->pos.y +i][1] |= 
								iv->invaders[np][i] << (iv->pos.x-8);
			}
		}
	}
	
	/*draw invader shots*/
	
	for(i = 0; i < MAX_SHOTS; ++i) {
		//if(ishots[i]->x > RIGHT_BORDER) continue;
		if(ishots[i].x < RIGHT_BORDER && ishots[i].y < RIGHT_BORDER) {
			setpixel(ishots[i], 3);
		}
	}

	/*draw player shot*/
	if(!(cn->ready)) {
		setpixel(*(shot), 3);
	}
	

	wait(WAIT_MS);
}
