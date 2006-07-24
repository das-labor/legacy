#ifndef __WEB_H__
#define __WEB_H__

#include <gtk/gtk.h>

#include "cache.h"     /* for CA_Callback_t */
#include "html.h"      /* for DilloHtmlLB */
#include "image.h"     /* for DilloImage */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Flag defines
 */
#define WEB_RootUrl  1
#define WEB_Image    2
#define WEB_Download 4   /* Half implemented... */


typedef struct _DilloWeb DilloWeb;

struct _DilloWeb {
  DilloUrl *url;              /* Requested URL */
  BrowserWindow *bw;          /* The requesting browser window [reference] */
  gint flags;                 /* Additional info */

  DilloImage *Image;          /* For image urls [reference] */
  FILE *stream;               /* File stream for local saving */
  gint SavedBytes;            /* Also for local saving */
};


DilloWeb* a_Web_new (const DilloUrl* url);
gint a_Web_valid(DilloWeb *web);
void a_Web_free (DilloWeb*);
gint a_Web_dispatch_by_type (const char *Type, DilloWeb *web,
                             CA_Callback_t *Call, void **Data);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __WEB_H__ */
