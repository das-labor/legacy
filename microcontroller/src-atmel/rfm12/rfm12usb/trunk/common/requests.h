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

#define RFMUSB_RQ_ECHO          0x00
/* Request that the device sends back wValue and wIndex. This is used with
 * random data to test the reliability of the communication.
 */

//put data into the rfm12 transmit buffer
#define RFMUSB_RQ_RFM12_PUT			0x01

//poll for rfm12 received data
#define RFMUSB_RQ_RFM12_GET			0x02

//poll for notifications
#define RFMUSB_RQ_GETNOTIFICATION	0x03


#endif /* __REQUESTS_H_INCLUDED__ */
