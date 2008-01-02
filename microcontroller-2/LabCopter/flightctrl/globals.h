/***************************************************************************
 *            globals.h
 *
 *  Wed Jan  2 19:53:24 2008
 *  Copyright  2008  gast
 *  <gast@<host>>
 ****************************************************************************/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */

//these vars store the current ADC measurement values to used by the main program
extern volatile int  CurrentNick, CurrentRoll, CurrentGier;
extern volatile int  CurrentACC_X, CurrentACC_Y, CurrentACC_Z;
extern volatile int  CurrentUBat;
