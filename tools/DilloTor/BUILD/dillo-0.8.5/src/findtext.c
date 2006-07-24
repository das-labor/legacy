/*
 * File: findtext.c
 *
 * Copyright 2002 Sebastian Geerken <s.geerken@ping.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * This module contains all the functionality for finding text, based
 * on DwWordIterator.
 *
 * Search string semantics: "foo" searches for a word containing
 * "foo", " foo" for a word beginning with "foo", "foo " for a word
 * ending with "foo", and " foo " for the word "foo" itself. Similar,
 * if you specify several words, spaces at the beginning and at the
 * end will be treated in the same way.
 *
 * Searching is done in a slight variant of Knuth-Morris-Pratt, with
 * words instead of characters, and an extended comparison, via
 * Findtext_compare, both in the construction of the next table
 * (Findtext_key_new), and the actual searching (Findtext_search0).
 * This works, since other comparison that FINDTEXT_FULL are only
 * allowed at the beginning (FINDTEXT_RIGHT), and at the end
 * (FINDTEXT_LEFT); they have following effects:
 *
 *    - Constructing the next table (Findtext_key_new): In
 *      comparisons, the first word is only used as the "needle", so
 *      there may be a state for which nexttab has the value 0
 *      (nexttab[0] itself is always -1). In Findtext_search0, this
 *      will cause the comparison correctly start again with the
 *      second word, since the word in the text before, which is
 *      /identical/ with the respective word in the key, so contains
 *      the first word in the key. The last word is never used in
 *      Findtext_key_new.
 *
 *    - Actual searching (Findtext_search0): Since nexttab[0] is
 *      always -1, a failed comparison will restart the search. The
 *      state, which is switched to after the comparison with the last
 *      word failed depends only on the words before.
 */

/*
 * todo: If a one-word key is found twice in a word, the second
 *       occurence is ignored (a side-effect of word-by-word
 *       searching).
 */

#include "msg.h"
#include "findtext.h"
#include "misc.h"
#include "debug.h"
#include <string.h>
#include <ctype.h>

/*
 * Searches needle in haystack, according to type:
 *
 *    FINDTEXT_FULL   Needle equals haystack.
 *    FINDTEXT_FREE   Haystack contains needle.
 *    FINDTEXT_LEFT   Needle is the left part of haystack.
 *    FINDTEXT_RIGHT  Needle is the right part of haystack.
 *
 * Returns pointer of occurrence in haystack, or NULL.
 */
static gchar *Findtext_compare (gchar *haystack, gchar *needle,
                                FindtextPatternType type,
                                gboolean case_sens)
{
   gchar *p;

   if (case_sens)
      switch (type) {
      case FINDTEXT_FULL:
         return (strcmp (haystack, needle) == 0) ? haystack : NULL;
      case FINDTEXT_FREE:
         return strstr (haystack, needle);
      case FINDTEXT_LEFT:
         return (strncmp (haystack, needle, strlen(needle)) == 0) ?
            haystack : NULL;
      case FINDTEXT_RIGHT:
         p = haystack + strlen (haystack) - strlen (needle);
         return (strcmp (p, needle) == 0) ? p : NULL;
      }
   else
      switch (type) {
      case FINDTEXT_FULL:
         return (g_strcasecmp (haystack, needle) == 0) ? haystack : NULL;
      case FINDTEXT_FREE:
         return a_Misc_stristr (haystack, needle);
      case FINDTEXT_LEFT:
         return (g_strncasecmp (haystack, needle, strlen(needle)) == 0) ?
            haystack : NULL;
      case FINDTEXT_RIGHT:
         p = haystack + strlen (haystack) - strlen (needle);
         return (g_strcasecmp (p, needle) == 0) ? p : NULL;
      }

   /* compiler happiness */
   return FALSE;
}

/*
 * Creates a new key from a string.
 */
