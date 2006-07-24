/*
 * File: dw_table.h
 *
 * Copyright (C) 2001 Sebastian Geerken <sgeerken@users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __DW_TABLE_H__
#define __DW_TABLE_H__

#include "dw_container.h"
#include "dw_table_cell.h"
#include "bitvec.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DW_TYPE_TABLE           (a_Dw_table_get_type ())
#define DW_TABLE(obj)           GTK_CHECK_CAST (obj,DW_TYPE_TABLE, DwTable)
#define DW_TABLE_CLASS(klass)   GTK_CHECK_CLASS_CAST (klass, DW_TYPE_TABLE, \
                                   DwTableClass)
#define DW_IS_TABLE(obj)        GTK_CHECK_TYPE (obj, DW_TYPE_TABLE)


typedef struct _DwTable       DwTable;
typedef struct _DwTableClass  DwTableClass;
typedef struct _DwTableChild  DwTableChild;
typedef struct _DwTableSub    DwTableSub;

struct _DwTableChild
{
   enum {
      DW_TABLE_CELL,                    /* cell starts here */
      DW_TABLE_SPAN_SPACE               /* part of a spanning cell */
   } type;

   union {
      struct {
         DwWidget *widget;
         gint colspan, rowspan;
      } cell;
      struct {
         gint start_col, start_row;     /* where the cell starts */
      } span_space;
   } data;
};

struct _DwTableSub
{
   DwTable *table;
   gint start_col, end_col;
   bitvec_t *spanning_rows, *removed_rows;
   DwExtremes span_extremes, total_extremes;
   gboolean fixed_width;
   gint use_percentage;
   gfloat percentage;

   DwTableSub *subs;
   gint num_subs;
};

struct _DwTable
{
   DwContainer container;

   /* set by set_... */
   gint32 avail_width, avail_ascent, avail_descent;

   gint cur_row, cur_col;

   gint32 *col_width;    // num_cols members
   gint num_cols;
   gint num_col_width_max;

   gint32 *cum_height;  // row cumulative height array:
   gint num_rows;       //  num_cum_height is (num_rows + 1),
   gint cum_height_max; //  cum_height[0] is always SPACING,
                        //  cum_height[num_rows] is the total table height

   DwStyle **row_style;
   gint row_style_max;

   gint32 *baseline;
   gint baseline_max;

   DwTableSub *sub;

   DwTableChild **children;
   gint num_children;
   gint num_children_max; /* number allocated */
};

struct _DwTableClass
{
   DwContainerClass parent_class;
};

GtkType      a_Dw_table_get_type     (void);
DwWidget*    a_Dw_table_new          (void);

void         a_Dw_table_add_cell     (DwTable *table,
                                      DwWidget *widget,
                                      gint colspan,
                                      gint rowspan);
void         a_Dw_table_add_row      (DwTable *table,
                                      DwStyle *style);
DwTableCell* a_Dw_table_get_cell_ref (DwTable *table);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DW_TABLE_H__ */
