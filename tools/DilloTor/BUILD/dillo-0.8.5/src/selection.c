/*
 * File: selection.c
 *
 * Copyright 2003 Sebastian Geerken <s.geerken@ping.de>,
 *                Eric Gaudet <eric@rti-zone.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * See doc/Selection.txt for informations about this module.
 */

#include "selection.h"
#include "strbuf.h"
#include "dw_gtk_viewport.h"
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

/*#define DEBUG_LEVEL 2*/
#include "debug.h"

static void Selection_reset_selection          (Selection *selection);
static void Selection_reset_link               (Selection *selection);
static void Selection_switch_link_to_selection (Selection *selection,
                                                DwIterator *it,
                                                gint32 char_pos);
static void Selection_adjust_selection         (Selection *selection,
                                                DwIterator *it,
                                                gint32 char_pos);
static gint Selection_correct_char_pos         (DwExtIterator *it,
                                                gint32 char_pos);
static void Selection_highlight                (Selection *selection,
                                                gboolean fl);
static void  Selection_highlight0              (gboolean fl,
                                                DwExtIterator *from,
                                                gint from_char,
                                                DwExtIterator *to,
                                                gint to_char);
static void Selection_copy                     (Selection *selection);

/* Not defined as static, to avoid compiler warnings. */
char *selection_state_descr[] = { "none", "selecting", "selected" };

Selection *a_Selection_new (void)
{
   Selection *selection;

   selection = g_new (Selection, 1);

   selection->selection_state = SELECTION_NONE;
   selection->from = NULL;
   selection->to = NULL;

   selection->link_state = SELECTION_LINK_NONE;
   selection->link = NULL;

   selection->dclick_callback = NULL;
   selection->callback_data = NULL;

   selection->owner = NULL;
   return selection;
}


/*
 * Sets a callback function for double click, this is normally
 * the function toggling the full screen mode.
 * (More than one is never needed, so this instead of a signal.)
 */
void a_Selection_set_dclick_callback (Selection *selection,
                                      void (*fn) (gpointer data),
                                      gpointer data)
{
   g_return_if_fail (selection->dclick_callback == NULL);
   selection->dclick_callback = fn;
   selection->callback_data = data;
}

/*
 * Set the widget that owns the selection.
 */
void a_Selection_set_owner(Selection *selection, gpointer owner)
{
   selection->owner = owner;
   a_Selection_init_selection(owner);
}

void a_Selection_reset (Selection *selection)
{
   Selection_reset_selection (selection);
   Selection_reset_link (selection);
}


void a_Selection_free (Selection *selection)
{
   a_Selection_reset (selection);
   g_free (selection);
}

static void Selection_reset_selection (Selection *selection)
{
   if (selection->from)
      a_Dw_ext_iterator_free(selection->from);
   selection->from = NULL;
   if (selection->to)
      a_Dw_ext_iterator_free(selection->to);
   selection->to = NULL;
   selection->selection_state = SELECTION_NONE;
}


static void Selection_reset_link (Selection *selection)
{
   if (selection->link)
      a_Dw_ext_iterator_free(selection->link);
   selection->link = NULL;
   selection->link_state = SELECTION_LINK_NONE;
}

