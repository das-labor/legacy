#ifndef __PREFS_H__
#define __PREFS_H__

#include "url.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DILLO_START_PAGE "about:splash"
#define DILLO_HOME "http://www.dillo.org/"
#define HTTP_PROXY "http://127.0.0.1:8118/"
#define D_GEOMETRY_DEFAULT_WIDTH   800
#define D_GEOMETRY_DEFAULT_HEIGHT  600
#define D_GEOMETRY_DEFAULT_XPOS  -9999
#define D_GEOMETRY_DEFAULT_YPOS  -9999

#define DW_COLOR_DEFAULT_GREY   0xd6d6d6
#define DW_COLOR_DEFAULT_BLACK  0x000000
#define DW_COLOR_DEFAULT_BLUE   0x0000ff
#define DW_COLOR_DEFAULT_PURPLE 0x800080
#define DW_COLOR_DEFAULT_BGND   0xd6d6c0

/* define enumeration values to be returned */
enum {
   PARSE_OK = 0,
   FILE_NOT_FOUND
};

/* define enumeration values to be returned for specific symbols */
typedef enum {
   DRC_TOKEN_FIRST = G_TOKEN_LAST,
   DRC_TOKEN_GEOMETRY,
   DRC_TOKEN_PROXY,
   DRC_TOKEN_PROXYUSER,
   DRC_TOKEN_NOPROXY,
   DRC_TOKEN_LINK_COLOR,
   DRC_TOKEN_VISITED_COLOR,
   DRC_TOKEN_BG_COLOR,
   DRC_TOKEN_ALLOW_WHITE_BG,
   DRC_TOKEN_FORCE_MY_COLORS,
   DRC_TOKEN_CONTRAST_VISITED_COLOR,
   DRC_TOKEN_TEXT_COLOR,
   DRC_TOKEN_USE_OBLIQUE,
   DRC_TOKEN_START_PAGE,
   DRC_TOKEN_HOME,
   DRC_TOKEN_PANEL_SIZE,
   DRC_TOKEN_SMALL_ICONS,
   DRC_TOKEN_FONT_FACTOR,
   DRC_TOKEN_SHOW_TOOLTIP,
   DRC_TOKEN_LIMIT_TEXT_WIDTH,
   DRC_TOKEN_W3C_PLUS_HEURISTICS,
   DRC_TOKEN_USE_DICACHE,
   DRC_TOKEN_SHOW_BACK,
   DRC_TOKEN_SHOW_FORW,
   DRC_TOKEN_SHOW_HOME,
   DRC_TOKEN_SHOW_RELOAD,
   DRC_TOKEN_SHOW_SAVE,
   DRC_TOKEN_SHOW_STOP,
   DRC_TOKEN_SHOW_BOOKMARKS,
   DRC_TOKEN_SHOW_MENUBAR,
   DRC_TOKEN_SHOW_CLEAR_URL,
   DRC_TOKEN_SHOW_URL,
   DRC_TOKEN_SHOW_SEARCH,
   DRC_TOKEN_SHOW_PROGRESS_BOX,
   DRC_TOKEN_FULLWINDOW_START,
   DRC_TOKEN_TRANSIENT_DIALOGS,
   DRC_TOKEN_FW_FONT,
   DRC_TOKEN_VW_FONT,
   DRC_TOKEN_GENERATE_SUBMIT,
   DRC_TOKEN_ENTERPRESS_FORCES_SUBMIT,
   DRC_TOKEN_SEARCH_URL,
   DRC_TOKEN_SHOW_MSG,
   DRC_TOKEN_SHOW_EXTRA_WARNINGS,

   DRC_TOKEN_LAST
} Dillo_Rc_TokenType;

typedef struct _DilloPrefs DilloPrefs;

struct _DilloPrefs {
   gint width;
   gint height;
   gint xpos;
   gint ypos;
   DilloUrl *http_proxy;
   gchar *http_proxyuser;
   gchar *no_proxy;
   gchar **no_proxy_vec;
   DilloUrl *start_page;
   DilloUrl *home;
   gint32 link_color;
   gint32 visited_color;
   gint32 bg_color;
   gint32 text_color;
   gboolean allow_white_bg;
   gboolean use_oblique;
   gboolean force_my_colors;
   gboolean contrast_visited_color;
   gboolean show_tooltip;
   gint panel_size;
   gboolean small_icons;
   gboolean limit_text_width;
   gboolean w3c_plus_heuristics;
   gdouble font_factor;
   gboolean use_dicache;
   gboolean show_back;
   gboolean show_forw;
   gboolean show_home;
   gboolean show_reload;
   gboolean show_save;
   gboolean show_stop;
   gboolean show_bookmarks;
   gboolean show_menubar;
   gboolean show_clear_url;
   gboolean show_url;
   gboolean show_search;
   gboolean show_progress_box;
   gboolean fullwindow_start;
   gboolean transient_dialogs;
   gchar *vw_fontname;
   gchar *fw_fontname;
   gboolean generate_submit;
   gboolean enterpress_forces_submit;
   gchar *search_url;
   gboolean show_msg;
   gboolean show_extra_warnings;
};

/* Global Data */
DilloPrefs prefs;

void a_Prefs_init(void);
void a_Prefs_freeall(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PREFS_H__ */
