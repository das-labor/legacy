#ifndef __DW_LIST_ITEM_H__
#define __DW_LIST_ITEM_H__

#include "dw_aligned_page.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DW_TYPE_LIST_ITEM          (a_Dw_list_item_get_type ())
#define DW_LIST_ITEM(obj)          GTK_CHECK_CAST (obj,DW_TYPE_LIST_ITEM, \
                                      DwListItem)
#define DW_LIST_ITEM_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, \
                                      DW_TYPE_LIST_ITEM, DwListItemClass)
#define DW_IS_LIST_ITEM(obj)       GTK_CHECK_TYPE (obj, DW_TYPE_LIST_ITEM)


typedef struct _DwListItem      DwListItem;
typedef struct _DwListItemClass DwListItemClass;

struct _DwListItem
{
   DwAlignedPage aligned_page;
};

struct _DwListItemClass
{
   DwAlignedPageClass parent_class;
};


GtkType   a_Dw_list_item_get_type         (void);
DwWidget* a_Dw_list_item_new              (DwListItem *ref_list_item);
void      a_Dw_list_item_init_with_widget (DwListItem *list_item,
                                           DwWidget *widget,
                                           DwStyle *style);
void      a_Dw_list_item_init_with_text   (DwListItem *list_item,
                                           gchar *text,
                                           DwStyle *style);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DW_LIST_ITEM_H__ */
