#ifndef __SELECTION_H__
#define __SELECTION_H__

#include "dw_ext_iterator.h"

#define SELECTION_EOW (1 << 30)

typedef struct
{
   /* selection */
   enum {
      SELECTION_NONE,
      SELECTION_SELECTING,
      SELECTION_SELECTED
   } selection_state;
   DwExtIterator *from, *to;
   gint from_char, to_char;

   /* link handling */
   enum {
      SELECTION_LINK_NONE,
      SELECTION_LINK_PRESSED
   } link_state;
   guint link_button;
   DwExtIterator *link;
   gint link_char, link_number;

   /* "full screen" feature */
   void (*dclick_callback) (gpointer data);
   gpointer callback_data;

   /* widget that owns this selection */
   gpointer owner;

} Selection;

Selection* a_Selection_new                 (void);
void       a_Selection_free                (Selection *selection);
void       a_Selection_reset               (Selection *selection);
void       a_Selection_set_dclick_callback (Selection *selection,
                                            void (*fn) (gpointer data),
                                            gpointer callback_data);
void       a_Selection_set_owner           (Selection *selection,
                                            gpointer owner);
gint       a_Selection_button_press        (Selection *selection,
                                            DwIterator *it,
                                            gint char_pos,
                                            gint link,
                                            GdkEventButton *event,
                                            gboolean within_content);
gint       a_Selection_button_release      (Selection *selection,
                                            DwIterator *it,
                                            gint char_pos,
                                            gint link,
                                            GdkEventButton *event,
                                            gboolean within_content);
gint       a_Selection_button_motion       (Selection *selection,
                                            DwIterator *it,
                                            gint char_pos,
                                            gint link,
                                            GdkEventButton *event,
                                            gboolean within_content);

void       a_Selection_init_selection           (GtkWidget *widget);
void       a_Selection_set_selection            (GtkWidget *widget,
                                                 gchar* str);
void       a_Selection_give_selection_callback  (GtkWidget *widget,
                                                 GtkSelectionData *data,
                                                 guint info,
                                                 guint time);
gint       a_Selection_clear_selection_callback (GtkWidget *,
                                                 GdkEventSelection *);


#endif /* __SELECTION_H__ */
