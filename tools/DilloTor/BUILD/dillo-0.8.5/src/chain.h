#ifndef __CHAIN_H__
#define __CHAIN_H__

/*
 * Concomitant control chain (CCC)
 * Theory and code by Jorge Arellano Cid <jcid@dillo.org>
 */

#include <glib.h>

/*
 * Supported CCC operations
 */
#define OpStart  1
#define OpSend   2
#define OpStop   3
#define OpEnd    4
#define OpAbort  5


/*
 * Linking direction
 */
#define FWD 1
#define BCK 2


typedef struct _ChainLink ChainLink;
typedef struct _DataBuf DataBuf;
typedef void (*ChainFunction_t)(int Op, int Branch, int Dir, ChainLink *Info,
                                void *Data1, void *Data2);

/* This is the main data structure for CCC nodes */
struct _ChainLink {
   void *LocalKey;

   ChainLink *FcbInfo;
   ChainFunction_t Fcb;
   gint FcbBranch;

   ChainLink *BcbInfo;
   ChainFunction_t Bcb;
   gint BcbBranch;
};

/* A convenience data structure for passing data chunks between nodes */
struct _DataBuf {
   gchar *Buf;
   gint Size;
   gint Code;
};



/*
 * Function prototypes
 */
ChainLink *a_Chain_new(void);
ChainLink *a_Chain_link_new(ChainLink *AInfo, ChainFunction_t AFunc,
                            gint Direction, ChainFunction_t BFunc,
                            gint AtoB_branch, gint BtoA_branch);
void a_Chain_del_link(ChainLink *Info, gint Direction);
gint a_Chain_fcb(int Op, ChainLink *Info, void *Data1, void *Data2);
gint a_Chain_bcb(int Op, ChainLink *Info, void *Data1, void *Data2);

DataBuf *a_Chain_dbuf_new(void *buf, gint size, gint code);
void a_Chain_debug_msg(char *FuncStr, int Op, int Branch, int Dir);

/*
 * CCC functions of subscribing modules
 */
void a_Cache_ccc(int Op, int Branch, int Dir, ChainLink *Info,
                 void *Data1, void *Data2);

#endif /* __CHAIN_H__ */
