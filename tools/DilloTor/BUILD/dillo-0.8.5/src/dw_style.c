/*
 * File: dw_style.c
 *
 * Copyright (C) 2001-2003 Sebastian Geerken <S.Geerken@ping.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "dw_style.h"
#include "dw_widget.h"

#include <gdk/gdk.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*#define DEBUG_LEVEL 1*/
#include "debug.h"

#define EQUIV(a, b) (((a) && (b)) || (!(a) && !(b)))

#define Dw_style_font_ref(font)   ((font)->ref_count++)
#define Dw_style_font_unref(font) if (--((font)->ref_count) == 0) \
                                     Dw_style_font_remove (font)

#define Dw_style_color_ref(color)   ((color)->ref_count++)
#define Dw_style_color_unref(color) if (--((color)->ref_count) == 0) \
                                       Dw_style_color_remove (color)

#define Dw_style_shaded_color_ref(color)   ((color)->ref_count++)
#define Dw_style_shaded_color_unref(color) if (--((color)->ref_count) == 0) \
                                              Dw_style_shaded_color_remove \
                                                 (color) \

static GHashTable *fonts_table;
static GHashTable *colors_table;
static GHashTable *shaded_colors_table;

static gint styles_num = 0;

/* Used by a_Dw_style_numtostr(). */
static const char
   *roman_I0[] ={"I","II","III","IV","V","VI","VII","VIII","IX"},
   *roman_I1[] ={"X","XX","XXX","XL","L","LX","LXX","LXXX","XC"},
   *roman_I2[] ={"C","CC","CCC","CD","D","DC","DCC","DCCC","CM"},
   *roman_I3[] ={"M","MM","MMM","MMMM"};


static gint Dw_style_font_equal (gconstpointer v1, gconstpointer v2);
static guint Dw_style_font_hash (gconstpointer key);
static void Dw_style_font_remove (DwStyleFont *font);

static void Dw_style_color_remove (DwStyleColor *color);
static void Dw_style_shaded_color_remove (DwStyleShadedColor *color);


/* ----------------------------------------------------------------------
 *
 *    Initialization / Cleaning up
 *
 * ----------------------------------------------------------------------
 */

/*
 * Initialize the DwStyle submodule.
 */
void a_Dw_style_init (void)
{
   fonts_table = g_hash_table_new (Dw_style_font_hash, Dw_style_font_equal);
   colors_table = g_hash_table_new (g_direct_hash, g_direct_equal);
   shaded_colors_table = g_hash_table_new (g_direct_hash, g_direct_equal);
}


/*
 * Called by a_Dw_style_freeall.
 */
static void Dw_style_count_fonts (gpointer key,
                                  gpointer value,
                                  gpointer user_data)
{
   DwStyleFont *font = (DwStyleFont*) key;
   gint *count = (int*)user_data;

   count[0]++;
   count[1] += font->ref_count;
}

/*
 * Called by a_Dw_style_freeall.
 */
static void Dw_style_count_colors (gpointer key,
                                   gpointer value,
                                   gpointer user_data)
{
   DwStyleColor *color = (DwStyleColor*) value;
   gint *count = (int*)user_data;

   count[0]++;
   count[1] += color->ref_count;
}

/*
 * Called by a_Dw_style_freeall.
 */
static void Dw_style_count_shaded_colors (gpointer key,
                                          gpointer value,
                                          gpointer user_data)
{
   DwStyleShadedColor *color = (DwStyleShadedColor*) value;
   gint *count = (int*)user_data;

   count[0]++;
   count[1] += color->ref_count;
}


/*
 * Free variables used by DwStyle, and do a check whether memory
 * management works properly.
 */
