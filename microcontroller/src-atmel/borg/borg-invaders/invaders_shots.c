#include "invaders.h"

pixel pixelToInvaderPos( pixel abs, unsigned char col, unsigned char row ) {

	if(abs.x < col || abs.x >= (col+8) || abs.y < row 
		|| abs.y >= row + INVADER_ROWS) {
		return (pixel){255,255};
	}
	return (pixel) {abs.x - col, abs.y -row};
}



void moveShots(Cannon* cn, pixel ishots[MAX_SHOTS], pixel* shot){
	int i;
	static unsigned char smv;
	
	/****************************************************************/
	/*							CANNON								*/
	/****************************************************************/
	if(smv%CANNON_SHOOTING_SPEED ==0) {
		if(!(cn->ready)) {
			shot->y--;
		}
	}
	
	/****************************************************************/
	/*							INVADER								*/
	/****************************************************************/
	if(smv%INVADER_SHOOTING_SPEED ==0) {
	
		for(i = 0; i < MAX_SHOTS; ++i) {
			if(ishots[i].y <= RIGHT_BORDER) {
				ishots[i].y++;
			}
		}
	}
	smv++;

}

void proc_shots(Game* gm, Player* pl, Invaders* iv, Cannon* cn, 
						pixel ishots[MAX_SHOTS], pixel* shot) {
						
	signed char i;
	signed char np;
						
	/****************************************************************/
	/*							Canon								*/
	/****************************************************************/
	if(!cn->ready) {	
		if(shot->y == 0) {
			cn->ready = 1;
		} else if(shot->y == GUARD_LINE) {
			unsigned char lineByte = 0;
			unsigned char off = 0;
	
			if( shot->x >= 8) {
				lineByte =1;
				off =  shot->x -8;
			}else {
				off = shot->x;
			} 
			unsigned char guardPos = shl_table[off];
	
			for( np = NUMPLANE-1; np >= 0; --np) {
				if( (gm->guards[np][lineByte] & guardPos)) {
					gm->guards[np][lineByte] -= guardPos; 
					cn->ready = 1;
					break;
				}
			}
	
		} else if(shot->y < 13) {
			pixel iv_pos = pixelToInvaderPos(*(shot), iv->pos.x, iv->pos.y);
			
			if(!(iv_pos.x == 255 || iv_pos.y > iv->depth)) {
				
				unsigned char sh_pos =  shl_table[iv_pos.x];
				for( np = NUMPLANE-1; np >= 0; --np) {
					if( (iv->invaders[np][iv_pos.y] & sh_pos) ) {
						iv->invaders[np][iv_pos.y] -= sh_pos;
						cn->ready = 1;
						
						if(np == 0) {
							iv->colums_hit[iv_pos.x]++;
							iv->nHits++;
							//printf("hits: %d\n", iv->nHits);

							if(iv->nHits % SPEED_INC_RATE == 0) {
								iv->speed -= SPEED_INC_VALUE;
								printf("speed: %d\n", iv->speed);
							}
							
							signed char d;
							for(d = iv->depth-1; d >= 0; --d) {
								if(iv->invaders[0][d] == 0) {
									iv->depth--;
									break;
								}
							}
											
							if(iv->colums_hit[iv_pos.x] == INVADER_ROWS) {
									/*berechnen iv_clear*/
								iv->clear_r = 0;
								iv->clear_l = 0;
								
								for(i = 0; i < 8; ++i) {
									if(iv->colums_hit[i] == INVADER_ROWS) {
										iv->clear_r++;
									}
									else break;
								}
								for(i = 7; i >= 0; --i) {
									if(iv->colums_hit[i] == INVADER_ROWS) {
										iv->clear_l++;
									}
									else break;
								}
							
							} //if a column is cleaned
						}
						break;
					}
				}
			}
		} 
	} //if !cn->ready

	/****************************************************************/
	/*							INVADER								*/
	/****************************************************************/

	for(i = 0; i < MAX_SHOTS; ++i) {
		if(ishots[i].y == GUARD_LINE) {
				unsigned char lineByte = 0;
				unsigned char off = 0;

				if( ishots[i].x >= 8) {
					lineByte =1;
					off =  ishots[i].x -8;
				}else {
					off = ishots[i].x;
				} 
				unsigned char guardPos = shl_table[off];

				for( np = NUMPLANE-1; np >= 0; --np) {
					if( (gm->guards[np][lineByte] & guardPos)) {
						gm->guards[np][lineByte] -= guardPos; 
						ishots[i].y =255;
						ishots[i].x =255;
						break;
					}
				}
		
		}
		if(ishots[i].y == 15) {
			if(ishots[i].x > cn->pos-1 && ishots[i].x < cn->pos+1 ) {
				pl->lives--; 
			}
			ishots[i].y =255;
			ishots[i].x =255;
		}
		
	} 

}
