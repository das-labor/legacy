#ifndef _LAP_H
#define _LAP_H

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
	can_addr addr_src;
	can_addr addr_dst;
	can_port port_src;
	can_port port_dst;
	uint8_t dlc;
	uint8_t cmd;
	uint16_t index;
	uint16_t size;
	uint16_t address;
} sdo_message;

// "inherits" from can_message
typedef struct {
	can_addr addr_src;
	can_addr addr_dst;
	can_port port_src;
	can_port port_dst;
	uint8_t dlc;
	uint8_t cmd;
	uint8_t data[7];
} pdo_message;

/****************************************************************************
 * Known ports
 */

typedef enum {
	PORT_SDO		= 0x15,
	PORT_SDO_DATA	= 0x16,
	PORT_MOOD		= 0x17,
	PORT_LAPD		= 0x18,
	PORT_LAMPE		= 0x20,
	PORT_REMOTE		= 0x21,
	PORT_GATE		= 0x22,
	PORT_BORG		= 0x23,
	PORT_CHUCK		= 0x26,
	PORT_MGT		= 0x30
} ports;

/****************************************************************************
 * Known services
 */
typedef enum {
	FKT_MGT_PING,
	FKT_MGT_PONG,
	FKT_MGT_RESET,
	FKT_MGT_AWAKE
} lap_mgt_fkts;

typedef enum {
	FKT_LAMPE_SET,
	FKT_LAMPE_SETMASK,
	FKT_LAMPE_SETDELAY,
	FKT_LAMPE_ADD
} lap_lampe_fkts;

typedef enum {
	FKT_BORG_INFO,
	FKT_BORG_MODE,
	FKT_BORG_SCROLLTEXT_RESET,
	FKT_BORG_SCROLLTEXT_APPEND
} lap_borg_fkts;

typedef enum {
	FKT_ONOFF_INFO,
	FKT_ONOFF_SET,
	FKT_ONOFF_GET
} lap_lapd_fkts;

typedef enum {
	FKT_MOOD_INFO,
	FKT_MOOD_GET,
	FKT_MOOD_SET,
	FKT_MOOD_ONOFF
} lap_mood_fkts;

#define SDO_CMD_READ			0x20
#define SDO_CMD_REPLY			0x21
#define SDO_CMD_INFO			0x22
#define SDO_CMD_READ_BLK		0x40
#define SDO_CMD_READ_BLK_ACK	0x41
#define SDO_CMD_WRITE_BLK		0x48
#define SDO_CMD_WRITE_BLK_ACK	0x49
#define SDO_CMD_ERROR_INDEX		0x80

#define SDO_TYPE_UINT8_RO		0x00
#define SDO_TYPE_UINT8_RW		0x01
#define SDO_TYPE_UINT16_RO		0x04
#define SDO_TYPE_UINT16_RW		0x05
#define SDO_TYPE_UINT32_RO		0x08
#define SDO_TYPE_UINT32_RW		0x09
#define SDO_TYPE_STRING_RO		0x80
#define SDO_TYPE_STRING_RW		0x81
#define SDO_TYPE_STRING_WO		0x82


/****************************************************************************
 * STUBS: LAP Core Services 
 */

// send ping to dst
void lap_ping(can_addr dst);

// send reset request to dst
void lap_reset(can_addr dst);

/**
 * ServiceDataObject routinen
 */

unsigned char *sdo_readbuf(sdo_message *first_message, 
		unsigned char length, unsigned char *actuallength);

unsigned char sdo_sendbuf(sdo_message *fst_msg, unsigned char *buf, unsigned char len);
unsigned char sdo_sendbuf_nb(sdo_message *fst_msg, unsigned char *buf, unsigned char len);



#endif // _LAP_H
