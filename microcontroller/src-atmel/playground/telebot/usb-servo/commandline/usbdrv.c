/**
 * \file usbdrv.c
 * \brief USB-driver-parts for implementing a client.
 * \author Ronald Schaten
 * \version $Id: usbdrv.c,v 1.1 2006/10/28 12:40:42 rschaten Exp $
 *
 * License: See documentation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>                /* this is libusb, see
                                   http://libusb.sourceforge.net/ */

#include "usbdrv.h"
#include "usbservo.h"

int usbGetStringAscii(usb_dev_handle * dev, int index, int langid,
                      char *buf, int buflen) {
    char buffer[256];
    int rval, i;

    if ((rval = usb_control_msg(dev, USB_ENDPOINT_IN, USB_REQ_GET_DESCRIPTOR,
                         (USB_DT_STRING << 8) + index, langid, buffer,
                         sizeof(buffer), 1000)) < 0) {
        return rval;
    }
    if (buffer[1] != USB_DT_STRING) {
        return 0;
    }
    if ((unsigned char)buffer[0] < rval) {
        rval = (unsigned char)buffer[0];
    }
    rval /= 2;
    /* lossy conversion to ISO Latin1 */
    for (i = 1; i < rval; i++) {
        if (i > buflen) {
            /* destination buffer overflow */
            break;
        }
        buf[i - 1] = buffer[2 * i];
        if (buffer[2 * i + 1] != 0) {
            /* outside of ISO Latin1 range */
            buf[i - 1] = '?';
        }
    }
    buf[i - 1] = 0;
    return i - 1;
}

int usbOpenDevice(usb_dev_handle ** device, int vendor, char *vendorName,
                  int product, char *productName) {
    struct usb_bus *bus;
    struct usb_device *dev;
    usb_dev_handle *handle = NULL;
    int errorCode = USB_ERROR_NOTFOUND;
    static int didUsbInit = 0;

    if (!didUsbInit) {
        didUsbInit = 1;
        usb_init();
    }
    usb_find_busses();
    usb_find_devices();
    for (bus = usb_get_busses(); bus; bus = bus->next) {
        for (dev = bus->devices; dev; dev = dev->next) {
            if (dev->descriptor.idVendor == vendor
                && dev->descriptor.idProduct == product) {
                char string[256];
                int len;
                handle = usb_open(dev); /* we need to open the device in
                                           order to query strings */
                if (!handle) {
                    errorCode = USB_ERROR_ACCESS;
                    fprintf(stderr,
                            "Warning: cannot open USB device: %s\n",
                            usb_strerror());
                    continue;
                }
                if (vendorName == NULL && productName == NULL) {
                    /* name does not matter */
                    break;
                }
                /* now check whether the names match: */
                len = usbGetStringAscii(handle,
                                      dev->descriptor.iManufacturer,
                                      0x0409, string, sizeof(string));
                if (len < 0) {
                    errorCode = USB_ERROR_IO;
                    fprintf(stderr,
                            "Warning: cannot query manufacturer for device: %s\n",
                            usb_strerror());
                } else {
                    errorCode = USB_ERROR_NOTFOUND;
                    if (strcmp(string, vendorName) == 0) {
                        len = usbGetStringAscii(handle,
                                              dev->descriptor.iProduct,
                                              0x0409, string,
                                              sizeof(string));
                        if (len < 0) {
                            errorCode = USB_ERROR_IO;
                            fprintf(stderr,
                                    "Warning: cannot query product for device: %s\n",
                                    usb_strerror());
                        } else {
                            errorCode = USB_ERROR_NOTFOUND;
                            if (strcmp(string, productName) == 0) {
                                break;
                            }
                        }
                    }
                }
                usb_close(handle);
                handle = NULL;
            }
        }
        if (handle) {
            break;
        }
    }
    if (handle != NULL) {
        errorCode = 0;
        *device = handle;
    }
    return errorCode;
}

int dev_test(usb_dev_handle * handle, int argc, char **argv) {
    unsigned char buffer[8];
    int nBytes;
    int i, v, r;
    if (argc != 2) {
        return 1;
    }
    for (i = 0; i < 1000; i++) {
        v = rand() & 0xffff;
        nBytes = usb_control_msg(handle,
                            USB_TYPE_VENDOR | USB_RECIP_DEVICE |
                            USB_ENDPOINT_IN, CMD_ECHO, v, 0,
                            (char *)buffer, sizeof(buffer), 5000);
        if (nBytes < 2) {
            if (nBytes < 0) {
                fprintf(stderr, "USB error: %s\n", usb_strerror());
            }
            fprintf(stderr, "only %d bytes received in iteration %d\n",
                    nBytes, i);
            exit(1);
        }
        r = buffer[0] | (buffer[1] << 8);
        if (r != v) {
            fprintf(stderr,
                    "data error: received 0x%x instead of 0x%x in iteration %d\n",
                    r, v, i);
            exit(1);
        }
    }
    printf("test succeeded\n");
    return 0;
}

int dev_set(usb_dev_handle * handle, int argc, char **argv) {
    unsigned char buffer[8];
    int nBytes;
    if (argc != 4) {
        return 1;
    }
    int channel = atoi(argv[2]);
    int angle = atoi(argv[3]);
    if ((channel < 0) || (channel > 13)) {
            fprintf(stderr, "invalid channel: %d\n", channel);
            exit(1);
    }
    if ((angle < 0) || (angle > 255)) {
        fprintf(stderr, "invalid angle: %d\n", angle);
        exit(1);
    }
    int mes_out;
    mes_out = (channel<<8) | angle;
    nBytes = usb_control_msg(handle,
                        USB_TYPE_VENDOR | USB_RECIP_DEVICE |
                        USB_ENDPOINT_OUT, CMD_SET, mes_out, 0,
                        (char *)buffer, sizeof(buffer), 5000);

    if (nBytes < 0) {
        fprintf(stderr, "USB error: %s\n", usb_strerror());
        return 2;
    }
    return 0;
}

int dev_status(usb_dev_handle * handle, int argc, char **argv) {
    int nBytes;
    unsigned char buffer[8];
    if (argc != 2) {
        return 1;
    }
    nBytes = usb_control_msg(handle,
                        USB_TYPE_VENDOR | USB_RECIP_DEVICE |
                        USB_ENDPOINT_IN, CMD_GET, 0, 0, (char *)buffer,
                        sizeof(buffer), 5000);
    if (nBytes < 0) {
        fprintf(stderr, "USB error: %s\n", usb_strerror());
//        exit(1);
        return ((255<<8) | 255);
    }
    if (nBytes < 2) {
        fprintf(stderr, "only %d bytes status received\n", nBytes);
//        exit(1);
        return ((255<<8) | 255);
    }
    printf("Current servo channel: %d\n", buffer[0]);
    printf("Current servo angle:   %d\n", buffer[1]);
    return ((buffer[0]<<8) | buffer[1]);
}

