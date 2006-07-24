/*
 * File: dw_ext_iterator.c
 *
 * Copyright (C) 2002 Sebastian Geerken <S.Geerken@ping.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * DwExtIterator is an extension of DwIterator, which stores
 * DwIterator's in a stack, for cases where recursions are not
 * possible.
 *
 * DwWordIterator is a simple wrapper for DwExtIterator, which splits
 * text segments into words, and is used for text search (in
 * findtext.c)
 *
 * todo: In some cases (e.g. for "<i>foo</i>bar"), DwIterator and
 *       DwExtIterator return two words, where there is actually only
 *       one.  In this case, iterator->content.space is set to FALSE.
 *       DwWordIterator currently ignores this flag, but instead
 *       returns two words, instead of concaternating them to one.
 */

#include "dw_ext_iterator.h"
#include "list.h"
#include "misc.h"

/*#define DEBUG_LEVEL 1*/
#include "debug.h"

/*
 * The following two functions are used by a_Dw_extiterator_new, when the
 * passed DwIterator points to a widget. Since a DwExtIterator never returns
 * a widget, the DwIterator has to be corrected, by searching for the next
 * content downwards (within the widget pointed to), forwards, and backwards
 * (in the traversed tree).
 */

/*
 * Search downwards. If from_end is TRUE, start search at the end,
 * otherwise at the beginning.
 * The pararameter indent is only for debugging purposes.
 */
static DwIterator *Dw_ext_iterator_search_downward (DwIterator *it,
                                                    gboolean from_end,
                                                    int indent)
{
   DwIterator *it2, *it3;

   DEBUG_MSG (1, "%*smoving down (%swards) from %s\n",
              indent, "", from_end ? "back" : "for", a_Dw_iterator_text (it));

   g_return_val_if_fail (it->content.type == DW_CONTENT_WIDGET, NULL);
   it2 = a_Dw_widget_iterator (it->content.data.widget, it->mask, from_end);

   if (it2 == NULL) {
      /* Moving downwards failed. */
      DEBUG_MSG (1, "%*smoving down failed\n", indent, "");
      return NULL;
   }

   while (from_end ? a_Dw_iterator_prev (it2) : a_Dw_iterator_next (it2)) {
      DEBUG_MSG (1, "%*sexamining %s\n",
                 indent, "", a_Dw_iterator_text (it2));

      if (it2->content.type == DW_CONTENT_WIDGET) {
         /* Another widget. Search in it downwards. */
         it3 = Dw_ext_iterator_search_downward (it2, from_end, indent + 3);
         if (it3 != NULL) {
            a_Dw_iterator_free (it2);
            return it3;
         }
         /* Else continue in this widget. */
      } else {
         /* Success! */
         DEBUG_MSG (1, "%*smoving down succeeded: %s\n",
                    indent, "", a_Dw_iterator_text (it2));
         return it2;
      }
   }

   /* Nothing found. */
   a_Dw_iterator_free (it2);
   DEBUG_MSG (1, "%*smoving down failed (nothing found)\n", indent, "");
   return NULL;
}

/*
 * Search sidewards. from_end specifies the direction, FALSE means forwards,
 * TRUE means backwards.
 * The pararameter indent is only for debugging purposes.
 */
static DwIterator *Dw_ext_iterator_search_sideward (DwIterator *it,
                                                    gboolean from_end,
                                                    int indent)
{
   DwIterator *it2, *it3;

   DEBUG_MSG (1, "%*smoving %swards from %s\n",
              indent, "", from_end ? "back" : "for", a_Dw_iterator_text (it));

   g_return_val_if_fail (it->content.type == DW_CONTENT_WIDGET, NULL);
   it2 = a_Dw_iterator_clone (it);

   while (from_end ? a_Dw_iterator_prev (it2) : a_Dw_iterator_next (it2)) {
      if (it2->content.type == DW_CONTENT_WIDGET) {
         /* Search downwards in this widget. */
         it3 = Dw_ext_iterator_search_downward (it2, from_end, indent + 3);
         if (it3 != NULL) {
            a_Dw_iterator_free (it2);
            DEBUG_MSG (1, "%*smoving %swards succeeded: %s\n",
                       indent, "", from_end ? "back" : "for",
                       a_Dw_iterator_text (it3));
            return it3;
         }
         /* Else continue in this widget. */
      } else {
         /* Success! */
         DEBUG_MSG (1, "%*smoving %swards succeeded: %s\n",
                    indent, "", from_end ? "back" : "for",
                    a_Dw_iterator_text (it2));
         return it2;
      }
   }

   /* Nothing found, go upwards in the tree (if possible). */
   a_Dw_iterator_free (it2);
   if (it->widget->parent) {
      it2 = a_Dw_widget_iterator (it->widget->parent, it->mask, FALSE);
      while (TRUE) {
         if (!a_Dw_iterator_next(it2)) {
            g_warning ("BUG in DwExtIterator!");
            a_Dw_iterator_free (it2);
            return NULL;
         }
         if (it2->content.type == DW_CONTENT_WIDGET &&
             it2->content.data.widget == it->widget) {
            it3 = Dw_ext_iterator_search_sideward (it2, from_end, indent + 3);
            a_Dw_iterator_free (it2);
            DEBUG_MSG (1, "%*smoving %swards succeeded: %s\n",
                       indent, "", from_end ? "back" : "for",
                       a_Dw_iterator_text (it3));
            return it3;
         }
      }
   }

   /* Nothing found at all. */
   DEBUG_MSG (1, "%*smoving %swards failed (nothing found)\n",
              indent, "", from_end ? "back" : "for");
   return NULL;
}


