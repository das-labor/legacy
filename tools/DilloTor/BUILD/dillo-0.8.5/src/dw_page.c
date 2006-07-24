/*
 * File: dw_page.c
 *
 * Copyright (C) 1997       Raph Levien <raph@acm.org>
 * Copyright (C) 1999       Luca Rota <drake@freemail.it>
 * Copyright (C) 2001-2003  Sebastian Geerken <s.geerken@ping.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * This module contains the Dw_page widget, which is the "back end" to
 * Web text widgets including html.
 */

#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include <gtk/gtk.h>

#include "msg.h"
#include "list.h"
#include "dw_page.h"
#include "dw_marshal.h"
#include "dw_gtk_viewport.h"

#include "prefs.h"

#define DEBUG_REWRAP_LEVEL    0
#define DEBUG_SIZE_LEVEL     10
#define DEBUG_EVENT_LEVEL     0
#define DEBUG_ITERATOR_LEVEL  0

/*#define DEBUG_LEVEL 10*/
#include "debug.h"

#define MAX3(a, b, c) (MAX (a, MAX (b, c)))

static void Dw_page_init          (DwPage *page);
static void Dw_page_class_init    (DwPageClass *klass);

static void Dw_page_destroy       (GtkObject *object);

static void Dw_page_size_request      (DwWidget *widget,
                                      DwRequisition *requisition);
static void Dw_page_get_extremes      (DwWidget *widget,
                                       DwExtremes *extremes);
static void Dw_page_size_allocate     (DwWidget *widget,
                                       DwAllocation *allocation);
static void Dw_page_mark_change       (DwWidget *widget,
                                       gint ref);
static void Dw_page_set_width         (DwWidget *widget,
                                       gint32 width);
static void Dw_page_set_ascent        (DwWidget *widget,
                                       gint32 ascent);
static void Dw_page_set_descent       (DwWidget *widget,
                                       gint32 descent);
static void Dw_page_draw              (DwWidget *page,
                                       DwRectangle *area,
                                       GdkEventExpose *event);
static gboolean Dw_page_button_press  (DwWidget *widget,
                                       gint32 x,
                                       gint32 y,
                                       GdkEventButton *event);
static gboolean Dw_page_button_release(DwWidget *widget,
                                       gint32 x,
                                       gint32 y,
                                       GdkEventButton *event);
static gboolean Dw_page_motion_notify (DwWidget *widget,
                                       gint32 x,
                                       gint32 y,
                                       GdkEventMotion *event);
static gboolean Dw_page_leave_notify  (DwWidget *widget,
                                       DwWidget *next_widget,
                                       GdkEventMotion *event);


static void Dw_page_add               (DwContainer *container,
                                       DwWidget *widget);
static void Dw_page_remove            (DwContainer *container,
                                       DwWidget *widget);
static void Dw_page_forall            (DwContainer *container,
                                       DwCallback callback,
                                       gpointer callback_data);
static void Dw_page_real_word_wrap    (DwPage *page,
                                       gint word_ind);

static DwIterator* Dw_page_iterator               (DwWidget *widget,
                                                   gint mask,
                                                   gboolean at_end);
static gboolean    Dw_page_iterator_next          (DwIterator *it);
static gboolean    Dw_page_iterator_prev          (DwIterator *it);
static void        Dw_page_iterator_highlight      (DwIterator *it,
                                                    gint start,
                                                    gint end,
                                                    DwHighlightLayer layer);
static void        Dw_page_iterator_get_allocation (DwIterator *it,
                                                    gint start,
                                                    gint end,
                                                    DwAllocation *allocation);

static void Dw_page_rewrap            (DwPage *page);

/*
 * Returns the x offset (the indentation plus any offset needed for
 * centering or right justification) for the line. The offset returned
 * is relative to the page *content* (i.e. without border etc.).
 */
#define Dw_page_line_x_offset(page, line) \
   ( (page)->inner_padding + (line)->left_offset + \
     ((line) == (page)->lines ? (page)->line1_offset_eff : 0) )

/*
 * Like Dw_style_box_offset_x, but relative to the allocation (i.e.
 * including border etc.).
 */
#define Dw_page_line_total_x_offset(page, line) \
   ( Dw_page_line_x_offset (page, line) + \
     p_Dw_style_box_offset_x (((DwWidget*)(page))->style) )

/*
 * Returns the y offset (within the widget) of a line.
 */
#define Dw_page_line_total_y_offset(page, line) \
   Dw_page_line_total_y_offset_allocation(page, line, \
                                          &((DwWidget*)(page))->allocation)

/*
 * Like Dw_page_line_total_y_offset, but with the allocation as parameter.
 */
#define Dw_page_line_total_y_offset_allocation(page, line, allocation) \
   ( (allocation)->ascent - (page)->lines[0].ascent + (line)->top )

/*
 * Similar to Dw_page_line_total_y_offset.
 */
#define Dw_page_line_total_y_offset_i(page, line_index) \
   Dw_page_line_total_y_offset(page, &(page)->lines[line_index])

#define Dw_page_word_wrap(page, word_ind) \
   (((DwPageClass*)(((GtkObject*)(page))->klass))->word_wrap (page, word_ind))

enum
{
   LINK_ENTERED,
   LINK_PRESSED,
   LINK_RELEASED,
   LINK_CLICKED,
   LAST_SIGNAL
};

static DwContainerClass *parent_class;
static guint page_signals[LAST_SIGNAL] = { 0 };


/*
 * Return the type of DwPage
 */
GtkType a_Dw_page_get_type (void)
{
   static GtkType type = 0;

   if (!type) {
      GtkTypeInfo info = {
         "DwPage",
         sizeof (DwPage),
         sizeof (DwPageClass),
         (GtkClassInitFunc) Dw_page_class_init,
         (GtkObjectInitFunc) Dw_page_init,
         (GtkArgSetFunc) NULL,
         (GtkArgGetFunc) NULL,
         (GtkClassInitFunc) NULL
      };

      type = gtk_type_unique (DW_TYPE_CONTAINER, &info);
   }

   return type;
}


/*
 * Create a new DwPage
 */
DwWidget* a_Dw_page_new (void)
{
   GtkObject *object;

   object = gtk_object_new (DW_TYPE_PAGE, NULL);
   DBG_OBJ_CREATE (object, "DwPage");
   return DW_WIDGET (object);
}


/*
 * Initialize a DwPage
 */
static void Dw_page_init (DwPage *page)
{
   DW_WIDGET_SET_FLAGS (page, DW_USES_HINTS);

   page->list_item = FALSE;
   page->inner_padding = 0;
   page->line1_offset = 0;
   page->line1_offset_eff = 0;
   page->ignore_line1_offset_sometimes = FALSE;

   /*
    * The initial sizes of page->lines and page->words should not be
    * too high, since this will waste much memory with tables
    * containing many small cells. The few more calls to realloc
    * should not decrease the speed considerably.
    * (Current setting is for minimal memory usage. An interesting fact
    * is that high values decrease speed due to memory handling overhead!)
    * todo: Some tests would be useful.
    */
   page->num_lines_max = 1; /* 2 */
   page->num_lines = 0;
   page->lines = NULL;      /* g_new(DwPageLine, page->num_lines_max); */

   DBG_OBJ_SET_NUM(page, "num_lines", page->num_lines);

   page->num_words_max = 1; /* 8 */
   page->num_words = 0;
   page->words = NULL;      /* g_new(DwPageWord, page->num_words_max); */

   page->last_line_width = 0;
   page->last_line_par_min = 0;
   page->last_line_par_max = 0;
   page->wrap_ref = -1;

   DBG_OBJ_SET_NUM(page, "last_line_width", page->last_line_width);
   DBG_OBJ_SET_NUM(page, "last_line_par_min", page->last_line_par_min);
   DBG_OBJ_SET_NUM(page, "last_line_par_max", page->last_line_par_max);
   DBG_OBJ_SET_NUM(page, "wrap_ref", page->wrap_ref);

   page->hover_link = -1;

   /* random values */
   page->avail_width = 100;
   page->avail_ascent = 100;
   page->avail_descent = 0;

   page->hover_tooltip = NULL;
}

/*
 * Initialize the DwPage's class
 */
static void Dw_page_class_init (DwPageClass *klass)
{
   GtkObjectClass *object_class;
   DwWidgetClass *widget_class;
   DwContainerClass *container_class;

   object_class = (GtkObjectClass*) klass;
   widget_class = (DwWidgetClass*) klass;
   container_class = (DwContainerClass*) klass;
   parent_class = gtk_type_class (DW_TYPE_CONTAINER);

   page_signals[LINK_ENTERED] =
      gtk_signal_new ("link_entered",
                      GTK_RUN_LAST,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (DwPageClass, link_entered),
                      p_Dw_marshal_link_enter,
                      GTK_TYPE_BOOL,
                      3, GTK_TYPE_INT, GTK_TYPE_INT, GTK_TYPE_INT);
   page_signals[LINK_PRESSED] =
      gtk_signal_new ("link_pressed",
                      GTK_RUN_LAST,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (DwPageClass, link_pressed),
                      p_Dw_marshal_link_button,
                      GTK_TYPE_BOOL,
                      4, GTK_TYPE_INT, GTK_TYPE_INT, GTK_TYPE_INT,
                      GTK_TYPE_GDK_EVENT);
   page_signals[LINK_RELEASED] =
      gtk_signal_new ("link_released",
                      GTK_RUN_LAST,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (DwPageClass, link_released),
                      p_Dw_marshal_link_button,
                      GTK_TYPE_BOOL,
                      4, GTK_TYPE_INT, GTK_TYPE_INT, GTK_TYPE_INT,
                      GTK_TYPE_GDK_EVENT);
   page_signals[LINK_CLICKED] =
      gtk_signal_new ("link_clicked",
                      GTK_RUN_LAST,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (DwPageClass, link_clicked),
                      p_Dw_marshal_link_button,
                      GTK_TYPE_BOOL,
                      4, GTK_TYPE_INT, GTK_TYPE_INT, GTK_TYPE_INT,
                      GTK_TYPE_GDK_EVENT);
   gtk_object_class_add_signals (object_class, page_signals, LAST_SIGNAL);

   object_class->destroy = Dw_page_destroy;

   widget_class->size_request = Dw_page_size_request;
   widget_class->get_extremes = Dw_page_get_extremes;
   widget_class->size_allocate = Dw_page_size_allocate;
   widget_class->mark_size_change = Dw_page_mark_change;
   widget_class->mark_extremes_change = Dw_page_mark_change;
   widget_class->set_width = Dw_page_set_width;
   widget_class->set_ascent = Dw_page_set_ascent;
   widget_class->set_descent = Dw_page_set_descent;
   widget_class->draw = Dw_page_draw;
   widget_class->button_press_event = Dw_page_button_press;
   widget_class->button_release_event = Dw_page_button_release;
   widget_class->motion_notify_event = Dw_page_motion_notify;
   widget_class->leave_notify_event = Dw_page_leave_notify;
   widget_class->iterator = Dw_page_iterator;

   container_class->add = Dw_page_add;
   container_class->remove = Dw_page_remove;
   container_class->forall = Dw_page_forall;

   klass->link_entered = NULL;
   klass->link_pressed = NULL;
   klass->link_released = NULL;
   klass->link_clicked = NULL;
   klass->word_wrap = Dw_page_real_word_wrap;
}

