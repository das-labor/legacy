/**
 * \file usbdrv.h
 * \brief USB-driver-parts for implementing a client.
 * \author Ronald Schaten
 * \version $Id: usbdrv.h,v 1.1 2006/10/28 12:40:42 rschaten Exp $
 *
 * License: See documentation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>                /* this is libusb, see http://libusb.sourceforge.net/ */

#include "usbservo.h"

#define USBDEV_SHARED_VENDOR    0x16C0  /**< VOTI */
#define USBDEV_SHARED_PRODUCT   0x05DC  /**< Obdev's free shared PID. Use obdev's generic shared VID/PID pair and follow the rules outlined in firmware/usbdrv/USBID-License.txt. */

/* These are error codes for the communication via USB. */
#define USB_ERROR_NOTFOUND  1 /**< Error code if the device isn't found. */
#define USB_ERROR_ACCESS    2 /**< Error code if the device isn't accessible. */
#define USB_ERROR_IO        3 /**< Error code if errors in the communication with the device occur. */

/**
 * Reads and converts a string from USB. The conversion to ASCII is 'lossy' (unknown characters become '?').
 * \param dev Handle of the USB-Device.
 * \param index Index of the required data.
 * \param langid Index of the expected language.
 * \param buf Buffer to contain the return-string.
 * \param buflen Length of buf.
 * \return Length of the string.
 */
int usbGetStringAscii(usb_dev_handle * dev, int index, int langid, char *buf, int buflen);

/**
 * Connect to the USB-device. Loops through all connected USB-Devices and
 * searches our counterpart.
 * \param device Handle to address the device.
 * \param vendor USBDEV_SHARED_VENDOR as defined.
 * \param vendorName In our case "www.schatenseite.de".
 * \param product USBDEV_SHARED_PRODUCT as defined.
 * \param productName In our case "USB-Servo".
 * \return Error code.
 */
int usbOpenDevice(usb_dev_handle ** device, int vendor, char *vendorName, int product, char *productName);

/**
 * Test connection to the device. The test consists of writing 1000 random
 * numbers to the device and checking the echo. This should discover systematic
 * bit errors (e.g. in bit stuffing).
 * \param handle Handle to talk to the device.
 * \param argc Number of arguments.
 * \param argv Arguments.
 */
int dev_test(usb_dev_handle * handle, int argc, char **argv);

/**
 * Set the angle of the Servo.
 * \param handle Handle to talk to the device.
 * \param argc Number of arguments.
 * \param argv Arguments.
 */
int dev_set(usb_dev_handle * handle, int argc, char **argv);

/**
 * Get the status of the device. Status information is printed in detail (we
 * dont't have too many details with only one servo).
 * \param handle Handle to talk to the device.
 * \param argc Number of arguments.
 * \param argv Arguments.
 */
int dev_status(usb_dev_handle * handle, int argc, char **argv);

