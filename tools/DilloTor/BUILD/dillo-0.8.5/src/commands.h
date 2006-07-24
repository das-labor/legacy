#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "gtk_ext_button.h"

void a_Commands_new_callback(GtkWidget * widget, gpointer client_data);
void a_Commands_openfile_callback (GtkWidget *widget, gpointer client_data);
void a_Commands_openurl_callback (GtkWidget *widget, gpointer client_data);
void a_Commands_prefs_callback(GtkWidget * widget, gpointer client_data);
void a_Commands_close_callback(GtkWidget * widget, gpointer client_data);
void a_Commands_exit_callback (GtkWidget *widget, gpointer client_data);

void a_Commands_viewsource_callback (GtkWidget *widget, gpointer client_data);
void a_Commands_view_page_bugs_callback (GtkWidget *button,
                                         gpointer client_data);
void a_Commands_selectall_callback (GtkWidget *widget, gpointer client_data);
void a_Commands_findtext_callback (GtkWidget *widget, gpointer client_data);
void a_Commands_print_callback (GtkWidget *widget, gpointer client_data);

GtkMenu *a_Commands_forw_button_menu_creator_callback(GtkExtButton *button,
                                                      gpointer client_data);
GtkMenu *a_Commands_back_button_menu_creator_callback(GtkExtButton *button,
                                                      gpointer client_data);
void a_Commands_history_callback_same_bw(GtkWidget *widget,
                                         gpointer client_data);
void a_Commands_history_callback_new_bw(GtkWidget *widget,
                                        gpointer client_data);

void a_Commands_back_callback (GtkWidget *widget, gpointer client_data);
void a_Commands_forw_callback (GtkWidget *widget, gpointer client_data);
void a_Commands_reload_callback (GtkWidget *widget, gpointer client_data);
void a_Commands_stop_callback (GtkWidget *widget, gpointer client_data);
void a_Commands_home_callback (GtkWidget *widget, gpointer client_data);
void a_Commands_save_callback (GtkWidget *widget, gpointer client_data);
void a_Commands_save_link_callback (GtkWidget *widget, gpointer client_data);

void a_Commands_addbm_callback (GtkWidget *widget, gpointer client_data);
void a_Commands_viewbm_callback (GtkWidget *widget, gpointer client_data);

void a_Commands_helphome_callback (GtkWidget *widget, gpointer client_data);

void a_Commands_open_link_callback(GtkWidget *widget, gpointer client_data);
void a_Commands_open_link_nw_callback(GtkWidget *widget, gpointer client_data);
void a_Commands_select_popup_url_callback(GtkWidget *widget, gpointer data);

void a_Commands_ob_w3c_callback(GtkWidget *widget, gpointer client_data);
void a_Commands_ob_wdg_callback(GtkWidget *widget, gpointer client_data);
void a_Commands_ob_info_callback(GtkWidget *widget, gpointer client_data);

#endif /* __COMMANDS_H__ */
