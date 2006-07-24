/* This module contains the dw_button widget, which is the "back end" to
   Web text widgets including html. */

#ifndef __DW_BUTTON_H__
#define __DW_BUTTON_H__

#include "dw_container.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DW_TYPE_BUTTON            (a_Dw_button_get_type ())
#define DW_BUTTON(obj)            GTK_CHECK_CAST ((obj), DW_TYPE_BUTTON, \
                                                  DwButton)
#define DW_BUTTON_CLASS(klass)    GTK_CHECK_CLASS_CAST ((klass), \
                                                        DW_TYPE_BUTTON, \
                                                        DwButtonClass)
#define DW_IS_BUTTON(obj)         GTK_CHECK_TYPE ((obj), DW_TYPE_BUTTON)
#define DW_IS_BUTTON_CLASS(klass) GTK_CHECK_CLASS_TYPE ((klass), \
                                                        DW_TYPE_BUTTON)

typedef struct _DwButton        DwButton;
typedef struct _DwButtonClass   DwButtonClass;



struct _DwButton
{
   DwContainer container;

   DwWidget *child;
   gboolean relief, in_button, pressed, sensitive;
};


struct _DwButtonClass
{
   DwContainerClass parent_class;

   void (*clicked)    (DwButton *button);
   void (*clicked_at) (DwButton *button,
                       gint32 x,
                       gint32 y);
};


GtkType    a_Dw_button_get_type      (void);
DwWidget*  a_Dw_button_new           (gint flags,
                                      gboolean relief);
void       a_Dw_button_set_sensitive (DwButton *button,
                                      gboolean sensitive);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __DW_BUTTON_H__ */