gint a_Selection_button_press  (Selection *selection,
                                DwIterator *it,
                                gint char_pos,
                                gint link,
                                GdkEventButton *event,
                                gboolean within_content)
{
   DwWidget *it_widget = it->widget;
   gboolean ret = FALSE, dummy;

   DEBUG_MSG (3, "PRESS%s(%s, %d, %d): %s -> ...\n",
              (event && event->type == GDK_2BUTTON_PRESS) ? "2" : "",
              a_Dw_iterator_text (it), char_pos, link,
              selection_state_descr[selection->selection_state]);

#if defined (DEBUG_LEVEL) && 1 >= DEBUG_LEVEL
   a_Dw_widget_print_tree (GTK_DW_VIEWPORT(it->widget->viewport)->child);
#endif

   if (event && event->button == 1 &&
       !within_content && event->type == GDK_2BUTTON_PRESS) {
      /* When the user double-clicks on empty parts, call the callback
       * function instead of normal processing. Used for full screen
        * mode. */
      if (selection->dclick_callback)
         selection->dclick_callback (selection->callback_data);
      /* Reset everything, so that a_Selection_release will ignore the
       * "release" event following soon. */
      Selection_highlight (selection, FALSE);
      a_Selection_reset (selection);
      ret = TRUE;
   } else {
      if (link != -1) {
         /* link handling */
         if (event) {
            gtk_signal_emit_by_name (GTK_OBJECT (it_widget),
                                     "link_pressed", link, -1, -1, event,
                                     &dummy);
            Selection_reset_link (selection);
            selection->link_state = SELECTION_LINK_PRESSED;
            selection->link_button = event->button;
            selection->link = a_Dw_ext_iterator_new (it);

            /* It may be that the user has pressed on something activatable
             * (link != -1), but there is no contents, e.g. with images
             * without ALTernative text. */
            if (selection->link) {
               selection->link_char =
                  Selection_correct_char_pos (selection->link, char_pos);
               selection->link_number = link;
            }
            /* We do not return the value of the signal function (dummy),
             * but we do actually process this event. */
            ret = TRUE;
         }
      } else {
         /* normal selection handling */
         if (event && event->button == 1) {
            Selection_highlight (selection, FALSE);
            Selection_reset_selection (selection);

            selection->from = a_Dw_ext_iterator_new (it);
            /* a_Dw_ext_iterator_new may return NULL, if the page has no
             * contents. */
            if (selection->from) {
               selection->selection_state = SELECTION_SELECTING;
               selection->from_char =
                  Selection_correct_char_pos (selection->from, char_pos);
               selection->to = a_Dw_ext_iterator_clone (selection->from);
               selection->to_char =
                  Selection_correct_char_pos (selection->to, char_pos);
               ret = TRUE;
            } else
               /* if there is no content */
               ret = FALSE;
         }
      }
   }

   DEBUG_MSG (3, "  ... -> %s, processed = %s\n",
              selection_state_descr[selection->selection_state],
              ret ? "TRUE" : "FALSE");
   return ret;
}


gint a_Selection_button_release (Selection *selection,
                                 DwIterator *it,
                                 gint char_pos,
                                 gint link,
                                 GdkEventButton *event,
                                 gboolean within_content)
{
   DwWidget *it_widget = it->widget;
   gboolean ret = FALSE, dummy;

   DEBUG_MSG (3, "RELEASE(%s, %d, %d): %s -> ...\n",
              a_Dw_iterator_text (it), char_pos, link,
              selection_state_descr[selection->selection_state]);

   if (selection->link_state == SELECTION_LINK_PRESSED &&
       event && event->button == selection->link_button) {
      /* link handling */
      ret = TRUE;
      if (link != -1)
         gtk_signal_emit_by_name (GTK_OBJECT (it_widget),
                                  "link_released", link, -1, -1, event,
                                  &dummy);

      /* The link where the user clicked the mouse button? */
      if (link == selection->link_number) {
         Selection_reset_link (selection);
         gtk_signal_emit_by_name (GTK_OBJECT (it_widget),
                                  "link_clicked", link, -1, -1, event,
                                  &dummy);
      } else {
         if (event->button == 1)
            /* Reset links and switch to selection mode. The selection
             * state will be set to SELECTING, which is handled some lines
             * below. */
            Selection_switch_link_to_selection (selection, it, char_pos);
      }
   }

   if (selection->selection_state == SELECTION_SELECTING &&
       event && event->button == 1) {
      /* normal selection */
      ret = TRUE;
      Selection_adjust_selection (selection, it, char_pos);

      if (a_Dw_ext_iterator_compare (selection->from, selection->to) == 0 &&
          selection->from_char == selection->to_char)
         /* nothing selected */
         Selection_reset_selection (selection);
      else {
         Selection_copy (selection);
         selection->selection_state = SELECTION_SELECTED;
      }
   }

   DEBUG_MSG (3, "  ... -> %s, processed = %s\n",
              selection_state_descr[selection->selection_state],
              ret ? "TRUE" : "FALSE");
   return ret;
}


gint a_Selection_button_motion (Selection *selection,
                                DwIterator *it,
                                gint char_pos,
                                gint link,
                                GdkEventButton *event,
                                gboolean within_content)
{
   DEBUG_MSG (3, "MOTION (%s, %d, %d): %s -> ...\n",
              a_Dw_iterator_text (it), char_pos, link,
              selection_state_descr[selection->selection_state]);

   if (selection->link_state == SELECTION_LINK_PRESSED) {
      /* link handling */
      if (link != selection->link_number)
         /* No longer the link where the user clicked the mouse button.
          * Reset links and switch to selection mode. */
         Selection_switch_link_to_selection (selection, it, char_pos);
      /* Still in link: do nothing. */
   } else if (selection->selection_state == SELECTION_SELECTING) {
      /* selection */
      Selection_adjust_selection (selection, it, char_pos);
   }

   DEBUG_MSG (3, "  ... -> %s, processed = TRUE\n",
              selection_state_descr[selection->selection_state]);
   return TRUE;
}