void a_Dw_style_freeall (void)
{
   gint count[2];

   if (styles_num)
      g_warning ("%d styles left", styles_num);

   count[0] = count[1] = 0;
   g_hash_table_foreach (fonts_table, Dw_style_count_fonts, count);
   if (count[0] || count[1])
      g_warning ("%d fonts (%d references) left", count[0], count[1]);

   count[0] = count[1] = 0;
   g_hash_table_foreach (colors_table, Dw_style_count_colors, count);
   if (count[0] || count[1])
      g_warning ("%d colors (%d references) left", count[0], count[1]);

   count[0] = count[1] = 0;
   g_hash_table_foreach (shaded_colors_table, Dw_style_count_shaded_colors,
                         count);
   if (count[0] || count[1])
      g_warning ("%d shaded colors (%d references) left", count[0], count[1]);

   g_hash_table_destroy (fonts_table);
   g_hash_table_destroy (colors_table);
}


/* ----------------------------------------------------------------------
 *
 *    Styles
 *
 * ----------------------------------------------------------------------
 */


/*
 * Set all style fields except font and color to reasonable defaults.
 */
void a_Dw_style_init_values (DwStyle *style_attrs,
                             GdkWindow *window)
{
   style_attrs->x_link = -1;
   style_attrs->x_tooltip = NULL;
   style_attrs->text_decoration = 0;
   style_attrs->text_align = DW_STYLE_TEXT_ALIGN_LEFT;
   style_attrs->list_style_type = DW_STYLE_LIST_STYLE_TYPE_DISC;
   style_attrs->valign = DW_STYLE_VALIGN_MIDDLE;
   style_attrs->background_color = NULL;
   style_attrs->width = DW_STYLE_LENGTH_AUTO;
   style_attrs->height = DW_STYLE_LENGTH_AUTO;

   a_Dw_style_box_set_val (&style_attrs->margin, 0);
   a_Dw_style_box_set_val (&style_attrs->border_width, 0);
   a_Dw_style_box_set_val (&style_attrs->padding, 0);
   a_Dw_style_box_set_border_color (style_attrs, NULL);
   a_Dw_style_box_set_border_style (style_attrs, DW_STYLE_BORDER_NONE);
   style_attrs->border_spacing = 0;

   style_attrs->display = DW_STYLE_DISPLAY_INLINE;
   style_attrs->white_space = DW_STYLE_WHITE_SPACE_NORMAL;
}


/*
 * Reset those style attributes to their standard values, which are
 * not inherited, according to CSS.
 */
void a_Dw_style_reset_values (DwStyle *style_attrs,
                              GdkWindow *window)
{
   style_attrs->x_link = -1;
   style_attrs->background_color = NULL;
   style_attrs->x_tooltip = NULL;

   style_attrs->text_align = DW_STYLE_TEXT_ALIGN_LEFT; /* ??? */
   style_attrs->valign = DW_STYLE_VALIGN_MIDDLE;
   style_attrs->text_align_char = '.';
   style_attrs->background_color = NULL;
   style_attrs->width = DW_STYLE_LENGTH_AUTO;
   style_attrs->height = DW_STYLE_LENGTH_AUTO;

   a_Dw_style_box_set_val (&style_attrs->margin, 0);
   a_Dw_style_box_set_val (&style_attrs->border_width, 0);
   a_Dw_style_box_set_val (&style_attrs->padding, 0);
   a_Dw_style_box_set_border_color (style_attrs, NULL);
   a_Dw_style_box_set_border_style (style_attrs, DW_STYLE_BORDER_NONE);
   style_attrs->border_spacing = 0;

   style_attrs->display = DW_STYLE_DISPLAY_INLINE;
   style_attrs->white_space = DW_STYLE_WHITE_SPACE_NORMAL;
}

/*
 * Return a new DwStyle, with increased reference pointer.
 */
DwStyle* a_Dw_style_new (DwStyle *style_attrs,
                         GdkWindow *window)
{
   DwStyle *style;

   style = g_new (DwStyle, 1);
   *style = *style_attrs;
   style->ref_count = 1;
   Dw_style_font_ref (style->font);
   Dw_style_color_ref (style->color);
   if (style->background_color)
      Dw_style_color_ref (style->background_color);
   if (style->border_color.top)
      Dw_style_shaded_color_ref (style->border_color.top);
   if (style->border_color.right)
      Dw_style_shaded_color_ref (style->border_color.right);
   if (style->border_color.bottom)
      Dw_style_shaded_color_ref (style->border_color.bottom);
   if (style->border_color.left)
      Dw_style_shaded_color_ref (style->border_color.left);
   if (style->x_tooltip)
      a_Dw_tooltip_ref (style->x_tooltip);

   styles_num++;
   return style;
}

