/*
 * File: url.c
 *
 * Copyright (C) 2001 Jorge Arellano Cid <jcid@dillo.org>
 *               2001 Livio Baldini Soares <livio@linux.ime.usp.br>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * Parse and normalize all URL's inside Dillo.
 *  - <scheme> <authority> <path> <query> and <fragment> point to 'buffer'.
 *  - 'url_string' is built upon demand (transparent to the caller).
 *  - 'hostname' and 'port' are also being handled on demand.
 */

/*
 * Regular Expression as given in RFC2396 for URL parsing.
 *
 *  ^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?
 *   12            3  4          5       6  7        8 9
 *
 *  scheme    = $2
 *  authority = $4
 *  path      = $5
 *  query     = $7
 *  fragment  = $9
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>

#include "url.h"

//#define DEBUG_LEVEL 2
#include "debug.h"


/*
 * Return the url as a string.
 * (initializing 'url_string' camp if necessary)
 */
gchar *a_Url_str(const DilloUrl *u)
{
   /* Internal url handling IS transparent to the caller */
   DilloUrl *url = (DilloUrl *) u;

   g_return_val_if_fail (url != NULL, NULL);

   if (!url->url_string) {
      url->url_string = g_string_sized_new(60);
      g_string_sprintf(
         url->url_string, "%s%s%s%s%s%s%s%s%s%s",
         url->scheme    ? url->scheme : "",
         url->scheme    ? ":" : "",
         url->authority ? "//" : "",
         url->authority ? url->authority : "",
         (url->path && url->path[0] != '/' && url->authority) ? "/" : "",
         url->path      ? url->path : "",
         url->query     ? "?" : "",
         url->query     ? url->query : "",
         url->fragment  ? "#" : "",
         url->fragment  ? url->fragment : "");
   }

   return url->url_string->str;
}

/*
 * Return the hostname as a string.
 * (initializing 'hostname' and 'port' camps if necessary)
 * Note: a similar approach can be taken for user:password auth.
 */
const gchar *a_Url_hostname(const DilloUrl *u)
{
   gchar *p;
   /* Internal url handling IS transparent to the caller */
   DilloUrl *url = (DilloUrl *) u;

   if (!url->hostname && url->authority) {
      if ((p = strchr(url->authority, ':'))) {
         url->port = strtol(p + 1, NULL, 10);
         url->hostname = g_strndup(url->authority,(guint)(p - url->authority));
      } else
         url->hostname = url->authority;
   }

   return url->hostname;
}

/*
 *  Create a DilloUrl object and initialize it.
 *  (buffer, scheme, authority, path, query and fragment).
 */
static DilloUrl *Url_object_new(const gchar *uri_str)
{
   DilloUrl *url;
   gchar *s, *p;

   g_return_val_if_fail (uri_str != NULL, NULL);

   url = g_new0(DilloUrl, 1);

   /* remove leading & trailing space from buffer */
   for (p = (gchar *)uri_str; isspace(*p); ++p);
   url->buffer = g_strchomp(g_strdup(p));

   s = (gchar *) url->buffer;
   p = strpbrk(s, ":/?#");
   if (p && p[0] == ':' && p > s) {                /* scheme */
      *p = 0;
      url->scheme = s;
      s = ++p;
   }
   /* p = strpbrk(s, "/"); */
   if (p == s && p[0] == '/' && p[1] == '/') {     /* authority */
      s = p + 2;
      p = strpbrk(s, "/?#");
      if (p) {
         memmove(s - 2, s, (size_t)MAX(p - s, 1));
         url->authority = s - 2;
         p[-2] = 0;
         s = p;
      } else if (*s) {
         url->authority = s;
         return url;
      }
   }

   p = strpbrk(s, "?#");
   if (p) {                                        /* path */
      url->path = (p > s) ? s : NULL;
      s = p;
   } else if (*s) {
      url->path = s;
      return url;
   }

   p = strpbrk(s, "?#");
   if (p && p[0] == '?') {                         /* query */
      *p = 0;
      s = p + 1;
      url->query = s;
      p = strpbrk(s, "#");
   }
   if (p && p[0] == '#') {                         /* fragment */
      *p = 0;
      s = p + 1;
      url->fragment = s;
   }

   return url;
}

/*
 *  Free a DilloUrl
 */
void a_Url_free(DilloUrl *url)
{
   if (url) {
      if (url->url_string)
         g_string_free(url->url_string, TRUE);
      if (url->hostname != url->authority)
         g_free((gchar *)url->hostname);
      g_free((gchar *)url->buffer);
      g_free((gchar *)url->data);
      g_free((gchar *)url->alt);
      g_free(url);
   }
}

