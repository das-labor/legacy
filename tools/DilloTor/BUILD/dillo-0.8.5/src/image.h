#ifndef __IMAGE_H__
#define __IMAGE_H__

/* The DilloImage data-structure and methods */

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "dw_image.h"
#include "bitvec.h"


typedef struct _DilloImage DilloImage;

typedef enum {
   DILLO_IMG_TYPE_INDEXED,
   DILLO_IMG_TYPE_RGB,
   DILLO_IMG_TYPE_GRAY,
   DILLO_IMG_TYPE_NOTSET    /* Initial value */
} DilloImgType;

/* These will reflect the Image's "state" */
typedef enum {
   IMG_Empty,      /* Just created the entry */
   IMG_SetParms,   /* Parameters set */
   IMG_SetCmap,    /* Color map set */
   IMG_Write,      /* Feeding the entry */
   IMG_Close,      /* Whole image got! */
   IMG_Abort       /* Image transfer aborted */
} ImageState;

struct _DilloImage {
   DwImage *dw;

   /* Parameters as told by image data */
   guint width;
   guint height;

   const guchar *cmap;     /* Color map (only for indexed) */
   DilloImgType in_type;   /* Image Type */
   gint32 bg_color;        /* Background color */

   gint ProcessedBytes;    /* Amount of bytes already decoded */
   bitvec_t *BitVec;       /* Bit vector for decoded rows */
   ImageState State;

   gint RefCount;          /* Reference counter */
};


/*
 * Function prototypes
 */
DilloImage *a_Image_new(gint width, gint height,
                        const char *alt_text, gint32 bg_color);
void a_Image_ref(DilloImage *Image);
void a_Image_unref(DilloImage *Image);

void a_Image_set_parms(DilloImage *Image, guchar *EntryBuf, DilloUrl *url,
                       gint version, guint width, guint height,
                       DilloImgType type);
void a_Image_set_cmap(DilloImage *Image, const guchar *cmap);
void a_Image_write(DilloImage *Image, const guchar *buf, guint y, gint decode);
void a_Image_close(DilloImage *Image);

#endif /* __IMAGE_H__ */

