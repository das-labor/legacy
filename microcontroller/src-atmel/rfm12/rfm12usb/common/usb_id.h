/* This header is shared between the firmware and the host software. It
 * defines the USB vid/pid numbers
 */

#ifndef __USB_ID_H_INCLUDED__
#define __USB_ID_H_INCLUDED__

#define  USB_CFG_VENDOR_ID       0xc0, 0x16
/* USB vendor ID for the device, low byte first. If you have registered your
 * own Vendor ID, define it here. Otherwise you use one of obdev's free shared
 * VID/PID pairs. Be sure to read USBID-License.txt for rules!
 */
#define  USB_CFG_DEVICE_ID       0xdc, 0x05
/* This is the ID of the product, low byte first. It is interpreted in the
 * scope of the vendor ID. If you have registered your own VID with usb.org
 * or if you have licensed a PID from somebody else, define it here. Otherwise
 * you use obdev's free shared VID/PID pair. Be sure to read the rules in
 * USBID-License.txt!
 */

#endif
