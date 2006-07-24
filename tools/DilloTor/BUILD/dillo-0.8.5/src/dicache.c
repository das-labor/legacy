/*
 * File: dicache.c
 *
 * Copyright 2000 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <glib.h>           /* GHashTable functions */
#include <gtk/gtk.h>
#include <sys/time.h>       /* for libc5 compatibility */
#include <string.h>         /* for memset */
#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "web.h"
#include "dicache.h"
#include "cache.h"
#include "interface.h"
#include "prefs.h"

typedef struct _DICacheHashEntry DICacheHashEntry;

struct _DICacheHashEntry {
   gint valid;
   DilloUrl *url;       /* "Key" of this hash entry */
   DICacheEntry* next;  /* pointer to the first dicache entry in this list */
};

static GHashTable *dicache_hash;

static gint dicache_size_total; /* invariant: dicache_size_total is
                                 * the sum of the image sizes of all
                                 * cache lines in dicache. */

/*
 * Determine if two hash entries are equal (used by GHashTable)
 */
static gint Dicache_hash_entry_equal(gconstpointer v1, gconstpointer v2)
{
   return (!a_Url_cmp((DilloUrl *)v1, (DilloUrl *)v2));
}

/*
 * Determine the hash value given the key (used by GHashTable)
 */
static guint Dicache_url_hash(gconstpointer key)
{
   return g_str_hash(URL_STR(((DilloUrl *)key)));
}

/*
 * Initialize dicache data
 */
void a_Dicache_init(void)
{
   dicache_hash = g_hash_table_new(Dicache_url_hash, Dicache_hash_entry_equal);
   dicache_size_total = 0;
}

/*
 * Create, and initialize a new, empty, dicache entry
 */
static DICacheEntry *Dicache_entry_new(void)
{
   DICacheEntry *entry = g_new(DICacheEntry, 1);

   entry->width = 0;
   entry->height = 0;
   entry->type = DILLO_IMG_TYPE_NOTSET;
   entry->cmap = NULL;
   entry->ImageBuffer = NULL;
   entry->RefCount = 1;
   entry->TotalSize = 0;
   entry->Y = 0;
   entry->BitVec = NULL;
   entry->State = DIC_Empty;
   entry->version = 0;
   entry->next = NULL;

   return entry;
}

/*
 * Add a new entry to the hash
 */
DICacheEntry *a_Dicache_add_entry(const DilloUrl *Url)
{
   DICacheEntry *entry;
   DICacheHashEntry *hash_entry;

   entry = Dicache_entry_new();

   if ( (hash_entry = g_hash_table_lookup(dicache_hash, Url))) {
      /* this URL is already in the hash, add entry at the END of the list */
      DICacheEntry *ptr = hash_entry->next;

      hash_entry->valid = 1;
      for (  ; ptr->next; ptr = ptr->next);
      ptr->next = entry;
      entry->version = ptr->version+1;
      entry->url = hash_entry->url;

   } else { /* no hash entry yet, so create one */
      DICacheHashEntry *hash_entry = g_new(DICacheHashEntry, 1);

      hash_entry->url = a_Url_dup(Url);
      entry->url = hash_entry->url;
      hash_entry->next = entry;
      hash_entry->valid = 1;
      g_hash_table_insert(dicache_hash, hash_entry->url, hash_entry);
   }

   return entry;
}

/*
 * Search an entry in the dicache (given the Url).
 * Return value: a pointer to the entry of the _newest_ (i.e. highest)
 *               version if found; NULL otherwise.
 */
DICacheEntry *a_Dicache_get_entry(const DilloUrl *Url)
{
   DICacheHashEntry *hash_entry = g_hash_table_lookup(dicache_hash, Url);
   DICacheEntry *entry;

   if (!hash_entry || !hash_entry->valid)
      return NULL;

   for (entry = hash_entry->next; (entry && entry->next); entry = entry->next);

   return entry;
}

/*
 * Search a particular version of an URL in the Dicache.
 * Return value: a pointer to the entry if found; NULL otherwise.
 */
static DICacheEntry *Dicache_get_entry_version(const DilloUrl *Url,
                                               gint version)
{
   DICacheHashEntry *hash_entry = g_hash_table_lookup(dicache_hash, Url);
   DICacheEntry *entry = hash_entry ? hash_entry->next : NULL;

   while (entry && entry->version != version)
      entry = entry->next;

   return entry;
}

