#ifndef __BOOKMARK_H__
#define __BOOKMARK_H__

#include "browser.h"


void a_Bookmarks_init(void);
void a_Bookmarks_add(GtkWidget *widget, gpointer client_data);
void a_Bookmarks_show(BrowserWindow *bw);

/* todo: this is for testing purposes */
void a_Bookmarks_chat_add(BrowserWindow *Bw, char *Cmd, char *answer);

#endif /* __BOOKMARK_H__ */
