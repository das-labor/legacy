#ifndef __DW_HRULER_H__
#define __DW_HRULER_H__

#include "dw_widget.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DW_TYPE_HRULER          (a_Dw_hruler_get_type ())
#define DW_HRULER(obj)          GTK_CHECK_CAST (obj,DW_TYPE_HRULER, DwHruler)
#define DW_HRULER_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, DW_TYPE_HRULER, \
                                   DwHrulerClass)
#define DW_IS_HRULER(obj)       GTK_CHECK_TYPE (obj, DW_TYPE_HRULER)

typedef struct _DwHruler      DwHruler;
typedef struct _DwHrulerClass DwHrulerClass;


struct _DwHruler
{
   DwWidget widget;
   gboolean selected[DW_HIGHLIGHT_NUM_LAYERS];
};


struct _DwHrulerClass
{
   DwWidgetClass parent_class;
};


GtkType         a_Dw_hruler_get_type    (void);
DwWidget*       a_Dw_hruler_new         (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DW_HRULER_H__ */
