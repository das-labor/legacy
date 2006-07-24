#ifndef __CACHE_H__
#define __CACHE_H__

#include <glib.h>
#include "chain.h"
#include "url.h"

/*
 * Cache Op codes
 */
#define CA_Send    (0)  /* Normal update */
#define CA_Close   (1)  /* Successful operation close */
#define CA_Abort   (2)  /* Operation abort */

/*
 * Flag Defines
 */
#define CA_GotHeader       (1)  /* True after header is completely got */
#define CA_GotContentType  (2)  /* True after Content-Type is known */
#define CA_GotLength       (4)  /* True if Content-Length is known */
#define CA_GotData         (8)  /* True if we have all Data in cache */
#define CA_FreeData       (16)  /* Free the cache Data on close */
#define CA_Redirect       (32)  /* Data actually points to a redirect */
#define CA_ForceRedirect  (64)  /* Unconditional redirect */
#define CA_TempRedirect  (128)  /* Temporal redirect */
#define CA_NotFound      (256)  /* True if remote server didn't find the URL */
#define CA_Stopped       (512)  /* True if the entry has been stopped */
#define CA_MsgErased    (1024)  /* Used to erase the bw's status bar */
#define CA_RedirectLoop (2048)  /* Redirect loop */

/*
 * Callback type for cache clients
 */
typedef struct _CacheClient CacheClient_t;
typedef void (*CA_Callback_t)(int Op, CacheClient_t *Client);

/*
 * Data structure for cache clients.
 */
struct _CacheClient {
   gint Key;                /* Primary Key for this client */
   const DilloUrl *Url;     /* Pointer to a cache entry Url */
   guchar *Buf;             /* Pointer to cache-data */
   guint BufSize;           /* Valid size of cache-data */
   CA_Callback_t Callback;  /* Client function */
   void *CbData;            /* Client function data */
   void *Web;               /* Pointer to the Web structure of our client */
};

/*
 * Function prototypes
 */
void a_Cache_init(void);
gint a_Cache_open_url(void *Web, CA_Callback_t Call, void *CbData);
gint a_Cache_get_buf(const DilloUrl *Url, gchar **PBuf, gint *BufSize);
void a_Cache_freeall(void);
void a_Cache_null_client(int Op, CacheClient_t *Client);
void a_Cache_stop_client(gint Key);


#endif /* __CACHE_H__ */

