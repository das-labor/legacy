#ifndef __COLORS_H__
#define __COLORS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

gint32 a_Color_parse (const char *subtag, gint32 default_color, gint *err);
gint32 a_Color_vc(gint32 candidate, gint32 c1, gint32 c2, gint32 c3);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __COLORS_H__ */
