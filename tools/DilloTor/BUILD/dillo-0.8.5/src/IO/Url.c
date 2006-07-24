/*
 * File: Url.c
 *
 * Copyright (C) 1997 Raph Levien <raph@acm.org>
 * Copyright (C) 1999 Randall Maas <randym@acm.org>,
 * Copyright (C) 1999, 2000 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/* Heavily modified by Jcid, Dec 1999 & Jun 2000
 *
 * This module selects the apropriate CCC-function for a given URL.
 */


#include <glib.h>
#include <stdlib.h>
#include <ctype.h>
#include "../list.h"
#include "../misc.h"
#include "Url.h"

typedef struct {
   const char *Name;   /* Method name */
   UrlOpener_t Data;   /* Pointer to a function */
} UrlMethod_t;

typedef struct {
   const char *Name;             /* Method name */
   ChainFunction_t cccFunction;  /* Pointer to a CCC function */
} UrlMethod2_t;


/*
 * Local data
 */
/*
static gint UrlOpenersSize = 0, UrlOpenersMax = 8;
static UrlOpener_t *UrlOpeners = NULL;

static gint UrlMethodsSize = 0, UrlMethodsMax = 8;
static UrlMethod_t *UrlMethods = NULL;
*/

/*
 * Add a method for Url handling
 * (currently we have File, About & Http)
 * -- Currently not used
 */
/*
static gint Url_add_method(const char *Name, UrlOpener_t Method)
{
   a_List_add(UrlMethods, UrlMethodsSize, UrlMethodsMax);
   UrlMethods[UrlMethodsSize].Name = Name;
   UrlMethods[UrlMethodsSize].Data = Method;
   UrlMethodsSize++;
   return 0;
}
*/

/*
 * Adds method 'F' to the list of URL openers.
 * UrlOpeners are called in LIFO order until success.
 * -- Currently not used
 */
/*
static gint Url_add_opener(UrlOpener_t F)
{
   a_List_add(UrlOpeners, UrlOpenersSize, UrlOpenersMax);
   UrlOpeners[UrlOpenersSize] = F;
   UrlOpenersSize++;
   return 0;
}
*/

/*
 * Search the cccList for a matching ccc function.
 */
ChainFunction_t a_Url_get_ccc_funct(const DilloUrl *Url)
{
   static UrlMethod2_t cccList[] = { {"http" , a_Http_ccc},
                                     {"file" , a_Dpi_ccc},
                                     {"about", a_About_ccc},
                                     {"dpi" , a_Dpi_ccc},
                                     {"ftp" , a_Dpi_ccc},
                                     {"https" , a_Dpi_ccc} };
   #define LSIZE (sizeof(cccList) / sizeof(cccList[0]))

   const gchar *Key;
   gint KeyLen, j;
   size_t i;

   /* Use the method inside the URL as a Key to decide what cccFunct matches */
   if ( !URL_SCHEME_(Url) )
      return NULL;

   Key = URL_SCHEME_(Url);
   KeyLen = strlen(Key);

   for ( i = 0; i < LSIZE; ++i ) {
      for ( j = 0; j < KeyLen; ++j )
         if ( tolower(Key[j]) != cccList[i].Name[j] )
            break;
      if ( j == KeyLen )
         return cccList[i].cccFunction;
   }
   return NULL;
}