/*
 * Remove a style (called when ref_count == 0).
 */
void Dw_style_remove (DwStyle *style)
{
   Dw_style_font_unref (style->font);
   Dw_style_color_unref (style->color);
   if (style->background_color)
      Dw_style_color_unref (style->background_color);
   if (style->border_color.top)
      Dw_style_shaded_color_unref (style->border_color.top);
   if (style->border_color.right)
      Dw_style_shaded_color_unref (style->border_color.right);
   if (style->border_color.bottom)
      Dw_style_shaded_color_unref (style->border_color.bottom);
   if (style->border_color.left)
      Dw_style_shaded_color_unref (style->border_color.left);
   if (style->x_tooltip)
      a_Dw_tooltip_unref (style->x_tooltip);

   g_free (style);
   styles_num--;
}



/* ----------------------------------------------------------------------
 *
 *    Fonts
 *
 * ----------------------------------------------------------------------
 */

/*
 * Create the GdkFont. font->name contains one name. If try_all is
 * TRUE, try also standard fonts, if anything else fails.
 */
static void Dw_style_font_realize (DwStyleFont *font, gboolean try_all)
{
   char fontname[256], *style_char_1 = NULL, *style_char_2 = NULL;

   switch (font->style) {
   case DW_STYLE_FONT_STYLE_NORMAL:
      style_char_1 = style_char_2 = "r";
      break;
   case DW_STYLE_FONT_STYLE_ITALIC:
      style_char_1 = "i";
      style_char_2 = "o";
      break;
   case DW_STYLE_FONT_STYLE_OBLIQUE:
      style_char_1 = "o";
      style_char_2 = "i";
      break;
   }

   sprintf (fontname, "-*-%s-%s-%s-*-*-%d-*-75-75-*-*-iso8859-1",
            font->name,
            (font->weight >= 500) ? "bold" : "medium",
            style_char_1, font->size);
   font->font = gdk_font_load (fontname);

   if (font->font == NULL && font->style != DW_STYLE_FONT_STYLE_NORMAL) {
      sprintf (fontname, "-*-%s-%s-%s-*-*-%d-*-75-75-*-*-iso8859-1",
               font->name,
               (font->weight >= 500) ? "bold" : "medium",
               style_char_2, font->size);
      font->font = gdk_font_load (fontname);
   }

   if (try_all) {
      if (font->font == NULL) {
         /* Can't load the font - substitute the default instead. */
         font->font =
            gdk_font_load
            ("-adobe-helvetica-medium-r-normal--*-100-*-*-*-*-iso8859-1");
      }

      if (font->font == NULL) {
         /* Try another platform-font that should be available. (iPaq) */
         font->font =
            gdk_font_load
            ("-misc-fixed-medium-r-normal--13-120-75-75-c-80-iso8859-1");
      }

      if (font->font == NULL) {
         /* Can't load any suitable font!  */
         g_warning ("Can't load any ISO8859-1 font!?! :(");
         font->font =
            gdk_font_load ("-adobe-helvetica-*-*-*--*-*-*-*-*-*-*-*");
      }
   }

   if (font->font) {
      font->space_width = gdk_char_width (font->font, ' ');
      font->x_height = gdk_char_height (font->font, 'x');
   }
}


/*
 * Used for the font_table hash table.
 */
static gint Dw_style_font_equal (gconstpointer v1, gconstpointer v2)
{
   const DwStyleFont *font1 = (DwStyleFont*) v1, *font2 = (DwStyleFont*) v2;

   return (font1->size == font2->size &&
           font1->weight == font2->weight &&
           font1->style == font2->style &&
           strcmp (font1->name, font2->name) == 0);
}


