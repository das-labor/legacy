#ifndef __GTK_EXT_MENU_H__
#define __GTK_EXT_MENU_H__

#include <gtk/gtkmenu.h>

#ifdef __cplusplus
extern "C" {

#endif /* __cplusplus */

#define GTK_EXT_MENU(obj)          (GTK_CHECK_CAST ((obj), \
                                      a_Gtk_ext_menu_get_type (), GtkExtMenu))
#define GTK_EXT_MENU_CLASS(klass)  (GTK_CHECK_CLASS_CAST ((klass), \
                                      a_Gtk_ext_menu_get_type (), \
                                      GtkExtMenuClass))
#define GTK_IS_EXT_MENU(obj)       (GTK_CHECK_TYPE ((obj), \
                                      a_Gtk_ext_menu_get_type ()))

typedef struct _GtkExtMenu      GtkExtMenu;
typedef struct _GtkExtMenuClass GtkExtMenuClass;

struct _GtkExtMenu
{
   GtkMenu menu;
};

struct _GtkExtMenuClass
{
   GtkMenuClass parent_class;
};

GtkType    a_Gtk_ext_menu_get_type  (void);
GtkWidget* a_Gtk_ext_menu_new       (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTK_EXT_MENU_H__ */
