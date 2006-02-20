#include "invaders.h"
#include "joystick.h"

/****************************************************************/
/*							MAIN FUNC							*/
/****************************************************************/
#define BIT_S(var,b) ((var&(1<<b))?1:0)

unsigned char random();

void borg_invaders() {
	Game 		gm;
	Player 		pl;
	Cannon 		cn;
	Invaders	iv;
	
	pixel		st[MAX_SHOTS] = 
						{{255,255},
						{255,255},
						{255,255},
						{255,255},
						{255,255}};
	
	pixel 		shot = {0,0};
	
	//char ctrl;	

	//initGame(&gm, 0 ,0x49, 0x24);
	initPlayer(&pl);
	
	gm.level = 0;
	cn.pos = 4;
	cn.ready = 1;
 	drawIntroScreen(2000);
 	
	do {
	
		initGame(&gm, gm.level ,0x49, 0x24);

		initLevel(&gm, &iv);
		//cn.pos = 4;
		cn.ready = 1;
		
		draw(&gm, &pl, &iv, &cn, st, &shot);
		
		while(1) {
	
			 //ctrl = getControl();
			 
			 userAction(&cn, &shot, ' ');
			 
			 moveShots(&cn, st, &shot);
			 invaderAttak(&iv, st);
			 proc_shots(&gm, &pl, &iv, &cn, st, &shot);
			 
			 moveInvaders(&iv);

			 draw(&gm, &pl, &iv, &cn, st, &shot);

			/*Pr\374fen ob gewonnen!*/
			if(iv.depth == 0) {
				gm.level = (gm.level+1) % MAX_LEVEL;
				drawCleared();
				break;
				
			}
			
			/*entfernen der guards wenn invaders in gardline*/
			if(iv.pos.y + iv.depth == GUARD_LINE) {
				initGame(&gm, gm.level ,0x00, 0x00);
			}
			
			/*Pr\374fen ob ein leben verloren wurde*/
			if(iv.pos.y + iv.depth > GUARD_LINE) {
				pl.lives--;
				break;
			}		
				
			if( pl.lives <= 0) {
				scrolltext("<80#GAME OVER");
				break;
			}
		}	
		
	} while(pl.lives > 0);
	return;
}




/****************************************************************/
/*							Functions							*/
/****************************************************************/

#define BIT_S(var,b) ((var&(1<<b))?1:0)

unsigned char ivrandom(){
	static unsigned int muh = 0xAA;
	unsigned char x;
	for(x=0;x<8;x++){
		muh = (muh<<1) ^ BIT_S(muh,1) ^ BIT_S(muh,8) 
			^ BIT_S(muh,9) ^ BIT_S(muh,13) ^ BIT_S(muh,15);
	}
	return (unsigned char) muh;

}





void invaderAttak(Invaders* iv, pixel ishots[MAX_SHOTS]) {

	unsigned char i;
	unsigned char spos = ivrandom()%16;
	if(spos >=8) return;
	
	unsigned char shoot = ivrandom();
	
	if(shoot  < SHOOTING_RATE ) {
		for(i = 0; i < MAX_SHOTS; ++i) {
			if(ishots[i].y > RIGHT_BORDER  && iv->colums_hit[spos] != INVADER_ROWS){
				ishots[i].y = iv->pos.y + (INVADER_ROWS - iv->colums_hit[spos]);
				ishots[i].x = iv->pos.x + spos;
				break;
			}
		}
	}
	
}

void moveInvaders(Invaders* iv) {
	static unsigned char mv = 0;

	if(mv % iv->speed == 0) {
		if(!(iv->isEdged) 
				&& ( (iv->pos.x == RIGHT_BORDER - (7- iv->clear_l)) 
				|| (iv->pos.x == LEFT_BORDER - iv->clear_r) ) ){
			iv->pos.y++;
			iv->direction *= -1;
			iv->isEdged = 1;
		} else {
			iv->pos.x += iv->direction;
			iv->isEdged = 0;
		}

	}
	mv++;
			
}



void userAction(Cannon* cn, pixel* shot, unsigned char ctrl) {

		if(JOYISRIGHT) {
			if(cn->pos != RIGHT_BORDER) {
				cn->pos++;
			}
		} else if (JOYISLEFT) {

			if(cn->pos != LEFT_BORDER) {
				cn->pos--;
			}				
		} else if (JOYISFIRE) {

			if(cn->ready) {
				shot->x = cn->pos;
				shot->y =  14;
				cn->ready = 0;
			}
		}
		
}