/*
 * Used for the font_table hash table.
 */
static guint Dw_style_font_hash (gconstpointer key)
{
   const DwStyleFont *font = (DwStyleFont*) key;
   guint h;

   h = g_str_hash (font->name);
   h = (h << 5) - h + font->size;
   h = (h << 5) - h + font->weight;
   h = (h << 5) - h + font->style;
   return h;
}


/*
 * Returns a new or already existing font. This function is only used
 * internally, and called by a_Dw_style_font_new and
 * a_Dw_style_font_new_from_list.
 *
 * Note that the reference counter is not increased/set to zero, it
 * will be increased by a_Dw_style_new. If 'try_all' is TRUE, try also
 * standard fonts, if anything else fails.
 */
static DwStyleFont* Dw_style_font_new_internal (DwStyleFont *font_attrs,
                                                gboolean try_all)
{
   DwStyleFont *font;

   g_return_val_if_fail (font_attrs->name != NULL, NULL);

   if ((font = g_hash_table_lookup (fonts_table, font_attrs))) {
      return font;
   } else {
      font = g_new (DwStyleFont, 1);
      font->size = font_attrs->size;
      font->weight = font_attrs->weight;
      font->style = font_attrs->style;
      font->name = g_strdup (font_attrs->name);
      font->ref_count = 0;

      Dw_style_font_realize (font, try_all);
      if (font->font) {
         g_hash_table_insert (fonts_table, font, font);
         return font;
      } else {
         g_free (font->name);
         g_free (font);
         return NULL;
      }
   }
}


/*
 * Return a new or already existing font, with one name in
 * font_attrs->name. See also Dw_style_font_new_internal.
 */
DwStyleFont* a_Dw_style_font_new (DwStyleFont *font_attrs)
{
   DwStyleFont *font;

   font = Dw_style_font_new_internal (font_attrs, TRUE);
   if (font == NULL)
      g_error ("Could not find any font.");

   return font;
}


/*
 * Return a new or already existing font, with font_attrs->name
 * containing a comma separated list of names. See also
 * Dw_style_font_new_internal.
 */
DwStyleFont* a_Dw_style_font_new_from_list (DwStyleFont *font_attrs,
                                            gchar *default_family)
{
   DwStyleFont *font = NULL, font_attrs2;
   char *comma, *list, *current;

   font_attrs2 = *font_attrs;
   current = list = g_strdup (font_attrs->name);

   while (current && (font == NULL)) {
      comma = strchr (current, ',');
      if (comma) *comma = 0;

      font_attrs2.name = current;
      font = Dw_style_font_new_internal (&font_attrs2, FALSE);
      if (font)
         break;

      if (comma) {
         current = comma + 1;
         while (isspace (*current)) current++;
      } else
         current = NULL;
   }

   g_free (list);

   if (font == NULL) {
      font_attrs2.name = default_family;
      font = Dw_style_font_new_internal (&font_attrs2, TRUE);
   }

   if (font == NULL)
      g_error ("Could not find any font.");

   return font;
}


/*
 * Remove a font (called when ref_count == 0).
 */
static void Dw_style_font_remove (DwStyleFont *font)
{
   g_hash_table_remove (fonts_table, font);
   g_free (font->name);
   gdk_font_unref (font->font);
   g_free (font);
}


/* ----------------------------------------------------------------------
 *
 *    Colors
 *
 * ----------------------------------------------------------------------
 */

/*
 * Copied from gtkstyle.c.
 * Convert RGB into HLS.
 */
