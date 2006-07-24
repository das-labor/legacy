#ifndef __DNS_H__
#define __DNS_H__

#include <gtk/gtk.h>
#include "chain.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void a_Dns_init (void);
void a_Dns_freeall(void);
void a_Dns_ccc(int Op, int Dir, int Br, ChainLink *Info,
               void *Data1, void *Data2);

#define DILLO_ADDR_MAX 16

typedef struct _DilloHost
{
  int af;
  int alen;
  char data[DILLO_ADDR_MAX];
} DilloHost;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DNS_H__ */
