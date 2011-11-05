#ifndef _LCAP_H
#define _LCAP_H

/****************************************************************************
 * Labor CAN Automation Protocol
 *
 * This header defines the (logical) LCAP frame format. For a full
 * description of this protocol and the wire format visit
 * http://www.das-labor.org/wiki/LCAP
 *
 * Author: Hansinator (sexyludernatascha [at] gmail.com)
 * License: GPLv2+
 * Version: 0
 */

#include <inttypes.h>

/*
 * This is the request ack bit in the flags field of a device addressed
 * LCAP message.
 */
#define LCAP_RACK_bm 0x01

/*
 * This is the long packet bit in the flags field of a device addressed
 * LCAP message.
 */
#define LCAP_LONG_bm 0x02

/*
 * This is the request bit in lcap_type_t. If set, the message is a
 * request. You may either check for this flag or use the values of
 * lcap_type_t to distinguish data and request messages.
 */
#define LCAP_REQ_bm 0x01

/*
 * This is the device address mode bit in lcap_type_t. If set, the
 * message is addressed to a device. You may either check for this flag
 * or use the values of lcap_type_t to distinguish data and request
 * messages.
 */
#define LCAP_DEV_bm 0x02

/*
 * This represents the message type and adressing mode. There are data
 * and request messages to either devices or topics.
 */
typedef enum { dst_topic = 0x0, dst_device = 0x1, req_topic = 0x2, req_device = 0x3 }  lcap_type_t;

/*
 * This is the logical view of an LCAP message. The field dst_type
 * determines the message type and addressing mode. The fields dst_addr
 * and flags are valid in device addressing mode only, while dst_topic
 * is valid in topic addressing mode. If len is zero, the data pointer
 * is invalid.
 */
typedef struct {
	lcap_type_t dst_type;
	union { struct { uint8_t dst_addr, flags; }; uint16_t dst_topic; };
	uint8_t src_addr;
	uint8_t sub_addr;
	uint8_t len;
	uint8_t *data[];
} lcap_msg_t;

#endif // _LCAP_H