/*
 * Resolve the URL as RFC2396 suggests.
 */
static GString *Url_resolve_relative(const gchar *RelStr,
                                     DilloUrl *BaseUrlPar,
                                     const gchar *BaseStr)
{
   gchar *p, *s, *e;
   gint i;
   GString *SolvedUrl, *Path;
   DilloUrl *RelUrl, *BaseUrl = NULL;

   /* parse relative URL */
   RelUrl = Url_object_new(RelStr);

   if (BaseUrlPar) {
      BaseUrl = BaseUrlPar;
   } else if (RelUrl->scheme == NULL) {
      /* only required when there's no <scheme> in RelStr */
      BaseUrl = Url_object_new(BaseStr);
   }

   SolvedUrl = g_string_sized_new(64);
   Path = g_string_sized_new(64);

   /* path empty && scheme, authority and query undefined */
   if (!RelUrl->path && !RelUrl->scheme &&
       !RelUrl->authority && !RelUrl->query) {
      g_string_append(SolvedUrl, BaseStr);

      if (RelUrl->fragment) {                    /* fragment */
         if (BaseUrl->fragment)
            g_string_truncate(SolvedUrl, BaseUrl->fragment-BaseUrl->buffer-1);
         g_string_append_c(SolvedUrl, '#');
         g_string_append(SolvedUrl, RelUrl->fragment);
      }
      goto done;

   } else if (RelUrl->scheme) {                  /* scheme */
      g_string_append(SolvedUrl, RelStr);
      goto done;

   } else if (RelUrl->authority) {               /* authority */
      // Set the Path buffer and goto "STEP 7";
      if (RelUrl->path)
         g_string_append(Path, RelUrl->path);

   } else if (RelUrl->path && RelUrl->path[0] == '/') {   /* path */
      g_string_append(Path, RelUrl->path);

   } else {
      // solve relative path
      if (BaseUrl->path) {
         g_string_append(Path, BaseUrl->path);
         for (i = Path->len; --i >= 0 && Path->str[i] != '/'; );
         if (Path->str[i] == '/')
            g_string_truncate(Path, ++i);
      }
      if (RelUrl->path)
         g_string_append(Path, RelUrl->path);

      // erase "./"
      while ((p=strstr(Path->str, "./")) &&
             (p == Path->str || p[-1] == '/'))
         g_string_erase(Path, p - Path->str, 2);
      // erase last "."
      if (Path->len && Path->str[Path->len - 1] == '.' &&
          (Path->len == 1 || Path->str[Path->len - 2] == '/'))
         g_string_truncate(Path, Path->len - 1);

      // erase "<segment>/../" and "<segment>/.."
      s = p = Path->str;
      while ( (p = strstr(p, "/..")) != NULL ) {
         if ((p[3] == '/' || !p[3]) && (p - s)) { //  "/../" | "/.."

            for (e = p + 3 ; p[-1] != '/' && p > s; --p);
            if (p[0] != '.' || p[1] != '.' || p[2] != '/') {
               g_string_erase(Path, p - Path->str, e - p + (*e != 0));
               p -= (p > Path->str);
            } else
               p = e;
         } else
            p += 3;
      }
   }

   /* STEP 7
    */

   /* scheme */
   if (BaseUrl->scheme) {
      g_string_append(SolvedUrl, BaseUrl->scheme);
      g_string_append_c(SolvedUrl, ':');
   }

   /* authority */
   if (RelUrl->authority) {
      g_string_append(SolvedUrl, "//");
      g_string_append(SolvedUrl, RelUrl->authority);
   } else if (BaseUrl->authority) {
      g_string_append(SolvedUrl, "//");
      g_string_append(SolvedUrl, BaseUrl->authority);
   }

   /* path */
   if ((RelUrl->authority || BaseUrl->authority) &&
       ((Path->len == 0 && (RelUrl->query || RelUrl->fragment)) ||
        (Path->len && Path->str[0] != '/')))
      g_string_append_c(SolvedUrl, '/'); /* hack? */
   g_string_append(SolvedUrl, Path->str);

   /* query */
   if (RelUrl->query) {
      g_string_append_c(SolvedUrl, '?');
      g_string_append(SolvedUrl, RelUrl->query);
   }

   /* fragment */
   if (RelUrl->fragment) {
      g_string_append_c(SolvedUrl, '#');
      g_string_append(SolvedUrl, RelUrl->fragment);
   }

done:
   g_string_free(Path, TRUE);
   a_Url_free(RelUrl);
   if (BaseUrl != BaseUrlPar)
      a_Url_free(BaseUrl);
   return SolvedUrl;
}