/*
 * Create a new DwExtIterator from an existing DwIterator. The content of the
 * return value will be the content of "it". If within the widget tree, there
 * is no non-widget content, NULL is returned.
 *
 * NOTES:
 * (i)  If you want to continue using "it", pass a_Dw_iterator_clone (it).
 * (ii) The mask of "it" must include DW_CONTENT_WIDGET, but
 *      a_Dw_ext_iterator_next will never return widgets.
 *
 * TODO: Change in the near future: NULL is by all other functions within this
 * module interpreted as an "empty" iterator, which makes code outside more
 * robust.
 */
DwExtIterator* a_Dw_ext_iterator_new (DwIterator *it)
{
   DwExtIterator *eit;
   DwIterator *it2;
   DwWidget *w;
   int sp;

   DEBUG_MSG (1, "a_Dw_ext_iterator_new: %s\n", a_Dw_iterator_text (it));

   /* If "it" points to a widget, find a near non-widget content,
    * since an DwExtIterator should never return widgets. */
   if (it->content.type == DW_CONTENT_WIDGET) {
      /* The second argument of Dw_ext_iterator_search_downward is
       * actually a matter of taste :-) */
      if ((it2 = Dw_ext_iterator_search_downward (it, FALSE, 3)) ||
          (it2 = Dw_ext_iterator_search_sideward (it, FALSE, 3)) ||
          (it2 = Dw_ext_iterator_search_sideward (it, TRUE, 3))) {
         a_Dw_iterator_free (it);
         it = it2;
      } else {
         /* This may happen, when a page does not contain any
          * non-widget content. */
         DEBUG_MSG (1, "a_Dw_ext_iterator_new got totally helpless!\n");
         a_Dw_iterator_free (it);
         return NULL;
      }
   }

   DEBUG_MSG (1, "  => %s\n", a_Dw_iterator_text (it));

   eit = g_new (DwExtIterator, 1);
   eit->stack_top = 0;
   /* If this widget has parents, we must construct appropiate iterators.
    * todo: There may be a faster way instead of iterating through the
    * parent widgets. */
   for (w = it->widget; w->parent != NULL; w = w->parent)
      eit->stack_top++;
   eit->stack_max = 4;
   while (eit->stack_top >= eit->stack_max)
      eit->stack_max <<= 1;
   eit->stack = g_new (DwIterator*, eit->stack_max);

   /* Construct the iterators. */
   for (w = it->widget, sp = eit->stack_top - 1;
       w->parent != NULL;
       w = w->parent, sp--) {
      eit->stack[sp] = a_Dw_widget_iterator (w->parent, it->mask, FALSE);
      while (TRUE) {
         if (!a_Dw_iterator_next(eit->stack[sp])) {
            g_warning ("BUG in DwExtIterator!");
            return NULL;
         }
         if (eit->stack[sp]->content.type == DW_CONTENT_WIDGET &&
             eit->stack[sp]->content.data.widget == w)
            break;
      }
   }

   eit->stack[eit->stack_top] = it;
   eit->content = it->content;
   return eit;
}


/*
 * This function is similar to a_Dw_ext_iterator_new, but is in many
 * cases faster, since it tries to copy parts of the stack of "base".
 * Used for selection, where the old and new position (which are
 * represented by DwExtIterator) are often quite next to each other.
 * "base" may be NULL.
 */
DwExtIterator* a_Dw_ext_iterator_new_variant (DwExtIterator *base,
                                              DwIterator *it)
{
   /* todo: Not yet implemented, and actually not yet needed very much. */
   return a_Dw_ext_iterator_new (it);
}


/*
 * Move iterator forward and store content in it. Returns TRUE on
 * success.
 */