/*
 * Actually free a dicache entry, given the URL and the version number.
 */
static void Dicache_remove(const DilloUrl *Url, gint version)
{
   DICacheHashEntry *hash_entry = g_hash_table_lookup(dicache_hash, Url);
   DICacheEntry *entry = hash_entry ? hash_entry->next : NULL, *prev = entry;

   while (entry && (entry->version != version) ) {
      prev = entry;
      entry = entry->next;
   }

   if ( entry ) {
      /* Eliminate this dicache entry */
      g_free(entry->cmap);
      if (entry->ImageBuffer) {
         free(entry->ImageBuffer);
         dicache_size_total -= entry->TotalSize;
      }
      a_Bitvec_free(entry->BitVec);

      if (hash_entry->next == entry) {
         if (!entry->next) {
            /* last entry with this URL. Remove the hash entry as well */
            g_hash_table_remove(dicache_hash, hash_entry->url);
            a_Url_free(hash_entry->url);
            g_free(hash_entry);
         } else
            hash_entry->next = entry->next;
      } else
         prev->next = entry->next;
      g_free(entry);
   }
}

/*
 * Unrefs the counter of a dicache entry, and _if_ no DwImage is acessing
 * this buffer _and_ there is a higher version of this image, then we call
 * Dicache_free to do the dirty job.
 */
void a_Dicache_unref(const DilloUrl *Url, gint version)
{
   DICacheEntry *entry;

   if ( (entry = Dicache_get_entry_version(Url, version)) )
      if (--entry->RefCount == 0 && (entry->next || !prefs.use_dicache) )
         Dicache_remove(Url, version);
}

/*
 * Refs the counter of a dicache entry.
 */

DICacheEntry* a_Dicache_ref(const DilloUrl *Url, gint version)
{
   DICacheEntry *entry;

   if ( (entry = Dicache_get_entry_version(Url, version)) )
      ++entry->RefCount;

   return entry;
}

/*
 * Invalidate this entry. This is used for the reloading mechanism.
 * Can't erase current versions, but a_Dicache_get_entry must return NULL.
 */
void a_Dicache_invalidate_entry(const DilloUrl *Url)
{
   DICacheHashEntry *hash_entry = g_hash_table_lookup(dicache_hash, Url);

   if (hash_entry) hash_entry->valid = 0;
}


/* ------------------------------------------------------------------------- */

/*
 * This function is a cache client; (but feeds its clients from dicache)
 */
void a_Dicache_callback(int Op, CacheClient_t *Client)
{
   /* todo: Handle Op = CA_Abort (to show what was got)  --Jcid */
   guint i;
   DilloWeb *Web = Client->Web;
   DilloImage *Image = Web->Image;
   DICacheEntry *DicEntry = a_Dicache_get_entry(Web->url);

   g_return_if_fail ( DicEntry != NULL );

   if ( Op == CA_Send ) {
      if ( Image->height == 0 && DicEntry->State >= DIC_SetParms ) {
         /* Set parms */
         a_Image_set_parms(
            Image, DicEntry->ImageBuffer, DicEntry->url,
            DicEntry->version, DicEntry->width, DicEntry->height,
            DicEntry->type);
      }
      if (DicEntry->State == DIC_Write) {
         for (i = 0; i < DicEntry->height; ++i)
            if (a_Bitvec_get_bit(DicEntry->BitVec, (gint)i) &&
                !a_Bitvec_get_bit(Image->BitVec, (gint)i) )
               a_Image_write(
                  Image, DicEntry->ImageBuffer + i*DicEntry->width*3, i,FALSE);

      }
   } else if ( Op == CA_Close || Op == CA_Abort ) {
      a_Image_close(Web->Image);
      a_Interface_close_client(Web->bw, Client->Key);
   }
}

/* ------------------------------------------------------------------------- */

/*
 * Set image's width, height & type
 * (By now, we'll use the image information despite the html tags --Jcid)
 */
