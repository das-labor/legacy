#ifndef __IO_H__
#define __IO_H__

#include<unistd.h>
#include<sys/uio.h>
#include <glib.h>

#include "../chain.h"

/*
 * IO Operations
 */
#define IORead   0
#define IOWrite  1
#define IOWrites 2
#define IOClose  3
#define IOAbort  4

/*
 * IO Flags
 */
#define IOFlag_FreeIOBuf   (1 << 1)
#define IOFlag_ForceClose  (1 << 2)
#define IOFlag_SingleWrite (1 << 3)

/*
 * IO constants
 */
#define IOBufLen         4096
#define IOBufLen_Http    4096
#define IOBufLen_File    4096
#define IOBufLen_Proto   4096
#define IOBufLen_About   4096


typedef struct {
   gint Key;              /* Primary Key (for klist) */
   gint Op;               /* IORead | IOWrite | IOWrites */
   gint FD;               /* Current File Descriptor */
   gint Flags;            /* Flag array (look definitions above) */
   glong Status;          /* Number of bytes read, or -errno code */

   void *Buf;             /* Buffer place */
   size_t BufSize;        /* Buffer length */
   void *BufStart;        /* PRIVATE: only used inside IO.c! */

   void *ExtData;         /* External data reference (not used by IO.c) */
   void *Info;            /* CCC Info structure for this IO */
   GIOChannel *GioCh;     /* IO channel */
   guint watch_id;        /* glib's event source id */
} IOData_t;


/*
 * Exported functions
 */
IOData_t* a_IO_new(gint op, gint fd);
void a_IO_set_buf(IOData_t *io, void *Buf, size_t BufSize);
void a_IO_add_buf(IOData_t *io, void *Buf, size_t BufSize);
/* Note: a_IO_ccc() is defined in Url.h together with the *_ccc() set */

void a_IO_write_chunk(gint FD, void *Key, void *Buf, size_t BufSize);

#endif /* __IO_H__ */

