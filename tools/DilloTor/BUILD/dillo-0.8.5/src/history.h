
#ifndef __DILLO_HISTORY_H__
#define __DILLO_HISTORY_H__

#include <glib.h>

#include "url.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int a_History_add_url(DilloUrl *url);
int a_History_set_title(gint idx, const gchar *title);
DilloUrl *a_History_get_url(gint idx);
const gchar *a_History_get_title(gint idx);
const gchar *a_History_get_title_by_url(DilloUrl *url, gint force);
void a_History_free(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DILLO_HISTORY_H__ */
