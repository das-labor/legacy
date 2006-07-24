/*
 * File: IO.c
 *
 * Copyright (C) 2000, 2001, 2002 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * Dillo's signal driven IO engine
 */

#include <pthread.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <gdk/gdk.h>
#include "../msg.h"
#include "../chain.h"
#include "../list.h"
#include "../klist.h"
#include "IO.h"

#define DEBUG_LEVEL 5
#include "../debug.h"


/*
 * Symbolic defines for shutdown() function
 * (Not defined in the same header file, for all distros --Jcid)
 */
#define IO_StopRd   0
#define IO_StopWr   1
#define IO_StopRdWr 2


/*
 * Local data
 */
static Klist_t *ValidIOs = NULL; /* Active IOs list. It holds pointers to
                                  * IOData_t structures. */

/*
 *  Forward declarations
 */
void a_IO_ccc(int Op, int Branch, int Dir, ChainLink *Info,
              void *Data1, void *Data2);

/*
 * Debug procedure...

static void IO_print_cond_status(gchar  *str,  GIOCondition  cond,
                                 GIOChannel *gio, gint key)
{
   MSG("%s FD=%d key=%d [", str, g_io_channel_unix_get_fd(gio), key);
   MSG(cond & G_IO_IN ? "G_IO_IN " : "");
   MSG(cond & G_IO_OUT ? "G_IO_OUT " : "");
   MSG(cond & G_IO_PRI ? "G_IO_PRI " : "");
   MSG(cond & G_IO_ERR ? "G_IO_ERR " : "");
   MSG(cond & G_IO_HUP ? "G_IO_HUP " : "");
   MSG(cond & G_IO_NVAL ? "G_IO_NVAL " : "");
   MSG("]\n");
}
 */


/* IO API  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*
 * Return a newly created, and initialized, 'io' struct
 */
IOData_t *a_IO_new(gint op, gint fd)
{
   IOData_t *io = g_new0(IOData_t, 1);
   io->Op = op;
   io->FD = fd;
   io->GioCh = g_io_channel_unix_new(fd);
   io->Flags = 0;
   io->Key = 0;
   return io;
}

/*
 * Set the transfer buffer.
 * (if we allocate it, we free it)
 */
void a_IO_set_buf(IOData_t *io, void *Buf, size_t BufSize)
{
   if (io->Flags & IOFlag_FreeIOBuf) {
      g_free(io->BufStart);
      io->Flags &= ~IOFlag_FreeIOBuf;
   }

   if (!Buf && BufSize) {
      io->BufStart = io->Buf = g_malloc(BufSize);
      io->BufSize = BufSize;
      io->Flags |= IOFlag_FreeIOBuf;

   } else {
      io->BufStart = io->Buf = Buf;
      io->BufSize = BufSize;
   }
}

/*
 * Add a new chunk to the transfer buffer.
 * (we allocate it, so we free it)
 */
void a_IO_add_buf(IOData_t *io, void *Buf, size_t BufSize)
{
   size_t offset;

   if (BufSize == 0) {
      /* This is the last chunk */
      io->Flags |= IOFlag_ForceClose;
      return;
   }

   offset = (io->Buf) ? (gchar*)io->Buf - (gchar*)io->BufStart : 0;
   io->BufStart = g_realloc(io->BufStart, offset + io->BufSize + BufSize);
   memcpy((gchar*)io->BufStart + offset + io->BufSize, Buf, BufSize);
   io->Buf = (gchar*)io->BufStart + offset;
   io->BufSize += BufSize;
   io->Flags |= IOFlag_FreeIOBuf;
}

/*
 * Return transfer buffer.
 *
void a_IO_get_buf(IOData_t *io, void **Buf, size_t *BufSize)
{
   *Buf = io->Buf;
   *BufSize = io->BufSize;
}
 */

typedef struct {
   pthread_t thrID;        /* Thread id */
   pthread_mutex_t mut;
   pthread_cond_t cond;

   gint FD;
   void *Key;

   void *Buf1Start;
   void *Buf1;
   size_t Buf1Size;
   void *Buf2;
   size_t Buf2Size;

   gint LastChunk;
   gint CloseFD;
   gint Done;
} thr_data_t;

/* Active data for threaded chunk transfers */
static thr_data_t **ThrData = NULL;
static gint ThrDataSize = 0;
static gint ThrDataMax = 8;

/*
 * Create a new ThrData node
 */
