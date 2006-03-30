#ifndef CANTUN_H
#define CANTUN_H

#include "Can.h"

/////////////////////////////////////////////////////////////////////////////
// Types

/////////////////////////////////////////////////////////////////////////////
// Interface
void CanTunInit();
void CanTunGet(CanMessage *msg);

// char CanTunGetNB(CanMessage *msg);

void CanTunSend(CanMessage *);
void CanTunReset();
void CanTunHello();


#endif