/*
 * Destroy the page (standard Gtk+ function)
 */
static void Dw_page_destroy (GtkObject *object)
{
   DwWidget *widget = DW_WIDGET (object);
   DwPage *page = DW_PAGE (object);
   DwPageWord *word;
   gint i;

   _MSG ("Dw_page_destroy\n");

   for (i = 0; i < page->num_words; i++) {
      word = &page->words[i];
      if (word->content.type == DW_CONTENT_WIDGET)
         gtk_object_unref (GTK_OBJECT(word->content.data.widget));
      else if (word->content.type == DW_CONTENT_TEXT)
         g_free (word->content.data.text);
      else if (word->content.type == DW_CONTENT_ANCHOR)
         /* This also frees the names (see  p_Dw_gtk_viewport_remove_anchor()
          * and related). */
         p_Dw_gtk_viewport_remove_anchor(widget, word->content.data.anchor);

      a_Dw_style_unref (word->style);
      a_Dw_style_unref (word->space_style);
   }

   g_free (page->lines);
   g_free (page->words);

   /* Make sure we don't own widgets anymore. Necessary before call of
      parent_class->destroy. */
   page->num_words = 0;
   page->num_lines = 0;

   DBG_OBJ_SET_NUM(page, "num_lines", page->num_lines);

   GTK_OBJECT_CLASS(parent_class)->destroy (object);
}


/*
 * Standard Dw function
 *
 * The ascent of a page is the ascent of the first line, plus
 * padding/border/margin. This can be used to align the first lines
 * of several pages in a horizontal line.
 */
static void Dw_page_size_request (DwWidget *widget,
                                  DwRequisition *requisition)
{
   DwPage *page = DW_PAGE (widget);
   DwPageLine *last_line;

   Dw_page_rewrap (page);

   if (page->num_lines > 0) {
      last_line = &page->lines[page->num_lines - 1];
      requisition->width =
         MAX (last_line->max_line_width, page->last_line_width);
      /* Note: the break_space of the last line is ignored, so breaks
         at the end of a page are not visible. */
      requisition->ascent = page->lines[0].ascent;
      requisition->descent = last_line->top
         + last_line->ascent + last_line->descent - page->lines[0].ascent;
   } else {
      requisition->width = page->last_line_width;
      requisition->ascent = 0;
      requisition->descent = 0;
   }

   requisition->width +=
      page->inner_padding + p_Dw_style_box_diff_width (widget->style);
   requisition->ascent += p_Dw_style_box_offset_y (widget->style);
   requisition->descent += p_Dw_style_box_rest_height (widget->style);

   if (requisition->width < page->avail_width)
      requisition->width = page->avail_width;
}


/*
 * Get the extremes of a word within a page.
 */
static void Dw_page_get_word_extremes (DwPageWord *word,
                                       DwExtremes *extremes)
{
   if (word->content.type == DW_CONTENT_WIDGET) {
      if (DW_WIDGET_USES_HINTS (word->content.data.widget))
         p_Dw_widget_get_extremes (word->content.data.widget, extremes);
      else {
         if (DW_STYLE_IS_PER_LENGTH(word->content.data.widget->style->width)) {
            extremes->min_width = 0;
            if (DW_WIDGET_HAS_CONTENT (word->content.data.widget))
               extremes->max_width = DW_INFINITY;
            else
               extremes->max_width = 0;
         } else if (DW_STYLE_IS_ABS_LENGTH
                    (word->content.data.widget->style->width)) {
            /* Fixed lengths are only applied to the content, so we have to
             * add padding, border and margin. */
            extremes->min_width = extremes->max_width =
               DW_STYLE_ABS_LENGTH_VAL(word->content.data.widget->style->width)
               + p_Dw_style_box_diff_width(word->style);
         } else
            p_Dw_widget_get_extremes (word->content.data.widget, extremes);
      }
   } else {
      extremes->min_width = word->size.width;
      extremes->max_width = word->size.width;
   }
}

/*
 * Standard Dw function
 */
static void Dw_page_get_extremes (DwWidget *widget,
                                  DwExtremes *extremes)
{
   DwPage *page = DW_PAGE (widget);
   DwExtremes word_extremes;
   DwPageLine *line;
   DwPageWord *word;
   gint word_index, line_index;
   gint32 par_min, par_max;
   gboolean nowrap;

   DBG_MSG (widget, "extremes", 0, "Dw_page_get_extremes");
   DBG_MSG_START (widget);

   if (page->num_lines == 0) {
      /* empty page */
      extremes->min_width = 0;
      extremes->max_width = 0;
   } else if (page->wrap_ref == -1) {
      /* no rewrap necessary -> values in lines are up to date */
      line = &page->lines[page->num_lines - 1];
      /* Historical note: The former distinction between lines with and without
       * words[first_word]->nowrap set is no longer necessary, since
       * Dw_page_real_word_wrap sets max_word_min to the correct value in any
       * case. */
      extremes->min_width = line->max_word_min;
      extremes->max_width = MAX (line->max_par_max, page->last_line_par_max);
      DBG_MSG (widget, "extremes", 0, "simple case");
   } else {
      /* Calculate the extremes, based on the values in the line from
         where a rewrap is necessary. */
      DBG_MSG (widget, "extremes", 0, "complex case");

      if (page->wrap_ref == 0) {
         extremes->min_width = 0;
         extremes->max_width = 0;
         par_min = 0;
         par_max = 0;
      } else {
         line = &page->lines[page->wrap_ref];
         extremes->min_width = line->max_word_min;
         extremes->max_width = line->max_par_max;
         par_min = line->par_min;
         par_max = line->par_max;

         DBG_MSGF (widget, "extremes", 0, "par_min = %d", par_min);
      }

      _MSG ("*** par_min = %d\n", par_min);

      for (line_index = page->wrap_ref; line_index < page->num_lines;
           line_index++) {

         DBG_MSGF (widget, "extremes", 0, "line %d", line_index);
         DBG_MSG_START (widget);

         line = &page->lines[line_index];
         nowrap = page->words[line->first_word].style->white_space
            != DW_STYLE_WHITE_SPACE_NORMAL;

         DEBUG_MSG (DEBUG_SIZE_LEVEL, "   line %d (of %d), nowrap = %d\n",
                    line_index, page->num_lines, nowrap);

         for (word_index = line->first_word; word_index < line->last_word;
              word_index++) {
            word = &page->words[word_index];
            Dw_page_get_word_extremes (word, &word_extremes);

            /* For the first word, we simply add the line1_offset. */
            if (!page->ignore_line1_offset_sometimes && word_index == 0) {
               word_extremes.min_width += page->line1_offset;
               DEBUG_MSG (DEBUG_SIZE_LEVEL + 1,
                          "      (next plus %d)\n", page->line1_offset);
            }

            if (nowrap) {
               par_min += word_extremes.min_width + word->orig_space;
               DBG_MSGF (widget, "extremes", 0, "par_min = %d", par_min);
            } else
               if (extremes->min_width < word_extremes.min_width)
                  extremes->min_width = word_extremes.min_width;

            par_max += word_extremes.max_width + word->orig_space;

            DEBUG_MSG (DEBUG_SIZE_LEVEL + 1,
                       "      word %s: max_width = %d\n",
                       a_Dw_content_text (&word->content),
                       word_extremes.max_width);
         }

         if ( ( line->last_word > line->first_word &&
                page->words[line->last_word - 1].content.type
                == DW_CONTENT_BREAK ) ||
              line_index == page->num_lines - 1 ) {
            word = &page->words[line->last_word - 1];
            par_max -= word->orig_space;

            DEBUG_MSG (DEBUG_SIZE_LEVEL + 2,
                       "   par_max = %d, after word %d (%s)\n",
                       par_max, line->last_word - 1,
                       a_Dw_content_text (&word->content));

            if (extremes->max_width < par_max)
               extremes->max_width = par_max;

            if (nowrap) {
               par_min -= word->orig_space;
               DBG_MSGF (widget, "extremes", 0, "par_min = %d", par_min);
               if (extremes->min_width < par_min)
                  extremes->min_width = par_min;

               DEBUG_MSG (DEBUG_SIZE_LEVEL + 2,
                          "   par_min = %d, after word %d (%s)\n",
                          par_min, line->last_word - 1,
                          a_Dw_content_text (&word->content));
            }

            par_min = 0;
            par_max = 0;
         }

         DBG_MSG_END (widget);
      }

      DEBUG_MSG (DEBUG_SIZE_LEVEL + 3, "   Result: %d, %d\n",
                 extremes->min_width, extremes->max_width);
   }

   DBG_MSGF (widget, "extremes", 0, "width difference: %d + %d",
             page->inner_padding, p_Dw_style_box_diff_width (widget->style));

   extremes->min_width +=
      page->inner_padding + p_Dw_style_box_diff_width (widget->style);
   extremes->max_width +=
      page->inner_padding + p_Dw_style_box_diff_width (widget->style);

   DBG_MSG_END (widget);
}


/*
 * Standard Dw function
 */
static void Dw_page_size_allocate (DwWidget *widget,
                                   DwAllocation *allocation)
{
   DwPage *page;
   int line_index, word_index;
   DwPageLine *line;
   DwPageWord *word;
   int x_cursor;
   DwAllocation child_allocation;

   page = DW_PAGE (widget);

   for (line_index = 0; line_index < page->num_lines; line_index++) {
      line = &(page->lines[line_index]);
      x_cursor = Dw_page_line_total_x_offset (page, line);

      for (word_index = line->first_word; word_index < line->last_word;
           word_index++) {
         word = &(page->words[word_index]);

         switch (word->content.type) {
         case DW_CONTENT_WIDGET:
            /* todo: justification within the line is done here! */
            child_allocation.x = x_cursor + allocation->x;
            /* align=top:
               child_allocation.y = line->top + allocation->y;
            */
            /* align=bottom (base line) */
            child_allocation.y = allocation->y +
               Dw_page_line_total_y_offset_allocation (page, line, allocation)
               + (line->ascent - word->size.ascent)
               - word->content.data.widget->style->margin.top;
            child_allocation.width = word->size.width;
            child_allocation.ascent = word->size.ascent +
               word->content.data.widget->style->margin.top;
            child_allocation.descent = word->size.descent +
               word->content.data.widget->style->margin.bottom;
            p_Dw_widget_size_allocate (word->content.data.widget,
                                       &child_allocation);
            break;

         case DW_CONTENT_ANCHOR:
            p_Dw_gtk_viewport_change_anchor
               (widget, word->content.data.anchor,
                Dw_page_line_total_y_offset_allocation (page, line,
                                                        allocation));
            break;

         default:
            /* make compiler happy */
            break;
         }

         x_cursor += (word->size.width + word->eff_space);
      }
   }
}


