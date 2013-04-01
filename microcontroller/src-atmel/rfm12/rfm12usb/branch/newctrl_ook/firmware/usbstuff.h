#include <avr/io.h>
#include <avr/wdt.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#pragma once

#include "rfm12.h"
#include "../common/rfm12usb_config.h"
#include "../common/requests.h"
#include "vusb/usbdrv.h"
#include "usbdebug.h"

/* modes for the internal state machine
 */
#define MODE_IDLE 0
#define MODE_RX   1
#define MODE_TX   2

/* states of the usb rx/tx buffer */
#define BUFSTATE_IDLE 0  /* buffer may be used */
#define BUFSTATE_RX   1  /* buffer contains received data from the radio interface */
#define BUFSTATE_TX   2  /* buffer contains data that is to be sent through the radio interface */

void usbstuff_init ();
void handle_rx ();
