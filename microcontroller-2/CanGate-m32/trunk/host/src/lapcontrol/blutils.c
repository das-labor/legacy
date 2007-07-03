/* blutils.c
 * Common routines to handle BlinkenLights movies.
 *
 * Copyright (C) 2001  Sven Neumann <sven@gimp.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>

#include "blutils.h"


char *
bl_fgets (char *s, 
          int   size, 
          FILE *stream)
{
  int i = 0;
  int c = 0;

  if (!s || size < 2)
    return NULL;

  while (i < size - 1)
    {
      c = fgetc (stream);
      if (c == EOF || c == '\r')
        break;
      s[i++] = (char) c;
      if (c == '\n')
        break;
    }

  if (c == '\r')
    {
      c = fgetc (stream);
      if (c != '\n' && c != EOF)
        ungetc (c, stream);
      s[i++] = '\n';
    }
 
  if (i)
    s[i++] = '\0';

  return i > 0 ? s : NULL;
}
