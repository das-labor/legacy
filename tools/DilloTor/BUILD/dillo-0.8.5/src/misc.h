#ifndef __MISC_H__
#define __MISC_H__
#include <stdlib.h>

gchar *a_Misc_prepend_user_home(const char *file);
gchar *a_Misc_escape_chars(const gchar *str, gchar *esc_set);
gchar *a_Misc_stuff_chars(const gchar *str, const gchar *esc_set);
gchar *a_Misc_stristr(char *src, char *str);
gchar *a_Misc_expand_tabs(const char *str);
gchar *a_Misc_strsep(char **orig, const char *delim);
#define d_strsep a_Misc_strsep
gint *a_Misc_strsplitpos(const gchar *str, const gchar *delim);
gint *a_Misc_strsplitposdup(gint *pos);
gint a_Misc_get_content_type_from_data(void *Data, size_t Size,
                                       const gchar **PT);
gint a_Misc_parse_geometry(gchar *geom, gint *x, gint *y, gint *w, gint *h);
gchar *a_Misc_encode_base64(const gchar *in);

/* Return a NULL-terminated string containing the characters from p1 to p2. */
#define a_Misc_strpdup(s, p1, p2) g_strndup((s) + (p1), (p2) - (p1))

#endif /* __MISC_H__ */

