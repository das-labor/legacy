/*
    MIME type handlers
    Randall Maas
    1999
*/
#ifndef __MIME_H__
#define __MIME_H__

#include <config.h>
#include <stddef.h>
#include <gtk/gtk.h>
#include "../dw_widget.h"
#include "../cache.h"

typedef DwWidget* (*Viewer_t) (const char*, void*, CA_Callback_t*, void**);

/*
 * Function prototypes defined elsewhere
 */
DwWidget* a_Html_text (const char *Type,void *web, CA_Callback_t *Call,
                       void **Data);
DwWidget* a_Plain_text(const char *Type,void *web, CA_Callback_t *Call,
                       void **Data);
#ifdef ENABLE_JPEG
DwWidget* a_Jpeg_image(const char *Type,void *web, CA_Callback_t *Call,
                       void **Data);
#endif
#ifdef ENABLE_PNG
DwWidget* a_Png_image (const char *Type,void *web, CA_Callback_t *Call,
                       void **Data);
#endif
#ifdef ENABLE_GIF
DwWidget* a_Gif_image (const char *Type,void *web, CA_Callback_t *Call,
                       void **Data);
#endif

/*
 * Functions defined inside Mime module
 */
void a_Mime_init(void);
DwWidget* a_Mime_set_viewer(const char *content_type, void *Ptr,
                            CA_Callback_t *Call, void **Data);

#endif /* __MIME_H__ */