static void Dw_style_rgb_to_hls (gdouble *r,
                                 gdouble *g,
                                 gdouble *b)
{
   gdouble min;
   gdouble max;
   gdouble red;
   gdouble green;
   gdouble blue;
   gdouble h, l, s;
   gdouble delta;

   red = *r;
   green = *g;
   blue = *b;

   if (red > green) {
      if (red > blue)
         max = red;
      else
         max = blue;

      if (green < blue)
         min = green;
      else
         min = blue;
   } else {
      if (green > blue)
         max = green;
      else
         max = blue;

      if (red < blue)
         min = red;
      else
         min = blue;
   }

   l = (max + min) / 2;
   s = 0;
   h = 0;

   if (max != min) {
      if (l <= 0.5)
         s = (max - min) / (max + min);
      else
         s = (max - min) / (2 - max - min);

      delta = max -min;
      if (red == max)
         h = (green - blue) / delta;
      else if (green == max)
         h = 2 + (blue - red) / delta;
      else if (blue == max)
         h = 4 + (red - green) / delta;

      h *= 60;
      if (h < 0.0)
         h += 360;
   }

   *r = h;
   *g = l;
   *b = s;
}

/*
 * Copied from gtkstyle.c.
 * Convert HLS into RGB.
 */
static void Dw_style_hls_to_rgb (gdouble *h,
                                 gdouble *l,
                                 gdouble *s)
{
   gdouble hue;
   gdouble lightness;
   gdouble saturation;
   gdouble m1, m2;
   gdouble r, g, b;

   lightness = *l;
   saturation = *s;

   if (lightness <= 0.5)
      m2 = lightness * (1 + saturation);
   else
      m2 = lightness + saturation - lightness * saturation;
   m1 = 2 * lightness - m2;

   if (saturation == 0) {
      *h = lightness;
      *l = lightness;
      *s = lightness;
   } else {
      hue = *h + 120;
      while (hue > 360)
         hue -= 360;
      while (hue < 0)
         hue += 360;

      if (hue < 60)
            r = m1 + (m2 - m1) * hue / 60;
      else if (hue < 180)
         r = m2;
      else if (hue < 240)
         r = m1 + (m2 - m1) * (240 - hue) / 60;
      else
         r = m1;

      hue = *h;
      while (hue > 360)
         hue -= 360;
      while (hue < 0)
         hue += 360;

      if (hue < 60)
         g = m1 + (m2 - m1) * hue / 60;
      else if (hue < 180)
         g = m2;
      else if (hue < 240)
         g = m1 + (m2 - m1) * (240 - hue) / 60;
      else
         g = m1;

      hue = *h - 120;
      while (hue > 360)
         hue -= 360;
      while (hue < 0)
         hue += 360;

      if (hue < 60)
         b = m1 + (m2 - m1) * hue / 60;
      else if (hue < 180)
         b = m2;
      else if (hue < 240)
         b = m1 + (m2 - m1) * (240 - hue) / 60;
      else
         b = m1;

      *h = r;
      *l = g;
      *s = b;
   }
}


/*
 * Create the necessary recources (GdkColor, GdkGC).
 * k is a factor the color is multiplied with before, this is needed
 * for shaded colors.
 */
static void Dw_style_color_create (gint color_val,
                                   GdkWindow *window,
                                   GdkColor *color,
                                   GdkGC **gc,
                                   gint d)
{
   gint red, green, blue;
   gdouble hue, lightness, saturation;

   red = (color_val >> 16) & 255;
   green = (color_val >> 8) & 255;
   blue = color_val & 255;

   if (d) {
      hue = (gdouble)red / 255;
      lightness = (gdouble)green / 255;
      saturation = (gdouble)blue / 255;

      DEBUG_MSG (1, "Shaded by %1.3g: (%1.3g, %1.3g, %1.3g) -> ",
                 k, hue, lightness, saturation);
      Dw_style_rgb_to_hls (&hue, &lightness, &saturation);

      if (lightness > 0.8) {
         if (d > 0)
            lightness -= 0.2;
         else
            lightness -= 0.4;
      } else if (lightness < 0.2) {
         if (d > 0)
            lightness += 0.4;
         else
            lightness += 0.2;
      } else
         lightness += d * 0.2;

      Dw_style_hls_to_rgb (&hue, &lightness, &saturation);
      DEBUG_MSG (1, "(%1.3g, %1.3g, %1.3g)\n", hue, lightness, saturation);

      red = hue * 255;
      green = lightness * 255;
      blue = saturation * 255;
   }

   blue |= blue << 8;
   red |= red << 8;
   green |= green << 8;

   color->red = red;
   color->green = green;
   color->blue = blue;
   gdk_color_alloc (gdk_window_get_colormap (window), color);

   *gc = gdk_gc_new (window);
   gdk_gc_set_foreground (*gc, color);
}

