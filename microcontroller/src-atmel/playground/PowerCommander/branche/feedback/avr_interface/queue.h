/* -*- Mode: C; tab-width: 2 -*- */
#ifndef POWERCOMMANDER_QUEUE_H
#define POWERCOMMANDER_QUEUE_H

#include <inttypes.h>

#define I2C_OUTDATACOUNT 4
#define I2C_INDATACOUNT 1
#define CAN_OUTDATACOUNT I2C_OUTDATACOUNT
#define CAN_INDATACOUNT I2C_INDATACOUNT

/*
	ab und an sendet der Powercommander auch ohne 
	expl. Aufforderung Daten auf den CanBus.
	Dafuer ist es notwendig genau identifizieren zu koennen,
	welche Funktion dieses Packet auf den CanBus geschmissen hat.
 */

// wir muessen nur 4 Bytes via i2c senden 
// und evt eins zurueck bekommen 
// dies gilt nur fuer den Powercommander
typedef struct {
	MessageControlBlock mcb;
	uint8_t outdata[I2C_OUTDATACOUNT];
}t_i2cMessage_out;

typedef struct {
	MessageControlBlock mcb;
	uint8_t indata[I2C_INDATACOUNT];
}t_i2cMessage_in;

typedef struct {
	MessageControlBlock mcb;
	uint8_t outdata[CAN_OUTDATACOUNT];
}t_canMessage_out;

typedef struct {
	MessageControlBlock mcb;
	uint8_t indata[CAN_INDATACOUNT];
}t_canMessage_in;

/*
	gesehen von interface aus
*/
MessageQueue        i2cQueue_out;    // The message queue
MessageQueue        i2cQueue_in;    // The message queue
MessageQueue        canQueue_out;    // The message queue
MessageQueue        canQueue_in;    // The message queue

#endif

	
