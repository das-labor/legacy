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
	//put data into the rfm12 transmit buffer
	RFMUSB_RQ_RADIO_PUT	= 0x00,
	//poll radio packets
	RFMUSB_RQ_RADIO_GET,
	//poll notifications
	RFMUSB_RQ_NOTIFY_GET,
	//configure radio interface
	RFMUSB_RQ_RADIO_CFG
} rfmusb_requests;

#endif /* __REQUESTS_H_INCLUDED__ */