/*
   Return a new or already existing color. color_val has the form
   0xrrggbb.
 */
DwStyleColor* a_Dw_style_color_new (gint color_val,
                                    GdkWindow *window)
{
   DwStyleColor *color;

   color = g_hash_table_lookup (colors_table, GINT_TO_POINTER (color_val));
   if (color == NULL) {
      color = g_new (DwStyleColor, 1);
      color->ref_count = 0;
      color->color_val = color_val;

      Dw_style_color_create (color_val, window,
                             &color->color, &color->gc, 0);
      Dw_style_color_create (color_val ^ 0xffffff, window,
                             &color->inverse_color, &color->inverse_gc, 0);
      g_hash_table_insert (colors_table, GINT_TO_POINTER (color_val), color);
      g_hash_table_insert (colors_table, GINT_TO_POINTER (color_val), color);
   }

   return color;
}

/*
 * Remove a color (called when ref_count == 0).
 */
static void Dw_style_color_remove (DwStyleColor *color)
{
   g_hash_table_remove (colors_table, GINT_TO_POINTER (color->color_val));
   gdk_gc_destroy (color->gc);
   gdk_gc_destroy (color->inverse_gc);
   g_free (color);
}


/*
 * Return a new or already existing shaded color. color_val has the
 * form 0xrrggbb.
 */
DwStyleShadedColor* a_Dw_style_shaded_color_new (gint color_val,
                                                 GdkWindow *window)
{
   DwStyleShadedColor *color;

   color =
      g_hash_table_lookup (shaded_colors_table, GINT_TO_POINTER (color_val));
   if (color == NULL) {
      color = g_new (DwStyleShadedColor, 1);
      color->ref_count = 0;
      color->color_val = color_val;

      Dw_style_color_create (color_val, window,
                             &color->color, &color->gc, 0);
      Dw_style_color_create (color_val ^ 0xffffff, window,
                             &color->inverse_color, &color->inverse_gc, 0);
      Dw_style_color_create (color_val, window,
                             &color->color_dark, &color->gc_dark, -1);
      Dw_style_color_create (color_val, window,
                             &color->color_light, &color->gc_light, +1);
      g_hash_table_insert (shaded_colors_table,
                           GINT_TO_POINTER (color_val), color);
   }

   return color;
}

/*
 * Remove a shaded color (called when ref_count == 0).
 */
static void Dw_style_shaded_color_remove (DwStyleShadedColor *color)
{
   g_hash_table_remove (shaded_colors_table,
                        GINT_TO_POINTER (color->color_val));
   gdk_gc_destroy (color->gc);
   gdk_gc_destroy (color->gc_dark);
   gdk_gc_destroy (color->gc_light);
   g_free (color);
}

/*
 * This function returns whether something may change its size, when
 * its style changes from style1 to style2. It is mainly for
 * optimizing style changes where only colors etc change (where FALSE
 * would be returned), in some cases it may return TRUE, although a
 * size change does not actually happen (e.g. when in a certain
 * context a particular attribute is ignored).
 */
gboolean a_Dw_style_size_diffs (DwStyle *style1,
                                DwStyle *style2)
{
   /* todo: Should for CSS implemented properly. Currently, size
    * changes are not needed, so always FALSE is returned. See also
    * a_Dw_widget_set_style(). */
   return FALSE;
}


/* ----------------------------------------------------------------------
 *
 *    Drawing functions
 *
 * ----------------------------------------------------------------------
 */

/*
 * Draw a part of a border.
 */
