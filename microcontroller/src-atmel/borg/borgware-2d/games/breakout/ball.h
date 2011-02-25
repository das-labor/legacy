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

#ifndef BALL_H
#define BALL_H


typedef struct
{
	int16_t x;
	int16_t y;
	int16_t dir_x; /* direction vector */
	int16_t dir_y;
	uint8_t strength;
} ball_t;

/* @description Called once per game tick. Move the ball further along it's vector.
 */
void ball_think (ball_t *in_ball);

void ball_draw (ball_t *);

void ball_spawn_default (ball_t *in_b);
#endif /* BALL_H */
