/*
 * A few notes:
 *
 *    - Currently, a window is created every time before it is shown, and
 *      destroyed, before it is hidden. This saves (probably?) some
 *      memory, but can simply be changed. An alternative is having a
 *      global window for all tooltips.
 *
 *    - Tooltips are positioned near the pointer, as opposed to Gtk+
 *      tooltips, which are positioned near the widget.
 *
 * Sebastian
 */

#include <gtk/gtk.h>
#include "dw_tooltip.h"

/* The amount of space around the text, including the border. */
#define PADDING 4

/* The difference between pointer position and upper left corner of the
 * tooltip. */
#define DIFF 10

static gboolean Dw_tooltip_draw (DwTooltip *tooltip);

static DwTooltip *Dw_tooltip_new0 (const char *text, gint ref_count)
{
   DwTooltip *tooltip;

   tooltip = g_new (DwTooltip, 1);
   tooltip->ref_count = ref_count;
   tooltip->window = NULL;
   tooltip->timeout_id = 0;
   tooltip->text = g_strdup (text);
   return tooltip;
}

/*
 * Create a new tooltip, with ref_count set to 1.
 */
DwTooltip* a_Dw_tooltip_new (const gchar *text)
{
   return Dw_tooltip_new0(text, 1);
}

/*
 * Create a new tooltip, with ref_count set to 0. Tyically used for DwStyle.
 */
DwTooltip* a_Dw_tooltip_new_no_ref (const gchar *text)
{
   return Dw_tooltip_new0(text, 0);
}


/*
 * Destroy the tooltip. Used by Dw_tooltip_unref.
 */
void Dw_tooltip_destroy (DwTooltip *tooltip)
{
   a_Dw_tooltip_on_leave (tooltip);
   g_free (tooltip->text);
   g_free (tooltip);
}


/*
 * Call this function if the pointer has entered the widget/word.
 */
void a_Dw_tooltip_on_enter (DwTooltip *tooltip)
{
   a_Dw_tooltip_on_leave (tooltip);
   tooltip->timeout_id = gtk_timeout_add (500, (GtkFunction)Dw_tooltip_draw,
                                          tooltip);
}


/*
 * Call this function if the pointer has left the widget/word.
 */
void a_Dw_tooltip_on_leave (DwTooltip *tooltip)
{
   if (tooltip->timeout_id != 0) {
      gtk_timeout_remove (tooltip->timeout_id);
      tooltip->timeout_id = 0;
   }

   if (tooltip->window != NULL) {
      gtk_widget_destroy (tooltip->window);
      tooltip->window = NULL;
   }
}


/*
 * Call this function if the pointer has moved within the widget/word.
 */
void a_Dw_tooltip_on_motion (DwTooltip *tooltip)
{
   a_Dw_tooltip_on_enter (tooltip);
}

/*
 *  Draw the tooltip. Called as a timeout function.
 */
static gboolean Dw_tooltip_draw (DwTooltip *tooltip)
{
   GtkStyle *style;
   gint px, py, x, y, width, ascent, descent, screen_w, screen_h, ttw, tth;

   gdk_window_get_pointer (NULL, &px, &py, NULL);
   x = px + DIFF;
   y = py + DIFF;

   tooltip->window = gtk_window_new (GTK_WINDOW_POPUP);
   gtk_widget_set_app_paintable (tooltip->window, TRUE);
   gtk_widget_set_name (tooltip->window, "gtk-tooltips");
   gtk_widget_ensure_style (tooltip->window);
   style = tooltip->window->style;
   width = gdk_string_width (style->font, tooltip->text);
   ascent = style->font->ascent;
   descent = style->font->descent;

   ttw = width + 2 * PADDING;
   tth = ascent + descent + 2 * PADDING;
   gtk_widget_set_usize (tooltip->window, ttw, tth);

   screen_w = gdk_screen_width();
   screen_h = gdk_screen_height();

   if (ttw >= screen_w)
      /* If the width of a tooltips does not fit into the screen, put
       * them at x = 0. (Yes, that's far from perfect ...) */
      x = 0;
   else if (x + ttw > screen_w)
      /* If they would otherwise reach out of the screen, move them
       * a bit left. */
      x = screen_w - ttw;

   /* The case that the height of a tooltip of the screen is greater
    * that the screen height is ignored ;-) If the tooltip reaches
    * out of the screen at the bottom, it is displayed *above* the
    * pointer: to process events properly, it is necessary to keep
    * the pointer out of the tooltip. */
   if (y + tth > screen_h)
      y = py - tth - DIFF;

   gtk_widget_popup (tooltip->window, x, y);
   style = tooltip->window->style;
   gtk_paint_flat_box (style, tooltip->window->window,
                       GTK_STATE_NORMAL, GTK_SHADOW_OUT,
                       NULL, GTK_WIDGET (tooltip->window), "tooltip",
                       0, 0, -1, -1);
   gtk_paint_string (style, tooltip->window->window,
                     GTK_STATE_NORMAL,
                     NULL, GTK_WIDGET (tooltip->window), "tooltip",
                     PADDING, ascent + PADDING,
                     tooltip->text);

   tooltip->timeout_id = 0;
   return FALSE;
}



