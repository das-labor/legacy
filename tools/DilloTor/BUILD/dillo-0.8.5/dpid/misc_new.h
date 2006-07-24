#ifndef MISC_NEW_H
#define MISC_NEW_H

#include <glib.h>

gint a_Misc_close_fd(gint fd);

gchar *a_Misc_get_home(void);

gchar *a_Misc_get_user(void);

gchar *a_Misc_prepend_user_home(const char *file);

char *a_Misc_get_attr_value(char *tag, size_t tagsize, char *attrname);

char *a_Misc_get_line(FILE *stream);

GString *a_Misc_rdtag(int socket);

char *a_Misc_readtag(int sock);

char *a_Misc_mkdtemp(char *template);

#endif
