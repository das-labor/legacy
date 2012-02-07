#include <stdlib.h>
#include <stdint.h>

#pragma once

#ifdef MCU /* if we're compiling for the µC ... */
	#define BUILD_MCU
#endif

#ifdef BUILD_MCU
	#include "../firmware/rfm12_config.h"
	#define RFM12_BUFFER_SIZE RFM12_RX_BUFFER_SIZE
#else
	#define RFM12_BUFFER_SIZE 1024
#endif

#ifndef NL_PROTOCOL_H
#define NL_PROTOCOL_H

/* defines for the nakkaloader protocol
 **/

#define NLPROTO_EEPROM_READ   0x01   /* read an eeprom page from device */
#define NLPROTO_ERROR         0x02
#define NLPROTO_PAGE_CHKSUM   0x03   /* checksum command */
#define NLPROTO_MASTER_EHLO   0x04   /* ehlo response from master/flashing device */
#define NLPROTO_SLAVE_CONFIG  0x05   /* slave sends its configuration */
#define NLPROTO_PAGE_COMMITED 0x06   /* device has written the page */

/* write operations */
#define NLPROTO_EEPROM_WRITE  0x10
#define NLPROTO_PAGE_FILL     0x11
#define NLPROTO_PAGE_COMMIT   0x12   /* checksum correct, go on and flash */
#define NLPROTO_BOOT          0x13   /* boot the application */

#define NLPROTO_SETADDR       0xff

#define NL_PACKETTYPE         0xE0

#pragma pack(push)
#pragma pack(1)
typedef struct
{
	uint8_t pkttype;
	uint8_t payload[RFM12_BUFFER_SIZE-1];
} nl_packet;

#ifdef BUILD_MCU
typedef const struct
#else
typedef struct
#endif
{
	uint16_t pagesize;
	uint8_t rxbufsize;
	uint8_t version;
} nl_config;

typedef struct
{
	uint32_t pagenum;
	uint16_t addr_start;
	uint16_t addr_end;
} nl_flashcmd;
#pragma pack(pop)
/* nakkaloader version */
#define NL_VERSION 0x00

#endif
