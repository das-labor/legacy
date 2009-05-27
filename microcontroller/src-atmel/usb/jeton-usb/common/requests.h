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

#define I2CUSB_RQ_ECHO          	0x00
/* Request that the device sends back wValue and wIndex. This is used with
 * random data to test the reliability of the communication.
 */

//write card data
#define I2CUSB_RQ_CARD_WRITE		0x01

//read card data
#define I2CUSB_RQ_CARD_READ		0x02

//poll card status
#define I2CUSB_RQ_CARD_STATUS		0x03

//open cash tray
#define I2CUSB_RQ_TRAY_OPEN		0x04

//set scanner power through usb->index
#define I2CUSB_SET_SCANNER_POWER 0x05

#define I2CUSB_NO_CARD 0x80

#endif /* __REQUESTS_H_INCLUDED__ */
