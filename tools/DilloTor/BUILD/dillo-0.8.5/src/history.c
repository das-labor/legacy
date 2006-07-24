/*
 * File: history.c
 *
 * Copyright (C) 2001, 2002 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * Linear history (it also provides indexes for the navigation stack)
 */

#include "list.h"
#include "history.h"


typedef struct {
   DilloUrl *url;
   gchar *title;
} H_Item;


/* Global history list */
static H_Item *history = NULL;
static gint history_size = 0;        /* [1 based] */
static gint history_size_max = 16;


/*
 * Add a new H_Item at the end of the history list
 * (taking care of not making a duplicate entry)
 */
int a_History_add_url(DilloUrl *url)
{
   gint i, idx;

   for (i = 0; i < history_size; ++i)
      if (a_Url_cmp(history[i].url, url) == 0)
         return i;

   idx = history_size;
   a_List_add(history, history_size, history_size_max);
   history[idx].url = a_Url_dup(url);
   history[idx].title = NULL;
   ++history_size;
   return idx;
}

/*
 * Set the page-title for a given URL (by idx)
 * (this is known when the first chunks of HTML data arrive)
 */
int a_History_set_title(gint idx, const gchar *title)
{
   g_return_val_if_fail(idx >= 0 && idx < history_size, 0);

   g_free(history[idx].title);
   history[idx].title = g_strdup(title);
   return 1;
}

/*
 * Return the DilloUrl camp (by index)
 */
DilloUrl *a_History_get_url(gint idx)
{
   g_return_val_if_fail(idx >= 0 && idx < history_size, NULL);

   return history[idx].url;
}

/*
 * Return the title camp (by index)
 */
const gchar *a_History_get_title(gint idx)
{
   g_return_val_if_fail(idx >= 0 && idx < history_size, NULL);

   if (history[idx].title)
      return history[idx].title;
   else
      return URL_STR_(history[idx].url);
}

/*
 * Return the title camp (by url)
 * ('force' returns URL_STR when there's no title)
 */
const gchar *a_History_get_title_by_url(DilloUrl *url, gint force)
{
   gint i;

   g_return_val_if_fail(url != NULL, NULL);

   for (i = 0; i < history_size; ++i)
      if (a_Url_cmp(url, history[i].url) == 0)
         break;

   if (i < history_size && history[i].title)
      return history[i].title;
   else if (force)
      return URL_STR_(url);
   return NULL;
}


/*
 * Free all the memory used by this module
 */
void a_History_free()
{
   gint i;

   for (i = 0; i < history_size; ++i) {
      a_Url_free(history[i].url);
      g_free(history[i].title);
   }
   g_free(history);
}