gboolean a_Dw_ext_iterator_next (DwExtIterator *eit)
{
   DwIterator *it = eit->stack[eit->stack_top];

   if (a_Dw_iterator_next(it)) {
      if (it->content.type == DW_CONTENT_WIDGET) {
         /* Widget: new iterator on stack, to search in this widget. */
         eit->stack_top++;
         a_List_add (eit->stack, eit->stack_top, eit->stack_max);
         eit->stack[eit->stack_top] =
            a_Dw_widget_iterator (it->content.data.widget, it->mask, FALSE);
         return a_Dw_ext_iterator_next (eit);
      } else {
         /* Simply return the content of the iterartor. */
         eit->content = it->content;
         return TRUE;
      }
   } else {
      /* No more data in the top-most widget. */
      if (eit->stack_top > 0) {
         /* Pop iterator from stack, and move to next item in the old one. */
         a_Dw_iterator_free (it);
         eit->stack_top--;
         return a_Dw_ext_iterator_next (eit);
      } else {
         /* Stack is empty. */
         eit->content.type = DW_CONTENT_END;
         return FALSE;
      }
   }
}

/*
 * Move iterator backward and store content in it. Returns TRUE on
 * success.
 */
gboolean a_Dw_ext_iterator_prev (DwExtIterator *eit)
{
   DwIterator *it = eit->stack[eit->stack_top];

   if (a_Dw_iterator_prev(it)) {
      if (it->content.type == DW_CONTENT_WIDGET) {
         /* Widget: new iterator on stack, to search in this widget. */
         eit->stack_top++;
         a_List_add (eit->stack, eit->stack_top, eit->stack_max);
         eit->stack[eit->stack_top] =
            a_Dw_widget_iterator (it->content.data.widget, it->mask, TRUE);
         return a_Dw_ext_iterator_prev (eit);
      } else {
         /* Simply return the content of the iterartor. */
         eit->content = it->content;
         return TRUE;
      }
   } else {
      /* No more data in the top-most widget. */
      if (eit->stack_top > 0) {
         /* Pop iterator from stack, and move to previous item in the
            old one. */
         a_Dw_iterator_free (it);
         eit->stack_top--;
         return a_Dw_ext_iterator_prev (eit);
      } else {
         /* Stack is empty. */
         eit->content.type = DW_CONTENT_START;
         return FALSE;
      }
   }
}

/*
 * Create an exact copy of the iterator, which then can be used
 * independantly of the original one.
 */
DwExtIterator* a_Dw_ext_iterator_clone (DwExtIterator *eit)
{
   int i;
   DwExtIterator *eit2 = g_new (DwExtIterator, 1);

   *eit2 = *eit;
   eit2->stack = g_new (DwIterator*, eit2->stack_max);
   for (i = 0; i <= eit2->stack_top; i++)
      eit2->stack[i] = a_Dw_iterator_clone (eit->stack[i]);

   return eit2;
}

/*
 * Return a value < 0, if first iterator is smaller, > 0, if it is
 * greater, or 0 for equal iterators. It is assumed, that both terators
 * belong to the same widget tree (i.e. they have the same top-level
 * widget).
 */
gint a_Dw_ext_iterator_compare (DwExtIterator *eit1,
                                DwExtIterator *eit2)
{
   int nea = 0;

   while (eit1->stack[nea]->widget == eit2->stack[nea]->widget) {
      if (nea == eit1->stack_top || nea == eit2->stack_top)
         break;
      nea++;
   }
   if (eit1->stack[nea]->widget != eit2->stack[nea]->widget)
      nea--;
   return a_Dw_iterator_compare (eit1->stack[nea], eit2->stack[nea]);
}

/*
 * Free memory of iterator.
 */
void a_Dw_ext_iterator_free (DwExtIterator *eit)
{
   int i;
   for (i = 0; i <= eit->stack_top; i++)
      a_Dw_iterator_free (eit->stack[i]);
   g_free (eit->stack);
   g_free (eit);
}

/*
 * Create a new word iterator. <widget> is the top of the widget
 * tree over which is iterated.
 */
DwWordIterator* a_Dw_word_iterator_new (DwWidget *widget)
{
   DwWordIterator *it;
   DwIterator *it0;

   it = g_new (DwWordIterator, 1);
   it->word = NULL;
   it0 = a_Dw_widget_iterator (widget, DW_CONTENT_TEXT | DW_CONTENT_WIDGET,
                               FALSE);
   it->iterator = a_Dw_ext_iterator_new (it0);
   it->word_splitpos = NULL;
   it->content_hl_start = -1;
   it->content_hl_end = -1;
   return it;
}

/*
 * Move iterator forward and store word in it. Returns TRUE on
 * success.
 */
