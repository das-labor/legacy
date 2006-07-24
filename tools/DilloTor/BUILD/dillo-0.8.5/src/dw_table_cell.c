/*
 * File: dw_table_cell.c
 *
 * Copyright (C) 2003  Sebastian Geerken <S.Geerken@ping.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * This widget is used for table cells with character alignment.
 *
 * Some notes: If the character is at a position quite far within the
 * text, this would result in very wide table columns. Since I do not
 * think that an author intends this, DwTableCell makes use of the
 * DwPage::ignore_line1_offset_sometimes feature, which will in this
 * case disable alignment (see comment in dw_page.h). Actually for the
 * whole column, which is simpler to implement (DwTableCell still deals
 * with too large values, but DwPage simply ignores them).
 */

#include "dw_table_cell.h"

/*#define DEBUG_LEVEL 1*/
#include "debug.h"

static void   Dw_table_cell_class_init    (DwTableCellClass *klass);
static void   Dw_table_cell_init          (DwTableCell *table_cell);

static void   Dw_table_cell_word_wrap     (DwPage *page,
                                           gint word_ind);
static gint32 Dw_table_cell_get_value     (DwAlignedPage *aligned_page);
static void   Dw_table_cell_set_max_value (DwAlignedPage *aligned_page,
                                           gint32 max_value,
                                           gint32 value);
static DwAlignedPageClass *parent_class;


/*
 * Standard Gtk+ function.
 */
GtkType a_Dw_table_cell_get_type (void)
{
   static GtkType type = 0;

   if (!type) {
      GtkTypeInfo info = {
         "DwTableCell",
         sizeof (DwTableCell),
         sizeof (DwTableCellClass),
         (GtkClassInitFunc) Dw_table_cell_class_init,
         (GtkObjectInitFunc) Dw_table_cell_init,
         (GtkArgSetFunc) NULL,
         (GtkArgGetFunc) NULL,
         (GtkClassInitFunc) NULL
      };

      type = gtk_type_unique (DW_TYPE_ALIGNED_PAGE, &info);
   }

   return type;
}


/*
 * Standard Gtk+ function.
 */
static void Dw_table_cell_init (DwTableCell *table_cell)
{
   DW_PAGE(table_cell)->ignore_line1_offset_sometimes = TRUE;
   table_cell->char_word_index = -1;
}


/*
 * Standard Gtk+ function: Create a new table_cell item.
 * ref_cell is either another table cell in the same column, or NULL for
 * the first DwTableCell in the column. Typically used with
 * a_Dw_table_get_cell_ref().
 */
DwWidget* a_Dw_table_cell_new (DwTableCell *ref_cell)
{
   GtkObject *object;

   object = gtk_object_new (DW_TYPE_TABLE_CELL, NULL);
   DBG_OBJ_CREATE (object, "DwTableCell");
   p_Dw_aligned_page_set_ref_page (DW_ALIGNED_PAGE (object),
                                   (DwAlignedPage*)ref_cell);
   return DW_WIDGET (object);
}


/*
 * Standard Gtk+ function.
 */
static void Dw_table_cell_class_init (DwTableCellClass *klass)
{
   DwAlignedPageClass *aligned_page_class = DW_ALIGNED_PAGE_CLASS (klass);

   DW_PAGE_CLASS(klass)->word_wrap = Dw_table_cell_word_wrap;
   aligned_page_class->get_value = Dw_table_cell_get_value;
   aligned_page_class->set_max_value = Dw_table_cell_set_max_value;

   parent_class = gtk_type_class (DW_TYPE_ALIGNED_PAGE);
}

/*
 * Standard DwPage function.
 * Extended to use for the first word containing the alignnment character.
 */
static void Dw_table_cell_word_wrap (DwPage *page,
                                     gint word_ind)
{
   DwTableCell *cell = DW_TABLE_CELL (page);
   DwPageWord *word;
   char *p;

   DW_PAGE_CLASS(parent_class)->word_wrap (page, word_ind);

   if (cell->char_word_index == -1) {
      word = &page->words[word_ind];
      if (word->content.type == DW_CONTENT_TEXT) {
         if ((p = strchr (word->content.data.text,
                          word->style->text_align_char))) {
            cell->char_word_index = word_ind;
            cell->char_word_pos = p - word->content.data.text + 1;
         } else if (word->style->text_align_char == ' ' &&
                    word->content.space) {
            cell->char_word_index = word_ind + 1;
            cell->char_word_pos = 0;
         }
      }
   }

   if (word_ind == cell->char_word_index)
      p_Dw_aligned_page_update_value (DW_ALIGNED_PAGE (cell));
}


/*
 * Implementation of DwAlignedPage::get_value.
 */
static gint32 Dw_table_cell_get_value (DwAlignedPage *aligned_page)
{
   DwTableCell *cell = DW_TABLE_CELL (aligned_page);
   DwPage *page = DW_PAGE (aligned_page);
   DwPageWord *word;
   int i, word_index;
   gint32 w;

   if (cell->char_word_index == -1)
      word_index = page->num_words -1;
   else
      word_index = cell->char_word_index;

   w = 0;
   for (i = 0; i < word_index; i++) {
      word = &page->words[i];
      w += word->size.width + word->orig_space;
   }

   if (cell->char_word_index == -1) {
      if (page->num_words > 0) {
         word = &page->words[page->num_words - 1];
         w += word->size.width;
      }
   } else {
      word = &page->words[cell->char_word_index];
      w += gdk_text_width (word->style->font->font, word->content.data.text,
                           cell->char_word_pos);
   }

   return w;
}


/*
 * Implementation of DwAlignedPage::set_max_value.
 */
static void Dw_table_cell_set_max_value (DwAlignedPage *aligned_page,
                                         gint32 max_value,
                                         gint32 value)
{
   DEBUG_MSG (1, "Dw_table_cell_set_max_value([\"%s\" ...], %d, %d)\n",
              (DW_PAGE(aligned_page)->num_words > 0 ?
               (DW_PAGE(aligned_page)->words[0].content.type
                == DW_CONTENT_TEXT ?
                DW_PAGE(aligned_page)->words[0].content.data.text : "(?)")
               : "(empty)"),
              max_value, value);

   DW_PAGE(aligned_page)->line1_offset = max_value - value;
   p_Dw_widget_queue_resize (DW_WIDGET (aligned_page), 0, TRUE);
}