static FindtextKey *Findtext_key_new (gchar *string, gboolean case_sens)
{
   FindtextKey *key;
   gint i, j, *splitpos;
   gboolean first_spaces, last_spaces;

   key = g_new (FindtextKey, 1);
   splitpos = a_Misc_strsplitpos (string, " \t\n");

   if (splitpos[0] == -1) {
      /* Only spaces in the key. Construct a key with the words
       * <empty-right> <space> <empty-left>, which will find spaces. */
      key->len = 2;
      key->words = g_new (gchar*, key->len);
      key->types = g_new (FindtextPatternType, key->len);
      key->nexttab = g_new (gint, key->len);

      key->words[0] = g_strdup ("");
      key->words[1] = g_strdup ("");
      key->nexttab[0] = key->nexttab[1] = -1; /* Actually not needed, since
                                               * comparison always succeeds. */
      key->types[0] = FINDTEXT_RIGHT;
      key->types[1] = FINDTEXT_LEFT;
   } else {
      first_spaces = (string[0] == ' ');
      last_spaces = (string[strlen(string) - 1] == ' ');

      key->len = 0;
      while (splitpos[2 * key->len] != -1)
         key->len++;

      key->words = g_new (gchar*, key->len);
      for (i = 0; i < key->len; i++)
         key->words[i] =
            a_Misc_strpdup (string, splitpos[2 * i], splitpos[2 * i + 1]);

      key->types = g_new (FindtextPatternType, key->len);
      key->nexttab = g_new (gint, key->len);

      if (key->len == 1) {
         key->nexttab[0] = -1;
         if (first_spaces && last_spaces)
            key->types[0] = FINDTEXT_FULL;
         else if (!first_spaces && !last_spaces)
            key->types[0] = FINDTEXT_FREE;
         else if (first_spaces && !last_spaces)
            key->types[0] = FINDTEXT_LEFT;
         else /*if (!first_spaces && last_spaces)*/
            key->types[0] = FINDTEXT_RIGHT;
      } else {
         for (i = 0; i < key->len; i++)
            key->types[i] = FINDTEXT_FULL;

         /* Begin and end. */
         if (!first_spaces)
            key->types[0] = FINDTEXT_RIGHT;
         if (!last_spaces)
            key->types[key->len - 1] = FINDTEXT_LEFT;

         /* Build the next table. */
         i = 0;
         j = -1;
         key->nexttab[0] = -1;

         do {
            if (j == -1 || Findtext_compare (key->words[i], key->words[j],
                                             key->types[j], case_sens)) {
               i++;
               j++;
               key->nexttab[i] = j;
               _MSG ("nexttab[%d] = %d\n", i, j);
            } else {
               j = key->nexttab[j];
            }
         } while (i < key->len - 1);
      }
   }

   g_free (splitpos);
   return key;
}

/*
 * Frees the memory allocated by a key.
 */
static void Findtext_key_destroy (FindtextKey *key)
{
   int i;
   for (i = 0; i < key->len; i++)
      g_free (key->words[i]);
   g_free (key->words);
   g_free (key->types);
   g_free (key->nexttab);
   g_free (key);
}



/*
 * Creates a new findtext state. The widget must be set by
 * a_Findtext_state_set_widget before the search can be started.
 */
FindtextState *a_Findtext_state_new (void)
{
   FindtextState *fts;

   fts = g_new0 (FindtextState, 1);
   DBG_OBJ_CREATE (fts, "FindtextState");
   return fts;
}


/*
 * Frees the memory allocated by a findtext state.
 */
void a_Findtext_state_destroy (FindtextState *state)
{
   if (state->key)
      Findtext_key_destroy (state->key);
   g_free (state->keystr);
   if (state->iterator)
      a_Dw_word_iterator_free (state->iterator);
   if (state->hl_iterator)
      a_Dw_word_iterator_free (state->hl_iterator);
   g_free (state);
}


/*
 * Sets the widget to search in. Widget may be NULL, in which case
 * a_Findtext_search will always return FINDTEXT_NOT_FOUND.
 */
void a_Findtext_state_set_widget (FindtextState *state,
                                  DwWidget *widget)
{
   state->widget = widget;
   if (state->iterator)
      a_Dw_word_iterator_free (state->iterator);
   state->iterator = NULL;
   if (state->hl_iterator)
      a_Dw_word_iterator_free (state->hl_iterator);
   state->hl_iterator = NULL;
   /* A widget change will restart the search. */
   if (state->key)
      Findtext_key_destroy (state->key);
   state->key = NULL;
   g_free (state->keystr);
   state->keystr = NULL;
}

static gboolean Findtext_search0 (FindtextState *state,
                                  gboolean case_sens)
{
   int i, j;
   gboolean nextit;
   gchar *p = NULL;

   if (state->iterator->word == NULL)
      return FALSE;

   j = 0;
   nextit = TRUE;

   do {
      if (j == -1 || (p = Findtext_compare (state->iterator->word,
                                            state->key->words[j],
                                            state->key->types[j],
                                            case_sens))) {
         if (j == 0)
            state->first_hl_start = p - state->iterator->word;
         if (j == state->key->len - 1) {
            state->last_hl_end = strlen (state->key->words[j]);
            if (j == 0)
               state->last_hl_end += state->first_hl_start;
         }

         j++;
         nextit = a_Dw_word_iterator_next (state->iterator);
      } else
         j = state->key->nexttab[j];
   } while (nextit && j < state->key->len);

   if (j >= state->key->len) {
      /* Go back to where the word was found. */
      for (i = 0; i < state->key->len; i++)
         a_Dw_word_iterator_prev (state->iterator);
      return TRUE;
   } else
      return FALSE;
}