/*
 * This function is called when the user decides not to activate a link,
 * but instead select text.
 */
static void Selection_switch_link_to_selection (Selection *selection,
                                                DwIterator *it,
                                                gint32 char_pos)
{
   /* It may be that selection->link is NULL, see a_Selection_button_press. */
   if (selection->link) {
      /* Reset old selection. */
      Selection_highlight (selection, FALSE);
      Selection_reset_selection (selection);

      /* Transfer link state into selection state. */
      selection->from = a_Dw_ext_iterator_clone (selection->link);
      selection->from_char = selection->link_char;
      selection->to = a_Dw_ext_iterator_new_variant (selection->from, it);
      selection->to_char =
         Selection_correct_char_pos (selection->to, char_pos);
      selection->selection_state = SELECTION_SELECTING;

      /* Reset link status. */
      Selection_reset_link (selection);

      Selection_highlight (selection, TRUE);

      DEBUG_MSG (2, "   after switching: from (%s, %d) to (%s, %d)\n",
                 a_Dw_ext_iterator_text (selection->from),
                 selection->from_char,
                 a_Dw_ext_iterator_text (selection->to), selection->to_char);
   } else {
      /* A link was pressed on, but there is nothing to select. Reset
       * everything. */
      Selection_reset_selection (selection);
      Selection_reset_link (selection);
   }
}

/*
 * This function is used by a_Selection_button_motion and
 * a_Selection_button_release, and changes the second limit of the already
 * existing selection region.
 */
static void Selection_adjust_selection (Selection *selection,
                                        DwIterator *it,
                                        gint32 char_pos)
{
   DwExtIterator *new_to;
   gint new_to_char, cmp_old, cmp_new, cmp_diff, len;
   gboolean brute_highlighting = FALSE;

   new_to = a_Dw_ext_iterator_new_variant (selection->to, it);
   new_to_char = Selection_correct_char_pos (new_to, char_pos);

   cmp_old = a_Dw_ext_iterator_compare (selection->to, selection->from);
   cmp_new = a_Dw_ext_iterator_compare (new_to, selection->from);

   if (cmp_old == 0 || cmp_new == 0) {
      /* Either before, or now, the limits differ only by the character
       * position. */
      brute_highlighting = TRUE;
   } else if (cmp_old * cmp_new < 0) {
      /* The selection order has changed, i.e. the user moved the selection
       * end again beyond the position he started. */
      brute_highlighting = TRUE;
   } else {
      /* Here, cmp_old and cmp_new are equivalent and != 0. */
      cmp_diff = a_Dw_ext_iterator_compare (new_to, selection->to);

      DEBUG_MSG (2, "Selection_adjust_selection: cmp_old = cmp_new = %d, "
                 "cmp_diff = %d\n", cmp_old, cmp_diff);

      if (cmp_old * cmp_diff > 0) {
         /* The user has enlarged the selection. Highlight the difference. */
         if (cmp_diff < 0) {
            len = Selection_correct_char_pos (selection->to, SELECTION_EOW);
            Selection_highlight0 (TRUE, new_to, new_to_char, selection->to,
                                  len + 1);
         } else {
            Selection_highlight0 (TRUE, selection->to, 0, new_to, new_to_char);
         }
      } else {
         if (cmp_old * cmp_diff < 0) {
            /* The user has reduced the selection. Unighlight the difference.
             */
            Selection_highlight0 (FALSE, selection->to, 0, new_to, 0);
         }

         /* Otherwise, the user has changed the position only slightly.
          * In both cases, re-highlight the new position. */
         if (cmp_old < 0) {
            len = Selection_correct_char_pos (new_to, SELECTION_EOW);
            a_Dw_ext_iterator_highlight (new_to, new_to_char, len + 1,
                                         DW_HIGHLIGHT_SELECTION);
         } else
            a_Dw_ext_iterator_highlight (new_to, 0, new_to_char,
                                         DW_HIGHLIGHT_SELECTION);
      }
   }

   if (brute_highlighting)
      Selection_highlight (selection, FALSE);

   a_Dw_ext_iterator_free(selection->to);
   selection->to = new_to;
   selection->to_char = new_to_char;

   if (brute_highlighting)
      Selection_highlight (selection, TRUE);

   DEBUG_MSG (2, "   selection now from (%s, %d) to (%s, %d)\n",
              a_Dw_ext_iterator_text (selection->from), selection->from_char,
              a_Dw_ext_iterator_text (selection->to), selection->to_char);
}

