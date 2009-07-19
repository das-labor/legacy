/* vim:fdm=marker et ai
 * {{{
 *
 * Copyright (c) 2007,2008 by Stefan Siegl <stesie@brokenpipe.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 }}} */


/*
   Ethersex multi-uIP-stack-support routines 
 ---------------------------------------------
  
   In order to be able to have more than one uIP stack available, we
   need to do some demultiplexing.  On the whole there are some
   variables shared between all stacks and some are separate.

   If multi-stack-support is activated every uip_ symbol is actually
   a pointer to the real variable, function, etc.  In order to achieve
   this, we rename every symbol from uip_ to uip_real_.
   Further stacks have alternate names.
*/

#include "uip.h"

#if UIP_MULTI_STACK

#ifdef IPV6_SUPPORT

#define STACK_FUNCS(stackname)			\
  {						\
    &stackname ## _hostaddr,			\
    &stackname ## _prefix_len,                  \
    &stackname ## _stat,			\
  }

#else /* !IPV6_SUPPORT */

#define STACK_FUNCS(stackname)			\
  {						\
    &stackname ## _hostaddr,			\
    &stackname ## _netmask,			\
    &stackname ## _stat,			\
  }

#endif

struct uip_stack uip_stacks[STACK_LEN] = {
#ifdef ENC28J60_SUPPORT
  STACK_FUNCS (enc_stack),
#endif

#if defined(RFM12_SUPPORT)
  STACK_FUNCS (rfm12_stack),
#endif

#if defined(USB_NET_SUPPORT)
  STACK_FUNCS (usb_stack),
#endif

#if defined(ZBUS_SUPPORT)
  STACK_FUNCS (zbus_stack),
#endif
};

struct uip_stack *uip_stack = &uip_stacks[0];

#ifdef ENC28J60_SUPPORT
STACK_DEFINITIONS(enc_stack);
#endif

#endif /* UIP_MULTI_STACK */
