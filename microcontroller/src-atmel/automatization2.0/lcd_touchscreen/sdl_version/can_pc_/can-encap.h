#ifndef CANENCAP_H
#define CANENCAP_H

/*****************************************************************************
 * Encapsulate CAN packets for serial transport
 *
 * Use with UART, TCP/IP or alike.
 */

#define CAN_RAW

#include "can.h"

#define RS232CAN_MAXLENGTH 20

typedef enum
{
	RS232CAN_RESET=0x00,
	RS232CAN_SETFILTER=0x10,
	RS232CAN_PKT=0x11,
	RS232CAN_SETMODE=0x12,
	RS232CAN_ERROR=0x13,
	RS232CAN_NOTIFY_RESET=0x14,
	RS232CAN_PING_GATEWAY=0x15,
	RS232CAN_RESYNC=0x16,
	RS232CAN_VERSION=0x17,
	RS232CAN_IDSTRING=0x18,
	RS232CAN_PACKETCOUNTERS=0x19,
	RS232CAN_ERRORCOUNTERS=0x1A,
	RS232CAN_POWERDRAW=0x1B,
	RS232CAN_READ_CTRL_REG=0x1C,
	RS232CAN_WRITE_CTRL_REG=0x1D,
	RS232CAN_GET_RESETCAUSE=0x1E,
	RS232CAN_NOTIFY_TX_OVF=0x1F
} rs232can_cmd;


typedef struct {
	unsigned char cmd;
	unsigned char len;
	char data[RS232CAN_MAXLENGTH];
} rs232can_msg;


//void rs232can_setmode(can_mode_t mode);
void rs232can_rs2can(can_message_raw *cmsg, rs232can_msg *rmsg);
void rs232can_can2rs(rs232can_msg *rmsg, can_message_raw *cmsg);

#endif