static thr_data_t *IO_thrdata_new(gint FD, void *Key)
{
   thr_data_t *data;

   data = g_new(thr_data_t, 1);
   pthread_mutex_init(&data->mut, NULL);
   pthread_cond_init(&data->cond, NULL);
   data->FD = FD;
   data->Key = Key;
   data->Buf1Start = data->Buf1 = data->Buf2 = NULL;
   data->Buf1Size = data->Buf2Size = 0;
   data->LastChunk = 0;
   data->CloseFD = 0;
   data->Done = 0;
   return data;
}

/*
 * Free a ThrData node
 */
static void IO_thrdata_free(thr_data_t *td)
{
   /* EBUSY should not happen: IO_thrdata_free is called after the
    * thread is done */
   while (pthread_cond_destroy(&td->cond) == EBUSY) {
      g_warning("IO_thrdata_free: EBUSY\n");
      if (pthread_cancel(td->thrID) == ESRCH)
         break;
   }

   pthread_mutex_destroy(&td->mut);
   g_free(td);
}

/*
 * Search data node for a FD
 *  - The FD is searched using a Key because using the FD itself may fail
 *  due to a race condition between the FD close and its reuse.
 *  - This function also frees already closed data.
 */
static thr_data_t *IO_thrdata_find(void *Key)
{
   gint i, idx = -1;

   _MSG("TL(%p): [", Key);
   for (i = 0; i < ThrDataSize; ++i) {
      _MSG(" %d%s", ThrData[i]->FD, ThrData[i]->Done ? "D" : "");
      if (ThrData[i]->Done) {
         IO_thrdata_free(ThrData[i]);
         a_List_remove(ThrData, i, ThrDataSize);
         --i;
      } else if (ThrData[i]->Key == Key) {
         idx = i;
         _MSG("*");
      }
   }
   _MSG("]\n");

   if (idx != -1)
      return ThrData[idx];
   return NULL;
}


/*
 * Write the data buffer through a FD.
 * [This function runs on its own thread]
 */
static void *IO_write_chunk(void *ptr)
{
   gint lock = 0;
   thr_data_t *data = ptr;
   ssize_t St;
   gint st, done;

   _MSG("thr::\n");
   _MSG("      [thrID:%lu]\n", (gulong)data->thrID);

   while (1) {
      _MSG("thr:: trying to lock mutex\n");
      if (!lock) {
         pthread_mutex_lock(&data->mut);
         lock = 1;
      }
      _MSG("thr:: mutex locked!\n");
      _MSG("thr:: Buf1:%p Buf2:%p LastChunk:%d Done:%d\n",
           data->Buf1, data->Buf2, data->LastChunk, data->Done);
      if (data->Buf2) {
         /* Buf1 := Buf2;  Buf2 := NULL */
         g_free(data->Buf1Start);
         data->Buf1Start = data->Buf1 = data->Buf2;
         data->Buf1Size = data->Buf2Size;
         data->Buf2 = NULL;
         data->Buf2Size = 0;
         _MSG("thr:: Buf1:%p Buf2:%p LastChunk:%d Done:%d\n",
              data->Buf1, data->Buf2, data->LastChunk, data->Done);
         pthread_mutex_unlock(&data->mut);
         lock = 0;
         _MSG("thr:: mutex unlocked!\n");

         /*** write all ***/
         done = 0;
         while (!done) {
            St = write(data->FD, data->Buf1, data->Buf1Size);
            _MSG("thr:: St=%d\n", St);
            if (St < 0) {
               if (errno == EINTR) {
                  continue;
               } else {
                  perror("IO_write_chunk");
                  return NULL;
               }
            } else if ((size_t)St < data->Buf1Size) {
               /* Not all data written */
               data->Buf1 = (gchar *)data->Buf1 + St;
               data->Buf1Size -= St;
            } else {
               /* All data in buffer written */
               //sleep(1);
               done = 1;
            }
         }
      }

      /* Buf1 was written, prepare the next step... */
      if (!lock) {
         pthread_mutex_lock(&data->mut);
         lock = 1;
      }
      if (data->Buf2)
         continue;
      else if (data->LastChunk) {
         /* Only pipes are closed, sockets are left for the server to close */
         if (data->CloseFD) {
            do
               st = close(data->FD);
            while (st < 0 && errno == EINTR);
         }
         g_free(data->Buf1Start);
         data->Done = 1;
         pthread_mutex_unlock(&data->mut);
         _MSG("thr:: LastChunk:%d Done:%d  --Bailing out!\n",
              data->LastChunk, data->Done);
         return NULL;
      } else {
         _MSG("thr:: going to cond_wait...\n");
         pthread_cond_wait(&data->cond, &data->mut);
         lock = 1;
      }

   }/* while (1) */
}

