#include <gtk/gtk.h>
#include "dw_embed_gtk.h"
#include "dw_gtk_viewport.h"
#include "debug.h"


static void Dw_embed_gtk_init           (DwEmbedGtk *embed_gtk);
static void Dw_embed_gtk_class_init     (DwEmbedGtkClass *klass);

static void Dw_embed_gtk_destroy        (GtkObject *object);

static void Dw_embed_gtk_size_request   (DwWidget *widget,
                                         DwRequisition *requisition);
static gint Dw_embed_gtk_move_idle      (void *data);
static void Dw_embed_gtk_size_allocate  (DwWidget *widget,
                                         DwAllocation *allocation);
static void Dw_embed_gtk_draw           (DwWidget *widget,
                                         DwRectangle *area,
                                         GdkEventExpose *event);

static void Dw_embed_gtk_remove_gtk     (DwEmbedGtk *embed_gtk);

static GtkWidgetClass *parent_class;

/*
 * Standard Gtk+ function
 */
GtkType a_Dw_embed_gtk_get_type (void)
{
   static GtkType type = 0;

   if (!type) {
      GtkTypeInfo info = {
         "DwEmbedGtk",
         sizeof (DwEmbedGtk),
         sizeof (DwEmbedGtkClass),
         (GtkClassInitFunc) Dw_embed_gtk_class_init,
         (GtkObjectInitFunc) Dw_embed_gtk_init,
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
DwWidget* a_Dw_embed_gtk_new (void)
{
   GtkObject *object;

   object = gtk_object_new (DW_TYPE_EMBED_GTK, NULL);
   DBG_OBJ_CREATE (object, "DwEmbedGtk");
   return DW_WIDGET (object);
}


/*
 * Standard Gtk+ function
 */
static void Dw_embed_gtk_init (DwEmbedGtk *embed_gtk)
{
   embed_gtk->child = NULL;
   embed_gtk->idle_id = 0;
   /*embed_gtk->child_size_request_lock = FALSE;*/
}


/*
 * Standard Gtk+ function
 */
static void Dw_embed_gtk_class_init (DwEmbedGtkClass *klass)
{
   GtkObjectClass *object_class;
   DwWidgetClass *widget_class;

   parent_class = gtk_type_class (DW_TYPE_WIDGET);

   object_class = GTK_OBJECT_CLASS (klass);
   object_class->destroy = Dw_embed_gtk_destroy;

   widget_class = (DwWidgetClass*) klass;
   widget_class->size_allocate = Dw_embed_gtk_size_allocate;
   widget_class->size_request = Dw_embed_gtk_size_request;
   widget_class->draw = Dw_embed_gtk_draw;
}


/*
 * Standard Gtk+ function
 */
static void Dw_embed_gtk_destroy (GtkObject *object)
{
   DwEmbedGtk *embed_gtk;

   embed_gtk = DW_EMBED_GTK (object);
   if (embed_gtk->child)
      gtk_object_destroy (GTK_OBJECT (embed_gtk->child));
   if (embed_gtk->idle_id != 0)
      gtk_idle_remove (embed_gtk->idle_id);

   GTK_OBJECT_CLASS(parent_class)->destroy (object);
}



/*
 * Standard Gtk+ function
 */
static void Dw_embed_gtk_size_request   (DwWidget *widget,
                                         DwRequisition *requisition)
{
   DwEmbedGtk *embed_gtk;
   GtkRequisition child_requisition;

   embed_gtk = DW_EMBED_GTK (widget);
   if (embed_gtk->child) {
      /*embed_gtk->child_size_request_lock = TRUE;*/
      gtk_widget_size_request (embed_gtk->child, &child_requisition);
      /*embed_gtk->child_size_request_lock = FALSE;*/

      requisition->width = child_requisition.width;
      requisition->ascent = child_requisition.height;
      requisition->descent = 0;
   } else {
      requisition->width = 0;
      requisition->ascent = 0;
      requisition->descent = 0;
   }
}


/*
 * Standard Gtk+ function
 */
static gint Dw_embed_gtk_move_idle (void *data)
{
   DwWidget *widget;
   DwEmbedGtk *embed_gtk;

   widget = DW_WIDGET (data);
   embed_gtk = DW_EMBED_GTK (data);

   if (embed_gtk->child) {
      if (embed_gtk->child->parent)
         gtk_layout_move (GTK_LAYOUT (widget->viewport), embed_gtk->child,
                          widget->allocation.x, widget->allocation.y);
      else
         gtk_layout_put (GTK_LAYOUT (widget->viewport), embed_gtk->child,
                         widget->allocation.x, widget->allocation.y);
   }

   embed_gtk->idle_id = 0;
   return FALSE;
}


/*
 * Standard Gtk+ function
 */
static void Dw_embed_gtk_size_allocate (DwWidget *widget,
                                        DwAllocation *allocation)
{
   DwEmbedGtk *embed_gtk;

   embed_gtk = DW_EMBED_GTK (widget);

   if (embed_gtk->child && widget->viewport) {
      if (allocation->width == 0 ||
          allocation->ascent + allocation->descent == 0)
         gtk_widget_hide (embed_gtk->child);
      else {
         gtk_widget_show (embed_gtk->child);

         if (allocation->width != widget->allocation.width ||
             allocation->ascent + allocation->descent
             != DW_WIDGET_HEIGHT(widget))
            /* todo: try implementing it by gtk_widget_size_allocate */
            gtk_widget_set_usize (embed_gtk->child,
                                  allocation->width,
                                  allocation->ascent + allocation->descent);

         if (allocation->x != widget->allocation.x ||
             allocation->y != widget->allocation.y) {
            /* A simple call to gtk_layout_move does not seem to work, so this
               strange idle function. */
            if (embed_gtk->idle_id == 0)
               embed_gtk->idle_id =
                  gtk_idle_add (Dw_embed_gtk_move_idle, (void*) embed_gtk);
         }
      }
   }
}


static void Dw_embed_gtk_draw (DwWidget *widget,
                               DwRectangle *area,
                               GdkEventExpose *event)
{
   /* This is the job of GtkDwViewport (resp. the base class GtkLayout). */
}


/*
 * This function is called when the embedded Gtk+ widget is destroyed.
 * Don't use this function directly!
 */
static void Dw_embed_gtk_remove_gtk (DwEmbedGtk *embed_gtk)
{
   embed_gtk->child = NULL;
   p_Dw_widget_queue_resize (DW_WIDGET (embed_gtk), 0, TRUE);
}


/*
 * This function is called when the embedded Gtk+ widget is focused and
 * will set the adjustments of the viewport.
 * Don't use this function directly!
 */
static int Dw_embed_gtk_child_focus_in (DwEmbedGtk *embed_gtk)
{
   DwWidget *widget;
   GtkLayout *layout;
   gint vx, vy, vw, vh, wx, wy, ww, wh;

   widget = DW_WIDGET (embed_gtk);
   layout = GTK_LAYOUT (widget->viewport);

   vx = layout->xoffset;
   vy = layout->yoffset;
   vw = GTK_WIDGET(layout)->allocation.width;
   vh = GTK_WIDGET(layout)->allocation.height;

   wx = widget->allocation.x;
   wy = widget->allocation.y;
   ww = widget->allocation.width;
   wh = DW_WIDGET_HEIGHT(widget);

   if (vx > wx)
      gtk_adjustment_set_value (layout->hadjustment, wx);
   else if (vx < wx + ww - vw)
      gtk_adjustment_set_value (layout->hadjustment, wx + ww - vw);

   if (vy > wy)
      gtk_adjustment_set_value (layout->vadjustment, wy);
   else if (vy < wy + wh - vh)
      gtk_adjustment_set_value (layout->vadjustment, wy + wh - vh);

   return FALSE;
}

#if 0
/*
 * Called when the embedded Gtk widget emits a "size_request" signal, and used
 * to resise the embedding Dw widget.
 *
 * Currently not used.
 */
static void Dw_embed_gtk_child_size_request (DwEmbedGtk *embed_gtk)
{
   DwWidget *widget;
   GtkRequisition child_requisition;

   g_return_if_fail (embed_gtk->child);

   if (!embed_gtk->child_size_request_lock) {
      widget = DW_WIDGET (embed_gtk);
      embed_gtk->child_size_request_lock = TRUE;
      gtk_widget_size_request (embed_gtk->child, &child_requisition);
      embed_gtk->child_size_request_lock = FALSE;

      /* Check wether the size has changed at all. */
      if (widget->requisition.width != child_requisition.width ||
          widget->requisition.ascent != child_requisition.height)
         p_Dw_widget_queue_resize (widget, 0, TRUE);
   }
}
#endif

/*
 * Add the Gtk+ widget to be embedded.
 * If there is already one, you have to destroy it before.
 */
void a_Dw_embed_gtk_add_gtk (DwEmbedGtk *embed_gtk,
                             GtkWidget *widget)
{
   /* todo: problems with reparent's? */

   g_return_if_fail (embed_gtk->child == NULL);

   embed_gtk->child = widget;

   if (DW_WIDGET(embed_gtk)->viewport)
      gtk_layout_put (GTK_LAYOUT (DW_WIDGET (embed_gtk)->viewport),
                      widget,
                      DW_WIDGET (embed_gtk)->allocation.x,
                      DW_WIDGET (embed_gtk)->allocation.y);

   /* todo: We somehow must regognize size changes of the embedded Gtk+
    * widget. Can this be done with size_request? Anyway, currently
    * deactivated, since it is not used in dillo. */
#if 0
   gtk_signal_connect_object (GTK_OBJECT (embed_gtk->child),
                              "size_request",
                              GTK_SIGNAL_FUNC(Dw_embed_gtk_child_size_request),
                              GTK_OBJECT (embed_gtk));
#endif

   /* for setting the adjustments */
   gtk_signal_connect_object (GTK_OBJECT (embed_gtk->child),
                              "focus_in_event",
                              GTK_SIGNAL_FUNC (Dw_embed_gtk_child_focus_in),
                              GTK_OBJECT (embed_gtk));

   /* todo: An idea: use "remove" signal of DwGtkContainer instead. */
   gtk_signal_connect_object (GTK_OBJECT (embed_gtk->child),
                              "destroy",
                              GTK_SIGNAL_FUNC (Dw_embed_gtk_remove_gtk),
                              GTK_OBJECT (embed_gtk));

   p_Dw_widget_queue_resize (DW_WIDGET (embed_gtk), 0, TRUE);
}
