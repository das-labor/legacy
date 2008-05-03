/**
 * \file main.c
 * \brief Firmware for the USB-Servo-2
 * \author Ronald Schaten
 * \updated Duncan Law
 * \version $Id: main.c,v 2.1 2007/02/21 23:40:42 rschaten Exp $
 *
 * License: See documentation.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "usbdrv/usbdrv.h"
#include "usbdrv/oddebug.h"
#include "../common/usbservo.h"

/** Global variable, contains the angle of the servo. */
//static uint8_t channel;
static int angle;
static int channel;

/**
 * USB-Setup-Handler. Handles setup-calls that are received from the
 * USB-Interface.
 * \param data Eight bytes of data.
 * \return The number of returned bytes (in replyBuffer[]).
 */
uchar usbFunctionSetup(uchar data[8]) {
    static uchar replyBuffer[8];
    uchar replyLength;

    replyBuffer[0] = msgOK;
    switch (data[1]) {
    case CMD_ECHO:             /* echo */
        replyBuffer[0] = data[2];
        replyBuffer[1] = data[3];
        replyLength = 2;
        break;
    case CMD_GET:              /* read status */
		replyBuffer[0] = channel;
		replyBuffer[1] = angle;
        replyLength = 2;
        break;
    case CMD_SET:              /* set status */
		channel = data[3];	     
		angle = data[2];
		replyBuffer[0] = data[2];	/*debug*/
        replyBuffer[1] = data[3];
		replyLength = 2;
        break;
    default:                   /* WTF? */
        replyBuffer[0] = msgErr;
        replyLength = 1;
        break;
    }
    usbMsgPtr = replyBuffer;
    return replyLength;
}

/**
 * Main-function. Initializes the hardware and starts the main loop of the
 * application.
 * \return An integer. Whatever... :-)
 */
int main(void) {
    uchar i, j;
    odDebugInit();
    DDRD = ~0;                  /* output SE0 for USB reset */
    PORTD = 0x00;               /* no pullups on USB pins */
    
	j = 0;
    while (--j) {               /* USB Reset by device only required on
                                   Watchdog Reset */
		i = 0;
    	while (--i);            /* delay >10ms for USB reset */
    }

    DDRD = ~USBMASK;            /* all outputs except USB data */
    TCCR0 = 5;                  /* set prescaler to 1/1024 */
    usbInit();
    sei();

    for (i = 0; i < 10; i++) {  /* wait one second to prevent strange
                                   effects when the USB-bus isn't
                                   initialized (e. g. when the host system
                                   is on standby. */
        _delay_ms(100);
    }

	// set port pins to output
	DDRD |= 0x60;

    channel = 0;		
    angle = 100;

    while (1) {                 /* main event loop */
        usbPoll();

		if ((channel >= 0) && (channel < 2)) {
			servoSetPosition(channel,angle);
		}
    }
    return 0;
}


pwm_init(){
	


}



