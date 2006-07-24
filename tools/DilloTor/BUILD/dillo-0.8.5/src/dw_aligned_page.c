/*
 * File: dw_aligned_page.c
 *
 * Copyright (C) 2002, 2003  Sebastian Geerken <S.Geerken@ping.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * This is an abstract class for horizontical alignment of an vertical
 * array of text blocks, e.g. list items (with list item markers of
 * different width) and (in the future) table cells aligned at
 * characters (e.g. decimal point/comma).
 *
 * The aligned text blocks are represented by instances of sub-classes
 * of DwAlignedPage, which define a value used for alignment
 * (e.g. list item marker width), and implement the following two
 * methods:
 *
 *    get_value:     Returns the value independent of all other
 *                   values.
 *
 *    set_max_value: This method is called to set the actual value,
 *                   calculated from the maximum of all values.
 */

#include "dw_aligned_page.h"
#include "list.h"

static void   Dw_aligned_page_class_init         (DwAlignedPageClass *klass);
static void   Dw_aligned_page_init               (DwAlignedPage *aligned_page);

static void   Dw_aligned_page_destroy            (GtkObject *object);

static gint32 Dw_aligned_page_real_get_value     (DwAlignedPage *aligned_page);
static void   Dw_aligned_page_real_set_max_value (DwAlignedPage *aligned_page,
                                                  gint32 max_value,
                                                  gint32 value);

static DwPageClass *parent_class;

/*
 * Standard Gtk+ function.
 */
GtkType a_Dw_aligned_page_get_type (void)
{
   static GtkType type = 0;

   if (!type) {
      GtkTypeInfo info = {
         "DwAlignedPage",
         sizeof (DwAlignedPage),
         sizeof (DwAlignedPageClass),
         (GtkClassInitFunc) Dw_aligned_page_class_init,
         (GtkObjectInitFunc) Dw_aligned_page_init,
         (GtkArgSetFunc) NULL,
         (GtkArgGetFunc) NULL,
         (GtkClassInitFunc) NULL
      };

      type = gtk_type_unique (DW_TYPE_PAGE, &info);
   }

   return type;
}


/*
 * Standard Gtk+ function.
 */
static void Dw_aligned_page_class_init (DwAlignedPageClass *klass)
{
   parent_class = gtk_type_class (DW_TYPE_PAGE);
   GTK_OBJECT_CLASS(klass)->destroy = Dw_aligned_page_destroy;
   klass->get_value = Dw_aligned_page_real_get_value;
   klass->set_max_value = Dw_aligned_page_real_set_max_value;
}


/*
 * Standard Gtk+ function.
 */
static void Dw_aligned_page_init (DwAlignedPage *aligned_page)
{
   aligned_page->list = NULL;
}


/*
 * Standard Gtk+ function.
 */
static void Dw_aligned_page_destroy (GtkObject *object)
{
   DwAlignedPage *aligned_page = DW_ALIGNED_PAGE (object);

   if (aligned_page->list) {
      if (aligned_page->list->refcount == 1) {
         /* This is the last page in the array, so the list is removed. */
         g_free (aligned_page->list->pages);
         g_free (aligned_page->list->values);
         g_free (aligned_page->list);
      } else {
         /* Remove this page from the list. The values are simply set to
            NULL and 0. */
         aligned_page->list->pages[aligned_page->list_pos] = NULL;
         aligned_page->list->values[aligned_page->list_pos] = 0;
         aligned_page->list->refcount--;
      }
   }

   GTK_OBJECT_CLASS(parent_class)->destroy (object);
}


/*
 * This function should be called by the sub-class to define the
 * relations between the aligned pages. ref_page is either NULL (this
 * will create a new array), or one page in an other array.
 */
void p_Dw_aligned_page_set_ref_page (DwAlignedPage *aligned_page,
                                     DwAlignedPage *ref_page)
{
   DwAlignedPageList *list;

   if (ref_page == NULL) {
      list = g_new (DwAlignedPageList, 1);
      list->num = 0;
      list->refcount = 0;
      list->pages = NULL;
      list->values = NULL;
      list->num_pages_max = 4;
      list->num_values_max = 4;
      list->max_value = 0;
   } else
      list = ref_page->list;

   list->num++;
   list->refcount++;
   a_List_add (list->pages, list->num, list->num_pages_max);
   list->pages[list->num - 1] = aligned_page;
   a_List_add (list->values, list->num, list->num_values_max);
   list->values[list->num - 1] = 0;
   aligned_page->list = list;
   aligned_page->list_pos = list->num - 1;
   p_Dw_aligned_page_update_value (aligned_page);
}


/*
 * This function should be called by the sub-class whenever the
 * alignment value changes.
 */
void p_Dw_aligned_page_update_value (DwAlignedPage *aligned_page)
{
   DwAlignedPageClass *klass;
   int i;

   if (aligned_page->list) {
      klass = DW_ALIGNED_PAGE_CLASS (GTK_OBJECT(aligned_page)->klass);
      aligned_page->list->values[aligned_page->list_pos] =
         klass->get_value (aligned_page);

      if (aligned_page->list->values[aligned_page->list_pos] >
          aligned_page->list->max_value) {
         /* New value greater than current maximum -> apply it to others. */
         aligned_page->list->max_value =
            aligned_page->list->values[aligned_page->list_pos];

         for (i = 0; i < aligned_page->list->num; i++)
            if (aligned_page->list->pages[i]) {
               klass = DW_ALIGNED_PAGE_CLASS
                  (GTK_OBJECT(aligned_page->list->pages[i])->klass);
               klass->set_max_value (aligned_page->list->pages[i],
                                     aligned_page->list->max_value,
                                     aligned_page->list->values[i]);
            }
      } else {
         /* No change, apply old max_value only to this page. */
         klass = DW_ALIGNED_PAGE_CLASS (GTK_OBJECT(aligned_page)->klass);
         klass->set_max_value (aligned_page, aligned_page->list->max_value,
                               aligned_page->list->values
                                  [aligned_page->list_pos]);
      }
   }
}


/*
 * Standard implementation of DwAlignedPage::get_value.
 */
static gint32 Dw_aligned_page_real_get_value (DwAlignedPage *aligned_page)
{
   g_warning ("DwAlignedPage::get_value not implemented for `%s'",
              gtk_type_name (GTK_OBJECT_TYPE (aligned_page)));
   return 0;
}


/*
 * Standard implementation of DwAlignedPage::set_max_value.
 */
static void Dw_aligned_page_real_set_max_value (DwAlignedPage *aligned_page,
                                                gint32 max_value,
                                                gint32 value)
{
   g_warning ("DwAlignedPage::set_max_value not implemented for `%s'",
              gtk_type_name (GTK_OBJECT_TYPE (aligned_page)));
}