/*
 * Implementation for both mark_size_change and mark_extremes_change.
 */
static void Dw_page_mark_change (DwWidget *widget,
                                 gint ref)
{
   DwPage *page;

   if (ref != -1) {
      page = DW_PAGE (widget);

      DBG_MSGF (page, "wrap", 0, "Dw_page_mark_size_change (ref = %d)", ref);

      if (page->wrap_ref == -1)
         page->wrap_ref = ref;
      else
         page->wrap_ref = MIN (page->wrap_ref, ref);

      DBG_OBJ_SET_NUM (page, "wrap_ref", page->wrap_ref);
   }
}

/*
 * Standard Dw function
 */
static void Dw_page_set_width (DwWidget *widget,
                               gint32 width)
{
   DwPage *page;

   page = DW_PAGE (widget);

   /* If limit_text_width is set to YES, a queue_resize may also be
      necessary. */
   if (page->avail_width != width || prefs.limit_text_width) {
      DEBUG_MSG(DEBUG_REWRAP_LEVEL,
                "Dw_page_set_width: Calling p_Dw_widget_queue_resize, "
                "in page with %d word(s)\n",
                page->num_words);

      page->avail_width = width;
      p_Dw_widget_queue_resize (widget, 0, FALSE);
      page->must_queue_resize = FALSE;
   }
}


/*
 * Standard Dw function
 */
static void Dw_page_set_ascent (DwWidget *widget,
                                gint32 ascent)
{
   DwPage *page;

   page = DW_PAGE (widget);

   if (page->avail_ascent != ascent) {
      DEBUG_MSG(DEBUG_REWRAP_LEVEL,
                "Dw_page_set_ascent: Calling p_Dw_widget_queue_resize, "
                "in page with %d word(s)\n",
                page->num_words);

      page->avail_ascent = ascent;
      p_Dw_widget_queue_resize (widget, 0, FALSE);
      page->must_queue_resize = FALSE;
   }
}


/*
 * Standard Dw function
 */
static void Dw_page_set_descent (DwWidget *widget,
                                 gint32 descent)
{
   DwPage *page;

   page = DW_PAGE (widget);

   if (page->avail_descent != descent) {
      DEBUG_MSG(DEBUG_REWRAP_LEVEL,
                "Dw_page_set_descent: Calling p_Dw_widget_queue_resize, "
                "in page with %d word(s)\n",
                page->num_words);

      page->avail_descent = descent;
      p_Dw_widget_queue_resize (widget, 0, FALSE);
      page->must_queue_resize = FALSE;
   }
}


/*
 * Standard Dw function
 */
static void Dw_page_add (DwContainer *container,
                         DwWidget *widget)
{
   /* todo */
}


/*
 * Standard Dw function
 */
static void Dw_page_remove (DwContainer *container,
                            DwWidget *widget)
{
   /* todo */
}


/*
 * Standard Dw function
 */
static void Dw_page_forall (DwContainer *container,
                            DwCallback callback,
                            gpointer callback_data)
{
   DwPage *page;
   int word_index;
   DwPageWord *word;

   page = DW_PAGE (container);

   for (word_index = 0; word_index < page->num_words; word_index++) {
      word = &page->words[word_index];

      if (word->content.type == DW_CONTENT_WIDGET)
         (*callback) (word->content.data.widget, callback_data);
   }
}


/*
 * ...
 *
 * avail_line is passed from Dw_page_real_word_wrap, to avoid calculating
 * it twice.
 */
static void Dw_page_justify_line (DwPage *page, DwPageLine *line,
                                  gint32 avail_width)
{
   /* To avoid rounding errors, the calculation is based on accumulated
    * values (*_cum). */
   gint i;
   gint32 orig_space_sum, orig_space_cum;
   gint32 eff_space_diff_cum, last_eff_space_diff_cum;
   gint32 diff;

   diff = avail_width - page->last_line_width;
   if (diff > 0) {
      orig_space_sum = 0;
      for (i = line->first_word; i < line->last_word - 1; i++)
         orig_space_sum += page->words[i].orig_space;

      orig_space_cum = 0;
      last_eff_space_diff_cum = 0;
      for (i = line->first_word; i < line->last_word - 1; i++) {
         orig_space_cum += page->words[i].orig_space;

         if (orig_space_cum == 0)
            eff_space_diff_cum = last_eff_space_diff_cum;
         else
            eff_space_diff_cum = diff * orig_space_cum / orig_space_sum;

         page->words[i].eff_space = page->words[i].orig_space +
            (eff_space_diff_cum - last_eff_space_diff_cum);
         DBG_OBJ_ARRSET_NUM (page, "words.%d.eff_space", i,
                             page->words[i].eff_space);

         last_eff_space_diff_cum = eff_space_diff_cum;
      }
   }
}

/*
 * ...
 */
static void Dw_page_add_line (DwPage *page, gint word_ind, gboolean new_par)
{
   DwPageLine *last_line, *plast_line;

   DBG_MSG (page, "wrap", 0, "Dw_page_add_line");
   DBG_MSG_START (page);

   page->num_lines++;
   a_List_add (page->lines, page->num_lines, page->num_lines_max);
   DBG_OBJ_SET_NUM(page, "num_lines", page->num_lines);

   DEBUG_MSG (DEBUG_REWRAP_LEVEL, "--- new line %d in %p, with word %d of %d"
              "\n", page->num_lines - 1, page, word_ind, page->num_words);

   last_line = &page->lines[page->num_lines - 1];

   if (page->num_lines == 1)
      plast_line = NULL;
   else
      plast_line = &page->lines[page->num_lines - 2];

   if (plast_line) {
      /* second or more lines: copy values of last line */
      last_line->top =
         plast_line->top + plast_line->ascent +
         plast_line->descent + plast_line->break_space;
      last_line->max_line_width = plast_line->max_line_width;
      last_line->max_word_min = plast_line->max_word_min;
      last_line->max_par_max = plast_line->max_par_max;
      last_line->par_min = plast_line->par_min;
      last_line->par_max = plast_line->par_max;
   } else {
      /* first line: initialize values */
      last_line->top = 0;
      last_line->max_line_width = page->line1_offset_eff;
      last_line->max_word_min = 0;
      last_line->max_par_max = 0;
      last_line->par_min =  page->line1_offset_eff;
      last_line->par_max =  page->line1_offset_eff;
   }

   DBG_OBJ_ARRSET_NUM (page, "lines.%d.top", page->num_lines - 1,
                       last_line->top);
   DBG_OBJ_ARRSET_NUM (page, "lines.%d.max_line_width", page->num_lines - 1,
                       last_line->max_line_width);
   DBG_OBJ_ARRSET_NUM (page, "lines.%d.max_word_min", page->num_lines - 1,
                       last_line->max_word_min);
   DBG_OBJ_ARRSET_NUM (page, "lines.%d.max_par_max", page->num_lines - 1,
                       last_line->max_par_max);
   DBG_OBJ_ARRSET_NUM (page, "lines.%d.par_min", page->num_lines - 1,
                       last_line->par_min);
   DBG_OBJ_ARRSET_NUM (page, "lines.%d.par_max", page->num_lines - 1,
                       last_line->par_max);

   last_line->first_word = word_ind;
   last_line->ascent = 0;
   last_line->descent = 0;
   last_line->margin_descent = 0;
   last_line->break_space = 0;
   last_line->left_offset = 0;

   DBG_OBJ_ARRSET_NUM (page, "lines.%d.ascent", page->num_lines - 1,
                       last_line->ascent);
   DBG_OBJ_ARRSET_NUM (page, "lines.%d.descent", page->num_lines - 1,
                       last_line->descent);

   /* update values in line */
   last_line->max_line_width =
      MAX (last_line->max_line_width, page->last_line_width);

   if (page->num_lines > 1)
      page->last_line_width = 0;
   else
      page->last_line_width = page->line1_offset_eff;

   if (new_par) {
      last_line->max_par_max =
         MAX (last_line->max_par_max, page->last_line_par_max);
      DBG_OBJ_ARRSET_NUM (page, "lines.%d.max_par_max", page->num_lines - 1,
                          last_line->max_par_max);

      if (page->num_lines > 1) {
         last_line->par_min = 0;
         last_line->par_max = 0;
      } else {
         last_line->par_min = page->line1_offset_eff;
         last_line->par_max = page->line1_offset_eff;
      }
      page->last_line_par_min = 0;
      page->last_line_par_max = 0;

      DBG_OBJ_SET_NUM(page, "last_line_par_min", page->last_line_par_min);
      DBG_OBJ_SET_NUM(page, "last_line_par_max", page->last_line_par_max);
   }

   last_line->par_min = page->last_line_par_min;
   last_line->par_max = page->last_line_par_max;

   DBG_OBJ_ARRSET_NUM (page, "lines.%d.par_min", page->num_lines - 1,
                       last_line->par_min);
   DBG_OBJ_ARRSET_NUM (page, "lines.%d.par_max", page->num_lines - 1,
                       last_line->par_max);

   DBG_MSG_END (page);
}


/*
 * This function is called in two cases: (i) when a word is added (by
 * Dw_page_add_word), and (ii) when a page has to be (partially)
 * rewrapped. It does word wrap, and adds new lines, if necesary.
 */