/*
 * Write a data chunk from a pthread
 * (!Buf && !BufSize -> LastChunk -> close tansfer, don't close FD)
 * (!Buf && BufSize == 1 -> LastChunk -> close tansfer, close FD)
 */
void a_IO_write_chunk(gint FD, void *Key, void *Buf, size_t BufSize)
{
   thr_data_t *data;
   gint new_thread = 0;
   static pthread_attr_t thrATTR;
   static gint thrATTRInitialized = 0;

   /* set the thread attribute to the detached state */
   if (!thrATTRInitialized) {
      pthread_attr_init(&thrATTR);
      pthread_attr_setdetachstate(&thrATTR, PTHREAD_CREATE_DETACHED);
      thrATTRInitialized = 1;
   }

   /* Search data node for this FD */
   data = IO_thrdata_find(Key);
   _MSG(" a_IO_write_chunk: data=%p Buf=%p, BufSize=%d, FD=%d\n",
        data, Buf, BufSize, FD);

   if (!data && Buf) {
      data = IO_thrdata_new(FD, Key);
      a_List_add(ThrData, ThrDataSize, ThrDataMax);
      ThrData[ThrDataSize] = data;
      ThrDataSize++;
      new_thread = 1;
   }

   pthread_mutex_lock(&data->mut);
    if (Buf) {
       data->Buf2 = g_realloc(data->Buf2, data->Buf2Size + BufSize);
       memcpy((gchar*)data->Buf2 + data->Buf2Size, Buf, BufSize);
       data->Buf2Size = data->Buf2Size + BufSize;
    } else {
       data->LastChunk = 1;
       data->CloseFD = (BufSize == 1) ? 1 : 0;
    }
    pthread_cond_signal(&data->cond);
   pthread_mutex_unlock(&data->mut);

   if (new_thread)
      pthread_create(&data->thrID, &thrATTR, IO_write_chunk, data);
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*
 * Register an IO in ValidIOs
 */
static void IO_ins(IOData_t *io)
{
   io->Key = a_Klist_insert(&ValidIOs, (gpointer)io);
}

/*
 * Remove an IO from ValidIOs
 */
static void IO_del(IOData_t *io)
{
   a_Klist_remove(ValidIOs, io->Key);
}

/*
 * Return a io by its Key (NULL if not found)
 */
static IOData_t *IO_get(gint Key)
{
   return a_Klist_get_data(ValidIOs, Key);
}

/*
 * Free an 'io' struct
 */
static void IO_free(IOData_t *io)
{
   g_return_if_fail(IO_get(io->Key) == NULL);
 
   if (io->Flags & IOFlag_FreeIOBuf)
      a_IO_set_buf(io, NULL, 0);
   g_free(io);
}

/*
 * Close an open FD, and remove io controls.
 * (This function can be used for Close and Abort operations)
 */
static void IO_close_fd(IOData_t *io, gint CloseCode)
{
   gint st;

   /* With HTTP, if we close the writing part, the reading one also gets
    * closed! (other clients may set 'IOFlag_ForceClose') */
   if ((io->Flags & IOFlag_ForceClose) || (CloseCode != IO_StopWr))
      do
         st = close(io->FD);
      while (st < 0 && errno == EINTR);

   /* Remove this IOData_t reference, from our ValidIOs list
    * We don't deallocate it here, just remove from the list.*/
   IO_del(io);

   /* Stop the polling on this FD */
   g_source_remove(io->watch_id);
}

/*
 * Abort an open FD.
 *  This function is called to abort a FD connection due to an IO error
 *  or just because the connection is not required anymore.
 */
static gboolean IO_abort(IOData_t *io)
{
   /* Close and finish this FD's activity */
   IO_close_fd(io, IO_StopRdWr);

   return FALSE;
}

/*
 * Read data from a file descriptor into a specific buffer
 */
static gboolean IO_read(IOData_t *io)
{
   ssize_t St;
   gboolean ret, DataPending;

   DEBUG_MSG(3, "  IO_read\n");

   do {
      ret = FALSE;
      DataPending = FALSE;

      St = read(io->FD, io->Buf, io->BufSize);
      io->Status = St;
      DEBUG_MSG(3, "  IO_read: %s [errno %d] [St %ld]\n",
                g_strerror(errno), errno, (glong)St);

      if ( St < 0 ) {
         /* Error */
         io->Status = -errno;
         if (errno == EINTR)
            continue;
         else if (errno == EAGAIN)
            ret = TRUE;

      } else if ( St == 0 ) {
         /* All data read (EOF) */
         IO_close_fd(io, IO_StopRd);
         a_IO_ccc(OpEnd, 2, FWD, io->Info, io, NULL);

      } else if ( (size_t)St < io->BufSize ){
         /* We have all the new data */
         a_IO_ccc(OpSend, 2, FWD, io->Info, io, NULL);
         ret = TRUE;

      } else { /* BytesRead == io->BufSize */
         /* We have new data, and maybe more... */
         a_IO_ccc(OpSend, 2, FWD, io->Info, io, NULL);
         DataPending = TRUE;
      }
   } while (DataPending);

   return ret;
}

/*
 * Write data, from a specific buffer, into a file descriptor
 * todo: Implement IOWrites.
 */
static gboolean IO_write(IOData_t *io)
{
   ssize_t St;
   gboolean ret, DataPending;

   DEBUG_MSG(3, "  IO_write\n");

   do {
      ret = FALSE;
      DataPending = FALSE;
      St = write(io->FD, io->Buf, io->BufSize);
      io->Status = St;

      DEBUG_MSG(3, "  IO_write: %s [errno %d] [St %ld]\n",
                g_strerror(errno), errno, (glong)St);

      if ( St < 0 ) {
         /* Error */
         io->Status = -errno;
         if (errno == EINTR) {
            continue;
         } else if (errno == EAGAIN) {
            DEBUG_MSG(4, "  IO_write: EAGAIN\n");
            ret = TRUE;
         }
      } else if ( (size_t)St < io->BufSize ){
         /* Not all data written */
         io->BufSize -= St;
         io->Buf = (gchar *)io->Buf + St;
         DataPending = TRUE;
         DEBUG_MSG(4, "  IO_write: Changing io->Buf (%ld)\n", (glong)St);

      } else {
         /* All data in buffer written */
         if ( io->Op == IOWrite ) {
            /* Single write */
            IO_close_fd(io, IO_StopWr);
            a_IO_ccc(OpEnd, 1, FWD, io->Info, io, NULL);
         } else if ( io->Op == IOWrites ) {
            /* Writing in small chunks */
            /* clear the buffer, and wait for a new chunk */
            a_IO_set_buf(io, NULL, 0);
            if (io->Flags & IOFlag_ForceClose) {
               IO_close_fd(io, IO_StopWr);
               a_IO_ccc(OpEnd, 1, FWD, io->Info, io, NULL);
            }
         }
      }
   } while (DataPending);

   return ret;
}

/*
 * Handle background IO for a given FD (reads | writes)
 * (This function gets called by glib when there's activity in the FD)
 */
static gboolean IO_callback(GIOChannel *src, GIOCondition cond, gpointer data)
{
   gboolean ret = FALSE;
   gint io_key = GPOINTER_TO_INT(data);
   IOData_t *io = IO_get(io_key);

   // IO_print_cond_status("IO_callback: ", cond, src, io_key);

   /* There should be no more glib events on already closed FDs  --Jcid */
   if ( io == NULL ) {
      g_warning("IO_callback: call on already closed io!\n");
      g_assert_not_reached();
      return FALSE;
   }

   if ( cond & (G_IO_IN | G_IO_HUP) ){      /* Read */
      ret = IO_read(io);
   } else if ( cond & G_IO_OUT ){           /* Write */
      ret = IO_write(io);
      io = IO_get(io_key);   /* IO_write may have freed 'io' */
      if (io && io->Status == -EAGAIN)
         ret = TRUE;   /* wait for another G_IO_OUT event... */
   }

   if ( cond & G_IO_ERR ){     /* Error */
      /* IO_read/IO_write may free 'io' */
      if ((io = IO_get(io_key))) {
         io->Status = -EIO;
         ret = IO_abort(io);
      } else {
         ret = FALSE;
      }
   } else if ( cond & (G_IO_PRI | G_IO_NVAL) ){
      /* Ignore these exceptional conditions */
      ret = FALSE;
   }

   return ret;
}

/*
 * Receive an IO request (IORead | IOWrite | IOWrites),
 * Set the GIOChannel and let it flow!
 */
static void IO_submit(IOData_t *r_io)
{
   /* Insert this IO in ValidIOs */
   IO_ins(r_io);

   /* Set FD to background and to close on exec. */
   fcntl(r_io->FD, F_SETFL,
         O_NONBLOCK | fcntl(r_io->FD, F_GETFL));
   fcntl(r_io->FD, F_SETFD,
         FD_CLOEXEC | fcntl(r_io->FD, F_GETFD));

   if ( r_io->Op == IORead ) {
      r_io->watch_id =
         g_io_add_watch(r_io->GioCh, G_IO_IN | G_IO_ERR | G_IO_HUP,
                        IO_callback, GINT_TO_POINTER (r_io->Key));
      g_io_channel_unref(r_io->GioCh);

   } else if ( r_io->Op == IOWrite || r_io->Op == IOWrites ) {
      r_io->watch_id =
         g_io_add_watch(r_io->GioCh, G_IO_OUT | G_IO_ERR,
                        IO_callback, GINT_TO_POINTER (r_io->Key));
      g_io_channel_unref(r_io->GioCh);
   }
}

/*
 * Receive IO request (IORead | IOWrite | IOWrites),
 * and either start or keep it flowing.
 */
static void IO_send(IOData_t *io)
{
   if (!io->Key)
      IO_submit(io);
}


/*
 * CCC function for the IO module
 * ( Data1 = IOData_t* ; Data2 = NULL )
 */
void a_IO_ccc(int Op, int Branch, int Dir, ChainLink *Info,
              void *Data1, void *Data2)
{
   IOData_t *io = Data1;

   a_Chain_debug_msg("a_IO_ccc", Op, Branch, Dir);

   if (Branch == 1) {
      if (Dir == BCK) {
         /* Write data */
         switch (Op) {
         case OpStart:
            io->Info = Info;
            Info->LocalKey = io;
            break;
         case OpSend:
            /* this part submits the io */
            IO_send(io);
            break;
         case OpAbort:
            io = Info->LocalKey;
            IO_abort(io);
            IO_free(io);
            g_free(Info);
            break;
         }
      } else {  /* FWD */
         /* Write-data status */
         switch (Op) {
         case OpEnd:
            a_Chain_fcb(OpEnd, Info, io, NULL);
            IO_free(io);
            break;
         case OpAbort:
            a_Chain_fcb(OpAbort, Info, NULL, NULL);
            IO_free(io);
            break;
         }
      }

   } else if (Branch == 2) {
      if (Dir == BCK) {
         /* This part catches the reader's messages */
         switch (Op) {
         case OpStart:
            Info->LocalKey = io;
            io->Info = Info;
            IO_submit(io);
            break;
         case OpAbort:
            io = Info->LocalKey;
            IO_abort(io);
            IO_free(io);
            g_free(Info);
            break;
         }
      } else {  /* FWD */
         /* Send read-data */
         switch (Op) {
         case OpStart:
            io->Info = Info;
            Info->LocalKey = io;
            a_Chain_link_new(Info, a_IO_ccc, FWD, a_Cache_ccc, 2, 2);
            a_Chain_fcb(OpStart, Info, io, io->ExtData);
            IO_submit(io);
            break;
         case OpSend:
            a_Chain_fcb(OpSend, Info, io, NULL);
            break;
         case OpEnd:
            a_Chain_fcb(OpEnd, Info, io, NULL);
            IO_free(io);
            break;
         case OpAbort:
            MSG(" Not implemented\n");
            break;
         }
      }

   } else if (Branch == 3) {
      if (Dir == BCK) {
         /* Write data using a thread */
         switch (Op) {
         case OpStart:
            {
            gint *fd = g_new(gint, 1);
            *fd = *(int*)Data1; /* SockFD */
            Info->LocalKey = fd;
            break;
            }
         case OpEnd:
            _MSG(" Info=%p Info->LocalKey=%p FD=%d Data1=%p\n",
                 Info, Info->LocalKey, *(int*)Info->LocalKey, Data1);
            a_IO_write_chunk(*(int*)Info->LocalKey, Info,
                             NULL, (Data1) ? (size_t)1 : (size_t)0);
            g_free(Info->LocalKey);
            break;
         case OpSend:
            {
            /* this part submits the data to the thread */
            DataBuf *dbuf = Data1;
            a_IO_write_chunk(*(int*)Info->LocalKey, Info,
                             dbuf->Buf, (size_t)dbuf->Size);
            break;
            }
         case OpAbort:
            g_free(Info->LocalKey);
            g_free(Info);
            break;
         }
      } else {  /* FWD */
         /* Write-data status */
         switch (Op) {
         case OpEnd:
            a_Chain_fcb(OpEnd, Info, io, NULL);
            IO_free(io);
            break;
         case OpAbort:
            a_Chain_fcb(OpAbort, Info, NULL, NULL);
            IO_free(io);
            break;
         }
      }
   }
}