FindtextResult a_Findtext_search (FindtextState *state,
                                  gchar *str,
                                  gboolean case_sens)
{
   int i;
   gboolean new_key = FALSE, was_highlighted = FALSE, first_trial;
   FindtextResult result2;

   if (state->widget == NULL)
      return FINDTEXT_NOT_FOUND;

   /* If there is still highlighted text */
   if (state->hl_iterator) {
      was_highlighted = TRUE;
      for (i = 0; i < state->key->len - 1; i++) {
         a_Dw_word_iterator_unhighlight (state->hl_iterator,
                                         DW_HIGHLIGHT_FINDTEXT);
         a_Dw_word_iterator_next (state->hl_iterator);
      }
      a_Dw_word_iterator_unhighlight (state->hl_iterator,
                                      DW_HIGHLIGHT_FINDTEXT);
      a_Dw_word_iterator_free (state->hl_iterator);
      state->hl_iterator = NULL;
   }

   /* If the key (or the widget) changes (including case sensitivity),
      the search is started from the beginning. */
   if (state->keystr == NULL || state->case_sens != case_sens ||
       strcmp(state->keystr, str) != 0) {
      new_key = TRUE;

      g_free (state->keystr);
      state->keystr = g_strdup (str);
      state->case_sens = case_sens;

      if (state->key != NULL)
         Findtext_key_destroy (state->key);
      state->key = Findtext_key_new (str, case_sens);

      if (state->iterator)
         a_Dw_word_iterator_free (state->iterator);
      state->iterator = a_Dw_word_iterator_new (state->widget);
      a_Dw_word_iterator_next (state->iterator);
   }

   first_trial = !was_highlighted || new_key;

   if (Findtext_search0 (state, case_sens)) {
      /* Highlighlighting is done with a clone. */
      state->hl_iterator = a_Dw_word_iterator_clone (state->iterator);
      if (state->key->len == 1) {
         a_Dw_word_iterator_scroll_to (state->iterator, state->iterator,
                                       state->first_hl_start,
                                       state->last_hl_end,
                                       DW_HPOS_INTO_VIEW, DW_VPOS_CENTER);
         a_Dw_word_iterator_highlight (state->hl_iterator,
                                       state->first_hl_start,
                                       state->last_hl_end,
                                       DW_HIGHLIGHT_FINDTEXT);
         DBG_MSGF (state, "findtext", 0, "highlighting %s from %d to %d",
                   state->hl_iterator->word,
                   state->first_hl_start, state->last_hl_end);
      } else {
         a_Dw_word_iterator_highlight (state->hl_iterator,
                                       state->first_hl_start,
                                       strlen (state->hl_iterator->word) + 1,
                                       DW_HIGHLIGHT_FINDTEXT);
         DBG_MSGF (state, "findtext", 0, "highlighting '%s' from %d to %d",
                   state->hl_iterator->word, state->first_hl_start,
                   strlen (state->hl_iterator->word) + 1);
         a_Dw_word_iterator_next (state->hl_iterator);
         for (i = 1; i < state->key->len - 1; i++) {
            a_Dw_word_iterator_highlight (state->hl_iterator, 0,
                                          strlen (state->hl_iterator->word)
                                          + 1,
                                          DW_HIGHLIGHT_FINDTEXT);
            DBG_MSGF (state, "findtext", 0, "highlighting '%s' from %d to %d",
                      state->hl_iterator->word,
                      0, strlen (state->hl_iterator->word) + 1);
            a_Dw_word_iterator_next (state->hl_iterator);
         }
         a_Dw_word_iterator_highlight (state->hl_iterator, 0,
                                       state->last_hl_end,
                                       DW_HIGHLIGHT_FINDTEXT);
         DBG_MSGF (state, "findtext", 0, "highlighting '%s' from %d to %d",
                   state->hl_iterator->word, 0, state->last_hl_end);

         a_Dw_word_iterator_scroll_to (state->iterator, state->hl_iterator,
                                       state->first_hl_start,
                                       state->last_hl_end,
                                       DW_HPOS_INTO_VIEW, DW_VPOS_CENTER);
      }
      for (i = 0; i < state->key->len - 1; i++)
         a_Dw_word_iterator_prev (state->hl_iterator);

      /* The search will continue from the word after the found position. */
      a_Dw_word_iterator_next (state->iterator);

      return FINDTEXT_SUCCESS;
   }

   if (first_trial)
      return FINDTEXT_NOT_FOUND;
   else {
      /* Nothing found anymore, reset the state for the next trial. */
      a_Dw_word_iterator_free (state->iterator);
      state->iterator = a_Dw_word_iterator_new (state->widget);
      a_Dw_word_iterator_next (state->iterator);

      /* We expect a success. */
      result2 = a_Findtext_search (state, str, case_sens);
      g_assert (result2 == FINDTEXT_SUCCESS);
      return FINDTEXT_RESTART;
   }
}

/*
 * This function is called when the user closes the "find text" dialog.
 */
void a_Findtext_reset_search (FindtextState *state)
{
   int i;

   g_free (state->keystr);
   state->keystr = NULL;

   if (state->hl_iterator) {
      for (i = 0; i < state->key->len - 1; i++) {
         a_Dw_word_iterator_unhighlight (state->hl_iterator,
                                         DW_HIGHLIGHT_FINDTEXT);
         a_Dw_word_iterator_next (state->hl_iterator);
      }
      a_Dw_word_iterator_unhighlight (state->hl_iterator,
                                      DW_HIGHLIGHT_FINDTEXT);
      a_Dw_word_iterator_free (state->hl_iterator);
      state->hl_iterator = NULL;
   }
}
