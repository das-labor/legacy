#ifndef __STRBUF_H__
#define __STRBUF_H__

#include <glib.h>

typedef struct _Strbuf Strbuf_t;

struct _Strbuf {
   GSList *list;
   int num_chars;
   gchar *str;
   gboolean str_valid;
};

Strbuf_t* a_Strbuf_new    (void);
void      a_Strbuf_free   (Strbuf_t *strbuf);
void      a_Strbuf_append (Strbuf_t *strbuf,
                           gchar *str);
gchar*    a_Strbuf_chars  (Strbuf_t *strbuf);



#endif /* __STRBUF_H__ */
