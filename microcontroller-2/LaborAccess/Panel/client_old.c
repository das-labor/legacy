
/*
	This is the Client, that sends the requests to the Labor Access Master.
	Some of the requests need credentials in Form of card id and key, and some don't.
	
	Requests that don't need credentials:
		-Unlock downstairs door (when upstairs is unlocked)
		-Lock downstairs door (when upstairs is unlocked)
		
	Requests that need credentials:
		-Unlock upstairs door
		-Lock upstairs door
		
	Requests that need credentials from an admin:
		-Activate new card
		-Deactivate card by number
		-Reactivate card by number
		-Show data by number
		-Dump database to CAN
	
	Requests that need credentials from 2 admins:
		-Set admin flag for card
		-Delete database entry
		
	
	When a card is inserted, the Panel assumes a door open or lock request, depending on the
	state the door is in at that time.
	When one holds the tape-button while inserting a card, the admin mode is entered. One
	can now do the admin tasks.

*/


#include <stdlib.h>
#include <avr/pgmspace.h>
#include "avrx.h"

#include "enum.h"

#include "borg_hw.h"
#include "reader.h"
#include "i2csw.h"
#include "7seg.h"
#include "pixel.h"

#include "AvrXSerialIo.h"

#include "asn1.h"
#include "channel.h"

#include "protocol.h"

#include "util.h"



#define KEY_DOOR_DOWNSTAIRS 0x62


ReaderMsg_t msg;

uint8_t doorstate;






void segputhex(uint8_t c){
	char buf[3];
	itoa(c, buf, 16);
	seg_putstr(buf);
}


