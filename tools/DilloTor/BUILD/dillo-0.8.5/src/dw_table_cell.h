#ifndef __DW_TABLE_CELL_H__
#define __DW_TABLE_CELL_H__

#include "dw_aligned_page.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DW_TYPE_TABLE_CELL          (a_Dw_table_cell_get_type ())
#define DW_TABLE_CELL(obj)          GTK_CHECK_CAST (obj,DW_TYPE_TABLE_CELL, \
                                      DwTableCell)
#define DW_TABLE_CELL_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, \
                                      DW_TYPE_TABLE_CELL, DwTableCellClass)
#define DW_IS_TABLE_CELL(obj)       GTK_CHECK_TYPE (obj, DW_TYPE_TABLE_CELL)


typedef struct _DwTableCell      DwTableCell;
typedef struct _DwTableCellClass DwTableCellClass;

struct _DwTableCell
{
   DwAlignedPage aligned_page;
   gint char_word_index, char_word_pos;
};

struct _DwTableCellClass
{
   DwAlignedPageClass parent_class;
};


GtkType   a_Dw_table_cell_get_type         (void);
DwWidget* a_Dw_table_cell_new              (DwTableCell *ref_cell);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DW_TABLE_CELL_H__ */
