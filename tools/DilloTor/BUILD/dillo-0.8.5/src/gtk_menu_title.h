#ifndef __GTK_MENU_TITLE_H__
#define __GTK_MENU_TITLE_H__

#include <gtk/gtkmenuitem.h>

#ifdef __cplusplus
extern "C" {

#endif /* __cplusplus */

#define GTK_MENU_TITLE(obj)          (GTK_CHECK_CAST ((obj), \
                                        a_Gtk_menu_title_get_type(), \
                                        GtkMenuTitle))
#define GTK_MENU_TITLE_CLASS(klass)  (GTK_CHECK_CLASS_CAST ((klass), \
                                        a_Gtk_menu_title_get_type(), \
                                        GtkMenuTitleClass))
#define GTK_IS_MENU_TITLE           (GTK_CHECK_TYPE ((obj), \
                                        a_Gtk_menu_title_get_type()))

typedef struct _GtkMenuTitle      GtkMenuTitle;
typedef struct _GtkMenuTitleClass GtkMenuTitleClass;

struct _GtkMenuTitle
{
   GtkMenuItem gtk_menu_item;
   char *label;
};

struct _GtkMenuTitleClass
{
   GtkMenuItemClass parent_class;
};

GtkType    a_Gtk_menu_title_get_type  (void);
GtkWidget* a_Gtk_menu_title_new (const char *label);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTK_MENU_TITLE_H__ */
