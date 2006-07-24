#ifdef AMIGA
#ifndef AMIGA_H_INCLUDED
#define AMIGA_H_INCLUDED
#define AMIGA_H_VERSION "$Id: amiga.h,v 1.9 2002/03/26 22:29:54 swa Exp $"
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/Attic/amiga.h,v $
 *
 * Purpose     :  Amiga-specific declarations.
 *
 * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
 *                Privoxy team. http://www.privoxy.org/
 *
 *                This program is free software; you can redistribute it 
 *                and/or modify it under the terms of the GNU General
 *                Public License as published by the Free Software
 *                Foundation; either version 2 of the License, or (at
 *                your option) any later version.
 *
 *                This program is distributed in the hope that it will
 *                be useful, but WITHOUT ANY WARRANTY; without even the
 *                implied warranty of MERCHANTABILITY or FITNESS FOR A
 *                PARTICULAR PURPOSE.  See the GNU General Public
 *                License for more details.
 *
 *                The GNU General Public License should be included with
 *                this file.  If not, you can view it at
 *                http://www.gnu.org/copyleft/gpl.html
 *                or write to the Free Software Foundation, Inc., 59
 *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Revisions   :
 *    $Log: amiga.h,v $
 *    Revision 1.9  2002/03/26 22:29:54  swa
 *    we have a new homepage!
 *
 *    Revision 1.8  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.7  2002/03/03 09:18:03  joergs
 *    Made jumbjuster work on AmigaOS again.
 *
 *    Revision 1.6  2001/10/13 12:46:08  joergs
 *    Added #undef EINTR to avoid warnings
 *
 *    Revision 1.5  2001/07/29 18:43:08  jongfoster
 *    Changing #ifdef _FILENAME_H to FILENAME_H_INCLUDED, to conform to
 *    ANSI C rules.
 *
 *    Revision 1.4  2001/05/29 20:05:06  joergs
 *    Fixed exit() macro not exiting if called before InitAmiga()
 *    (junkbuster --help and --version).
 *
 *    Revision 1.3  2001/05/25 21:53:27  jongfoster
 *    Fixing indentation
 *
 *    Revision 1.2  2001/05/23 00:13:58  joergs
 *    AmigaOS support fixed.
 *
 *    Revision 1.1.1.1  2001/05/15 13:58:46  oes
 *    Initial import of version 2.9.3 source tree
 *
 *
 *********************************************************************/


#define _KERNEL
#include <sys/socket.h>
#undef _KERNEL

#define __NOLIBBASE__
#include <proto/socket.h>
#undef __NOLIBBASE__

#define __CONSTLIBBASEDECL__ const
#define DEVICES_TIMER_H
#include <proto/exec.h>
#include <exec/tasks.h>
#include <proto/dos.h>
#include <dos/dostags.h>

struct UserData
{
   struct Library *sb;
   int eno;
};

#define SocketBase ((struct Library *)(((struct UserData *)(FindTask(NULL)->tc_UserData))->sb))
#define errno (((struct UserData *)(FindTask(NULL)->tc_UserData))->eno)
#define select(a,b,c,d,e) WaitSelect(a,b,c,d,e,NULL)
#define inet_ntoa(x) Inet_NtoA(x.s_addr)

extern int childs;
extern struct Task *main_task;

void InitAmiga(void);
void amiga_exit(void);
void __memCleanUp(void);
__saveds ULONG server_thread(void);

#define exit(x)                                             \
{                                                           \
   if(main_task)                                            \
   {                                                        \
      if(main_task == FindTask(NULL))                       \
      {                                                     \
         while(childs) Delay(10*TICKS_PER_SECOND); exit(x); \
      }                                                     \
      else                                                  \
      {                                                     \
         CloseLibrary(SocketBase);                          \
         childs--;                                          \
         RemTask(NULL);                                     \
      }                                                     \
   }                                                        \
   else                                                     \
   {                                                        \
      exit(x);                                              \
   }                                                        \
}

#undef EINTR
#define EINTR 0

#endif /* ndef AMIGA_H_INCLUDED */
#endif /* def AMIGA */
