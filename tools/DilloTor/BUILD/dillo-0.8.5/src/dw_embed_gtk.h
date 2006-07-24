#ifndef __DW_EMBED_GTK_H__
#define __DW_EMBED_GTK_H__

#include "dw_widget.h"
#include <gtk/gtkwidget.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DW_TYPE_EMBED_GTK               (a_Dw_embed_gtk_get_type ())
#define DW_EMBED_GTK(obj)               GTK_CHECK_CAST (obj, \
                                           DW_TYPE_EMBED_GTK, DwEmbedGtk)
#define DW_EMBED_GTK_CLASS(klass)       GTK_CHECK_CLASS_CAST (klass, \
                                           DW_TYPE_EMBED_GTK,  DwEmbedGtkClass)
#define DW_IS_EMBED_GTK(obj)            GTK_CHECK_TYPE (obj, DW_TYPE_EMBED_GTK)

typedef struct _DwEmbedGtk       DwEmbedGtk;
typedef struct _DwEmbedGtkClass  DwEmbedGtkClass;

struct _DwEmbedGtk
{
   DwWidget container;

   GtkWidget *child;
   gint idle_id;
   /*gboolean child_size_request_lock*/;
};


struct _DwEmbedGtkClass
{
   DwWidgetClass parent_class;
};

GtkType         a_Dw_embed_gtk_get_type (void);
DwWidget*       a_Dw_embed_gtk_new      (void);
void            a_Dw_embed_gtk_add_gtk  (DwEmbedGtk *embed_gtk,
                                         GtkWidget *widget);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DW_EMBED_GTK_H__ */
