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

typedef struct{
	uint8_t		id[4];
	uint8_t		dlc;
	uint8_t		data[8];
} CanMessage;

typedef enum { normal, mode_sleep, loopback, listenonly, config } CanMode;


/*****************************************************************************
 * Management
 */

void CanInit();
void CanSetFilter();
void CanSetMode(CanMode);
void CanSetLED(uint8_t led, uint8_t state);

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



