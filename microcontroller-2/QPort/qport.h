#ifndef QPORT_H_
#define QPORT_H_
/**
 * \file	qport.c
 * \author	Daniel Otte
 * \date	29.10.2006
 * \par License:
 * GPL
 * \brief security layer for charachter based point-to-point conection based on preshared keys  
 * 
 */

#include <stdint.h>
#include "error-handling.h"
#include "qcrypt.h"
#include "prng.h"
#include "rtc.h"
/*
#define QP_RESET_CODE	0x23
#define QP_ESC_CODE		0x42

#define QP_RESET_ESC	0x01
#define QP_ESC_ESC		0x02

#define QP_TYPE_KEYTX	0x10
#define QP_TYPE_KEYREQ	0x11
#define QP_TYPE_NULL	0x00
*/

extern void (*msgrx_hook) (uint16_t length, uint8_t* buffer);

/********************************************************************************************************/
void qp_byterx(uint8_t b);
/********************************************************************************************************/
void msgrx(uint8_t b);
/********************************************************************************************************/
void msgtx(uint16_t length, uint8_t *msg);
/********************************************************************************************************/
void streamrx(uint8_t b);
/********************************************************************************************************/
//#define SIZEOF_KEYPACKET 176 /* Byte (size of a single block, one is recived per encrytion algo) */
#define RECV_ALGOS 3
void keyingrx(uint8_t b);
/*****************************************************************************/
void make_keypacket(void *buffer);
void rekey(void);



#endif /*QPORT_H_*/
