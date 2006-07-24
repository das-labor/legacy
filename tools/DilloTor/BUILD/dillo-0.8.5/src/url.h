/*
 * File : url.h - Dillo
 *
 * Copyright (C) 2001 Jorge Arellano Cid   <jcid@users.sourceforge.net>
 *               2001 Livio Baldini Soares <livio@linux.ime.usp.br>
 *
 * Parse and normalize all URL's inside Dillo.
 */

#ifndef __URL_H__
#define __URL_H__

#include <string.h>       /* for strcmp */
#include <glib.h>


#define DILLO_URL_HTTP_PORT        80
#define DILLO_URL_HTTPS_PORT       443
#define DILLO_URL_FTP_PORT         21
#define DILLO_URL_MAILTO_PORT      25
#define DILLO_URL_NEWS_PORT        119
#define DILLO_URL_TELNET_PORT      23
#define DILLO_URL_GOPHER_PORT      70


/*
 * Values for DilloUrl->flags.
 * Specifies which which action to perform with an URL.
 */
#define URL_Get                 (1 << 0)
#define URL_Post                (1 << 1)
#define URL_ISindex             (1 << 2)
#define URL_Ismap               (1 << 3)
#define URL_RealmAccess         (1 << 4)

#define URL_E2EReload           (1 << 5)
#define URL_ReloadImages        (1 << 6)
#define URL_ReloadPage          (1 << 7)
#define URL_ReloadFromCache     (1 << 8)

#define URL_ReloadIncomplete    (1 << 9)
#define URL_SpamSafe            (1 << 10)

/*
 * Access methods to fields inside DilloURL.
 * (non '_'-ended macros MUST use these for initialization sake)
 */
/* these MAY return NULL: */
#define URL_SCHEME_(u)         u->scheme
#define URL_AUTHORITY_(u)      u->authority
#define URL_PATH_(u)           u->path
#define URL_QUERY_(u)          u->query
#define URL_FRAGMENT_(u)       u->fragment
#define URL_HOST_(u)           a_Url_hostname(u)
#define URL_DATA_(u)           u->data
#define URL_ALT_(u)            u->alt
#define URL_STR_(u)            a_Url_str(u)
/* these return an integer */
#define URL_PORT_(u)           (URL_HOST(u) ? u->port : u->port)
#define URL_FLAGS_(u)          u->flags
#define URL_POSX_(u)           u->scrolling_position_x
#define URL_POSY_(u)           u->scrolling_position_y
#define URL_ILLEGAL_CHARS_(u)  url->illegal_chars

/*
 * Access methods that always return a string:
 * When the "empty" and "undefined" concepts of RFC-2396 are irrelevant to
 * the caller, and a string is required, use these methods instead:
 */
#define NPTR2STR(p)           ((p) ? (p) : "")
#define URL_SCHEME(u)         NPTR2STR(URL_SCHEME_(u))
#define URL_AUTHORITY(u)      NPTR2STR(URL_AUTHORITY_(u))
#define URL_PATH(u)           NPTR2STR(URL_PATH_(u))
#define URL_QUERY(u)          NPTR2STR(URL_QUERY_(u))
#define URL_FRAGMENT(u)       NPTR2STR(URL_FRAGMENT_(u))
#define URL_HOST(u)           NPTR2STR(URL_HOST_(u))
#define URL_DATA(u)           NPTR2STR(URL_DATA_(u))
#define URL_ALT(u)            NPTR2STR(URL_ALT_(u))
#define URL_STR(u)            NPTR2STR(URL_STR_(u))
#define URL_PORT(u)           URL_PORT_(u)
#define URL_FLAGS(u)          URL_FLAGS_(u)
#define URL_POSX(u)           URL_POSX_(u)
#define URL_POSY(u)           URL_POSY_(u)
#define URL_ILLEGAL_CHARS(u)  URL_ILLEGAL_CHARS_(u)



/* URL-camp compare methods */
#define URL_STRCAMP_EQ(s1,s2) \
   ((!(s1) && !(s2)) || ((s1) && (s2) && !strcmp(s1,s2)))
#define URL_STRCAMP_I_EQ(s1,s2) \
   ((!(s1) && !(s2)) || ((s1) && (s2) && !g_strcasecmp(s1,s2)))
#define URL_GSTRCAMP_EQ(s1,s2) \
   ((!(s1) && !(s2)) || ((s1) && (s2) && !strcmp((s1)->str,(s2)->str)))
#define URL_GSTRCAMP_I_EQ(s1,s2) \
   ((!(s1) && !(s2)) || ((s1) && (s2) && !g_strcasecmp((s1)->str,(s2)->str)))


typedef struct _DilloUrl DilloUrl;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct _DilloUrl {
   GString *url_string;
   const gchar *buffer;
   const gchar *scheme;            //
   const gchar *authority;         //
   const gchar *path;              // These are references only
   const gchar *query;             // (no need to free them)
   const gchar *fragment;          //
   const gchar *hostname;          //
   gint port;
   gint flags;
   const gchar *data;              /* POST */
   const gchar *alt;               /* "alt" text (used by image maps) */
   gint ismap_url_len;             /* Used by server side image maps */
   gint32 scrolling_position_x, scrolling_position_y;
                                   /* remember position of visited urls */
   gint illegal_chars;             /* number of illegal chars */
};


DilloUrl* a_Url_new(const gchar *url_str, const gchar *base_url,
                    gint flags, gint32 posx, gint32 posy);
void a_Url_free(DilloUrl *u);
gchar *a_Url_str(const DilloUrl *url);
const gchar *a_Url_hostname(const DilloUrl *u);
DilloUrl* a_Url_dup(const DilloUrl *u);
gint a_Url_cmp(const DilloUrl *A, const DilloUrl *B);
void a_Url_set_flags(DilloUrl *u, gint flags);
void a_Url_set_data(DilloUrl *u, gchar *data);
void a_Url_set_alt(DilloUrl *u, const gchar *alt);
void a_Url_set_pos(DilloUrl *u, gint32 posx, gint32 posy);
void a_Url_set_ismap_coords(DilloUrl *u, gchar *coord_str);
gchar *a_Url_decode_hex_str(const gchar *str);
gchar *a_Url_encode_hex_str(const gchar *str);
gchar *a_Url_string_strip_delimiters(gchar *str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __URL_H__ */
