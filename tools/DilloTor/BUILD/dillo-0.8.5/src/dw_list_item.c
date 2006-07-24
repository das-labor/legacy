/*
 * File: dw_aligned_page.c
 *
 * Copyright (C) 2002  Sebastian Geerken <S.Geerken@ping.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "dw_list_item.h"
#include "debug.h"

static void   Dw_list_item_class_init    (DwListItemClass *klass);
static void   Dw_list_item_init          (DwListItem *list_item);

static gint32 Dw_list_item_get_value     (DwAlignedPage *aligned_page);
static void   Dw_list_item_set_max_value (DwAlignedPage *aligned_page,
                                          gint32 max_value,
                                          gint32 value);

/*
 * Standard Gtk+ function.
 */
GtkType a_Dw_list_item_get_type (void)
{
   static GtkType type = 0;

   if (!type) {
      GtkTypeInfo info = {
         "DwListItem",
         sizeof (DwListItem),
         sizeof (DwListItemClass),
         (GtkClassInitFunc) Dw_list_item_class_init,
         (GtkObjectInitFunc) Dw_list_item_init,
         (GtkArgSetFunc) NULL,
         (GtkArgGetFunc) NULL,
         (GtkClassInitFunc) NULL
      };

      type = gtk_type_unique (DW_TYPE_ALIGNED_PAGE, &info);
   }

   return type;
}


/*
 * Standard Gtk+ function: Create a new list item.
 * ref_list_item is either another item in the same list, or NULL for
 * the first item in the list.
 */
DwWidget* a_Dw_list_item_new (DwListItem *ref_list_item)
{
   GtkObject *object;

   object = gtk_object_new (DW_TYPE_LIST_ITEM, NULL);
   DBG_OBJ_CREATE (object, "DwListItem");
   p_Dw_aligned_page_set_ref_page (DW_ALIGNED_PAGE (object),
                                   (DwAlignedPage*)ref_list_item);
   return DW_WIDGET (object);
}


/*
 * Standard Gtk+ function.
 */
static void Dw_list_item_class_init (DwListItemClass *klass)
{
   DwAlignedPageClass *aligned_page_class = DW_ALIGNED_PAGE_CLASS (klass);

   aligned_page_class->get_value = Dw_list_item_get_value;
   aligned_page_class->set_max_value = Dw_list_item_set_max_value;
}


/*
 * Standard Gtk+ function.
 */
static void Dw_list_item_init (DwListItem *list_item)
{
   DW_PAGE(list_item)->list_item = TRUE;
}


/*
 * Implementation of DwAlignedPage::get_value.
 */
static gint32 Dw_list_item_get_value (DwAlignedPage *aligned_page)
{
   DwPage *page = DW_PAGE (aligned_page);

   if (page->num_words == 0)
      return 0;
   else
      return page->words[0].size.width + page->words[0].orig_space;
}


/*
 * Implementation of DwAlignedPage::set_max_value.
 */
static void Dw_list_item_set_max_value (DwAlignedPage *aligned_page,
                                        gint32 max_value,
                                        gint32 value)
{
   DwPage *page = DW_PAGE (aligned_page);

   page->inner_padding = max_value;
   page->line1_offset = - value;
   p_Dw_widget_queue_resize (DW_WIDGET (aligned_page), 0, TRUE);
}


/*
 * This function sets a widget as the list item marker (typically
 * DwBullet).
 * (Note: Currently, the following order should be used:
 *    1. a_Dw_list_item_new,
 *    2. add the list item to the parent widget, and
 *    3. a_Dw_list_item_init_with_widget or a_Dw_list_item_init_with_text.)
 */
void a_Dw_list_item_init_with_widget (DwListItem *list_item,
                                      DwWidget *widget,
                                      DwStyle *style)
{
   DwPage *page = DW_PAGE (list_item);

   a_Dw_page_add_widget (page, widget, style);
   a_Dw_page_add_space (page, style);
   p_Dw_aligned_page_update_value (DW_ALIGNED_PAGE (list_item));
}


/*
 * This function sets a text word as the list item marker (typically a
 * number).
 */
void a_Dw_list_item_init_with_text (DwListItem *list_item,
                                    gchar *text,
                                    DwStyle *style)
{
   DwPage *page = DW_PAGE (list_item);

   a_Dw_page_add_text (page, text, style);
   a_Dw_page_add_space (page, style);
   p_Dw_aligned_page_update_value (DW_ALIGNED_PAGE (list_item));
}