static void Dw_style_draw_polygon (GdkDrawable *drawable,
                                   GdkGC *gc,
                                   gint32 x1, gint32 y1, gint32 x2, gint32 y2,
                                   gint32 width, gint32 w1, gint32 w2)
{
   GdkPoint points[4];

   if (width != 0) {
      if (width == 1) {
         if (x1 == x2)
            gdk_draw_line (drawable, gc, x1, y1, x2, y2 - 1);
         else
            gdk_draw_line (drawable, gc, x1, y1, x2 - 1, y2);
      } else if (width == -1) {
         if (x1 == x2)
            gdk_draw_line (drawable, gc, x1 - 1, y1, x2 - 1, y2 - 1);
         else
            gdk_draw_line (drawable, gc, x1, y1 - 1, x2 - 1, y2 - 1);
      } else {
         points[0].x = x1;
         points[0].y = y1;
         points[1].x = x2;
         points[1].y = y2;

         if (x1 == x2) {
            points[2].x = x1 + width;
            points[2].y = y2 + w2;
            points[3].x = x1 + width;
            points[3].y = y1 + w1;
         } else {
            points[2].x = x2 + w2;
            points[2].y = y1 + width;
            points[3].x = x1 + w1;
            points[3].y = y1 + width;
         }

         gdk_draw_polygon (drawable, gc, TRUE, points, 4);
      }
   }
}


/*
 * Draw the border of a region in window, according to style. Used by
 * Dw_widget_draw_box and Dw_widget_draw_widget_box.
 */

#define LIMIT(v)    if ((v) < -30000) v = -30000; \
                    if ((v) > 30000) v = 30000

void p_Dw_style_draw_border (GdkWindow *window,
                             GdkRectangle *area,
                             gint32 vx,
                             gint32 vy,
                             gint32 x,
                             gint32 y,
                             gint32 width,
                             gint32 height,
                             DwStyle *style,
                             gboolean inverse)
{
   /* todo: a lot! */
   GdkGC *dark_gc, *light_gc, *normal_gc;
   GdkGC *top_gc, *right_gc, *bottom_gc, *left_gc;
   gint32 xb1, yb1, xb2, yb2, xp1, yp1, xp2, yp2;

   if (style->border_style.top == DW_STYLE_BORDER_NONE)
      return;

   xb1 = x + style->margin.left - vx;
   yb1 = y + style->margin.top - vy;
   xb2 = xb1 + width - style->margin.left - style->margin.right;
   yb2 = yb1 + height - style->margin.top - style->margin.bottom;

   xp1 = xb1 + style->border_width.top;
   yp1 = yb1 + style->border_width.left;
   xp2 = xb2 + style->border_width.bottom;
   yp2 = yb2 + style->border_width.right;

   /* Make sure that we pass 16-bit values to Gdk functions. */
   LIMIT (xb1);
   LIMIT (xb2);
   LIMIT (yb1);
   LIMIT (yb2);

   light_gc = inverse ? style->border_color.top->gc_dark :
      style->border_color.top->gc_light;
   dark_gc = inverse ? style->border_color.top->gc_light :
      style->border_color.top->gc_dark;
   normal_gc = inverse ? style->border_color.top->inverse_gc :
      style->border_color.top->gc;

   switch (style->border_style.top) {
   case DW_STYLE_BORDER_INSET:
      top_gc = left_gc = dark_gc;
      right_gc = bottom_gc = light_gc;
      break;

   case DW_STYLE_BORDER_OUTSET:
      top_gc = left_gc = light_gc;
      right_gc = bottom_gc = dark_gc;
      break;

   default:
      top_gc = right_gc = bottom_gc = left_gc = normal_gc;
      break;
   }

   Dw_style_draw_polygon (window, top_gc, xb1, yb1, xb2, yb1,
                          style->border_width.top,
                          style->border_width.left,
                          - style->border_width.right);
   Dw_style_draw_polygon (window, right_gc, xb2, yb1, xb2, yb2,
                          - style->border_width.right,
                          style->border_width.top,
                          - style->border_width.bottom);
   Dw_style_draw_polygon (window, bottom_gc, xb1, yb2, xb2, yb2,
                          - style->border_width.bottom,
                          style->border_width.left,
                          - style->border_width.right);
   Dw_style_draw_polygon (window, left_gc, xb1, yb1, xb1, yb2,
                          style->border_width.left,
                          style->border_width.top,
                          - style->border_width.bottom);
}


