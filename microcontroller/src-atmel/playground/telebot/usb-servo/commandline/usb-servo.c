/**
 * \file usb-servo.c
 * \brief Commandline-tool for the USB-Servo.
 * \author Ronald Schaten
 * \version $Id: usb-servo.c,v 1.1 2006/10/28 12:40:42 rschaten Exp $
 *
 * License: See documentation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>                /* this is libusb, see
                                   http://libusb.sourceforge.net/ */

#include "usbdrv.h"

/**
 * Displays usage-information. This function is called if the parameters cannot
 * be parsed.
 * \param name The name of this application.
 */
void usage(char *name) {
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "  %s status\n", name);
    fprintf(stderr, "  %s set <channel> <angle>\n", name);
    fprintf(stderr, "  %s test\n\n", name);
    fprintf(stderr, "parameters:\n");
    fprintf(stderr, "  angle: Required angle for the servo.\n");
}

/**
 * Main function. Initializes the USB-device, parses commandline-parameters and
 * calls the functions that communicate with the device.
 * \param argc Number of arguments.
 * \param argv Arguments.
 * \return Error code.
 */
int main(int argc, char **argv) {
    usb_dev_handle *handle = NULL;

    if (argc < 2) {
        usage(argv[0]);
        exit(1);
    }
    usb_init();
    int n = 0;
    for (n = 1; n < 20 ; n++) {
        if (usbOpenDevice(&handle, USBDEV_SHARED_VENDOR, "mrdunk@gmail.com",
             USBDEV_SHARED_PRODUCT, "USB-Servo-2") != 0) {
            fprintf(stderr,
                "Could not find USB device \"USB-Servo\" with vid=0x%x pid=0x%x on attempt %x. retrying...\n",
                    USBDEV_SHARED_VENDOR, USBDEV_SHARED_PRODUCT, n);
//            exit(1);
        }
        else {
            break;
            }
    }
    if (n >= 20) { exit(1);}
    /* We have searched all devices on all busses for our USB device
       above. Now try to open it and perform the vendor specific control
       operations for the function requested by the user. */
    if (strcmp(argv[1], "test") == 0) {
        if (dev_test(handle, argc, argv) != 0) {
            usage(argv[1]);
        }
    } else if (strcmp(argv[1], "set") == 0) {
        for (n = 1; n < 20 ; n++) {
            int set = dev_set(handle, argc, argv);
            if (set == 1) {
                usage(argv[1]);
            }
            else if (set == 0) {
                int channel = atoi(argv[2]);
                int angle = atoi(argv[3]);
                int status = dev_status(handle, 2, argv);
                if (status == (channel <<8 | angle)){
                    break;
                }
                else {
                    fprintf(stderr, "Servo position does not match command. retrying... : %x\n", n);
                }
            }
            else {
                fprintf(stderr,
                "on attempt %x. retrying....\n", n);
            }
        }
    } else if (strcmp(argv[1], "status") == 0) {
        if (dev_status(handle, argc, argv) == 1) {
            usage(argv[1]);
        }
    } else {
        usage(argv[0]);
        exit(1);
    }
    usb_close(handle);
    return 0;
}
