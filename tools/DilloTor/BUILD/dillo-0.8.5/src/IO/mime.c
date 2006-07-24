/*
 * File: mime.c
 *
 * Copyright (C) 2000 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "mime.h"
#include "../msg.h"
#include "../list.h"


typedef struct {
   const char *Name;   /* MIME type name */
   Viewer_t Data;      /* Pointer to a function */
} MimeItem_t;


/*
 *  Local data
 */
static gint MimeMinItemsSize = 0, MimeMinItemsMax = 8;
static MimeItem_t *MimeMinItems = NULL;

static gint MimeMajItemsSize = 0, MimeMajItemsMax = 8;
static MimeItem_t *MimeMajItems = NULL;


/*
 * Add a specific MIME type (as "image/png") to our viewer list
 * 'Key' is the content-type string that identifies the MIME type
 * 'Method' is the function that handles it
 */
static gint Mime_add_minor_type(const char *Key, Viewer_t Method)
{
   a_List_add(MimeMinItems, MimeMinItemsSize, MimeMinItemsMax);
   MimeMinItems[MimeMinItemsSize].Name = Key;
   MimeMinItems[MimeMinItemsSize].Data = Method;
   MimeMinItemsSize++;
   return 0;
}

/*
 * Add a major MIME type (as "text") to our viewer list
 * 'Key' is the content-type string that identifies the MIME type
 * 'Method' is the function that handles it
 */
static gint Mime_add_major_type(const char *Key, Viewer_t Method)
{
   a_List_add(MimeMajItems, MimeMajItemsSize, MimeMajItemsMax);
   MimeMajItems[MimeMajItemsSize].Name = Key;
   MimeMajItems[MimeMajItemsSize].Data = Method;
   MimeMajItemsSize++;
   return 0;
}

/*
 * Search the list of specific MIME viewers, for a Method that matches 'Key'
 * 'Key' is the content-type string that identifies the MIME type
 */
static Viewer_t Mime_minor_type_fetch(const char *Key, guint Size)
{
   gint i;

   if (Size) {
      for ( i = 0; i < MimeMinItemsSize; ++i )
         if ( g_strncasecmp(Key, MimeMinItems[i].Name, Size) == 0 )
            return MimeMinItems[i].Data;
   }
   return NULL;
}

/*
 * Search the list of major MIME viewers, for a Method that matches 'Key'
 * 'Key' is the content-type string that identifies the MIME type
 */
static Viewer_t Mime_major_type_fetch(const char *Key, guint Size)
{
   gint i;

   if (Size) {
      for ( i = 0; i < MimeMajItemsSize; ++i )
         if ( g_strncasecmp(Key, MimeMajItems[i].Name, Size) == 0 )
            return MimeMajItems[i].Data;
   }
   return NULL;
}


/*
 * Initializes Mime module and, sets the supported Mime types.
 */
void a_Mime_init()
{
#ifdef ENABLE_GIF
   Mime_add_minor_type("image/gif", a_Gif_image);
#endif
#ifdef ENABLE_JPEG
   Mime_add_minor_type("image/jpeg", a_Jpeg_image);
   Mime_add_minor_type("image/pjpeg", a_Jpeg_image);
   Mime_add_minor_type("image/jpg", a_Jpeg_image);
#endif
#ifdef ENABLE_PNG
   Mime_add_minor_type("image/png", a_Png_image);
   Mime_add_minor_type("image/x-png", a_Png_image);    /* deprecated */
#endif
   Mime_add_minor_type("text/html", a_Html_text);

   /* Add a major type to handle all the text stuff */
   Mime_add_major_type("text", a_Plain_text);
}


/*
 * Call the handler for the MIME type to set Call and Data as appropriate
 *
 * Return Value:
 *   On success: a new Dw (and Call and Data properly set).
 *   On failure: NULL (and Call and Data untouched).
 */
DwWidget *a_Mime_set_viewer(const char *content_type, void *Ptr,
                            CA_Callback_t *Call, void **Data)
{

   Viewer_t viewer;
   guint MinSize, MajSize, i;
   const char *str = content_type;

   MajSize = 0;
   for (i = 0; str[i] && str[i] != ' ' && str[i] != ';'; ++i) {
      if (str[i] == '/' && !MajSize)
         MajSize = i;
   }
   MinSize = i;

   /* Try minor type */
   viewer = Mime_minor_type_fetch(content_type, MinSize);
   if (viewer)
      return viewer(content_type, Ptr, Call, Data);

   /* Try major type */
   viewer = Mime_major_type_fetch(content_type, MajSize);
   if (viewer)
      return viewer(content_type, Ptr, Call, Data);

   /* Type not handled */
   MSG_HTTP("\n  Unhandled MIME type: <%s>\n", content_type);
   return NULL;
}
