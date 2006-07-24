#ifndef __COOKIES_H__
#define __COOKIES_H__

#ifdef DISABLE_COOKIES
# define a_Cookies_get(url)  g_strdup("")
# define a_Cookies_init()    ;
# define a_Cookies_freeall() ;
#else
  char *a_Cookies_get(const DilloUrl *request_url);
  void  a_Cookies_set(GList *cookie_string, const DilloUrl *set_url);
  void  a_Cookies_init( void );
  void  a_Cookies_freeall( void );
#endif

#endif /* !__COOKIES_H__ */
