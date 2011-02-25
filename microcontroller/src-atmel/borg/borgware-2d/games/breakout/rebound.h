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

#ifndef REBOUND_H
#define REBOUND_H
void rebound_tick(ball_t *ball);
void rebound_draw();

extern unsigned char rbpos;
inline static uint8_t rebound_getpos ()
{
	return (rbpos + (REBOUND_SIZE / 2));
}

inline static void rebound_init()
{
	rbpos = (NUM_ROWS / 2) - (REBOUND_SIZE / 2);
	rebound_draw();
}

void rebound_reflect(ball_t *b, int8_t in_x);
#endif /* REBOUND_H */
