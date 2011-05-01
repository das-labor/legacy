#ifndef ICCOM_H
#define ICCOM_H
#include <inttypes.h>


#define ICC_WAITFORPACKET ((icc_status & 1) == 0)
#define ICC_SETSTATUSRECPACKET (icc_status |=1)
#define ICC_SETSTATUSWAITPACKET (icc_status &= ~1)

#define ICC_HEADER 0xAA
#define MAGIC_PACKET 0xA8

void icc_handle_uart(void);
static uint8_t ICC_getID(void *var);
static uint8_t ICC_send_uint8(uint8_t *c);
static uint8_t ICC_send_uint16(uint16_t *c);
static uint8_t ICC_send_uint32(uint32_t *c);

#define ICC_PACKETSIZE 8
#define ICC_MAX_VARS 8

#endif