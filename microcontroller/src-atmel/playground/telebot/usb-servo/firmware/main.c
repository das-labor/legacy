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

#include "AVRlib/global.h"		// globals from the AVRlib library
#include "AVRlib/servo.h"              // include RC servo driver library
#include "AVRlib/servoconf.h"
#include "AVRlib/rprintf.h"
#include "AVRlib/timer.h"

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
    DDRC = 0xff;                /* all outputs */
    PORTC = 0x00;

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

    
	// this bit is copy and pasted from AVRlib
	// 
	// do some examples
	// initialize RC servo system
	servoInit();
	// setup servo output channel-to-I/Opin mapping
	// format is servoSetChannelIO( CHANNEL#, PORT, PIN );
	servoSetChannelIO(0, _SFR_IO_ADDR(PORTC), PC0);
	servoSetChannelIO(1, _SFR_IO_ADDR(PORTC), PC1);
	servoSetChannelIO(2, _SFR_IO_ADDR(PORTC), PC2);
	servoSetChannelIO(3, _SFR_IO_ADDR(PORTC), PC3);
        servoSetChannelIO(4, _SFR_IO_ADDR(PORTC), PC4);
        servoSetChannelIO(5, _SFR_IO_ADDR(PORTC), PC5);

	// set port pins to output
        outb(DDRC, 0x0F);
        sbi(DDRB, PB1);
        sbi(DDRB, PB2);

    channel = 0;		
    angle = 100;

    while (1) {                 /* main event loop */
        usbPoll();

        if ((channel >= 0) && (channel < 6)) {
		servoSetPosition(channel,angle);
	}
        else if (channel == 6) {
            if (angle) {
                sbi(PORTD,PD3);
            }
            else {
                cbi(PORTD,PD3);
            }
        }
        else if (channel == 7) {
            if (angle) {
                sbi(PORTD,PD4);
            }
            else {
                cbi(PORTD,PD4);
            }
        }
        else if (channel == 8) {
            if (angle) {
                sbi(PORTD,PD5);
            }
            else {
                cbi(PORTD,PD5);
            }
        }
        else if (channel == 9) {
            if (angle) {
                sbi(PORTD,PD6);
            }
            else {
                cbi(PORTD,PD6);
            }
        }
        else if (channel == 10) {
            if (angle) {
                sbi(PORTD,PD7);
            }
            else {
                cbi(PORTD,PD7);
            }
        }
        else if (channel == 11) {
            if (angle) {
                sbi(PORTB,PB0);
            }
            else {
                cbi(PORTB,PB0);
            }
        }
        else if (channel == 12) {
            if (angle) {
                sbi(PORTB,PB1);
            }
            else {
                cbi(PORTB,PB1);
            }
        }
	else if (channel == 13) {
            if (angle) {
                sbi(PORTB,PB2);
            }
            else {
                cbi(PORTB,PB2);
            }
        }
    }
    return 0;
}
