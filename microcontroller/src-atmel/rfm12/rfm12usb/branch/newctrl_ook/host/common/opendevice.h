/* Name: opendevice.h
 * Project: AVR-USB host-side library
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-10
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt) or proprietary (CommercialLicense.txt)
 * This Revision: $Id: opendevice.h 553 2008-04-17 19:00:20Z cs $
 */

/*
General Description:
This module offers additional functionality for host side drivers based on
libusb or libusb-win32. It includes a function to find and open a device
based on numeric IDs and textual description. It also includes a function to
obtain textual descriptions from a device.

To use this functionality, simply copy opendevice.c and opendevice.h into your
project and add them to your Makefile. You may modify and redistribute these
files according to the GNU General Public License (GPL) version 2.
*/

#ifndef __OPENDEVICE_H_INCLUDED__
#define __OPENDEVICE_H_INCLUDED__

#include <usb.h>    /* this is libusb, see http://libusb.sourceforge.net/ */
#include <stdio.h>

int usbCountDevices(int vendorID, int productID);
int usbListDevices(struct usb_device **devices, int vendorID, int productID);

int usbOpenDevice(usb_dev_handle **device, int vendorID, int productID);
/* This function iterates over all devices on all USB busses and searches for
 * a device. Matching is done first by means of Vendor- and Product-ID (passed
 * in 'vendorID' and 'productID'. An ID of 0 matches any numeric ID (wildcard).
 * When a device matches by its IDs, matching by names is performed. Name
 * matching can be done on textual vendor name ('vendorNamePattern'), product
 * name ('productNamePattern') and serial number ('serialNamePattern'). A
 * device matches only if all non-null pattern match. If you don't care about
 * a string, pass NULL for the pattern. Patterns are Unix shell style pattern:
 * '*' stands for 0 or more characters, '?' for one single character, a list
 * of characters in square brackets for a single character from the list
 * (dashes are allowed to specify a range) and if the lis of characters begins
 * with a caret ('^'), it matches one character which is NOT in the list.
 * Other parameters to the function: If 'warningsFp' is not NULL, warning
 * messages are printed to this file descriptor with fprintf(). If
 * 'printMatchingDevicesFp' is not NULL, no device is opened but matching
 * devices are printed to the given file descriptor with fprintf().
 * If a device is opened, the resulting USB handle is stored in '*device'. A
 * pointer to a "usb_dev_handle *" type variable must be passed here.
 * Returns: 0 on success, an error code (see defines below) on failure.
 */

/* usbOpenDevice() error codes: */
#define USBOPEN_SUCCESS         0   /* no error */
#define USBOPEN_ERR_ACCESS      1   /* not enough permissions to open device */
#define USBOPEN_ERR_IO          2   /* I/O error */
#define USBOPEN_ERR_NOTFOUND    3   /* device not found */


/* Obdev's free USB IDs, see USBID-License.txt for details */

#define USB_VID_OBDEV_SHARED        5824    /* obdev's shared vendor ID */
#define USB_PID_OBDEV_SHARED_CUSTOM 1500    /* shared PID for custom class devices */
#define USB_PID_OBDEV_SHARED_HID    1503    /* shared PID for HIDs except mice & keyboards */
#define USB_PID_OBDEV_SHARED_CDCACM 1505    /* shared PID for CDC Modem devices */
#define USB_PID_OBDEV_SHARED_MIDI   1508    /* shared PID for MIDI class devices */

#endif /* __OPENDEVICE_H_INCLUDED__ */

