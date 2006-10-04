#ifndef EVENT_H
#define EVENT_H

#include <avr/io.h>
#include <avrx.h>

#define EventError           0x0100   /* EVENT CLASS */
#define EventErrorComm       0x0101  
#define EventReboot          0x0102 
#define EventCard            0x0200   /* EVENT CLASS */
#define EventCardInserted    0x0200   /* Terminal -> Master */
#define EventCardEjected     0x0201   /* Terminal -> Master */
#define EventCardJamed       0x0202   /* Terminal -> Master */
#define EventCardEject       0x0203   /* Master -> Terminal */
#define EventCardCollect     0x0204   /* Master -> Terminal */
#define EventCardRead        0x0205   
#define EventCardWrite       0x0206
#define EventKey             0x0400   /* EVENT CLASS */
#define EventKeyPressed      0x0401   /* Terminal -> Master */
#define EventLed             0x0300
#define EventPanel           0x0400   
#define EventPanelDoorMode   0x0401
#define EventPanelNormalMode 0x0402

// extern uint16_t EventIgnoreMask;
// extern uint16_t EventReplayMask;
// extern uint16_t EventAcceptMask;


typedef struct {       // Message
	MessageControlBlock mcb;
	uint16_t type;	
	uint8_t size;   //bytes in data
	
	uint8_t data[];
} Event;

void SendEvent(Event *ev);
Event *WaitEvent();


#endif
