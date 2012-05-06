#ifndef LAP_H
#define LAP_H

/****************************************************************************
 * Labor Automation Protocol
 *
 */

#include <inttypes.h>


/****************************************************************************
 * Types
 */

// "inherits" from can_message
typedef struct {
	can_addr_t addr_src;
	can_addr_t addr_dst;
	can_port_t port_src;
	can_port_t port_dst;
	unsigned char dlc;
	unsigned char cmd;
	uint16_t index;
	uint16_t size;
	uint16_t address;
} sdo_message;

// "inherits" from can_message
typedef struct {
	can_addr_t addr_src;
	can_addr_t addr_dst;
	can_port_t port_src;
	can_port_t port_dst;
	unsigned char dlc;
	unsigned char cmd;
	unsigned char data[7];
} pdo_message;

/****************************************************************************
 * Known ports and services
 */

typedef enum {
	PORT_MGT      = 0x30,
	PORT_LAMPE    = 0x20,
	PORT_SDO      = 0x15,
	PORT_SDO_DATA = 0x16,
	PORT_LAPD     = 0x18,
	PORT_BORG     = 0x23,
	PORT_MOOD     = 0x17,
	PORT_REMOTE   = 0x21,
	PORT_GATE     = 0x22,
	PORT_CHUCK    = 0x26
} ports;

typedef enum {
	FKT_MGT_PING        = 0x00,
	FKT_MGT_PONG        = 0x01,
	FKT_MGT_RESET       = 0x02,
	FKT_MGT_AWAKE       = 0x03,
	FKT_MGT_TIMEREQUEST = 0x04,
	FKT_MGT_TIMEREPLY   = 0x05
} lap_mgt_fkts;

typedef enum {
	FKT_LAMPE_SET      = 0x00,
	FKT_LAMPE_SETMASK  = 0x01,
	FKT_LAMPE_SETDELAY = 0x02,
	FKT_LAMPE_ADD      = 0x03
} lap_lampe_fkts;

typedef enum {
	FKT_BORG_INFO              = 0x00,
	FKT_BORG_MODE              = 0x01,
	FKT_BORG_SCROLLTEXT_RESET  = 0x02,
	FKT_BORG_SCROLLTEXT_APPEND = 0x03
} lap_borg_fkts;

typedef enum {
	FKT_ONOFF_INFO = 0,
	FKT_ONOFF_SET  = 1,
	FKT_ONOFF_GET  = 2
} lap_lapd_fkts;

typedef enum {
	FKT_MOOD_INFO  = 0x00,
	FKT_MOOD_GET   = 0x01,
	FKT_MOOD_SET   = 0x02,
	FKT_MOOD_ONOFF = 0x03
} lap_mood_fkts;

#define SDO_CMD_READ          0x20
#define SDO_CMD_REPLY         0x21
#define SDO_CMD_INFO          0x22
#define SDO_CMD_READ_BLK      0x40
#define SDO_CMD_READ_BLK_ACK  0x41
#define SDO_CMD_WRITE_BLK     0x48
#define SDO_CMD_WRITE_BLK_ACK 0x49

#define SDO_CMD_ERROR_INDEX   0x80

#define SDO_TYPE_UINT8_RO     0x00
#define SDO_TYPE_UINT8_RW     0x01
#define SDO_TYPE_UINT16_RO    0x04
#define SDO_TYPE_UINT16_RW    0x05
#define SDO_TYPE_UINT32_RO    0x08
#define SDO_TYPE_UINT32_RW    0x09
#define SDO_TYPE_STRING_RO    0x80
#define SDO_TYPE_STRING_RW    0x81
#define SDO_TYPE_STRING_WO    0x82


/****************************************************************************
 * STUBS: LAP Core Services
 */

// send ping to dst
void lap_ping(can_addr_t dst);

// send reset request to dst
void lap_reset(can_addr_t dst);

#ifdef LAP_TIME_EXTENSION
//variables to save the last received hours and  minutes
extern uint8_t lap_time_h, lap_time_m, lap_time_update;
#endif

/**
 * ServiceDataObject routinen
 *
unsigned char *sdo_readbuf(sdo_message *first_message,
		unsigned char length, unsigned char *actuallength);

unsigned char sdo_sendbuf(sdo_message *fst_msg, unsigned char *buf, unsigned char len);
unsigned char sdo_sendbuf_nb(sdo_message *fst_msg, unsigned char *buf, unsigned char len);

*/
/////////////////////////////////////////////////////////////////////////////
/* Usage

while(1) {
	lap_message msg = lap_rcvpacket();
	switch( msg->fkt_id ) {
	case FKT_BLA:
		unsigned char length = data[0]

		data

		char *buf = lap_read(msg, length);
		if ( !buf ) continue;

		// interpret buffer
}
*/

#endif