static void Dw_page_real_word_wrap (DwPage *page, gint word_ind)
{
   DwPageLine *last_line;
   DwPageWord *word, *prev_word;
   gint32 avail_width, last_space, left_offset;
   gboolean new_line = FALSE, new_par = FALSE;
   DwWidget *widget = DW_WIDGET (page);
   DwExtremes word_extremes;

   DBG_MSGF (page, "wrap", 0, "Dw_page_real_word_wrap (%d): %s, width = %d",
             word_ind, a_Dw_content_html (&page->words[word_ind].content),
             page->words[word_ind].size.width);
   DBG_MSG_START (page);

   avail_width =
      page->avail_width - p_Dw_style_box_diff_width (widget->style)
      - page->inner_padding;
   if (prefs.limit_text_width &&
       avail_width > widget->viewport->allocation.width - 10)
      avail_width = widget->viewport->allocation.width - 10;

   word = &page->words[word_ind];

   if (page->num_lines == 0) {
      DBG_MSG (page, "wrap", 0, "first line");
      new_line = TRUE;
      new_par = TRUE;
      last_line = NULL;
   } else {
      last_line = &page->lines[page->num_lines - 1];

      if (page->num_words > 0) {
         prev_word = &page->words[word_ind - 1];
         if (prev_word->content.type == DW_CONTENT_BREAK) {
            DBG_MSG (page, "wrap", 0, "after a break");
            /* previous word is a break */
            new_line = TRUE;
            new_par = TRUE;
         } else if (word->style->white_space != DW_STYLE_WHITE_SPACE_NORMAL) {
            DBG_MSGF (page, "wrap", 0, "no wrap (white_space = %d)",
                      word->style->white_space);
            new_line = FALSE;
            new_par = FALSE;
         } else {
            if (last_line->first_word != word_ind) {
               /* Does new word fit into the last line? */
               DBG_MSGF (page, "wrap", 0,
                         "word %d (%s) fits? (%d + %d + %d &lt;= %d)...",
                         word_ind, a_Dw_content_html (&word->content),
                         page->last_line_width, prev_word->orig_space,
                         word->size.width, avail_width);
               new_line = (page->last_line_width + prev_word->orig_space
                           + word->size.width > avail_width);
               DBG_MSGF (page, "wrap", 0, "... %s.",
                         new_line ? "No" : "Yes");
            }
         }
      }
   }

   /* Has sometimes the wrong value. */
   word->eff_space = word->orig_space;
   DBG_OBJ_ARRSET_NUM (page, "words.%d.eff_space", word_ind, word->eff_space);

   /* Test, whether line1_offset can be used. */
   if (word_ind == 0) {
      if (page->ignore_line1_offset_sometimes) {
         if (page->line1_offset + word->size.width > avail_width)
            page->line1_offset_eff = 0;
         else
            page->line1_offset_eff = page->line1_offset;
      } else
         page->line1_offset_eff = page->line1_offset;
   }

   if (last_line != NULL && new_line && !new_par &&
       word->style->text_align == DW_STYLE_TEXT_ALIGN_JUSTIFY)
      Dw_page_justify_line (page, last_line, avail_width);

   if (new_line) {
      Dw_page_add_line (page, word_ind, new_par);
      last_line = &page->lines[page->num_lines - 1];
   }

   last_line->last_word = word_ind + 1;
   last_line->ascent = MAX (last_line->ascent, word->size.ascent);
   last_line->descent = MAX (last_line->descent, word->size.descent);

   DBG_OBJ_ARRSET_NUM (page, "lines.%d.ascent", page->num_lines - 1,
                       last_line->ascent);
   DBG_OBJ_ARRSET_NUM (page, "lines.%d.descent", page->num_lines - 1,
                       last_line->descent);

   if (word->content.type == DW_CONTENT_WIDGET) {
      last_line->margin_descent =
         MAX (last_line->margin_descent,
              word->size.descent +
              word->content.data.widget->style->margin.bottom);

      DBG_OBJ_ARRSET_NUM (page, "lines.%d.descent", page->num_lines - 1,
                          last_line->descent);

      /* If the widget is not in the first line of the paragraph, its top
       * margin may make the line higher.
       */
      if (page->num_lines > 1) {
         /* Here, we know already what the break and the bottom margin
          * contributed to the space before this line.
          */
         last_line->ascent =
            MAX (last_line->ascent,
                 word->size.ascent
                 + word->content.data.widget->style->margin.top);

         DBG_OBJ_ARRSET_NUM (page, "lines.%d.ascent", page->num_lines - 1,
                             last_line->ascent);
      }
   } else
      last_line->margin_descent =
         MAX (last_line->margin_descent, last_line->descent);

   Dw_page_get_word_extremes (word, &word_extremes);
   last_space = (word_ind > 0) ? page->words[word_ind - 1].orig_space : 0;

   if (word->content.type == DW_CONTENT_BREAK)
      last_line->break_space =
         MAX3 (word->content.data.break_space,
               last_line->margin_descent - last_line->descent,
               last_line->break_space);

   page->last_line_width += word->size.width;
   if (!new_line)
      page->last_line_width += last_space;

   page->last_line_par_min += word_extremes.max_width;
   page->last_line_par_max += word_extremes.max_width;
   if (!new_par) {
      page->last_line_par_min += last_space;
      page->last_line_par_max += last_space;
   }

   if (word->style->white_space != DW_STYLE_WHITE_SPACE_NORMAL) {
      last_line->par_min += word_extremes.min_width + last_space;
      /* This may also increase the accumulated minimum word width.  */
      last_line->max_word_min =
         MAX (last_line->max_word_min, last_line->par_min);
      /* NOTE: Most code relies on that all values of nowrap are equal for all
       * words within one line. */
   } else
      /* Simple case. */
      last_line->max_word_min =
         MAX (last_line->max_word_min, word_extremes.min_width);

   DBG_OBJ_SET_NUM(page, "last_line_par_min", page->last_line_par_min);
   DBG_OBJ_SET_NUM(page, "last_line_par_max", page->last_line_par_max);
   DBG_OBJ_ARRSET_NUM (page, "lines.%d.par_min", page->num_lines - 1,
                       last_line->par_min);
   DBG_OBJ_ARRSET_NUM (page, "lines.%d.par_max", page->num_lines - 1,
                       last_line->par_max);
   DBG_OBJ_ARRSET_NUM (page, "lines.%d.max_word_min", page->num_lines - 1,
                       last_line->max_word_min);

   /* Finally, justify the line. Breaks are ignored, since the HTML
    * parser sometimes assignes the wrong style to them. (todo: ) */
   if (word->content.type != DW_CONTENT_BREAK) {
      switch (word->style->text_align) {
      case DW_STYLE_TEXT_ALIGN_LEFT:
      case DW_STYLE_TEXT_ALIGN_JUSTIFY:  /* see some lines above */
      case DW_STYLE_TEXT_ALIGN_STRING:   /* handled elsewhere (in future) */
         left_offset = 0;
         break;

      case DW_STYLE_TEXT_ALIGN_RIGHT:
         left_offset = avail_width - page->last_line_width;
         break;

      case DW_STYLE_TEXT_ALIGN_CENTER:
         left_offset = (avail_width - page->last_line_width) / 2;
         break;

      default:
         /* compiler happiness */
         left_offset = 0;
      }

      /* For large lines (images etc), which do not fit into the viewport: */
      if (left_offset < 0)
         left_offset = 0;

      if (page->list_item && last_line == page->lines) {
         /* List item markers are always on the left. */
         last_line->left_offset = 0;
         page->words[0].eff_space = page->words[0].orig_space + left_offset;
         DBG_OBJ_ARRSET_NUM (page, "words.%d.eff_space", 0,
                             page->words[0].eff_space);
      } else
         last_line->left_offset = left_offset;
   }

   page->must_queue_resize = TRUE;

   DBG_MSG_END (page);
}


/*
 * Calculate the size of a widget within the page.
 * (Subject of change in the near future!)
 */
static void Dw_page_calc_widget_size (DwPage *page,
                                      DwWidget *widget,
                                      DwRequisition *size)
{
   DwRequisition requisition;
   gint32 avail_width, avail_ascent, avail_descent;

   /* We ignore line1_offset[_eff]. */
   avail_width =
      page->avail_width - p_Dw_style_box_diff_width (DW_WIDGET(page)->style)
      - page->inner_padding;
   avail_ascent =
      page->avail_ascent - p_Dw_style_box_diff_height (DW_WIDGET(page)->style);
   avail_descent = page->avail_descent;

   if (DW_WIDGET_USES_HINTS (widget)) {
      p_Dw_widget_set_width (widget, avail_width);
      p_Dw_widget_set_ascent (widget, avail_ascent);
      p_Dw_widget_set_descent (widget, avail_descent);
      p_Dw_widget_size_request (widget, size);
      size->ascent -= widget->style->margin.top;
      size->descent -= widget->style->margin.bottom;
   } else {
      /* TODO: Use margin.{top|bottom} here, like above.
       * (No harm for the next future.) */
      if (widget->style->width == DW_STYLE_LENGTH_AUTO ||
          widget->style->height == DW_STYLE_LENGTH_AUTO)
         p_Dw_widget_size_request (widget, &requisition);

      if (widget->style->width == DW_STYLE_LENGTH_AUTO)
         size->width = requisition.width;
      else if (DW_STYLE_IS_ABS_LENGTH (widget->style->width))
         /* Fixed lengths are only applied to the content, so we have to
          * add padding, border and margin. */
         size->width = DW_STYLE_ABS_LENGTH_VAL (widget->style->width)
            + p_Dw_style_box_diff_width (widget->style);
      else
         size->width =
            DW_STYLE_PER_LENGTH_VAL (widget->style->width) * avail_width;

      if (widget->style->height == DW_STYLE_LENGTH_AUTO) {
         size->ascent = requisition.ascent;
         size->descent = requisition.descent;
      } else if (DW_STYLE_IS_ABS_LENGTH (widget->style->height)) {
         /* Fixed lengths are only applied to the content, so we have to
          * add padding, border and margin. */
         size->ascent =
            DW_STYLE_ABS_LENGTH_VAL (widget->style->height)
            + p_Dw_style_box_diff_height (widget->style);
         size->descent = 0;
      } else {
         size->ascent =
            DW_STYLE_PER_LENGTH_VAL (widget->style->height) * avail_ascent;
         size->descent =
            DW_STYLE_PER_LENGTH_VAL (widget->style->height) * avail_descent;
      }
   }
}


/*
 * Rewrap the page from the line from which this is necessary.
 * There are basically two times we'll want to do this:
 * either when the viewport is resized, or when the size changes on one
 * of the child widgets.
 */