/*
 *  Transform (and resolve) an URL string into the respective DilloURL.
 *  If URL  =  "http://dillo.sf.net:8080/index.html?long#part2"
 *  then the resulting DilloURL should be:
 *  DilloURL = {
 *     url_string         = "http://dillo.sf.net:8080/index.html?long#part2"
 *     scheme             = "http"
 *     authority          = "dillo.sf.net:8080:
 *     path               = "/index.html"
 *     query              = "long"
 *     fragment           = "part2"
 *     hostname           = "dillo.sf.net"
 *     port               = 8080
 *     flags              = 0
 *     data               = NULL
 *     alt                = NULL
 *     ismap_url_len      = 0
 *     scrolling_position = 0
 *  }
 *
 *  Return NULL if URL is badly formed.
 */
DilloUrl* a_Url_new(const gchar *url_str, const gchar *base_url,
                    gint flags, gint32 posx, gint32 posy)
{
   DilloUrl *url;
   gchar *urlstring, *p, *s;
   GString *SolvedUrl;
   gint n_ic;

   g_return_val_if_fail (url_str != NULL, NULL);

   /* denounce illegal characters (0x00-0x1F, 0x7F and space) */
   urlstring = (gchar *)url_str;
   for (p = s = urlstring; *s; s++)
      if (*s > 0x1F && *s != 0x7F && *s != ' ')
         p++;
   /* number of illegal characters */
   n_ic = s - p;

   /* let's use a heuristic to set http: as default */
   if (!base_url) {
      base_url = "http:";
      if (urlstring[0] != '/') {
         p = strpbrk(urlstring, "/#?:");
         if (!p || *p != ':')
            urlstring = g_strconcat("//", urlstring, NULL);
      } else if (urlstring[1] != '/')
         urlstring = g_strconcat("/", urlstring, NULL);
   }

   /* Resolve the URL */
   SolvedUrl = Url_resolve_relative(urlstring, NULL, base_url);
   DEBUG_MSG(2, "SolvedUrl = %s\n", SolvedUrl->str);
   g_return_val_if_fail (SolvedUrl != NULL, NULL);

   /* Fill url data */
   url = Url_object_new(SolvedUrl->str);
   url->url_string = SolvedUrl;
   url->flags = flags;
   url->scrolling_position_x = posx;
   url->scrolling_position_y = posy;
   url->illegal_chars = n_ic;

   return url;
}


/*
 *  Duplicate a Url structure
 */
DilloUrl* a_Url_dup(const DilloUrl *ori)
{
   DilloUrl *url;

   url = Url_object_new(URL_STR_(ori));
   g_return_val_if_fail (url != NULL, NULL);

   url->url_string           = g_string_new(URL_STR(ori));
   url->port                 = ori->port;
   url->flags                = ori->flags;
   url->data                 = g_strdup(ori->data);
   url->alt                  = g_strdup(ori->alt);
   url->ismap_url_len        = ori->ismap_url_len;
   url->scrolling_position_x = ori->scrolling_position_x;
   url->scrolling_position_y = ori->scrolling_position_y;

   return url;
}

/*
 *  Compare two Url's to check if they are the same.
 *  The fields which are compared here are:
 *  <scheme>, <authority>, <path>, <query> and <data>
 *  Other fields are left for the caller to check
 *
 *  Return value: 0 if equal, 1 otherwise
 */
gint a_Url_cmp(const DilloUrl *A, const DilloUrl *B)
{
   if (!A || !B)
      return 1;

   if (A == B ||
       (URL_STRCAMP_I_EQ(A->authority, B->authority) &&
        URL_STRCAMP_EQ(A->path, B->path) &&
        URL_STRCAMP_EQ(A->query, B->query) &&
        URL_STRCAMP_EQ(A->data, B->data) &&
        URL_STRCAMP_I_EQ(A->scheme, B->scheme)))
      return 0;
   return 1;
}

/*
 * Set DilloUrl flags
 */
void a_Url_set_flags(DilloUrl *u, gint flags)
{
   if (u)
      u->flags = flags;
}

/*
 * Set DilloUrl data (like POST info, etc.)
 */
void a_Url_set_data(DilloUrl *u, gchar *data)
{
   if (u) {
      g_free((gchar *)u->data);
      u->data = g_strdup(data);
   }
}

/*
 * Set DilloUrl alt (alternate text to the URL. Used by image maps)
 */
void a_Url_set_alt(DilloUrl *u, const gchar *alt)
{
   if (u) {
      g_free((gchar *)u->alt);
      u->alt = g_strdup(alt);
   }
}

