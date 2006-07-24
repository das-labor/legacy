/* This module contains the dw_page widget, which is the "back end" to
   Web text widgets including html. */

#ifndef __DW_PAGE_H__
#define __DW_PAGE_H__

#include <gdk/gdk.h>
#include "dw_container.h"
#include "url.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DW_TYPE_PAGE            (a_Dw_page_get_type ())
#define DW_PAGE(obj)            GTK_CHECK_CAST ((obj), DW_TYPE_PAGE, DwPage)
#define DW_PAGE_CLASS(klass)    GTK_CHECK_CLASS_CAST ((klass), DW_TYPE_PAGE, \
                                   DwPageClass)
#define DW_IS_PAGE(obj)         GTK_CHECK_TYPE ((obj), DW_TYPE_PAGE)
#define DW_IS_PAGE_CLASS(klass) GTK_CHECK_CLASS_TYPE ((klass), DW_TYPE_PAGE)

typedef struct _DwPage        DwPage;
typedef struct _DwPageClass   DwPageClass;

/* Internal data structures (maybe shouldn't be in public .h file? */
typedef struct _DwPageLine   DwPageLine;
typedef struct _DwPageWord   DwPageWord;

struct _DwPageLine
{
   gint first_word;    /* first-word's position in DwPageWord [0 based] */
   gint last_word;     /* last-word's position in DwPageWord [1 based] */

   /* "top" is always relative to the top of the first line, i.e.
    * page->lines[0].top is always 0. */
   gint32 top, ascent, descent, break_space, left_offset;

   /* This is similar to descent, but includes the bottom margins of the
    * widgets within this line. */
   gint32 margin_descent;

   /* The following members contain accumulated values, from the top
    * down to the line before. */
   gint32 max_line_width; /* maximum of all line widths */
   gint32 max_word_min;   /* maximum of all word minima */
   gint32 max_par_max;    /* maximum of all paragraph maxima */
   gint32 par_min;        /* the minimal total width down from the last
                             paragraph start, to the *beginning* of the
                             line */
   gint32 par_max;        /* the maximal total width down from the last
                             paragraph start, to the *beginning* of the
                             line */
};

struct _DwPageWord {
   /* todo: perhaps add a x_left? */
   DwRequisition size;
   /* Space after the word, only if it's not a break: */
   gint32 orig_space; /* from font, set by a_Dw_page_add_space */
   gint32 eff_space;  /* effective space, set by Dw_page_word_wrap,
                         used for drawing etc. */
   gint hl_start[DW_HIGHLIGHT_NUM_LAYERS], hl_end[DW_HIGHLIGHT_NUM_LAYERS];

   DwContent content;

   DwStyle *style;
   DwStyle *space_style; /* initially the same as of the word, later
                            set by a_Dw_page_add_space */
};


struct _DwPage
{
   DwContainer container;

   /* These fields provide some ad-hoc-functionality, used by sub-classes. */
   gboolean list_item;      /* If TRUE, the first word of the page is treated
                               specially (search in source). */
   gint32 inner_padding;    /* This is an additional padding on the left side
                               (used by DwListItem). */
   gint32 line1_offset;     /* This is an additional offset of the first line.
                               May be negative (shift to left) or positive
                               (shift to right). */
   gint32 line1_offset_eff; /* The "effective" value of line1_offset, may
                               differ from line1_offset when
                               ignore_line1_offset_sometimes is set to TRUE */

   /* The following is really hackish: It is used for DwTableCell (see
    * comment in dw_table_cell.c), to avoid too wide table columns. If
    * set to TRUE, it has following effects:
    *
    *  (i) line1_offset is ignored in calculating the minimal width
    *      (which is used by DwTable!), and
    * (ii) line1_offset is ignored (line1_offset_eff is set to 0),
    *      when line1_offset plus the width of the first word is
    *      greater than the the available witdh.
    *
    * todo: Eliminate all these ad-hoc features by a new, simpler and
    *       more elegant design. ;-)
    */
   gboolean ignore_line1_offset_sometimes;

   gboolean must_queue_resize;

   /* These values are set by set_... */
   gint32 avail_width, avail_ascent, avail_descent;

   gint32 last_line_width;
   gint32 last_line_par_min;
   gint32 last_line_par_max;
   gint wrap_ref;  /* [0 based] */

   DwPageLine *lines;
   gint num_lines;
   gint num_lines_max; /* number allocated */

   DwPageWord *words;
   gint num_words;
   gint num_words_max; /* number allocated */

   /* The word index of the link under a button press, and the char
    * position */
   gint link_pressed_index;
   gint link_pressed_char_pos;

   gint hover_link;          /* The link under the button. */
   DwTooltip *hover_tooltip; /* The tooltip under the button. No ref hold. */
};


struct _DwPageClass
{
   DwContainerClass parent_class;

   gboolean (*link_entered)  (DwPage *page,
                              gint link, gint x, gint y);
   gboolean (*link_pressed)  (DwPage *page,
                              gint link, gint x, gint y,
                              GdkEventButton *event);
   gboolean (*link_released) (DwPage *page,
                              gint link, gint x, gint y,
                              GdkEventButton *event);
   gboolean (*link_clicked)  (DwPage *page,
                              gint link, gint x, gint y,
                              GdkEventButton *event);

   void     (*word_wrap)     (DwPage *page,
                              gint word_ind);
};


DwWidget*  a_Dw_page_new           (void);
GtkType    a_Dw_page_get_type      (void);

void a_Dw_page_flush (DwPage *page);
void a_Dw_page_set_voffset (DwPage *page, gint32 voffset);
void a_Dw_page_add_text (DwPage *page, char *text, DwStyle *style);
void a_Dw_page_add_widget (DwPage *page,
                           DwWidget *widget,
                           DwStyle *style);
gboolean a_Dw_page_add_anchor (DwPage *page, const gchar *name, DwStyle *style);
void a_Dw_page_add_space(DwPage *page, DwStyle *style);
void a_Dw_page_add_parbreak (DwPage *page, gint space, DwStyle *style);
void a_Dw_page_add_linebreak (DwPage *page, DwStyle *style);
void a_Dw_page_hand_over_break (DwPage *page, DwStyle *style);
void a_Dw_page_change_link_color (DwPage *page, gint link, gint32 new_color);
void a_Dw_page_change_word_style (DwPage *page, gint from, gint to,
                                  DwStyle *style, gboolean include_first_space,
                                  gboolean include_last_space);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __DW_PAGE_H__ */
