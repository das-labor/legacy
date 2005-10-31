#ifndef DEBUG_H
#define DEBUG_H

extern int debug_level;    
extern int debug_syslog;   /* use syslog? */

void debug( int level, char *format, ... );
void debug_perror( int level, char *format, ... );
void debug_assert( int test,  char *msg, ... );
void debug_assert2(           char *msg, ... );
#ifdef NDEBUG
 #define debug_assert(test,msg,...) ((void)(test))
#else
 #define debug_assert(test,msg,...) ((void)((test) || debug_assert2(msg,__VA_ARGS__)))
#endif


#endif