/*
 * Set DilloUrl scrolling position
 */
void a_Url_set_pos(DilloUrl *u, gint32 posx, gint32 posy)
{
   if (u) {
      u->scrolling_position_x = posx;
      u->scrolling_position_y = posy;
   }
}

/*
 * Set DilloUrl ismap coordinates
 * (this is optimized for not hogging the CPU)
 */
void a_Url_set_ismap_coords(DilloUrl *u, gchar *coord_str)
{
   g_return_if_fail(u && coord_str);

   if ( !u->ismap_url_len ) {
      /* Save base-url length (without coords) */
      u->ismap_url_len  = URL_STR_(u) ? u->url_string->len : 0;
      a_Url_set_flags(u, URL_FLAGS(u) | URL_Ismap);
   }
   if (u->url_string) {
      g_string_truncate(u->url_string, u->ismap_url_len);
      g_string_append(u->url_string, coord_str);
      u->query = u->url_string->str + u->ismap_url_len + 1;
   }
}

/*
 * Given an hex octet (e.g., e3, 2F, 20), return the corresponding
 * character if the octet is valid, and -1 otherwise
 */
static int Url_decode_hex_octet(const gchar *s)
{
   gint hex_value;
   gchar *tail, hex[3];

   if (s && (hex[0] = s[0]) && (hex[1] = s[1])) {
      hex[2] = 0;
      hex_value = strtol(hex, &tail, 16);
      if (tail - hex == 2)
        return hex_value;
   }
   return -1;
}

/*
 * Parse possible hexadecimal octets in the URI path.
 * Returns a new allocated string.
 */
gchar *a_Url_decode_hex_str(const gchar *str)
{
   gchar *new_str, *dest;
   int i, val;

   if (!str)
      return NULL;

   /* most cases won't have hex octets */
   if (!strchr(str, '%'))
      return g_strdup(str);

   dest = new_str = g_new(gchar, strlen(str) + 1);

   for (i = 0; str[i]; i++) {
      *dest++ = (str[i] == '%' && (val = Url_decode_hex_octet(str+i+1)) >= 0) ?
                i+=2, val : str[i];
   }
   *dest++ = 0;

   new_str = g_realloc(new_str, sizeof(gchar) * (dest - new_str));
   return new_str;
}

/*
 * Urlencode 'str'
 * -RL :: According to the RFC 1738, only alphanumerics, the special
 *        characters "$-_.+!*'(),", and reserved characters ";/?:@=&" used
 *        for their *reserved purposes* may be used unencoded within a URL.
 * We'll escape everything but alphanumeric and "-_.*" (as lynx).  --Jcid
 *
 * Note: the content type "application/x-www-form-urlencoded" is used:
 *       i.e., ' ' -> '+' and '\n' -> CR LF (see HTML 4.01, Sec. 17.13.4)
 */
gchar *a_Url_encode_hex_str(const gchar *str)
{
   static const char *verbatim = "-_.*";
   static const char *hex = "0123456789ABCDEF";
   char *newstr, *c;

   if (!str)
      return NULL;

   newstr = g_new(char, 6*strlen(str)+1);

   for (c = newstr; *str; str++)
      if ((isalnum(*str) && !(*str & 0x80)) || strchr(verbatim, *str))
      /* we really need isalnum for the "C" locale */
         *c++ = *str;
      else if (*str == ' ')
         *c++ = '+';
      else if (*str == '\n') {
         *c++ = '%';
         *c++ = '0';
         *c++ = 'D';
         *c++ = '%';
         *c++ = '0';
         *c++ = 'A';
      } else {
         *c++ = '%';
         *c++ = hex[(*str >> 4) & 15];
         *c++ = hex[*str & 15];
      }
   *c = 0;

  return newstr;
}


/*
 * RFC-2396 suggests this stripping when "importing" URLs from other media.
 * Strip: "URL:", enclosing < >, and embedded whitespace.
 * (We also strip illegal chars: 00-1F and 7F)
 */
gchar *a_Url_string_strip_delimiters(gchar *str)
{
   gchar *p, *new_str, *text;

   new_str = text = g_strdup(str);

   if (new_str) {
      if (strncmp(new_str, "URL:", 4) == 0)
         text += 4;
      if (*text == '<')
         text++;

      for (p = new_str; *text; text++)
         if (*text > 0x1F && *text != 0x7F && *text != ' ')
            *p++ = *text;
      if (p > new_str && p[-1] == '>')
         --p;
      *p = 0;
   }
   return new_str;
}
