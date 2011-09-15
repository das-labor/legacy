#ifndef _H_CAN_HANDLER
#define _H_CAN_HANDLER

extern void can_handler();

void read_can_addr();

extern uint8_t myaddr;

void can_createDATAPACKET();

#endif // _H_CAN_HANDLER

