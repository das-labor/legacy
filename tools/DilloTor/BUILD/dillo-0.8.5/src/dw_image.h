#ifndef __DW_IMAGE_H__
#define __DW_IMAGE_H__

#include <stdio.h>
#include "dw_widget.h"
#include "url.h"           /* for DilloUrl */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DW_TYPE_IMAGE           (a_Dw_image_get_type ())
#define DW_IMAGE(obj)           GTK_CHECK_CAST (obj,DW_TYPE_IMAGE, DwImage)
#define DW_IMAGE_CLASS(klass)   GTK_CHECK_CLASS_CAST (klass, DW_TYPE_IMAGE, \
                                   DwImageClass)
#define DW_IS_IMAGE(obj)        GTK_CHECK_TYPE (obj, DW_TYPE_IMAGE)

typedef enum {
   DW_IMAGE_RGB
} DwImageType;

typedef struct _DwImage       DwImage;
typedef struct _DwImageClass  DwImageClass;

typedef struct _DwImageMap      DwImageMap;
typedef struct _DwImageMapList  DwImageMapList;
typedef struct _DwImageMapShape DwImageMapShape;


struct _DwImage
{
   DwWidget widget;

   DilloUrl *url;
   gint version;
   DwImageType type;
   guchar *buffer;
   gint width;
   gint height;
   gint alt_text_width;

   /* non NULL if image is scaled */
   guchar *scaled_buffer;

   /* ALT text (for selection) */
   gchar *alt_text;

   DwImageMapList *map_list;
   DilloUrl *usemap_url;
   gboolean ismap;

   gint hover_link;
   gint pressed_link;
   gboolean selected[DW_HIGHLIGHT_NUM_LAYERS];
};

struct _DwImageClass
{
   DwWidgetClass parent_class;

   gboolean (*link_entered)  (DwImage *page,
                              gint link, gint x, gint y);
   gboolean (*link_pressed)  (DwImage *page,
                              gint link, gint x, gint y,
                              GdkEventButton *event);
   gboolean (*link_released) (DwImage *page,
                              gint link, gint x, gint y,
                              GdkEventButton *event);
   gboolean (*link_clicked)  (DwImage *page,
                              gint link, gint x, gint y,
                              GdkEventButton *event);
};


/*
 * Image Maps
 */

#define DW_IMAGE_MAP_SHAPE_RECT    0
#define DW_IMAGE_MAP_SHAPE_CIRCLE  1
#define DW_IMAGE_MAP_SHAPE_POLY    2

struct _DwImageMapList
{
   DwImageMap *maps;
   gint num_maps;
   gint num_maps_max;

   DwImageMapShape *shapes;
   gint num_shapes;
   gint num_shapes_max;
};

struct _DwImageMap
{
   DilloUrl *url;
   gint start_shape;
};

struct _DwImageMapShape
{
   gint type;
   gint link;

   union {
      GdkRegion *poly;
      struct  {
         gint32 x;
         gint32 y;
         gint32 r2;
      } circle;
      struct  {
         gint32 top;
         gint32 bottom;
         gint32 left;
         gint32 right;
      } rect;
   } data;
};


/*
 * Function prototypes
 */
GtkType a_Dw_image_get_type (void);
DwWidget* a_Dw_image_new  (DwImageType type, const gchar *alt_text);
void a_Dw_image_size (DwImage *image, gint width, gint height);
void a_Dw_image_draw_row(DwImage *image,
                         gint Width, gint Height, gint x, gint y);
void a_Dw_image_set_buffer(DwImage *image, guchar *ImageBuffer,
                           DilloUrl *url, gint version);

void a_Dw_image_set_ismap (DwImage *image);
void a_Dw_image_set_usemap (DwImage *image,  DwImageMapList *map_list,
                            DilloUrl *usemap_url);

/* Image maps */
void a_Dw_image_map_list_init      (DwImageMapList *list);
void a_Dw_image_map_list_free      (DwImageMapList *list);

void a_Dw_image_map_list_add_map    (DwImageMapList *list,
                                     DilloUrl *url);
void a_Dw_image_map_list_add_shape  (DwImageMapList *list,
                                     gint type,
                                     gint link,
                                     GdkPoint *points,
                                     gint num_points);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DW_IMAGE_H__ */
