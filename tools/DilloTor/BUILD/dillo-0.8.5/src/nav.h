#ifndef __NAV_H__
#define __NAV_H__

#include "browser.h"
#include "dw_widget.h"    /* for DwWidget */


/* useful macros for the navigation stack */
#define NAV_IDX(bw, i)   (bw)->nav_stack[i]
#define NAV_TOP(bw)      (bw)->nav_stack[(bw)->nav_stack_ptr]


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void a_Nav_push(BrowserWindow *bw, const DilloUrl *url);
void a_Nav_push_nw(BrowserWindow *bw, const DilloUrl *url);
void a_Nav_vpush(void *vbw, const DilloUrl *url);
void a_Nav_back(BrowserWindow *bw);
void a_Nav_forw(BrowserWindow *bw);
void a_Nav_home(BrowserWindow *bw);
void a_Nav_reload(BrowserWindow *bw);
void a_Nav_init(BrowserWindow *bw);
void a_Nav_free(BrowserWindow *bw);
void a_Nav_cancel_expect (BrowserWindow *bw);
void a_Nav_expect_done(BrowserWindow *bw);
void a_Nav_jump_callback(GtkWidget *widget, gpointer client_data, gint NewBw);
gint a_Nav_stack_ptr(BrowserWindow *bw);
gint a_Nav_stack_size(BrowserWindow *bw);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NAV_H__ */


