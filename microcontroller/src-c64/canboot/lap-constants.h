
/****************************************************************************
 * Known ports and services
 */

#define PORT_MGT 0x30
#define PORT_LAMPE 0x20
#define PORT_SDO 0x15
#define PORT_SDO_DATA 0x16
#define PORT_BORG 0x23
#define PORT_MOOD 0x17
#define PORT_REMOTE 0x21

#define FKT_MGT_PING 0x00
#define FKT_MGT_PONG 0x01
#define	FKT_MGT_RESET 0x02
#define FKT_MGT_AWAKE 0x03

#define FKT_LAMPE_SET 0x00
#define FKT_LAMPE_SETMASK 0x01
#define FKT_LAMPE_SETDELAY 0x02
#define FKT_LAMPE_ADD 0x03

#define FKT_BORG_INFO 0x00
#define FKT_BORG_MODE 0x01
#define FKT_BORG_SCROLLTEXT_RESET 0x02
#define FKT_BORG_SCROLLTEXT_APPEND 0x03

#define FKT_MOOD_INFO 0x00
#define FKT_MOOD_GET 0x01
#define FKT_MOOD_SET 0x02
#define FKT_MOOD_ONOFF 0x03


#define SDO_CMD_READ 		0x20
#define SDO_CMD_REPLY 		0x21
#define SDO_CMD_INFO 		0x22
#define SDO_CMD_READ_BLK	0x40
#define SDO_CMD_READ_BLK_ACK	0x41
#define SDO_CMD_WRITE_BLK	0x48
#define SDO_CMD_WRITE_BLK_ACK	0x49


#define SDO_CMD_ERROR_INDEX 	0x80

#define SDO_TYPE_UINT8_RO	 0x00
#define SDO_TYPE_UINT8_RW	 0x01
#define SDO_TYPE_UINT16_RO 	 0x04
#define SDO_TYPE_UINT16_RW 	 0x05
#define SDO_TYPE_UINT32_RO 	 0x08
#define SDO_TYPE_UINT32_RW  	 0x09
#define SDO_TYPE_STRING_RO	 0x80
#define SDO_TYPE_STRING_RW	 0x81
#define SDO_TYPE_STRING_WO	 0x82
