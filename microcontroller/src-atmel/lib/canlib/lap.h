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
	PORT_MGT		= 0x30,
	PORT_NETVAR		= 0x37
} lap_ports;

/****************************************************************************
 * Known services
 */
typedef enum {
	FKT_MGT_PING		= 0x00,
	FKT_MGT_PONG		= 0x01,
	FKT_MGT_RESET		= 0x02,
	FKT_MGT_AWAKE		= 0x03
} lap_mgt_fkts;

typedef enum {
	FKT_LAMPE_SET		= 0x00,
	FKT_LAMPE_SETMASK	= 0x01,
	FKT_LAMPE_SETDELAY	= 0x02,
	FKT_LAMPE_ADD		= 0x03
} lap_lampe_fkts;

typedef enum {
	FKT_BORG_INFO				= 0x00,
	FKT_BORG_MODE				= 0x01,
	FKT_BORG_SCROLLTEXT_RESET	= 0x02,
	FKT_BORG_SCROLLTEXT_APPEND	= 0x03
} lap_borg_fkts;

typedef enum {
	FKT_ONOFF_INFO		= 0x00,
	FKT_ONOFF_SET		= 0x01,
	FKT_ONOFF_GET		= 0x02
} lap_lapd_fkts;

typedef enum {
	FKT_MOOD_INFO		= 0x00,
	FKT_MOOD_GET		= 0x01,
	FKT_MOOD_SET		= 0x02,
	FKT_MOOD_ONOFF		= 0x03
} lap_mood_fkts;

enum {
	SDO_CMD_READ			= 0x20,
	SDO_CMD_REPLY			= 0x21,
	SDO_CMD_INFO			= 0x22,
	SDO_CMD_READ_BLK		= 0x40,
	SDO_CMD_READ_BLK_ACK	= 0x41,
	SDO_CMD_WRITE_BLK		= 0x48,
	SDO_CMD_WRITE_BLK_ACK	= 0x49,
	SDO_CMD_ERROR_INDEX		= 0x80
} lap_sdo_cmds;

enum {
	SDO_TYPE_UINT8_RO		= 0x00,
	SDO_TYPE_UINT8_RW		= 0x01,
	SDO_TYPE_UINT16_RO		= 0x04,
	SDO_TYPE_UINT16_RW		= 0x05,
	SDO_TYPE_UINT32_RO		= 0x08,
	SDO_TYPE_UINT32_RW		= 0x09,
	SDO_TYPE_STRING_RO		= 0x80,
	SDO_TYPE_STRING_RW		= 0x81,
	SDO_TYPE_STRING_WO		= 0x82
} lap_sdo_types;


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
