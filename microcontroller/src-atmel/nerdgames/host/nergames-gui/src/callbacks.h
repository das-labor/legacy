#include <gtk/gtk.h>


gboolean
key_press_handler                      (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);

void
btn_clicked                            (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_c1_clicked                      (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_c2_clicked                      (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_c3_clicked                      (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_c4_clicked                      (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_backwards_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_forward_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_mode_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_reset_clicked                   (GtkButton       *button,
                                        gpointer         user_data);