/*
 * This function deals especially with the case that a widget passes
 * SELECTION_EOW. See Selection.txt in the doc dir for more informations.
 */
static gint Selection_correct_char_pos (DwExtIterator *it,
                                        gint32 char_pos)
{
   DwIterator *top = it->stack[it->stack_top];
   gint len;

   if (top->content.type == DW_CONTENT_TEXT)
      len = strlen(top->content.data.text);
   else
      len = 1;

   return MIN(char_pos, len);
}


static void Selection_highlight (Selection *selection,
                                 gboolean fl)
{
   Selection_highlight0 (fl, selection->from, selection->from_char,
                         selection->to, selection->to_char);
}


static void  Selection_highlight0 (gboolean fl,
                                   DwExtIterator *from,
                                   gint from_char,
                                   DwExtIterator *to,
                                   gint to_char)
{
   DwExtIterator *a, *b, *i;
   gint cmp, a_char, b_char;
   gboolean start;

   if (from && to) {
      DEBUG_MSG (2, "  %shighlighting from %s / %d to %s / %d\n",
                 fl ? "" : "un", a_Dw_ext_iterator_text (from),
                 from_char, a_Dw_ext_iterator_text (to), to_char);
      cmp = a_Dw_ext_iterator_compare (from, to);
      if (cmp == 0) {
         if (fl) {
            if (from_char < to_char)
               a_Dw_ext_iterator_highlight (from, from_char, to_char,
                                            DW_HIGHLIGHT_SELECTION);
            else
               a_Dw_ext_iterator_highlight (from, to_char, from_char,
                                            DW_HIGHLIGHT_SELECTION);
         } else
            a_Dw_ext_iterator_unhighlight (from, DW_HIGHLIGHT_SELECTION);
         return;
      }

      if (cmp < 0) {
         a = from;
         a_char = from_char;
         b = to;
         b_char = to_char;
      } else {
         a = to;
         a_char = to_char;
         b = from;
         b_char = from_char;
      }

      for (i = a_Dw_ext_iterator_clone (a), start = TRUE;
           (cmp = a_Dw_ext_iterator_compare (i, b)) <= 0;
           a_Dw_ext_iterator_next (i), start = FALSE) {
         if (i->content.type == DW_CONTENT_TEXT) {
            if (fl) {
               if (start) {
                  DEBUG_MSG (2, "    highlighting %s from %d to %d\n",
                             a_Dw_ext_iterator_text (i), a_char,
                             strlen (i->content.data.text) + 1);
                  a_Dw_ext_iterator_highlight (i, a_char,
                                               strlen (i->content.data.text)
                                               + 1,
                                               DW_HIGHLIGHT_SELECTION);
               } else if (cmp == 0) {
                  /* the end */
                  DEBUG_MSG (2, "    highlighting %s from %d to %d\n",
                             a_Dw_ext_iterator_text (i), 0, b_char);
                  a_Dw_ext_iterator_highlight (i, 0, b_char,
                                               DW_HIGHLIGHT_SELECTION);
               } else {
                  DEBUG_MSG (2, "    highlighting %s from %d to %d\n",
                             a_Dw_ext_iterator_text (i), 0,
                             strlen (i->content.data.text) + 1);
                  a_Dw_ext_iterator_highlight (i, 0,
                                               strlen (i->content.data.text)
                                               + 1,
                                               DW_HIGHLIGHT_SELECTION);
               }
            } else {
               DEBUG_MSG (2, "    unhighlighting %s\n",
                          a_Dw_ext_iterator_text (i));
               a_Dw_ext_iterator_unhighlight (i, DW_HIGHLIGHT_SELECTION);
            }
         }
      }
      a_Dw_ext_iterator_free (i);
   }
}