static void Dw_page_rewrap (DwPage *page)
{
   DwWidget *widget;
   gint i, word_index;
   DwPageWord *word;
   DwPageLine *last_line;

   if (page->wrap_ref == -1)
      /* page does not have to be rewrapped */
      return;

   widget = DW_WIDGET (page);

   DBG_MSGF (page, "wrap", 0,
             "Dw_page_rewrap: page->wrap_ref = %d, in page with %d word(s)",
             page->wrap_ref, page->num_words);
   DBG_MSG_START (page);

   /* All lines up from page->wrap_ref will be rebuild from the word list,
    * the line list up from this position is rebuild. */
   page->num_lines = page->wrap_ref;
   page->last_line_width = 0;
   DBG_OBJ_SET_NUM(page, "num_lines", page->num_lines);
   DBG_OBJ_SET_NUM(page, "last_line_width", page->last_line_width);

   /* In the word list, we start at the last word, plus one (see definition
    * of last_word), in the line before. */
   if (page->wrap_ref > 0) {
      /* Note: In this case, Dw_page_real_word_wrap will immediately find
       * the need to rewrap the line, since we start with the last one (plus
       * one). This is also the reason, why page->last_line_width is set
       * to the length of the line. */
      last_line = &page->lines[page->num_lines - 1];

      page->last_line_par_min = last_line->par_min;
      page->last_line_par_max = last_line->par_max;

      word_index = last_line->last_word;
      for (i = last_line->first_word; i < last_line->last_word - 1; i++)
         page->last_line_width += (page->words[i].size.width +
                                   page->words[i].orig_space);
      page->last_line_width +=
         page->words[last_line->last_word - 1].size.width;
   } else {
      page->last_line_par_min = 0;
      page->last_line_par_max = 0;

      word_index = 0;
   }

   for (; word_index < page->num_words; word_index++) {
      word = &page->words[word_index];

      if (word->content.type == DW_CONTENT_WIDGET)
         Dw_page_calc_widget_size (page, word->content.data.widget,
                                   &word->size);
      Dw_page_word_wrap (page, word_index);

      if (word->content.type == DW_CONTENT_WIDGET) {
         word->content.data.widget->parent_ref = page->num_lines - 1;
         DBG_OBJ_SET_NUM (word->content.data.widget, "parent_ref",
                          word->content.data.widget->parent_ref);
      }

      DEBUG_MSG(DEBUG_REWRAP_LEVEL,
                "Assigning parent_ref = %d to rewrapped word %d, "
                "in page with %d word(s)\n",
                page->num_lines - 1, word_index, page->num_words);

      if ( word->content.type == DW_CONTENT_ANCHOR )
         p_Dw_gtk_viewport_change_anchor
            (widget, word->content.data.anchor,
             Dw_page_line_total_y_offset (page,
                                          &page->lines[page->num_lines - 1]));
   }

   /* Next time, the page will not have to be rewrapped. */
   page->wrap_ref = -1;

   DBG_MSG_END (page);
}

/*
 * Paint a line
 * - x and y are toplevel dw coordinates (Question: what Dw? Changed. Test!)
 * - area is used always (ev. set it to event->area)
 * - event is only used when is_expose
 */
static void Dw_page_draw_line (DwPage *page,
                               DwPageLine *line,
                               DwRectangle *area,
                               GdkEventExpose *event)
{
   DwWidget *widget;
   DwPageWord *word;
   gint word_index;
   gint32 x_widget, y_widget, x_viewport, y_viewport, y_viewport_base;
   gint32 start_hl, width_hl;
   gint32 diff, word_len, eff_hl_end, layer;
   DwWidget *child;
   DwRectangle child_area;
   GdkWindow *window;
   GdkGC *gc, *hl_gc;
   DwStyleColor *page_bg_color, *word_bg_color;

   /* Here's an idea on how to optimize this routine to minimize the number
    * of calls to gdk_draw_string:
    *
    * Copy the text from the words into a buffer, adding a new word
    * only if: the attributes match, and the spacing is either zero or
    * equal to the width of ' '. In the latter case, copy a " " into
    * the buffer. Then draw the buffer. */

   widget = DW_WIDGET (page);
   window = DW_WIDGET_WINDOW (widget);
   page_bg_color = p_Dw_widget_get_bg_color (widget);

   x_widget = Dw_page_line_total_x_offset(page,line);
   x_viewport =
      p_Dw_widget_x_world_to_viewport (widget,
                                       widget->allocation.x + x_widget);
   y_widget = Dw_page_line_total_y_offset(page,line);
   y_viewport =
      p_Dw_widget_y_world_to_viewport (widget,
                                       widget->allocation.y + y_widget);
   y_viewport_base = y_viewport + line->ascent;

   for (word_index = line->first_word; word_index < line->last_word;
        word_index++) {
      word = &page->words[word_index];
      diff = 0;
      gc = word->style->color->gc;


      DBG_OBJ_ARRSET_NUM (page, "words.%d.<i>drawn at</i>.x", word_index,
                          x_widget);
      DBG_OBJ_ARRSET_NUM (page, "words.%d.<i>drawn at</i>.y", word_index,
                          y_widget);

      switch (word->content.type) {
      case DW_CONTENT_TEXT:
         if (word->style->background_color)
            word_bg_color = word->style->background_color;
         else
            word_bg_color = page_bg_color;

         /* Adjust the text baseline if the word is <SUP>-ed or <SUB>-ed. */
         if (word->style->valign == DW_STYLE_VALIGN_SUB)
            diff = word->size.ascent / 2;
         else if (word->style->valign == DW_STYLE_VALIGN_SUPER)
            diff -= word->size.ascent / 3;

         /* Draw background (color, image), when given. */
         /* todo: Test (word->style->background_color) is incomplete, and
          * should in future include background images. */
         if (word->style->background_color &&
             word->size.width > 0)
            p_Dw_widget_draw_box (widget, word->style, area,
                                  x_widget,
                                  y_widget + line->ascent - word->size.ascent,
                                  word->size.width,
                                  word->size.ascent + word->size.descent,
                                  FALSE);

         /* Draw space background (color, image), when given. */
         /* todo: Test (word->space_style->background_color) is incomplete, and
          * should in future include background images. */
         if (word->space_style->background_color &&
             word->eff_space > 0)
            p_Dw_widget_draw_box (widget, word->space_style, area,
                                  x_widget + word->size.width,
                                  y_widget + line->ascent - word->size.ascent,
                                  word->eff_space,
                                  word->size.ascent + word->size.descent,
                                  FALSE);

         gdk_draw_string (window, word->style->font->font, gc,
                          x_viewport, y_viewport_base + diff,
                          word->content.data.text);

         /* underline */
         if (word->style->text_decoration & DW_STYLE_TEXT_DECORATION_UNDERLINE)
            gdk_draw_line (window, gc,
                           x_viewport, y_viewport_base + 1 + diff,
                           x_viewport + word->size.width - 1,
                           y_viewport_base + 1 + diff);
         if (word_index + 1 < line->last_word &&
             (word->space_style->text_decoration
              & DW_STYLE_TEXT_DECORATION_UNDERLINE))
            gdk_draw_line (window, gc,
                           x_viewport + word->size.width,
                           y_viewport_base + 1 + diff,
                           x_viewport + word->size.width + word->eff_space - 1,
                           y_viewport_base + 1 + diff);

         /* strike-through */
         if (word->style->text_decoration
             & DW_STYLE_TEXT_DECORATION_LINE_THROUGH)
            gdk_draw_line (window, gc,
                           x_viewport,
                           y_viewport_base - word->size.ascent / 2 + diff,
                           x_viewport + word->size.width - 1,
                           y_viewport_base - word->size.ascent / 2 + diff);
         if (word_index + 1 < line->last_word &&
             (word->space_style->text_decoration
              & DW_STYLE_TEXT_DECORATION_LINE_THROUGH))
            gdk_draw_line (window, gc,
                           x_viewport + word->size.width,
                           y_viewport_base - word->size.ascent / 2 + diff,
                           x_viewport + word->size.width + word->eff_space - 1,
                           y_viewport_base - word->size.ascent / 2 + diff);

         for (layer = 0; layer < DW_HIGHLIGHT_NUM_LAYERS; layer++) {
            if (word->hl_start[layer] != -1) {
               word_len = strlen (word->content.data.text);
               eff_hl_end = MIN (word_len, word->hl_end[layer]);
               start_hl = x_viewport +
                  gdk_text_width (word->style->font->font,
                                  word->content.data.text,
                                  word->hl_start[layer]);
               width_hl =
                  gdk_text_width (word->style->font->font,
                                  word->content.data.text
                                  + word->hl_start[layer],
                                  eff_hl_end - word->hl_start[layer]);

               /* If the space after this word highlighted, and this word
                * is not the last one in this line, highlight also the
                * space. */
               /* todo: This should also be done with spaces after non-text
                * words, but this is not yet defined very well. */
               if (word->hl_end[layer] > eff_hl_end &&
                   word_index < page->num_words &&
                   word_index != line->last_word - 1)
                  width_hl += word->eff_space;


               if (width_hl != 0) {
                  /* Draw background for highlighted text. */
                  gdk_draw_rectangle (window, word_bg_color->inverse_gc,
                                      TRUE, start_hl,
                                      y_viewport_base - word->size.ascent,
                                      width_hl,
                                      word->size.ascent + word->size.descent);

                  /* Highlight the text. */
                  hl_gc = word->style->color->inverse_gc;
                  gdk_draw_text (window, word->style->font->font, hl_gc,
                                 start_hl, y_viewport_base + diff,
                                 word->content.data.text
                                 + word->hl_start[layer],
                                 eff_hl_end - word->hl_start[layer]);

                  /* underline and strike-through */
                  if (word->style->text_decoration
                      & DW_STYLE_TEXT_DECORATION_UNDERLINE)
                     gdk_draw_line (window, hl_gc,
                                    start_hl, y_viewport_base + 1 + diff,
                                    start_hl + width_hl - 1,
                                    y_viewport_base + 1 + diff);
                  if (word->style->text_decoration
                      & DW_STYLE_TEXT_DECORATION_LINE_THROUGH)
                     gdk_draw_line (window, hl_gc,
                                    start_hl,
                                    y_viewport_base
                                    - word->size.ascent / 2 + diff,
                                    start_hl + width_hl - 1,
                                    y_viewport_base
                                    - word->size.ascent / 2 + diff);
               }
            }
         }
         break;

      case DW_CONTENT_WIDGET:
         child = word->content.data.widget;
         if (p_Dw_widget_intersect (child, area, &child_area))
            p_Dw_widget_draw (child, &child_area, event);
         break;

      case DW_CONTENT_ANCHOR: case DW_CONTENT_BREAK:
         /* nothing - an anchor/break isn't seen */
         /*
          * Historical note:
          * > BUG: sometimes anchors have x_space;
          * > we subtract that just in case --EG
          * This is inconsistent with other parts of the code, so it should
          * be tried to prevent this earlier.--SG
          */
         /*
          * x_viewport -= word->size.width + word->eff_space;
          * x_widget -= word->size.width + word->eff_space;
          */
#if 0
         /* Useful for testing: draw breaks. */
         if (word->content.type == DW_CONTENT_BREAK)
            gdk_draw_rectangle (window, gc, TRUE,
                                p_Dw_widget_x_world_to_viewport (widget,
                                   widget->allocation.x +
                                      Dw_page_line_total_x_offset(page, line)),
                                y_viewport_base + line->descent,
                                DW_WIDGET_CONTENT_WIDTH(widget),
                                word->content.data.break_space);
#endif
         break;

      default:
         g_warning ("BUG!!! at (%d, %d).", x_viewport, y_viewport_base + diff);
         break;
      }

      x_viewport += word->size.width + word->eff_space;
      x_widget += word->size.width + word->eff_space;
   }
}

/*
 * Find the first line index that includes y, relative to top of widget.
 */
