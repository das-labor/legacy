#ifndef __CAPI_H__
#define __CAPI_H__

#include <glib.h>
#include "cache.h"
#include "web.h"

/*
 * Function prototypes
 */
gint a_Capi_open_url(DilloWeb *web, CA_Callback_t Call, void *CbData);
gint a_Capi_get_buf(const DilloUrl *Url, gchar **PBuf, gint *BufSize);
gint a_Capi_dpi_send_cmd(DilloWeb *web, void *bw, char *cmd, char *server,
                         gint flags);
gint a_Capi_url_uses_dpi(gchar *url_str, gchar **server_ptr);



#endif /* __CAPI_H__ */