gboolean a_Dw_word_iterator_next (DwWordIterator *it)
{
   if (it->word) {
      g_free (it->word);
      it->word = NULL;
   }

   while (it->word_splitpos == NULL ||
          it->word_splitpos[2 * (it->word_pos + 1)] == -1) {
      if (it->word_splitpos) {
         g_free (it->word_splitpos);
         it->word_splitpos = NULL;
         it->content_hl_start = -1;
         it->content_hl_end = -1;
      }
      if (!a_Dw_ext_iterator_next (it->iterator))
         return FALSE;
      it->word_splitpos =
         a_Misc_strsplitpos (it->iterator->content.data.text, " \t\n");
      it->word_pos = -1;
   }

   it->word_pos++;
   it->word = a_Misc_strpdup (it->iterator->content.data.text,
                              it->word_splitpos[2 * it->word_pos],
                              it->word_splitpos[2 * it->word_pos + 1]);
   return TRUE;
}


/*
 * Move iterator backward and store word in it. Returns TRUE on
 * success.
 */
gboolean a_Dw_word_iterator_prev (DwWordIterator *it)
{
   if (it->word) {
      g_free (it->word);
      it->word = NULL;
   }

   while (it->word_splitpos == NULL || it->word_pos == 0) {
      if (it->word_splitpos) {
         g_free (it->word_splitpos);
         it->word_splitpos = NULL;
         it->content_hl_start = -1;
         it->content_hl_end = -1;
      }
      if (!a_Dw_ext_iterator_prev (it->iterator))
         return FALSE;
      it->word_splitpos =
         a_Misc_strsplitpos(it->iterator->content.data.text, " \t\n");
      it->word_pos = 0;
      while (it->word_splitpos[2 * it->word_pos] != -1)
         it->word_pos++;
   }

   it->word_pos--;
   it->word = a_Misc_strpdup (it->iterator->content.data.text,
                              it->word_splitpos[2 * it->word_pos],
                              it->word_splitpos[2 * it->word_pos + 1]);
   return TRUE;
}

/*
 * Highlight a part of the current word. Unhighlight the current word
 * by passing -1 as start.
 */
void a_Dw_word_iterator_highlight (DwWordIterator *it,
                                   gint start,
                                   gint end,
                                   DwHighlightLayer layer)
{
   gint new_start, new_end;

   if (start == -1) {
      /* Unhighlight the whole content word.
       * todo: This works incorrect, by unhighlighting the whole
       * current content of the DwExtIterator. Anyway, a correct
       * behavior is not needed. */
      it->content_hl_start = -1;
      it->content_hl_end = -1;
      a_Dw_ext_iterator_unhighlight (it->iterator, layer);
   } else {
      new_start = it->word_splitpos[2 * it->word_pos] + start;
      new_end = it->word_splitpos[2 * it->word_pos] + end;

      if (it->content_hl_start == -1) {
         /* nothing selected yet */
         it->content_hl_start = new_start;
         it->content_hl_end = new_end;
      } else {
         it->content_hl_start = MIN (it->content_hl_start, new_start);
         it->content_hl_end = MAX (it->content_hl_end, new_end);
      }

      a_Dw_ext_iterator_highlight (it->iterator, it->content_hl_start,
                                   it->content_hl_end, layer);
   }
}

void a_Dw_word_iterator_get_allocation (DwWordIterator *it,
                                        gint start,
                                        gint end,
                                        DwAllocation *allocation)
{
   /* todo: Implement this. (Although it is not used yet.) */
   g_assert_not_reached ();
}

void a_Dw_word_iterator_scroll_to (DwWordIterator *it1,
                                   DwWordIterator *it2,
                                   gint start,
                                   gint end,
                                   DwHPosition hpos,
                                   DwVPosition vpos)
{
   gint real_start, real_end;

   real_start = it1->word_splitpos[2 * it1->word_pos] + start;
   real_end = it2->word_splitpos[2 * it2->word_pos] + end;
   a_Dw_ext_iterator_scroll_to (it1->iterator, it2->iterator,
                                real_start, real_end, hpos, vpos);
}


DwWordIterator* a_Dw_word_iterator_clone (DwWordIterator *it)
{
   DwWordIterator *it2;

   it2 = g_new (DwWordIterator, 1);
   *it2 = *it;
   it2->iterator = a_Dw_ext_iterator_clone (it->iterator);
   it2->word = it->word ? g_strdup (it->word) : NULL;
   it2->word_splitpos =
      it->word_splitpos ? a_Misc_strsplitposdup (it->word_splitpos) : NULL;
   return it2;
}

void a_Dw_word_iterator_free (DwWordIterator *it)
{
   a_Dw_ext_iterator_free (it->iterator);
   g_free (it->word);
   g_free (it->word_splitpos);
   g_free (it);
}
