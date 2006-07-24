#ifndef __GTK_EXT_MENU_ITEM_H__
#define __GTK_EXT_MENU_ITEM_H__

#include <gtk/gtkmenuitem.h>

#ifdef __cplusplus
extern "C" {

#endif /* __cplusplus */

#define GTK_EXT_MENU_ITEM(obj)          (GTK_CHECK_CAST ((obj), \
                                           a_Gtk_ext_menu_item_get_type (), \
                                           GtkExtMenuItem))
#define GTK_EXT_MENU_ITEM_CLASS(klass)  (GTK_CHECK_CLASS_CAST ((klass), \
                                           a_Gtk_ext_menu_item_get_type (), \
                                           GtkExtMenuItemClass))
#define GTK_IS_EXT_MENU_ITEM(obj)       (GTK_CHECK_TYPE ((obj), \
                                           a_Gtk_ext_menu_item_get_type ()))

typedef struct _GtkExtMenuItem      GtkExtMenuItem;
typedef struct _GtkExtMenuItemClass GtkExtMenuItemClass;

struct _GtkExtMenuItem
{
   GtkMenuItem menu_item;
};

struct _GtkExtMenuItemClass
{
   GtkMenuItemClass parent_class;
   
   void (*activate1) (GtkExtMenuItem *menu_item);
   void (*activate2) (GtkExtMenuItem *menu_item);
   void (*activate3) (GtkExtMenuItem *menu_item);
};

GtkType    a_Gtk_ext_menu_item_get_type               (void);
GtkWidget* a_Gtk_ext_menu_item_new                    (void);
GtkWidget* a_Gtk_ext_menu_item_new_with_label         (const gchar *label);

void       p_Gtk_ext_menu_item_prepare_button_release (GtkExtMenuItem *item,
                                                       GdkEventButton *event);
void       p_Gtk_ext_menu_item_finish_button_release  (GtkExtMenuItem *item,
                                                       GdkEventButton *event);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTK_EXT_MENU_ITEM_H__ */
