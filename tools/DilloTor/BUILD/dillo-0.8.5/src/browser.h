#ifndef __BROWSER_H__
#define __BROWSER_H__

#include <sys/types.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include "url.h"     /* for DilloUrl */


typedef struct _BrowserWindow BrowserWindow;
typedef struct _DilloMenuPopup DilloMenuPopup;

typedef struct {
    DilloUrl *Url;   /* URL-key for this cache connection */
    gint Flags;      /* {WEB_RootUrl, WEB_Image, WEB_Download} */
} BwUrls;

/* The popup menus so that we can call them. */
struct _DilloMenuPopup
{
   GtkWidget *over_page;
   GtkWidget *over_link;
   GtkWidget *over_image;
   GtkWidget *over_back;
   GtkWidget *over_forw;
   GtkWidget *over_bug;
   DilloUrl *url;
   DilloUrl *url2;
   GtkWidget *ol_oi_submenu;
};

/* browser_window contains all widgets to create a single window */
struct _BrowserWindow
{
   /* Control-Panel handleboxes --used for hiding */
   GSList *PanelHandles;
   /* Flag: TRUE when control-panel is hidden */
   gboolean fullwindow;

   /* widgets for the main window */
   GtkWidget *main_window;
   GtkWidget *back_button;
   GtkWidget *forw_button;
   GtkWidget *home_button;
   GtkWidget *reload_button;
   GtkWidget *save_button;
   GtkWidget *stop_button;
   GtkWidget *bookmarks_button;
   GtkWidget *menubar;
   GtkWidget *clear_url_button;
   GtkWidget *location;
   GtkWidget *search_button;
   GtkWidget *progress_box;
   GtkWidget *status_box;
   GtkWidget *status;
   GtkWidget *status_bug_meter;
   gint status_is_link;
   GtkWidget *imgprogress;
   GtkWidget *progress;

   /* the keyboard accelerator table */
   GtkAccelGroup *accel_group;

   /* Popup menu for this BrowserWindow */
   DilloMenuPopup menu_popup;

   /* The "Headings" and "Anchors" menus */
   GtkWidget *pagemarks_menuitem;
   GtkWidget *pagemarks_menu;
   GtkWidget *pagemarks_last;

   /* "View page Bugs" menuitem so we can set its sensitivity */
   GtkWidget *viewbugs_menuitem;

   /* This is the main document widget. (HTML rendering or whatever) */
   GtkWidget *docwin;

   /* Current cursor type */
   GdkCursorType CursorType;

   /* A list of active cache clients in the window (The primary Key) */
   gint *RootClients;
   gint NumRootClients;
   gint MaxRootClients;

   /* Image Keys for all active connections in the window */
   gint *ImageClients;
   gint NumImageClients;
   gint MaxImageClients;
   /* Number of different images in the page */
   gint NumImages;
   /* Number of different images already loaded */
   gint NumImagesGot;

   /* List of all Urls requested by this page (and its types) */
   BwUrls *PageUrls;
   gint NumPageUrls;
   gint MaxPageUrls;

   /* widgets for dialog boxes off main window */
   GtkWidget *open_dialog_window;
   GtkWidget *open_dialog_entry;
   GtkWidget *openfile_dialog_window;
   GtkWidget *quit_dialog_window;
   GtkWidget *save_dialog_window;
   GtkWidget *save_link_dialog_window;
   GtkWidget *findtext_dialog_window;
   GtkWidget *findtext_dialog_check;
   GtkWidget *findtext_dialog_entry;
   GtkWidget *search_dialog_window;
   GtkWidget *search_dialog_entry;
   GtkWidget *proxy_passwd_dialog_window;
   GtkWidget *proxy_passwd_dialog_entry;
   GtkWidget *question_dialog_window;
   gpointer  question_dialog_data;
   gpointer  question_dialog_answer;
   GtkWidget *viewsource_window;
   GtkWidget *pagebugs_window;
   GtkWidget *full_screen_off_button;

   /* Dillo navigation stack (holds indexes to history list) */
   gint *nav_stack;
   gint nav_stack_size;       /* [1 based] */
   gint nav_stack_size_max;
   /* 'nav_stack_ptr' refers to what's being displayed */
   gint nav_stack_ptr;        /* [0 based] */
   /* When the user clicks a link, the URL isn't pushed directly to history;
    * nav_expect_url holds it until the first answer-bytes are got. Only then
    * it is sent to history and referenced in 'nav_stack[++nav_stack_ptr]' */
   DilloUrl *nav_expect_url;
   /* 'nav_expecting' is true if the last URL is being loaded for
    * the first time and has not gotten the dw yet. */
   gboolean nav_expecting;

   /* Counter for the number of hops on a redirection. Used to stop
    * redirection loops (accounts for WEB_RootUrl only) */
   gint redirect_level;

   /* The id for the idle function that sets button sensitivity. */
   guint sens_idle_id;
};



#endif /* __BROWSER_H__ */

