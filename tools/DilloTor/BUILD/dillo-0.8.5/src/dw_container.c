#include <gtk/gtk.h>
#include "dw_container.h"

static void Dw_container_init          (DwContainer *container);
static void Dw_container_class_init    (DwContainerClass *klass);

static void Dw_container_destroy       (GtkObject *object);

enum
{
   ADD,
   REMOVE,
   LAST_SIGNAL
};

// static guint container_signals[LAST_SIGNAL] = { 0 };

static DwWidgetClass *parent_class;


/*
 * Standard Gtk+ function
 */
GtkType a_Dw_container_get_type (void)
{
   static GtkType type = 0;

   if (!type) {
      GtkTypeInfo info = {
         "DwContainer",
         sizeof (DwContainer),
         sizeof (DwContainerClass),
         (GtkClassInitFunc) Dw_container_class_init,
         (GtkObjectInitFunc) Dw_container_init,
         (GtkArgSetFunc) NULL,
         (GtkArgGetFunc) NULL,
         (GtkClassInitFunc) NULL
      };

      type = gtk_type_unique (DW_TYPE_WIDGET, &info);
   }

   return type;
}


/*
 * Standard Gtk+ function
 */
static void Dw_container_init (DwContainer *container)
{
}


/*
 * Standard Gtk+ function
 */
static void Dw_container_class_init (DwContainerClass *klass)
{
   GtkObjectClass *object_class;

   parent_class = gtk_type_class (DW_TYPE_WIDGET);
   object_class = GTK_OBJECT_CLASS (klass);

#if 0
   container_signals[ADD] =
      gtk_signal_new ("add",
                      GTK_RUN_FIRST,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (DwContainerClass, add),
                      gtk_marshal_NONE__POINTER,
                      GTK_TYPE_NONE, 1,
                      GTK_TYPE_WIDGET);
  container_signals[REMOVE] =
     gtk_signal_new ("remove",
                     GTK_RUN_FIRST,
                     object_class->type,
                     GTK_SIGNAL_OFFSET (DwContainerClass, remove),
                     gtk_marshal_NONE__POINTER,
                     GTK_TYPE_NONE, 1,
                     GTK_TYPE_WIDGET);
#endif

   object_class->destroy = Dw_container_destroy;
}


/*
 * Standard Gtk+ function
 */
static void Dw_container_destroy (GtkObject *object)
{
   a_Dw_container_forall (DW_CONTAINER (object),
                          (DwCallback) gtk_object_destroy,
                          NULL);

   GTK_OBJECT_CLASS(parent_class)->destroy (object);
}



/*
 * Add a widget in a "standard" way. Currently not used in dillo,
 * perhaps never needed.
 */
void a_Dw_container_add (DwContainer *container,
                         DwWidget *child)
{
   DwContainerClass *klass;

   klass = DW_CONTAINER_CLASS (GTK_OBJECT (container)->klass);
   if (klass->add)
      (* (klass->add)) (container, child);
}


/*
 *
 */
void a_Dw_container_forall (DwContainer *container,
                            DwCallback   callback,
                            gpointer     callback_data)
{
   DwContainerClass *klass;

   klass = DW_CONTAINER_CLASS (GTK_OBJECT (container)->klass);
   if (klass->forall)
      (* (klass->forall)) (container, callback, callback_data);

   /* The following code is handy for testing DwIterator: */

   /*
   DwIterator *it;

   if ((it = a_Dw_widget_iterator (DW_WIDGET (container),
                                   DW_CONTENT_WIDGET))) {
      while (a_Dw_iterator_next(it))
         callback (it->content.data.widget, callback_data);
      a_Dw_iterator_free (it);
   }
   */
}

void Dw_container_remove (DwContainer *container,
                          DwWidget *child)
{
   DwContainerClass *klass;

   klass = DW_CONTAINER_CLASS (GTK_OBJECT (container)->klass);
   if (klass->remove)
      (* (klass->remove)) (container, child);
}


