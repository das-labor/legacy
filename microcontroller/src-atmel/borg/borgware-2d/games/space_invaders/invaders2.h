/*  	Space INVADERS V0.2 
 *		
 *		by: Fabian Bremerich
 *		Thanx to: Peter F. 
 *
 *		date: Mi, 08.03.2006 
 *
 */

#ifndef INVADERS2_H
#define INVADERS2_H

#include <stdint.h>
#include "../../config.h"
#include "../../pixel.h"

/****************************************************************/
/*                   GLOBALE VAR                                */
/****************************************************************/
#define P 3
extern uint16_t const peter[8];
extern uint16_t const hans[7];

/****************************************************************/
/*                          DEFINES                             */
/****************************************************************/
#define START_LIVES 3

#define SPACESHIP_LINE 1

#define GUARD_LINE (NUM_ROWS - 3)

#define MAX_INVADER_HEIGHT  8
#define MAX_INVADER_WIDTH  12
#define MAX_INVADER_LIVES   3

#define POINTS_FOR_HIT         5
#define POINTS_FOR_KILL       25
#define POINTS_FOR_SPACESHIP  75
#define POINTS_FOR_LEVEL     100

#define MAX_SHOTS        7
#define MIN_SPEED       70
#define SPEED_INC_RATE   2
#define SPEED_INC_VALUE  3
#define MAX_LEVEL        5

#define SHOOTING_RATE           6
#define INVADER_SHOOTING_SPEED 10
#define CANNON_SHOOTING_SPEED   4
#define SPACESHIP_SPEED        30
#define NO_SPACESHIP 255

#define CANNON_SPEED 2

#define WAIT_MS 15

typedef struct
{
	signed char x;
	signed char y;
} spixel;

typedef struct
{
	unsigned char map[MAX_INVADER_WIDTH][MAX_INVADER_HEIGHT];
	spixel pos;

	unsigned char speed;
	unsigned char speedinc;
	signed char direction;
	unsigned char isEdged;
} Invaders;


typedef struct
{
	unsigned char pos;
	unsigned char lives;
} Spaceship;


typedef struct
{
	unsigned char pos;
	unsigned char ready;
} Cannon;


typedef struct
{
	unsigned char lives;
	unsigned int points;
} Player;

typedef unsigned char offScreen_t[NUMPLANE + 1][NUM_ROWS][LINEBYTES];


/****************************************************************/
/*                          FUNCTIONS                           */
/****************************************************************/

void borg_invaders();
/*----------------------main_level_funcs-------------------------*/

void procSpaceship(Spaceship * sp);
void procCannon(Cannon * cn, pixel * shot);

void procInvaders(Invaders * iv, pixel st[MAX_SHOTS]);
void procShots(Invaders * iv, Player * pl, Cannon * cn, Spaceship * sc,
		unsigned char guards[NUM_COLS], pixel st[MAX_SHOTS], pixel * shot);

unsigned char getStatus(Invaders * iv);

/*----------------------Initialization---------------------------*/

void initGuards(unsigned char guards[NUM_COLS]);
void initInvaders(Invaders * iv, unsigned char lv);

/*----------------------getter/setter----------------------------*/

unsigned char getInvaderPixel(Invaders * iv, unsigned char x, unsigned char y);

void setInvaderPixel(Invaders * iv, unsigned char x, unsigned char y,
		unsigned char val);

void setGuardPixel(unsigned char *guards, unsigned char x,
		unsigned char y, unsigned char val);

inline static unsigned char getGuardPixel(unsigned char *guards,
		unsigned char x, unsigned char y)
{
	if (x < NUM_COLS && y == GUARD_LINE)
		return guards[x];
	return 0;
}

/*----------------------drawing Method---------------------------*/

void draw(offScreen_t offscreen, Invaders * iv, Spaceship * sc, Player * pl,
		Cannon * cn, unsigned char *guards, pixel *st, pixel * shot);

#endif /* INVADERS2_H */
