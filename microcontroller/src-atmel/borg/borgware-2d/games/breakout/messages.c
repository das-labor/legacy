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

#include "messages.h"

void print_ballsleft (ball_t *in_b)
{
#ifdef SCROLLTEXT_SUPPORT
	uint8_t txt[20];
	snprintf (txt, sizeof(txt), "</#%u balls left", in_b->strength);
	scrolltext(txt);
#endif
}

void print_score ()
{
#ifdef SCROLLTEXT_SUPPORT
	uint8_t txt[32];
	snprintf (txt, sizeof(txt), "</#GAME OVER. Your score: %u", score_get());
	scrolltext(txt);
#endif

}
