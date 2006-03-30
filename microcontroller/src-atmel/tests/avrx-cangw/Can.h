#ifndef CAN_H
#define CAN_H

#include "config.h"

/*****************************************************************************
 * Simple CAN Library
 *
 */

/*****************************************************************************
 * Types
 */

typedef uint32_t CanId;

typedef struct{
	CanId		id;
	uint8_t		dlc;
	uint8_t		data[8];
}CanMessage;

typedef enum { normal, mode_sleep, loopback, listenonly, config } can_mode_t;


/*****************************************************************************
 * Management
 */

void CanInit();
void CanSetfilter();
void CanSetMode(can_mode_t);
void CanSetLED(unsigned char led, unsigned char state);

/*****************************************************************************
 * Global Message Buffer
 */

// extern can_message_t rx_msg;

/*****************************************************************************
 * Receiving
 */

void CanGet(CanMessage *msg);
char CanGetNB(CanMessage *msg);

/*****************************************************************************
 * Sending
 */

void CanSend(CanMessage *msg);
char CanSendNB(CanMessage *msg);

#endif



