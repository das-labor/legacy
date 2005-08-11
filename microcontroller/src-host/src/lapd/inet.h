#ifndef LAPDINET
#define LAPDINET

/* open listening socket and initialize */
void net_init(int port);

/* set bits in fd_set and return highest FD */
int net_fdset(fd_set *set);

#endif
