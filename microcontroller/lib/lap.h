#ifndef LAP_H
#define LAP_H

/****************************************************************************
 * Labor Automation Protocol
 *
 */

#include "config.h"
#include "can.h"


/****************************************************************************
 * Types
 */

// "inherits" from can_message 
typedef struct {
	can_addr addr_src;
	can_addr addr_dst;
	can_port port_src;
	can_port port_dst;
	unsigned char dlc;
	unsigned char fkt_id; 
	unsigned char data[7];	
} sdo_message;

// "inherits" from can_message 
typedef struct{
	can_addr addr_src;
	can_addr addr_dst;
	can_port port_src;
	can_port port_dst;
	unsigned char dlc;
	unsigned char  fkt_id; 
	char data[7];	
}pdo_message;

/****************************************************************************
 * Known ports and services
 */

typedef enum { PORT_MGT=0x30, PORT_LAMPE=0x20 }             	ports;

typedef enum { FKT_MGT_PING=0x00, FKT_MGT_ALIVE=0x01, 
		FKT_MGT_RESET=0x02 }				lap_mgt_fkts;
typedef enum { FKT_LAMPE_SET=0x00, FKT_LAMPE_SETMASK=0x01, 
		FKT_LAMPE_SETDELAY=0x02 }			lap_lampe_fkts;


/****************************************************************************
 * STUBS: LAP Core Services 
 */

// send ping to dst
void lap_ping( can_addr dst );

// send reset request to dst
void lap_reset( can_addr dst );

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
