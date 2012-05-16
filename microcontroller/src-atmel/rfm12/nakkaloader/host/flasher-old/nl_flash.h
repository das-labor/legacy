#ifndef _NL_FLASH_H
#define  _NL_FLASH_H

#include <stdint.h>
#include <stdio.h>

//include rfmusb c driver interface
#include "../../../rfm12usb/trunk/host/CDriver/RfmUsb.h"

//nakkaloader protocol
//#include "../../common/nl_protocol.h"

/* defines for the expect function */
#define EXP_ADD   0x00 /* add an entry to the list */
#define EXP_MATCH 0x01 /* match against current list */
#define EXP_DEL   0x02 /* delete a single entry */

uint_fast8_t nl_packet_match (uint_fast8_t in_len, rfmusb_packetbuffer *in_packet, uint_fast8_t in_function);
int nl_tx_packet(rfmusb_dev_handle *udhandle, uint8_t nl_type, uint8_t addr, uint8_t len, uint8_t * data);
void nl_dump_page(uint8_t *mem, uint32_t pageStart, uint32_t pageEnd);
void nl_push_page(rfmusb_dev_handle *udhandle, uint8_t dst, uint8_t *buf, size_t size, uint32_t, uint8_t rxbufsize);
void nl_flash(rfmusb_dev_handle *udhandle, char * filename, uint8_t addr, uint16_t pageSize, uint8_t pageCount, uint8_t rxbufsize);

#endif
