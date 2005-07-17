#ifndef LAP_H
#define LAP_H

#include "config.h"
#include "can.h"

/**
 * LaborAlthingProtocol
 */

/**
 * "inherits" from can_message 
 */
typedef struct{
	unsigned char flags;
	unsigned char addr_src;
	unsigned char addr_dest;
	unsigned char port_src;
	unsigned char port_dest;
	unsigned char dlc;
	unsigned int  fkt_id; 
	unsigned char data[6];	
}sdo_message;

/**
 * "inherits" from can_message 
 */
typedef struct{
	unsigned char flags;
	unsigned char addr_src;
	unsigned char addr_dest;
	unsigned char port_src;
	unsigned char port_dest;
	unsigned char dlc;
	unsigned int  fkt_id; 
	unsigned char data[6];	
}pdo_message;

/**
 * Known ports 
 */
typedef enum { PORT_MGT=0x30, PORT_LAMPE=0x20 }             ports;
typedef enum { FKT_MGT_PING=0x00, FKT_MGT_RESET=0x01 }      lap_mgt_fkts;

/**
 * ServiceDataObject routinen
 */
unsigned char *sdo_readbuf(sdo_message *first_message, 
		unsigned char length, unsigned char *actuallength);

unsigned char sdo_sendbuf(sdo_message *fst_msg, unsigned char *buf, unsigned char len);
unsigned char sdo_sendbuf_nb(sdo_message *fst_msg, unsigned char *buf, unsigned char len);


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
