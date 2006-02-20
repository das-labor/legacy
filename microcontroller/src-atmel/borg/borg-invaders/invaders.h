#ifndef INVADERS_H
#define INVADERS_H
/****************************************************************/
/*							INCLUDE								*/
/****************************************************************/
#include "pixel.h"
#include "scrolltext.h"


/****************************************************************/
/*							DEFINES								*/
/****************************************************************/
#define START_LIVES		3

#define GUARD_LINE		13

#define RIGHT_BORDER 15
#define LEFT_BORDER	0

#define INVADER_ROWS 	5
#define MAX_SHOTS 		5
#define MIN_SPEED		50
#define SPEED_INC_RATE 	2
#define SPEED_INC_VALUE 2
#define MAX_LEVEL		3

#define SHOOTING_RATE 	5
#define INVADER_SHOOTING_SPEED 8
#define CANNON_SHOOTING_SPEED  2

//#define WAIT_MS 		25
#define WAIT_MS 		20


/****************************************************************/
/*							TYPES								*/
/****************************************************************/

typedef struct {
	signed char x;
	signed char y;
} sig_pixel;


typedef struct {
	unsigned char level;
	unsigned char guards[NUMPLANE][LINEBYTES];
} Game;
	
typedef struct {
	signed char lives;
	unsigned char points;
} Player;

typedef struct {
	unsigned char pos;
	unsigned char ready;
}	Cannon;

typedef struct  {
	unsigned char invaders[NUMPLANE][INVADER_ROWS];
	sig_pixel	pos;
	unsigned char clear_r;
	unsigned char clear_l;
	unsigned char depth;
	unsigned char speed;
	signed char direction;
	signed char isEdged;
	unsigned char colums_hit[8];
	unsigned char nHits;
} Invaders;

typedef struct {
	pixel positions[MAX_SHOTS];
	//unsigned char count;
} Shots;



/****************************************************************/
/*							Functions							*/
/****************************************************************/

void borg_invaders();

void initGame(Game* gm, unsigned char level, unsigned char l, unsigned char r);
void initPlayer(Player* pl);
void initLevel(Game* gm, Invaders* iv);

void draw(Game* gm, Player* pl, Invaders* iv, Cannon* cn,
				pixel ishots[MAX_SHOTS], pixel* shot );

char getControl();

void moveInvaders(Invaders* iv);
void userAction(Cannon* cn, pixel* shot, unsigned char ctrl);

void invaderAttak(Invaders* iv, pixel ishots[MAX_SHOTS]);

void moveShots(Cannon* cn, pixel ishots[MAX_SHOTS], pixel* shot);
void proc_shots(Game* gm, Player* pl, Invaders* iv, Cannon* cn, 
					pixel ishots[MAX_SHOTS], pixel* shot);

/*ARTWORK*/
void drawIntroScreen(int ms);



#endif
