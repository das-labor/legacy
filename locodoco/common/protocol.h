/* common definitions for the locodoco protocol. see doc/protocol.txt for description.
 * this file is included by both, hostware and firmware.
 */

#include <stdint.h>

#pragma once

#pragma pack(push)
#pragma pack(1)

/* rfm12 header type - ASCII 'L' */
#define LDC_TYPE 0x4C

/* header from rfm12 driver
 */
typedef struct
{
	uint8_t len;
	uint8_t type;
} rfm12_header_t;

/* upper layer header */
typedef rfm12_header_t ldc_upper_hdr_t;

/* address type */
typedef uint16_t ldc_addr_t;

/* at last ... the locodoco header */
typedef struct
{
	ldc_upper_hdr_t uhdr;
	uint8_t ltype;
	uint16_t chksum;
	ldc_addr_t addr;
} ldc_header_t;

/* waypoint id */
typedef uint16_t ldc_waypoint_t;
/* distance */
typedef uint16_t ldc_dist_t;

#define LDC_WP_INVALID ((ldc_waypoint_t) -1)

/* position report */
typedef struct
{
	ldc_waypoint_t wp;
	ldc_dist_t;
} ldc_position_t;


 /************************************\
<  packet processing helper macros     >
 \************************************/

/* match a given buffer/packet */
#define LDC_MATCH(a) ((((rfm12_header_t*) a)->len >= sizeof(ldc_header_t) && \
		((rfm12_header_t*) a)->type == LDC_TYPE) ? 1 : 0)

//#define LDC_HEADER_SET

/* payload offset */
#define LDC_PAYLOAD(a) ((uint8_t *) ((size_t) a + sizeof(ldc_header_t)))

/* length of remaining packet derived from len field */
#define LDC_LEN_GET(buf) (((ldc_header_t *) buf)->len - sizeof(ldc_header_t))
#define LDC_LEN_SET(buf,l) (((ldc_header_t *) buf)->len = sizeof(ldc_header_t) + l)

/* locodoco type field */
#define LDC_TYPE_GET(buf) (((ldc_header_t *) buf)->ltype)
#define LDC_TYPE_SET(buf,t) (((ldc_header_t *) buf)->ltype = t)


/*** commands */

/* system management */
#define LDC_CMD_STOP              0x00
#define LDC_TIMESLOT_END          0x01
#define LDC_TIMESLOT_RQ           0x02
#define LDC_TIMESLOT_ACK          0x03
#define LDC_CMD_RESUME            0x04
#define LDC_DISASSOC              0x05
#define LDC_CMD_CONFIG_SET        0x06
#define LDC_CMD_CONFIG_GET        0x07
#define LDC_CMD_REBOOT            0x08
#define LDC_CMD_CALIBRATE         0x09

#define LDC_DEBUG                 0x0D
#define LDC_ERROR                 0x0E
#define LDC_ERROR_NOT_IMPL        0x00
#define LDC_ERROR_ARGUMENT        0x01
#define LDC_ERROR_CRC             0x02

/* loco control */
#define LDC_CMD_LIGHT_SET         0x10
#define LDC_CMD_LIGHT_GET         0x20

#define LDC_CMD_SERVO_SET         0x11
#define LDC_CMD_SERVO_GET         0x21

#define LDC_CMD_COUPLER_SET       0x12
#define LDC_CMD_COUPLER_GET       0x22

#define LDC_CMD_PANTO_SET         0x13
#define LDC_CMD_PANTO_GET         0x23

#define LDC_CMD_SMOKE_SET         0x14
#define LDC_CMD_SMOKE_GET         0x24

#define LDC_CMD_SPEED_SET         0x15
#define LDC_CMD_SPEED_GET         0x25

#define LDC_CMD_POS_SET           0x16
#define LDC_CMD_POS_GET           0x26

#define LDC_CMD_TARGET_SPEED_SET  0x17
#define LDC_CMD_TARGET_SPEED_GET  0x27

#define LDC_CMD_DRIVE_TO_SET      0x18
#define LDC_CMD_DRIVE_TO_GET      0x28

#pragma pack(pop)
