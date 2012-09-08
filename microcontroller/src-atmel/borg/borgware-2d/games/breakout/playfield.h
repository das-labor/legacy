/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 * Author & Copyright (C) 2010: Soeren Heisrath (forename@surename.org)
 *
 */

#include "common.h"
#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#define BOUNCE_NONE    0x00
#define BOUNCE_X       0x01
#define BOUNCE_Y       0x02
#define BOUNCE_UNDEF   0x04
#define BOUNCE_BRICK   0x08
#define BOUNCE_REBOUND 0x10

/* entries for the playing field */
enum game_field
{
	sp = 0, /* space */
	b1 = 1, b2 = 2, b3 = 3, /* bricks */
	bs = 4, /* solid (unbreakable) brick */
	bl, /* ball */
	rb /* rebound */
};
#ifdef NDEBUG
	typedef uint8_t game_field_t;
#else
	typedef enum game_field game_field_t;
#endif


extern game_field_t (*playfield)[NUM_COLS][NUM_ROWS];

/* @description draw the current field
 */
void playfield_draw();

/* @description set a field with given property.
 */
void playfield_set (uint8_t in_x, uint8_t in_y, game_field_t in_field);

/* @description Checks if there is an object in the way. If so, it returns 1
 */
int8_t check_bounce (int8_t in_x, int8_t in_y);

#endif /* PLAYFIELD_H */
