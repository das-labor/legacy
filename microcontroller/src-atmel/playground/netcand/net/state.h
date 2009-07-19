/* vim:fdm=marker ts=4 et ai
 * {{{
 *
 * (c) by Alexander Neumann <alexander@bumpern.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
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

#ifndef CONNECTION_STATE_H
#define CONNECTION_STATE_H

#include "../config.h"

#include "bootp_state.h"


/* uip appstate for tcp */
typedef union uip_tcp_connection_state {

#   ifdef ECMD_SUPPORT
    struct ecmd_connection_state_t ecmd;
#   endif

#   ifdef ECMD_SENDER_SUPPORT
    struct ecmd_sender_connection_state_t ecmd_sender;
#   endif

} uip_tcp_appstate_t;


/* uip appstate for udp */
typedef union uip_udp_connection_state {

#   ifdef BOOTP_SUPPORT
    struct bootp_connection_state_t bootp;
#   endif

} uip_udp_appstate_t;

#endif
