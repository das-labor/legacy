/* Name: requests.h
 * Project: custom-class, a basic USB example
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-09
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt) or proprietary (CommercialLicense.txt)
 * This Revision: $Id: requests.h 602 2008-05-13 15:15:13Z cs $
 */

/* This header is shared between the firmware and the host software. It
 * defines the USB request numbers (and optionally data types) used to
 * communicate between the host and the device.
 */

#ifndef __REQUESTS_H_INCLUDED__
#define __REQUESTS_H_INCLUDED__

/*
 * USB Request Types 
 */
typedef enum
{
	/* anchient request; not used; just there to retain compatibility */
	RFMUSB_RQ_ECHO          = 0x00,
	//put data into the rfm12 transmit buffer
	RFMUSB_RQ_RADIO_PUT	= 0x01,
	//poll radio packets
	RFMUSB_RQ_RADIO_GET     = 0x02,
	//poll notifications
	RFMUSB_RQ_NOTIFY_GET    = 0x03,
	//configure radio interface
	RFMUSB_RQ_RADIO_CFG     = 0x04,
	// send ook message
	RFMUSB_RQ_OOK_SEND      = 0x05
} rfmusb_requests;

#pragma pack(push)
#pragma pack(1)

#define OOK_RAW  0x00 /* send given bits as they are */
#define OOK_2722 0x01 /* encode 1 to 110, 0 to 100, add trailing bit, add trailing delay */

/* ook metadata header */
typedef struct
{
	uint8_t  len;   /* length in BITS */
	uint8_t  type;  /* see above for possible types */
	uint16_t delay; /* delay between bits */
	uint8_t  count; /* repeat message count times */
} rfmusb_ook_t;
#pragma pack(pop)

#endif /* __REQUESTS_H_INCLUDED__ */
