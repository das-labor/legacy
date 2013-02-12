#include <avr/boot.h>

extern uint8_t Station_id;



#define PORT_MGT 0x30
#define FKT_MGT_AWAKE 0x03

#define SDO_CMD_READ 		0x20
#define SDO_CMD_REPLY 		0x21
#define SDO_CMD_INFO 		0x22
#define SDO_CMD_READ_BLK	0x40
#define SDO_CMD_READ_BLK_ACK	0x41
#define SDO_CMD_WRITE_BLK	0x48
#define SDO_CMD_WRITE_BLK_ACK	0x49


#define SDO_CMD_ERROR_INDEX 	0x80

#define SDO_TYPE_UINT8_RO 0x00
#define SDO_TYPE_UINT8_RW 0x01
#define SDO_TYPE_UINT16_RO 0x04
#define SDO_TYPE_UINT16_RW 0x05
#define SDO_TYPE_UINT32_RO 0x08
#define SDO_TYPE_UINT32_RW 0x09
#define SDO_TYPE_STRING_RO 0x80
#define SDO_TYPE_STRING_RW 0x81
#define SDO_TYPE_STRING_WO 0x82

void bootloader(void) BOOTLOADER_SECTION __attribute__ ((naked));

//user can define own hardware setup function to be run on start
void user_init() BOOTLOADER_SECTION;

//user can do things like displaying status on bootloader entry
void user_bootloader_entry(void) BOOTLOADER_SECTION;

