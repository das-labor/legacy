#ifndef DEBUG_H
#define DEBUG_H

extern int debug_level;    
extern int debug_syslog;   /* use syslog? */

void debug( int level, char *format, ... );
void debug_perror( int level, char *format, ... );
void debug_assert( int test,  char *msg, ... );
void debug_hexdump(int level, char *text, unsigned char *addr, int size);

#endif
