/*
 * File: image.c
 *
 * Copyright (C) 1997 Raph Levien <raph@acm.org>
 * Copyright (C) 1999 James McCollough <jamesm@gtwn.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * This file implements image data transfer methods. It handles the transfer
 * of data from an Image to a DwImage widget.
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

#include "msg.h"
#include "image.h"

/*
 * Local data
 */
static size_t linebuf_size = 0;
static guchar *linebuf = NULL;


/*
 * Create and initialize a new image structure.
 */
DilloImage *a_Image_new(gint width,
                        gint height,
                        const char *alt_text,
                        gint32 bg_color)
{
   DilloImage *Image;

   Image = g_new(DilloImage, 1);
   Image->dw = (DwImage *) a_Dw_image_new(DW_IMAGE_RGB, alt_text);
   Image->width = 0;
   Image->height = 0;
   Image->cmap = NULL;
   Image->in_type = DILLO_IMG_TYPE_NOTSET;
   Image->bg_color = bg_color;
   Image->ProcessedBytes = 0;
   Image->BitVec = NULL;
   Image->State = IMG_Empty;

   Image->RefCount = 1;

   return Image;
}

/*
 * Deallocate an Image structure
 */
static void Image_free(DilloImage *Image)
{
   a_Bitvec_free(Image->BitVec);
   g_free(Image);
}

/*
 * Unref and free if necessary
 */
void a_Image_unref(DilloImage *Image)
{
   _MSG(" %d ", Image->RefCount);
   if (Image && --Image->RefCount == 0)
      Image_free(Image);
}

/*
 * Add a reference to an Image struct
 */
void a_Image_ref(DilloImage *Image)
{
   if (Image)
      ++Image->RefCount;
}

/*
 * Decode 'buf' (an image line) into RGB format.
 */
static guchar *
 Image_line(DilloImage *Image, const guchar *buf, const guchar *cmap, gint y)
{
   guint x;

   switch (Image->in_type) {
   case DILLO_IMG_TYPE_INDEXED:
      if (cmap) {
         for (x = 0; x < Image->width; x++)
            memcpy(linebuf + x * 3, cmap + buf[x] * 3, 3);
      } else {
         MSG("Gif:: WARNING, image lacks a color map\n");
      }
      break;
   case DILLO_IMG_TYPE_GRAY:
      for (x = 0; x < Image->width; x++)
         memset(linebuf + x * 3, buf[x], 3);
      break;
   case DILLO_IMG_TYPE_RGB:
      /* avoid a memcpy here!  --Jcid */
      return (guchar *)buf;
   case DILLO_IMG_TYPE_NOTSET:
      g_warning("ERROR: Image type not set...\n");
      break;
   }
   return linebuf;
}

/*
 * Set initial parameters of the image
 */
void a_Image_set_parms(DilloImage *Image, guchar *EntryBuf, DilloUrl *url,
                       gint version, guint width, guint height,
                       DilloImgType type)
{
   if ( !Image->dw->buffer )
      a_Dw_image_set_buffer(Image->dw, EntryBuf, url, version);
   if ( !Image->BitVec )
      Image->BitVec = a_Bitvec_new(height);
   Image->in_type = type;
   Image->width = width;
   Image->height = height;
   if (3 * width > linebuf_size) {
      linebuf_size = 3 * width;
      linebuf = g_realloc(linebuf, linebuf_size);
   }
   Image->State = IMG_SetParms;

   a_Dw_image_size(Image->dw, width, height);
}

/*
 * Reference the dicache entry color map
 */
void a_Image_set_cmap(DilloImage *Image, const guchar *cmap)
{
   Image->cmap = cmap;
   Image->State = IMG_SetCmap;
}

/*
 * Implement the write method
 */
void a_Image_write(DilloImage *Image, const guchar *buf, guint y, gint decode)
{
   guchar *newbuf;

   g_return_if_fail ( y < Image->height );

   if (decode) {
      /* Decode 'buf' and copy it into the DicEntry buffer */
      newbuf = Image_line(Image, buf, Image->cmap, y);
      memcpy(Image->dw->buffer + y*Image->width*3, newbuf, Image->width*3);
   }
   a_Bitvec_set_bit(Image->BitVec, y);
   Image->State = IMG_Write;

   /* Update the row in DwImage */
   a_Dw_image_draw_row(Image->dw, Image->width, Image->height, 0, y);
}

/*
 * Implement the close method
 */
void a_Image_close(DilloImage *Image)
{
   a_Image_unref(Image);
}