static gint Dw_page_find_line_index (DwPage *page, gint y)
{
   gint max_index = page->num_lines - 1;
   gint step, index, low = 0;

   step = (page->num_lines + 1) >> 1;
   while ( step > 1 ) {
      index = low + step;
      if (index <= max_index &&
          Dw_page_line_total_y_offset_i (page, index) < y)
         low = index;
      step = (step + 1) >> 1;
   }

   if (low < max_index && Dw_page_line_total_y_offset_i (page, low + 1) < y)
      low++;

   /*
    * This new routine returns the line number between (top) and
    * (top + size.ascent + size.descent + break_space): the space
    * _below_ the line is considered part of the line.  Old routine
    * returned line number between (top - previous_line->break_space)
    * and (top + size.ascent + size.descent): the space _above_ the
    * line was considered part of the line.  This is important for
    * Dw_page_find_link() --EG
    * That function has now been inlined into Dw_page_motion_notify() --JV
    */
   return low;
}

/*
 * Find the line of word <word_index>.
 */
static gint Dw_page_find_line_of_word (DwPage *page, gint word_index)
{
   gint high = page->num_lines - 1, index, low = 0;

   g_return_val_if_fail (word_index >= 0, -1);
   g_return_val_if_fail (word_index < page->num_words, -1);

   while (TRUE) {
      index = (low + high) / 2;
      if (word_index >= page->lines[index].first_word) {
         if (word_index < page->lines[index].last_word)
            return index;
         else
            low = index + 1;
      } else
         high = index - 1;
   }
}

/*
 * Draw the actual lines, starting at (x, y) in toplevel Dw coords.
 * (former Dw_page_expose_lines)
 */
static void Dw_page_draw (DwWidget *widget,
                          DwRectangle *area,
                          GdkEventExpose *event)
{
   DwPage *page;
   gint line_index;
   DwPageLine *line;

   p_Dw_widget_draw_widget_box (widget, area, FALSE);

   page = DW_PAGE (widget);
   line_index = Dw_page_find_line_index (page, area->y);

   for (; line_index < page->num_lines; line_index++) {
      line = &(page->lines[line_index]);
      if (Dw_page_line_total_y_offset(page, line) >= area->y + area->height)
         break;

      Dw_page_draw_line (page, line, area, event);
   }
}


/*
 * Find the index of the word, or -1.
 */
static gint Dw_page_find_word (DwPage *page, gint x, gint y)
{
   gint line_index, word_index;
   gint x_cursor, last_x_cursor;
   DwPageLine *line;
   DwPageWord *word;

   if ( (line_index = Dw_page_find_line_index (page, y)) >= page->num_lines )
      return -1;
   line = &page->lines[line_index];
   if (Dw_page_line_total_y_offset(page, line) + line->ascent + line->descent
       <= y)
      return -1;

   x_cursor = Dw_page_line_total_x_offset (page, line);
   for (word_index = line->first_word; word_index < line->last_word;
        word_index++) {
      word = &page->words[word_index];
      last_x_cursor = x_cursor;
      x_cursor += word->size.width + word->eff_space;
      if (last_x_cursor <= x && x_cursor > x)
         return word_index;
   }
   return -1;
}

/*
 * Construct an iterator for a word.
 */
static DwIterator *Dw_page_construct_iterator (DwPage *page, gint word_index)
{
   DwIterator *it;

   it = Dw_page_iterator (DW_WIDGET (page), DW_CONTENT_ALL, FALSE);
   //DW_CONTENT_TEXT | DW_CONTENT_WIDGET, FALSE);
   ((DwIteratorInt*)it)->pos = word_index;
   it->content = page->words[word_index].content;
   return it;
}

/*
 * Send event to selection.
 */
static gboolean Dw_page_send_selection_event (DwPage *page,
                                              gint (*fn) (Selection*,
                                                          DwIterator *,
                                                          gint, gint,
                                                          GdkEventButton*,
                                                          gboolean
                                                             within_content),
                                              gint32 x, gint32 y,
                                              GdkEventButton *event)
{
   DwIterator *it;
   DwPageLine *line, *last_line;
   gint32 next_word_start_x, word_start_x, word_x, next_word_x, yfirst, ylast;
   gint char_pos = 0, word_index, line_index, link;
   DwPageWord *word;
   gboolean found;
   gboolean within_content;

   DEBUG_MSG (DEBUG_EVENT_LEVEL,
              "Dw_page_send_selection_event: x = %d, y = %d\n", x, y);

   if (page->num_words == 0) {
      DEBUG_MSG (DEBUG_EVENT_LEVEL, "  no words\n");
      return FALSE;
   }

   /* In most cases true, so set here: */
   link = -1;
   within_content = TRUE;

   last_line = &page->lines[page->num_lines - 1];
   yfirst = Dw_page_line_total_y_offset_i (page, 0);
   ylast =
      Dw_page_line_total_y_offset (page, last_line) +
      last_line->ascent + last_line->descent;
   if (y < yfirst) {
      /* Above the first line: take the first word. */
      DEBUG_MSG (DEBUG_EVENT_LEVEL, "  above first line (at %d)\n", yfirst);
      within_content = FALSE;
      word_index = 0;
      char_pos = 0;
   } else if (y >= ylast) {
      /* Below the last line: take the last word. */
      DEBUG_MSG (DEBUG_EVENT_LEVEL, "  below last line (at %d)\n", ylast);
      within_content = FALSE;
      word_index = page->num_words - 1;
      word = &page->words[word_index];
      char_pos = word->content.type == DW_CONTENT_TEXT ?
         strlen (word->content.data.text) : 0;
   } else {
      line_index = Dw_page_find_line_index (page, y);
      line = &page->lines[line_index];
      DEBUG_MSG (DEBUG_EVENT_LEVEL, "  in line %d\n", line_index);

      /* Pointer within the break space? */
      if (y > (Dw_page_line_total_y_offset(page, line) + line->ascent +
               line->descent)) {
         /* Choose this break. */
         DEBUG_MSG (DEBUG_EVENT_LEVEL, "    break\n");
         within_content = FALSE;
         word_index = line->last_word - 1;
         char_pos = 0;
      } else if (x < Dw_page_line_total_x_offset (page, line)) {
         /* Left of the first word in the line. */
         DEBUG_MSG (DEBUG_EVENT_LEVEL, "    left of this line\n");
         word_index = line->first_word;
         within_content = FALSE;
         char_pos = 0;
      } else {
         next_word_start_x = Dw_page_line_total_x_offset (page, line);
         found = FALSE;
         for (word_index = line->first_word;
              !found && word_index < line->last_word;
              word_index++) {
            word = &page->words[word_index];
            word_start_x = next_word_start_x;
            next_word_start_x += word->size.width + word->eff_space;
            DEBUG_MSG (DEBUG_EVENT_LEVEL,
                       "    word %d (%s) from %d to %d, delta = %d + %d\n",
                       word_index, a_Dw_content_text (&word->content),
                       word_start_x, next_word_start_x,
                       word->size.width, word->eff_space);
            if (x >= word_start_x && x < next_word_start_x) {
               DEBUG_MSG (DEBUG_EVENT_LEVEL,
                          "    found word %d (%s)\n",
                          word_index, a_Dw_content_text (&word->content));

               /* We have found the word. */
               if (word->content.type == DW_CONTENT_TEXT) {
                  /* Search the character the mouse pointer is in.
                   * next_word_x is the right side of this character. */
                  char_pos = 0;
                  while ((next_word_x = word_start_x +
                          gdk_text_width(word->style->font->font,
                                         word->content.data.text,
                                         char_pos))
                         <= x)
                     char_pos++;

                  /* The left side of this character. */
                  word_x =
                     word_start_x + gdk_text_width (word->style->font->font,
                                                    word->content.data.text,
                                                    char_pos - 1);

                  /* If the mouse pointer is left from the middle, use the left
                   * position, otherwise, use the right one. */
                  if (x <= (word_x + next_word_x) / 2)
                     char_pos--;
               } else {
                  /* Depends on whether the pointer is within the left or
                   * right half of the (non-text) word. */
                  if (x >= (word_start_x + next_word_start_x) / 2)
                     char_pos = SELECTION_EOW;
                  else
                     char_pos = 0;
               }

               found = TRUE;
               link = word->style ? word->style->x_link : -1;
               break;
            }
         }

         if (!found) {
            /* No word found in this line (i.e. we are on the right side),
             * take the last of this line. */
            DEBUG_MSG (DEBUG_EVENT_LEVEL, "    not found\n");
            within_content = FALSE;
            word_index = line->last_word - 1;
            if (word_index >= page->num_words)
               word_index--;
            word = &page->words[word_index];
            char_pos = word->content.type == DW_CONTENT_TEXT ?
               strlen (word->content.data.text) : SELECTION_EOW;
         }
      }
   }

   word = &page->words[word_index];
   it = Dw_page_construct_iterator (page, word_index);
   DEBUG_MSG (DEBUG_EVENT_LEVEL, "-> word %d (of %d), char %d, link %d: %s\n",
              word_index, page->num_words, char_pos, link,
              a_Dw_iterator_text (it));
   return fn (GTK_DW_VIEWPORT(DW_WIDGET(page)->viewport)->selection,
              it, char_pos, link, event, within_content);
}


/*
 * Standard Dw function.
 */
static gboolean Dw_page_button_press  (DwWidget *widget,
                                       gint32 x,
                                       gint32 y,
                                       GdkEventButton *event)
{
   return Dw_page_send_selection_event (DW_PAGE (widget),
                                        a_Selection_button_press, x, y,
                                        event);
}


/*
 * Standard Dw function.
 */
static gboolean Dw_page_button_release (DwWidget *widget,
                                        gint32 x,
                                        gint32 y,
                                        GdkEventButton *event)
{
   return Dw_page_send_selection_event (DW_PAGE (widget),
                                        a_Selection_button_release, x, y,
                                        event);
}


/*
 * Standard Dw function.
 */
static gboolean Dw_page_motion_notify (DwWidget *widget,
                                       gint32 x,
                                       gint32 y,
                                       GdkEventMotion *event)
{
   DwPage *page = DW_PAGE (widget);
   gint link_old, word_index;
   DwTooltip *tooltip_old;
   gboolean return_val = FALSE;

   if (event && (event->state & GDK_BUTTON1_MASK))
      return Dw_page_send_selection_event (page,
                                           a_Selection_button_motion, x, y,
                                           NULL);
   else {
      /*DEBUG_MSG (DEBUG_EVENT_LEVEL, "Dw_page_motion_notify\n");*/
      word_index = Dw_page_find_word (page, x, y);

      link_old = page->hover_link;
      tooltip_old = page->hover_tooltip;

      if (word_index == -1) {
         page->hover_link = -1;
         page->hover_tooltip = NULL;
      } else {
         page->hover_link = page->words[word_index].style->x_link;
         page->hover_tooltip = page->words[word_index].style->x_tooltip;
      }

      /* Show/hide tooltip */
      if (tooltip_old != page->hover_tooltip) {
         if (tooltip_old)
            a_Dw_tooltip_on_leave (tooltip_old);
         if (page->hover_tooltip)
            a_Dw_tooltip_on_enter(page->hover_tooltip);
      } else if (page->hover_tooltip)
         a_Dw_tooltip_on_motion(page->hover_tooltip);

      if (page->hover_link != link_old) {
         gtk_signal_emit (GTK_OBJECT (widget), page_signals[LINK_ENTERED],
                          page->hover_link, -1, -1, &return_val);
         return return_val;
      } else
         return (page->hover_link != -1);
   }
}


