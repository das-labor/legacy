#ifndef __MSG_H__
#define __MSG_H__

#include "prefs.h"

/*
 * You can disable any MSG* macro by adding the '_' prefix.
 */
#define _MSG(fmt...)
#define _MSG_HTML(fmt...)
#define _MSG_HTTP(fmt...)


#define MSG(fmt...)                    \
   G_STMT_START {                      \
      if (prefs.show_msg)              \
         g_print(fmt);                 \
   } G_STMT_END

#define MSG_HTML(fmt...)               \
   G_STMT_START {                      \
         Html_msg(html, fmt);          \
   } G_STMT_END

#define MSG_HTTP(fmt...)  g_print("HTTP warning: " fmt)

#endif /* __MSG_H__ */
