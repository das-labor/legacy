#ifndef PLAYFIELD_H
#define PLAYFIELD_H
#include "common.h"

#define BOUNCE_X 0x01
#define BOUNCE_Y 0x02
#define BOUNCE_UNDEF 0x04

/* entries for the playing field */
enum game_field_t
{
	sp = 0, /* space */
	b1 = 1, b2 = 2, b3 = 3, /* bricks */
	bs = 4, /* solid (unbreakable) brick */
	bl, /* ball */
	rb /* rebound */
};


/* @description draw the current field
 */
void playfield_draw();

/* @description set a field with given property.
 */
void playfield_set (uint8_t in_x, uint8_t in_y, enum game_field_t in_field);

/* @description Checks if there is an object in the way. If so, it returns 1
 */
uint8_t check_bounce (int8_t in_x, int8_t in_y);

#endif /* PLAYFIELD_H */
