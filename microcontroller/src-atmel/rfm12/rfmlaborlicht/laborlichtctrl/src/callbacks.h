#include <gtk/gtk.h>


void
on_addrfield_changed                   (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_colorselection_color_changed        (GtkColorSelection *colorselection,
                                        gpointer         user_data);

void
on_btn_commit_released                 (GtkButton       *button,
                                        gpointer         user_data);

gboolean
llctrl_exit                            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
asjdjasd                               (GtkButton       *button,
                                        gpointer         user_data);
