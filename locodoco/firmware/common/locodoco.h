#pragma once

/* type bytes (locodoco protocol prefix)
 */
typedef uint16_t locodoco_addr_t;

#define LPROTO_T0 0x10
#define LPROTO_T1 0xd0

/* minimum packet len
 */
#define LPROTO_MINLEN (2+sizeof(locodoco_addr_t))

/* macros for getting the protocol fields out of a given buffer
 */
#define LPROTO_PREFIX(a) (a[0])
#define LPROTO_TYPE(a) (a[1])
#define LPROTO_ADDR(a) (a[2])
#define LPROTO_DATA(a) (a[2+sizeof(locodoco_addr_t)])

/****  packet types ******************************************/

/* ping & pong */
#define LPROTO_TYPE_PING  0x00
#define LPROTO_TYPE_PONG  0x01
/* reset the controller */
#define LPROTO_TYPE_RESET 0xA8

/* set the pwm (actual) value - this should NOT be used
 * to set the speed */
#define LPROTO_PWM_SET    0x02

/* set the speed. unit: mm per second */
#define LPROTO_SPEED_SET  0x03

/* init the calibration routine */
#define LPROTO_SPEED_CAL  0x04
