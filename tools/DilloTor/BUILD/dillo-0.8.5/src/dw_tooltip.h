#ifndef __DW_TOOLTIP_H__
#define __DW_TOOLTIP_H__

#include <gtk/gtkwidget.h>

typedef struct _DwTooltip DwTooltip;

struct _DwTooltip
{
   gint ref_count;
   GtkWidget *window;
   gchar *text;
   guint timeout_id;
};

DwTooltip* a_Dw_tooltip_new        (const gchar *text);
DwTooltip* a_Dw_tooltip_new_no_ref (const gchar *text);

void       a_Dw_tooltip_on_enter   (DwTooltip *tooltip);
void       a_Dw_tooltip_on_leave   (DwTooltip *tooltip);
void       a_Dw_tooltip_on_motion  (DwTooltip *tooltip);

#define a_Dw_tooltip_ref(tooltip)   ((tooltip)->ref_count++)
#define a_Dw_tooltip_unref(tooltip) if (--((tooltip)->ref_count) == 0) \
                                       Dw_tooltip_destroy (tooltip)

/* Don't use this function directly! */
void Dw_tooltip_destroy (DwTooltip *tooltip);

#endif /* __DW_TOOLTIP_H__ */