static void Selection_copy(Selection *selection)
{
   DwIterator *si;
   DwExtIterator *a, *b, *i;
   gint cmp, a_char, b_char;
   gboolean start;
   gchar *tmp;
   Strbuf_t *strbuf;

   if (selection->from && selection->to) {
      strbuf = a_Strbuf_new ();

      cmp = a_Dw_ext_iterator_compare (selection->from, selection->to);
      if (cmp == 0) {
         if (selection->from->content.type == DW_CONTENT_TEXT) {
            si = selection->from->stack[selection->from->stack_top];
            if (selection->from_char < selection->to_char)
               tmp = g_strndup(si->content.data.text + selection->from_char,
                               selection->to_char - selection->from_char);
            else
               tmp = g_strndup(si->content.data.text + selection->to_char,
                               selection->from_char - selection->to_char);
            a_Strbuf_append (strbuf, tmp);
            g_free (tmp);
         }
      } else {
         if (cmp < 0) {
            a = selection->from;
            a_char = selection->from_char;
            b = selection->to;
            b_char = selection->to_char;
         } else {
            a = selection->to;
            a_char = selection->to_char;
            b = selection->from;
            b_char = selection->from_char;
         }

         for (i = a_Dw_ext_iterator_clone (a), start = TRUE;
              (cmp = a_Dw_ext_iterator_compare (i, b)) <= 0;
              a_Dw_ext_iterator_next (i), start = FALSE) {
            si = i->stack[i->stack_top];
            switch (si->content.type) {
            case DW_CONTENT_TEXT:
               if (start) {
                  tmp = g_strndup(si->content.data.text + a_char,
                                  strlen (i->content.data.text) - a_char);
                  a_Strbuf_append (strbuf, tmp);
                  g_free (tmp);
               } else if (cmp == 0) {
                  /* the end */
                  tmp = g_strndup(si->content.data.text, b_char);
                  a_Strbuf_append (strbuf, tmp);
                  g_free (tmp);
               } else
                  a_Strbuf_append (strbuf, si->content.data.text);

               if (si->content.space && cmp != 0)
                  a_Strbuf_append (strbuf, " ");

               break;

            case DW_CONTENT_BREAK:
               if (si->content.data.break_space > 0)
                  a_Strbuf_append (strbuf, "\n\n");
               else
                  a_Strbuf_append (strbuf, "\n");
               break;
            default:
               /* Make pedantic compilers happy. Especially
                * DW_CONTENT_WIDGET is never returned by a DwExtIterator. */
               break;
            }
         }
         a_Dw_ext_iterator_free (i);
      }
      a_Selection_set_selection(selection->owner, a_Strbuf_chars(strbuf));
      a_Strbuf_free (strbuf);
   }
}


/* GENERAL SELECTION STUFF */

/*
 * Local data
 */
static char *selection = NULL;

/*
 * Sets the widget to copy the selection from.
 */
void a_Selection_init_selection(GtkWidget *widget)
{
   gtk_signal_connect(GTK_OBJECT(widget), "selection_clear_event",
      GTK_SIGNAL_FUNC(a_Selection_clear_selection_callback), NULL);
   gtk_signal_connect(GTK_OBJECT(widget), "selection_get",
      GTK_SIGNAL_FUNC(a_Selection_give_selection_callback), NULL);
   gtk_selection_add_target(widget, GDK_SELECTION_PRIMARY,
      GDK_SELECTION_TYPE_STRING, 1);
}

/*
 * Sets the selection, and associates it with the current widget for
 * copying.
 */
void a_Selection_set_selection(GtkWidget *widget, gchar *str)
{
   //g_print("a_Selection_set_selection\n  old: %s\n", Selection);
   if (gtk_selection_owner_set(widget,
                               GDK_SELECTION_PRIMARY,
                               GDK_CURRENT_TIME)) {
      /* --EG: Why would it fail? (this question points to that the
       * GTK+ example tests the return value.) */

      /* As the clear-selection callback is not being called automatically
       * before setting the new value, it'll be cleared here.  --Jcid
       * todo: this selection code needs a revision */
      g_free(selection);
      selection = g_strdup(str);
   }
}

/*
 * Callback for Paste (when another application wants the selection)
 */
void a_Selection_give_selection_callback(GtkWidget *widget,
        GtkSelectionData *data, guint info, guint time)
{
   gtk_selection_data_set(data, GDK_SELECTION_TYPE_STRING,
                          8, selection, strlen(selection));
}

/*
 * Clear selection
 */
gint a_Selection_clear_selection_callback(GtkWidget *widget,
                                         GdkEventSelection *event)
{
   //g_print("a_Selection_clear_selection_callback\n");
   g_free(selection);
   selection = NULL;

   /* here we should un-highlight the selected text */
   return TRUE;
}

