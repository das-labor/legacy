/*
 * File: strbuf.c
 *
 * Copyright 2003 Sebastian Geerken <s.geerken@ping.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * A simple structure for fast concatenation of a large number of strings.
 */

#include <string.h>
#include "strbuf.h"


/*
 * Create a new string buffer.
 */
Strbuf_t *a_Strbuf_new (void)
{
   Strbuf_t *strbuf;

   strbuf = g_new (Strbuf_t, 1);
   strbuf->list = NULL;
   strbuf->num_chars = 0;
   strbuf->str = NULL;
   strbuf->str_valid = FALSE;
   return strbuf;
}


/*
 * Free a string buffer.
 */
void a_Strbuf_free (Strbuf_t *strbuf)
{
   GSList *it;

   for (it = strbuf->list; it; it = it->next)
      g_free (it->data);
   g_slist_free (strbuf->list);
   g_free (strbuf->str);
   g_free (strbuf);
}

/*
 * Append a NUL-terminated string to the buffer.
 * A copy is kept in the buffer, so the caller does not have to care
 * about memory management.
 */
void a_Strbuf_append (Strbuf_t *strbuf,
                      gchar *str)
{
   /* Remember that g_slist_prepend is faster than g_slist_prepend! */
   strbuf->list = g_slist_prepend (strbuf->list, g_strdup (str));
   strbuf->num_chars += strlen (str);
   strbuf->str_valid = FALSE;
}

/*
 * Return a NUL-terminated strings containing all appended strings.
 * The caller does not have to free the string, this is done in
 * a_Strbuf_free().
 */
gchar* a_Strbuf_chars (Strbuf_t *strbuf)
{
   GSList *it;
   gchar *p, *s;
   int l;

   if (strbuf->str_valid)
      return strbuf->str;

   g_free (strbuf->str);
   strbuf->str = g_new (gchar, strbuf->num_chars + 1);

   /* The strings in the list are prepended so we have to start from the tail
    * and work our way to the start of the new string */
   p = strbuf->str + strbuf->num_chars;
   for (it = strbuf->list; it; it = it->next) {
      s = (gchar*)(it->data);
      l = strlen (s);
      p -= l;
      memcpy (p, s, l * sizeof (gchar));
   }

   p[strbuf->num_chars] = 0;
   strbuf->str_valid = TRUE;

   return strbuf->str;
}
