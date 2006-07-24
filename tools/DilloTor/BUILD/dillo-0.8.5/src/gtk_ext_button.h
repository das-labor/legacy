#ifndef __GTK_EXT_BUTTON_H__
#define __GTK_EXT_BUTTON_H__

#include <gtk/gtkbutton.h>
#include <gtk/gtkmenu.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define GTK_EXT_BUTTON(obj)          (GTK_CHECK_CAST ((obj), \
                                        a_Gtk_ext_button_get_type (), \
                                        GtkExtButton))
#define GTK_EXT_BUTTON_CLASS(klass)  (GTK_CHECK_CLASS_CAST ((klass), \
                                        a_Gtk_ext_button_get_type (), \
                                        GtkExtButtonClass))
#define GTK_IS_EXT_BUTTON(obj)       (GTK_CHECK_TYPE ((obj), \
                                        a_Gtk_ext_button_get_type ()))


typedef struct _GtkExtButton       GtkExtButton;
typedef struct _GtkExtButtonClass  GtkExtButtonClass;
   
typedef GtkMenu* (GtkExtButtonMenuCreator)(GtkExtButton *button,
                                            gpointer data);

struct _GtkExtButton
{
   GtkButton button;

   /*
    * What actually happens, when the user presses on the specific button.
    */
   enum {
      GTK_EXT_BUTTON_INACTIVE,      /* Nothing happens. */
      GTK_EXT_BUTTON_COMMAND,       /* Behaves like a normal command button. */
      GTK_EXT_BUTTON_MENU,          /* Pops up a fixed menu. */
      GTK_EXT_BUTTON_MENU_CREATOR   /* Pops up a menu, which may be created by
                                     * a passed function. */
   } action[3];

   union
   {
      GtkMenu *menu;
      struct
      {
         GtkExtButtonMenuCreator *func;
         gpointer data;
      } creator;
   } action_data[3];

   gint menu_signal_id;
   guint pressed_button;
   GtkMenu *active_menu;
};

struct _GtkExtButtonClass
{
   GtkButtonClass parent_class;

  void (*clicked1)  (GtkButton *button);
  void (*clicked2)  (GtkButton *button);
  void (*clicked3)  (GtkButton *button);
};


guint      a_Gtk_ext_button_get_type            (void);
GtkWidget *a_Gtk_ext_button_new                 (void);
GtkWidget *a_Gtk_ext_button_new_with_label      (const gchar *label);

void       a_Gtk_ext_button_set_inactive        (GtkExtButton *button,
                                                 gint button_no);
void       a_Gtk_ext_button_set_command         (GtkExtButton *button,
                                                 gint button_no);
void       a_Gtk_ext_button_attach_menu         (GtkExtButton *button,
                                                 gint button_no,
                                                 GtkMenu *menu);
void       a_Gtk_ext_button_attach_menu_creator (GtkExtButton *button,
                                                 gint button_no,
                                                 GtkExtButtonMenuCreator
                                                    *creator,
                                                 gpointer data);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __GTK_EXT_BUTTON_H__ */