/*
 * Standard Dw function.
 */
static gboolean Dw_page_leave_notify (DwWidget *widget,
                                      DwWidget *next_widget,
                                      GdkEventMotion *event)
{
   DwPage *page = DW_PAGE (widget);
   gboolean return_val = FALSE;

   if (page->hover_tooltip) {
      a_Dw_tooltip_on_leave(page->hover_tooltip);
      page->hover_tooltip = NULL;
   }

   if (page->hover_link != -1) {
      page->hover_link = -1;
      gtk_signal_emit (GTK_OBJECT (widget), page_signals[LINK_ENTERED],
                       -1, -1, -1, &return_val);
      return return_val;
   }

   return FALSE;
}


/*
 * Add a new word (text, widget etc.) to a page.
 */
static DwPageWord *Dw_page_add_word (DwPage *page,
                                     gint width,
                                     gint ascent,
                                     gint descent,
                                     DwStyle *style)
{
   DwPageWord *word;
   int i;

   page->num_words++;
   a_List_add (page->words, page->num_words, page->num_words_max);

   word = &page->words[page->num_words - 1];
   word->size.width = width;
   word->size.ascent = ascent;
   word->size.descent = descent;
   word->orig_space = 0;
   word->eff_space = 0;
   word->content.space = FALSE;

   DBG_OBJ_ARRSET_NUM (page, "words.%d.size.width", page->num_words - 1,
                       word->size.width);
   DBG_OBJ_ARRSET_NUM (page, "words.%d.size.descent", page->num_words - 1,
                       word->size.descent);
   DBG_OBJ_ARRSET_NUM (page, "words.%d.size.ascent", page->num_words - 1,
                       word->size.ascent);
   DBG_OBJ_ARRSET_NUM (page, "words.%d.orig_space", page->num_words - 1,
                       word->orig_space);
   DBG_OBJ_ARRSET_NUM (page, "words.%d.eff_space", page->num_words - 1,
                       word->eff_space);
   DBG_OBJ_ARRSET_NUM (page, "words.%d.content.space", page->num_words - 1,
                       word->content.space);

   for (i = 0; i < DW_HIGHLIGHT_NUM_LAYERS; i++) {
      word->hl_start[i] = -1;
      word->hl_end[i] = -1;
   }

   word->style = style;
   word->space_style = style;
   a_Dw_style_ref (style);
   a_Dw_style_ref (style);

   return word;
}

/*
 * Calculate the size of a text word.
 */
static void Dw_page_calc_text_size  (DwPage *page, char *text, DwStyle *style,
                                     DwRequisition *size)
{
   size->width = gdk_string_width (style->font->font, text);
   size->ascent = style->font->font->ascent;
   size->descent = style->font->font->descent;

   /* In case of a sub or super script we increase the word's height and
    * potentially the line's height.
    */
   if (style->valign == DW_STYLE_VALIGN_SUB)
      size->descent += (size->ascent / 2);
   else if (style->valign == DW_STYLE_VALIGN_SUPER)
      size->ascent += (size->ascent / 3);
}


/*
 * Add a word to the page structure. Stashes the argument pointer in
 * the page data structure so that it will be deallocated on destroy.
 */
void a_Dw_page_add_text (DwPage *page, char *text, DwStyle *style)
{
   DwPageWord *word;
   DwRequisition size;

   Dw_page_calc_text_size (page, text, style, &size);
   word = Dw_page_add_word (page, size.width, size.ascent, size.descent,
                            style);
   word->content.type = DW_CONTENT_TEXT;
   word->content.data.text = text;

   DBG_OBJ_ARRSET_STR (page, "words.%d.content.text", page->num_words - 1,
                       word->content.data.text);

   Dw_page_word_wrap (page, page->num_words - 1);
}

/*
 * Add a widget (word type) to the page.
 */
void a_Dw_page_add_widget (DwPage *page,
                           DwWidget *widget,
                           DwStyle *style)
{
   DwPageWord *word;
   DwRequisition size;

   /* We first assign -1 as parent_ref, since the call of widget->size_request
    * will otherwise let this DwPage be rewrapped from the beginning.
    * (parent_ref is actually undefined, but likely has the value 0.) At the,
    * end of this function, the correct value is assigned. */
   widget->parent_ref = -1;

   p_Dw_widget_set_parent (widget, DW_WIDGET (page));
   a_Dw_widget_set_style (widget, style);

   Dw_page_calc_widget_size (page, widget, &size);
   word =
      Dw_page_add_word (page, size.width, size.ascent, size.descent, style);

   word->content.type = DW_CONTENT_WIDGET;
   word->content.data.widget = widget;

   DBG_OBJ_ARRSET_PTR (page, "words.%d.content.widget", page->num_words - 1,
                       word->content.data.widget);

   Dw_page_word_wrap (page, page->num_words - 1);
   word->content.data.widget->parent_ref = page->num_lines - 1;
   DBG_OBJ_SET_NUM (word->content.data.widget, "parent_ref",
                    word->content.data.widget->parent_ref);

   DEBUG_MSG(DEBUG_REWRAP_LEVEL,
             "Assigning parent_ref = %d to added word %d, "
             "in page with %d word(s)\n",
             page->num_lines - 1, page->num_words - 1, page->num_words);
}


/*
 * Add an anchor to the page. "name" is copied, so no strdup is neccessary for
 * the caller.
 *
 * Return TRUE on success, and FALSE, when this anchor had already been
 * added to the widget tree.
 */
gboolean a_Dw_page_add_anchor (DwPage *page, const gchar *name, DwStyle *style)
{
   DwPageWord *word;
   char *copy;
   gint32 y;

   if (page->num_lines == 0)
      y = 0;
   else
      y = Dw_page_line_total_y_offset_i (page, page->num_lines - 1);

   /*
    * Since an anchor does not take any space, it is safe to call 
    * p_Dw_gtk_viewport_add_anchor already here.
    */
   if ((copy = p_Dw_gtk_viewport_add_anchor(DW_WIDGET(page), name, y)) == NULL)
      return FALSE;
   else {
      word = Dw_page_add_word (page, 0, 0, 0, style);
      word->content.type = DW_CONTENT_ANCHOR;
      word->content.data.anchor = copy;
      Dw_page_word_wrap (page, page->num_words - 1);
      
      DBG_OBJ_ARRSET_STR (page, "words.%d.content.anchor", page->num_words - 1,
                          word->content.data.anchor);

      return TRUE;
   }
}


/*
 * ?
 */
void a_Dw_page_add_space (DwPage *page, DwStyle *style)
{
   gint nl, nw;
   gint space;

   nl = page->num_lines - 1;
   if (nl >= 0) {
      nw = page->num_words - 1;
      if (nw >= 0) {
         /* todo: remove this test case */
         if (page->words[nw].orig_space != 0) {
            _MSG("   a_Dw_page_add_space:: already existing space!!!\n");
         }
         space = style->font->space_width;
         page->words[nw].orig_space = space;
         page->words[nw].eff_space = space;
         page->words[nw].content.space = TRUE;

         DBG_OBJ_ARRSET_NUM (page, "words.%d.orig_space", nw,
                             page->words[nw].orig_space);
         DBG_OBJ_ARRSET_NUM (page, "words.%d.eff_space", nw,
                             page->words[nw].eff_space);
         DBG_OBJ_ARRSET_NUM (page, "words.%d.content.space", nw,
                             page->words[nw].content.space);

         a_Dw_style_unref (page->words[nw].space_style);
         page->words[nw].space_style = style;
         a_Dw_style_ref (style);
      }
   }
}


/*
 * Cause a paragraph break
 */
void a_Dw_page_add_parbreak (DwPage *page, gint space, DwStyle *style)
{
   DwPageWord *word, *word2;
   DwWidget *widget;
   DwPage *page2;
   gboolean isfirst;
   gint lineno;

   /* A break may not be the first word of a page, or directly after
      the bullet/number (which is the first word) in a list item. (See
      also comment in Dw_page_size_request.) */
   if (page->num_words == 0 ||
       (page->list_item && page->num_words == 1)) {
      /* This is a bit hackish: If a break is added as the
         first/second word of a page, and the parent widget is also a
         DwPage, and there is a break before -- this is the case when
         a widget is used as a text box (lists, blockquotes, list
         items etc) -- then we simply adjust the break before, in a
         way that the space is in any case visible. */

      /* Find the widget where to adjust the break_space. */
      for (widget = DW_WIDGET (page);
           widget->parent && DW_IS_PAGE (widget->parent);
           widget = widget->parent) {
         page2 = DW_PAGE (widget->parent);
         if (page2->list_item)
            isfirst = (page2->words[1].content.type == DW_CONTENT_WIDGET
                       && page2->words[1].content.data.widget == widget);
         else
            isfirst = (page2->words[0].content.type == DW_CONTENT_WIDGET
                        && page2->words[0].content.data.widget == widget);
         if (!isfirst) {
            /* The page we searched for has been found. */
            lineno = widget->parent_ref;
            if (lineno > 0 &&
                (word2 = &page2->words[page2->lines[lineno - 1].first_word]) &&
                word2->content.type == DW_CONTENT_BREAK) {
               if (word2->content.data.break_space < space) {
                  word2->content.data.break_space = space;
                  p_Dw_widget_queue_resize (DW_WIDGET (page2), lineno, FALSE);
                  page2->must_queue_resize = FALSE;
               }
            }
            return;
         }
         /* Otherwise continue to examine parents. */
      }
      /* Return in any case. */
      return;
   }

   /* Another break before? */
   if ((word = &page->words[page->num_words - 1]) &&
       word->content.type == DW_CONTENT_BREAK) {
      word->content.data.break_space =
         MAX (word->content.data.break_space, space);
      return;
   }

   word = Dw_page_add_word (page, 0, 0, 0, style);
   word->content.type = DW_CONTENT_BREAK;
   word->content.data.break_space = space;
   Dw_page_word_wrap (page, page->num_words - 1);
}

/*
 * Cause a line break.
 */
