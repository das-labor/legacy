#ifndef __DW_CONTAINER_H__
#define __DW_CONTAINER_H__

#include "dw_widget.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DW_TYPE_CONTAINER               (a_Dw_container_get_type ())
#define DW_CONTAINER(obj)               GTK_CHECK_CAST (obj, \
                                           DW_TYPE_CONTAINER, DwContainer)
#define DW_CONTAINER_CLASS(klass)       GTK_CHECK_CLASS_CAST (klass, \
                                           DW_TYPE_CONTAINER, DwContainerClass)
#define DW_IS_CONTAINER(obj)            GTK_CHECK_TYPE (obj, DW_TYPE_CONTAINER)


typedef void (*DwCallback) (DwWidget *widget, gpointer data);

typedef struct _DwContainer       DwContainer;
typedef struct _DwContainerClass  DwContainerClass;

struct _DwContainer
{
   DwWidget widget;
};


struct _DwContainerClass
{
   DwWidgetClass parent_class;

   void (* add)                 (DwContainer *container,
                                 DwWidget *child);
   void (* remove)              (DwContainer *container,
                                 DwWidget *widget);
   void (* forall)              (DwContainer *container,
                                 DwCallback callback,
                                 gpointer callbabck_data);
};


GtkType a_Dw_container_get_type         (void);

void    a_Dw_container_add              (DwContainer *container,
                                         DwWidget *child);
void    a_Dw_container_forall           (DwContainer *container,
                                         DwCallback callback,
                                         gpointer callback_data);

void    Dw_container_remove             (DwContainer *container,
                                         DwWidget *child);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DW_CONTAINER_H__ */
