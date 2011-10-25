
#pragma once

/* type bytes (locodoco protocol prefix)
 */

#define LPROTO_T0 0x10
#define LPROTO_T1 0xd0

/* minimum packet len
 */
#define LPROTO_MINLEN 3

/* macros for getting the type fields out of a buffer
 */
#define LPROTO_GET_TYPE(a) (a[1])
#define LPROTO_GET_PREFIX(a) (a[0])

/* packet types
 */
#define LPROTO_TYPE_PING  0x00
#define LPROTO_TYPE_PONG  0x01
#define LPROTO_TYPE_RESET 0xA8