void a_Dw_page_add_linebreak (DwPage *page, DwStyle *style)
{
   DwPageWord *word;

   if (page->num_words == 0 ||
       page->words[page->num_words - 1].content.type == DW_CONTENT_BREAK)
       /* An <BR> in an empty line gets the height of the current font
        * (why would someone else place it here?), ...  */
      word = Dw_page_add_word (page, 0, style->font->font->ascent,
                               style->font->font->descent, style);
   else
      /* ... otherwise, it has no size (and does not enlarge the line). */
      word = Dw_page_add_word (page, 0, 0, 0, style);
   word->content.type = DW_CONTENT_BREAK;
   word->content.data.break_space = 0;
   word->style = style;
   Dw_page_word_wrap (page, page->num_words - 1);
}

/*
 * This function "hands" the last break of a page "over" to a parent
 * page. This is used for "collapsing spaces".
 */
void a_Dw_page_hand_over_break (DwPage *page, DwStyle *style)
{
   DwPageLine *last_line;
   DwWidget *parent;

   if (page->num_lines == 0)
      return;

   last_line = &page->lines[page->num_lines - 1];
   if (last_line->break_space != 0 &&
       (parent = DW_WIDGET(page)->parent) && DW_IS_PAGE (parent))
      a_Dw_page_add_parbreak (DW_PAGE (parent), last_line->break_space, style);
}

/*
 * Any words added by a_Dw_page_add_... are not immediately (queued to
 * be) drawn, instead, this function must be called. This saves some
 * calls to p_Dw_widget_queue_resize.
 *
 */
void a_Dw_page_flush (DwPage *page)
{
   if (page->must_queue_resize) {
      p_Dw_widget_queue_resize (DW_WIDGET (page), -1, TRUE);
      page->must_queue_resize = FALSE;
   }
}

/*
 * Change the color of all words with a specific link. Used to
 * visualize links opened in a new window.
 */
void a_Dw_page_change_link_color (DwPage *page,
                                  gint link,
                                  gint32 new_color)
{
   DwStyle *old_style, style_attrs;
   gint i, j, changed;
   DwWidget *widget = DW_WIDGET (page);

   for (i = 0; i < page->num_lines; i++) {
      changed = FALSE;

      for (j = page->lines[i].first_word; j < page->lines[i].last_word; j++)
         if (page->words[j].style->x_link == link) {
            old_style = page->words[j].style;

            style_attrs = *old_style;
            style_attrs.color =
               a_Dw_style_color_new (new_color, widget->viewport->window);
            page->words[j].style =
               a_Dw_style_new (&style_attrs, widget->viewport->window);
            /* unref'ing it before may crash dillo! */
            a_Dw_style_unref (old_style);
            changed = TRUE;
         }

      if (changed)
         p_Dw_widget_queue_draw_area (widget, 0,
                                      Dw_page_line_total_y_offset_i (page, i),
                                      widget->allocation.width,
                                      page->lines[i].ascent
                                      + page->lines[i].descent);
   }
}

/*
 * Assign a new style to some text words. Other words than text are not
 * affected.
 */
void a_Dw_page_change_word_style (DwPage *page, gint from, gint to,
                                  DwStyle *style, gboolean include_first_space,
                                  gboolean include_last_space)
{
   int i;
   DwStyle *old_style = NULL, *old_space_style;
   gint line, break_line = DW_INFINITY;
   DwWidget *widget = DW_WIDGET (page);
   gboolean diffs = FALSE, must_queue_resize = FALSE;
   DwPageWord *word;
   DwRequisition size;

   if (from == to)
      return;

   line = Dw_page_find_line_of_word (page, MAX (from - 1, 0));

   /* We also have to include the space before the first word. */
   for (i = MAX (from - 1, 0); i < to; i++) {
      word = &page->words[i];

      if (word->content.type == DW_CONTENT_TEXT) {
         if (i >= from) {
            old_style = word->style;
            word->style = style;
            a_Dw_style_ref (style);

            if ((diffs = a_Dw_style_size_diffs (old_style, style))) {
               must_queue_resize = TRUE;
               break_line = MIN (break_line, line);
               Dw_page_calc_text_size (page, word->content.data.text,
                                       style, &size);
               word->size = size;
            }
         }


         if ((i >= from || include_first_space) &&
             (i < to -1 || include_last_space)) {
            old_space_style = word->space_style;
            word->space_style = style;
            a_Dw_style_ref (style);

            /* In most cases, old_space_style will be old_style, so
             * that a_Dw_style_size_diffs does not have to be called.
             * Note: For the space before the first word, old_style is
             * NULL.
             */
            if (old_space_style == old_style ?
                diffs : a_Dw_style_size_diffs (old_space_style, style)) {
               must_queue_resize = TRUE;
               break_line = MIN (break_line, line);
               if (word->content.space)
                  word->orig_space = style->font->space_width;
            }

            a_Dw_style_unref (old_space_style);
         }

         if (old_style)
            a_Dw_style_unref (old_style);
      }

      /* Resizing will also imply redrawing, so if we resize anyway,
       * redrawing can be skipped. Otherwise, whole lines are redrawn,
       * i.e. when we have completed the whole task, or if this word
       * is the last one in the line.
       */
      if (!must_queue_resize &&
          (i == to - 1 || i == page->lines[line].last_word - 1)) {
         p_Dw_widget_queue_draw_area
            (widget, 0, Dw_page_line_total_y_offset_i (page, line),
             widget->allocation.width,
             page->lines[line].ascent + page->lines[line].descent);

         if (i < to - 1)
            line = Dw_page_find_line_of_word (page, i + 1);
      }
   }

   if (must_queue_resize)
      p_Dw_widget_queue_resize (widget, break_line, TRUE);
}


/*
 * Standard Dw function.
 */
static DwIterator *Dw_page_iterator (DwWidget *widget,
                                     gint mask,
                                     gboolean at_end)
{
   DwIteratorInt *it = g_new (DwIteratorInt, 1);
   it->it.widget = widget;
   it->it.mask = mask;
   it->it.next = Dw_page_iterator_next;
   it->it.prev = Dw_page_iterator_prev;
   it->it.clone = p_Dw_iterator_clone_std_int;
   it->it.compare = p_Dw_iterator_compare_std_int;
   it->it.free = p_Dw_iterator_free_std;
   it->it.highlight = Dw_page_iterator_highlight;
   it->it.get_allocation = Dw_page_iterator_get_allocation;

   if (at_end) {
      it->it.content.type = DW_CONTENT_END;
      it->pos = DW_PAGE(widget)->num_words;
   } else {
      it->it.content.type = DW_CONTENT_START;
      it->pos = -1;
   }

   DEBUG_MSG (DEBUG_ITERATOR_LEVEL, "Dw_page_iterator -> %p, at %s\n",
              it, at_end ? "end" : "start");

   return (DwIterator*)it;
}

/*
 * Standard Dw function.
 */
static gboolean Dw_page_iterator_next (DwIterator *it)
{
   DwPage *page = DW_PAGE (it->widget);
   DwIteratorInt *ii = (DwIteratorInt*)it;

   DEBUG_MSG (DEBUG_ITERATOR_LEVEL, "Dw_page_iterator_next (%p): %d of %d\n",
              it, ii->pos, page->num_words);

   if (it->content.type == DW_CONTENT_END)
      return FALSE;

   do {
      ii->pos++;
      if (ii->pos >= page->num_words) {
         it->content.type = DW_CONTENT_END;
         return FALSE;
      }
   } while ((page->words[ii->pos].content.type & it->mask) == 0);

   it->content = page->words[ii->pos].content;
   return TRUE;
}

/*
 * Standard Dw function.
 */
static gboolean Dw_page_iterator_prev (DwIterator *it)
{
   DwPage *page = DW_PAGE (it->widget);
   DwIteratorInt *ii = (DwIteratorInt*)it;

   DEBUG_MSG (DEBUG_ITERATOR_LEVEL, "Dw_page_iterator_prev (%p): %d of %d\n",
              it, ii->pos, page->num_words);

   if (it->content.type == DW_CONTENT_START)
      return FALSE;

   do {
      ii->pos--;
      if (ii->pos < 0) {
         it->content.type = DW_CONTENT_START;
         return FALSE;
      }
   } while ((page->words[ii->pos].content.type & it->mask) == 0);

   it->content = page->words[ii->pos].content;
   return TRUE;
}

/*
 * Standard Dw function.
 */
void Dw_page_iterator_highlight (DwIterator *it,
                                 gint start,
                                 gint end,
                                 DwHighlightLayer layer)
{
   DwIteratorInt *ii = (DwIteratorInt*)it;
   DwPage *page = DW_PAGE(it->widget);
   gint line;

   page->words[ii->pos].hl_start[layer] = start;
   page->words[ii->pos].hl_end[layer] = end;

   line = Dw_page_find_line_of_word (page, ii->pos);
   p_Dw_widget_queue_draw_area (it->widget, 0,
                                Dw_page_line_total_y_offset_i (page, line),
                                it->widget->allocation.width,
                                page->lines[line].ascent
                                + page->lines[line].descent);
}

/*
 * Standard Dw function.
 */
static void Dw_page_iterator_get_allocation (DwIterator *it,
                                             gint start,
                                             gint end,
                                             DwAllocation *allocation)
{
   DwIteratorInt *ii = (DwIteratorInt*)it;
   DwPage *page = DW_PAGE(it->widget);
   DwPageWord *word;
   DwPageLine *line;
   gint wi, l;
   gint32 diff;

   line = &page->lines[Dw_page_find_line_of_word (page, ii->pos)];
   word = &page->words[ii->pos];

   /* the difference at the start */
   if (start == 0)
      diff = 0;
   else {
      if (word->content.type == DW_CONTENT_TEXT)
         diff = gdk_text_width (word->style->font->font,
                                word->content.data.text, start);
      else
         diff = word->size.width;
   }

   allocation->x =
      Dw_page_line_total_x_offset (page, line) + it->widget->allocation.x
         + diff;
   wi = line->first_word;
   while (wi < ii->pos) {
      word = &page->words[wi];
      allocation->x += word->size.width + word->eff_space;
      wi++;
   }

   allocation->y =
      Dw_page_line_total_y_offset (page, line) + it->widget->allocation.y;

   word = &page->words[wi];
   if (word->content.type == DW_CONTENT_TEXT) {
      l = strlen (word->content.data.text);
      if (end == l)
         allocation->width = word->size.width;
      else if (end >= l)
         allocation->width = word->size.width + word->eff_space;
      else
         allocation->width = gdk_text_width (word->style->font->font,
                                             word->content.data.text, end);
   } else {
      if (end == 1)
         allocation->width = word->size.width;
      else if (end >= 1)
         allocation->width = word->size.width + word->eff_space;
      else
         allocation->width = 0;
   }

   allocation->width -= diff;

   allocation->ascent = page->words[wi].size.ascent;
   allocation->descent = page->words[wi].size.descent;

   DBG_MSGF (page, "scrolling", 0,
             "Dw_page_iterator_get_allocation: (%d, %d), (%d x %d x %d)",
             allocation->x, allocation->y,
             allocation->width, allocation->ascent, allocation->descent);
}