void a_Dicache_set_parms(DilloUrl *url, gint version, DilloImage *Image,
                         guint width, guint height, DilloImgType type)
{
   DICacheEntry *DicEntry;
   size_t Size = width * height * 3;

   g_return_if_fail ( Image != NULL && width && height );
   /* Find the DicEntry for this Image */
   DicEntry = Dicache_get_entry_version(url, version);
   g_return_if_fail ( DicEntry != NULL );

   /* Initialize the DicEntry */
   /* using malloc() instead of g_new() avoids crashes on oversized images */
   DicEntry->ImageBuffer = malloc(sizeof(guchar) * Size);
   g_return_if_fail ( DicEntry->ImageBuffer != NULL );
   DicEntry->TotalSize = Size;
   DicEntry->width = width;
   DicEntry->height = height;
   DicEntry->type = type;
   DicEntry->BitVec = a_Bitvec_new((gint)height);
   DicEntry->State = DIC_SetParms;

   dicache_size_total += Size;

   /* For giggles, make the background of the undrawn parts interesting */
   memset(DicEntry->ImageBuffer, 0xdd, DicEntry->TotalSize);

   /* Allocate and initialize this image */
   a_Image_set_parms(Image, DicEntry->ImageBuffer, url, version,
                     width, height, type);
}

/*
 * Implement the set_cmap method for the Image
 */
void a_Dicache_set_cmap(DilloUrl *url, gint version, DilloImage *Image,
                        const guchar *cmap, guint num_colors,
                        gint num_colors_max, gint bg_index)
{
   DICacheEntry *DicEntry = Dicache_get_entry_version(url, version);

   g_return_if_fail ( DicEntry != NULL );

   g_free(DicEntry->cmap);
   DicEntry->cmap = g_new0(guchar, 3 * num_colors_max);
   memcpy(DicEntry->cmap, cmap, 3 * num_colors);
   if (bg_index >= 0 && (guint)bg_index < num_colors) {
      DicEntry->cmap[bg_index * 3]     = (Image->bg_color >> 16) & 0xff;
      DicEntry->cmap[bg_index * 3 + 1] = (Image->bg_color >> 8) & 0xff;
      DicEntry->cmap[bg_index * 3 + 2] = (Image->bg_color) & 0xff;
   }

   a_Image_set_cmap(Image, DicEntry->cmap);
   DicEntry->State = DIC_SetCmap;
}

/*
 * Implement the write method
 * (Write a scan line into the Dicache entry)
 * buf: row buffer
 * Y  : row number
 * x  : horizontal offset? (always zero)
 */
void a_Dicache_write(DilloImage *Image, DilloUrl *url, gint version,
                     const guchar *buf, gint x, guint Y)
{
   DICacheEntry *DicEntry;

   g_return_if_fail ( Image != NULL );
   DicEntry = Dicache_get_entry_version(url, version);
   g_return_if_fail ( DicEntry != NULL );
   g_return_if_fail ( DicEntry->width > 0 && DicEntry->height > 0 );

   a_Image_write(Image, buf, Y, TRUE);
   DicEntry->Y = Y;
   a_Bitvec_set_bit(DicEntry->BitVec, (gint)Y);
   DicEntry->State = DIC_Write;
}

/*
 * Implement the close method of the decoding process
 */
void a_Dicache_close(DilloUrl *url, gint version, CacheClient_t *Client)
{
   DilloWeb *Web = Client->Web;
   DICacheEntry *DicEntry = Dicache_get_entry_version(url, version);

   g_return_if_fail ( DicEntry != NULL );

   DicEntry->State = DIC_Close;
   if (DicEntry->cmap) {
      g_free(DicEntry->cmap);
      DicEntry->cmap = NULL;
   }
   a_Image_close(Web->Image);
   a_Interface_close_client(Web->bw, Client->Key);
}

/* ------------------------------------------------------------------------- */

static gboolean
 Dicache_remove_hash_entry(gpointer key, gpointer value, gpointer user_data)
{
   DICacheHashEntry *hash_entry = (DICacheHashEntry *)value;
   DICacheEntry *entry = hash_entry->next;

   /* Eliminate this (last) dicache entry */
   g_free(entry->cmap);
   if (entry->ImageBuffer) {
      free(entry->ImageBuffer);
      dicache_size_total -= entry->TotalSize;
   }
   a_Bitvec_free(entry->BitVec);
   a_Url_free(hash_entry->url);
   g_free(hash_entry);

   return TRUE;
}

/*
 * Deallocate memory used by dicache module
 * (Call this one at exit time)
 */
void a_Dicache_freeall(void)
{
   /* Remove every dicache entry */
   g_hash_table_foreach_remove(dicache_hash,
                               (GHRFunc)Dicache_remove_hash_entry, NULL);
   /* Remove the dicache hash */
   g_hash_table_destroy(dicache_hash);
}