/*
 * Draw the background (content plus padding) of a region in window,
 * according to style. Used by Dw_widget_draw_box and
 * Dw_widget_draw_widget_box.
 */
void p_Dw_style_draw_background (GdkWindow *window,
                                 GdkRectangle *area,
                                 gint32 vx,
                                 gint32 vy,
                                 gint32 x,
                                 gint32 y,
                                 gint32 width,
                                 gint32 height,
                                 DwStyle *style,
                                 gboolean inverse)
{
   DwRectangle dw_area, bg_area, intersection;

   if (style->background_color) {
      dw_area.x = area->x + vx;
      dw_area.y = area->y + vy;
      dw_area.width = area->width;
      dw_area.height = area->height;

      bg_area.x = x + style->margin.left + style->border_width.left;
      bg_area.y = y + style->margin.top + style->border_width.top;
      bg_area.width =
         width - style->margin.left - style->border_width.left -
         style->margin.right - style->border_width.right;
      bg_area.height =
         height - style->margin.top - style->border_width.top -
         style->margin.bottom - style->border_width.bottom;

      if (p_Dw_rectangle_intersect (&dw_area, &bg_area, &intersection))
         gdk_draw_rectangle (window,
                             inverse ? style->background_color->inverse_gc :
                             style->background_color->gc,
                             TRUE, intersection.x - vx, intersection.y - vy,
                             intersection.width, intersection.height);
   }
}


/*
 * Convert a number into a string, in a given style. Used for ordered lists.
 */
void a_Dw_style_numtostr (gint num,
                          gchar *buf,
                          gint buflen,
                          DwStyleListStyleType list_style_tyle)
{
   int i3, i2, i1, i0;
   gboolean low = FALSE;

   switch(list_style_tyle){
   case DW_STYLE_LIST_STYLE_TYPE_LOWER_ALPHA:
      low = TRUE;
   case DW_STYLE_LIST_STYLE_TYPE_UPPER_ALPHA:
      i0 = num - 1;
      i1 = i0/26; i2 = i1/26;
      i0 %= 26;   i1 %= 26;
      if (i2 > 26) /* more than 26*26*26=17576 elements ? */
         sprintf(buf, "****.");
      else if (i2)
         sprintf(buf, "%c%c%c.", 'A'+i2-1,'A'+i1-1,'A'+i0);
      else if (i1)
         sprintf(buf, "%c%c.", 'A'+i1-1,'A'+i0);
      else
         sprintf(buf, "%c.", 'A'+i0);
      if (low)
         g_strdown(buf);
      break;
   case DW_STYLE_LIST_STYLE_TYPE_LOWER_ROMAN:
      low = TRUE;
   case DW_STYLE_LIST_STYLE_TYPE_UPPER_ROMAN:
      i0 = num - 1;
      i1 = i0/10; i2 = i1/10; i3 = i2/10;
      i0 %= 10;   i1 %= 10;   i2 %= 10;
      if (i3 > 4) /* more than 4999 elements ? */
         sprintf(buf, "****.");
      else if (i3)
         sprintf(buf, "%s%s%s%s.", roman_I3[i3-1], roman_I2[i2-1],
                 roman_I1[i1-1], roman_I0[i0]);
      else if (i2)
         sprintf(buf, "%s%s%s.", roman_I2[i2-1],
                 roman_I1[i1-1], roman_I0[i0]);
      else if (i1)
         sprintf(buf, "%s%s.", roman_I1[i1-1], roman_I0[i0]);
      else
         sprintf(buf, "%s.", roman_I0[i0]);
      if (low)
         g_strdown(buf);
      break;
   case DW_STYLE_LIST_STYLE_TYPE_DECIMAL:
   default:
      g_snprintf(buf, buflen, "%d.", num);
      break;
   }
}
