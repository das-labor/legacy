#ifndef COMM_H_
#define COMM_H_

#include "types.h"
#include <stdint.h>

#define MASTERUNIT_ID 0
#define TERMINALUNIT_ID 1

#define MSGID_SESSION_INIT  0
#define MSGID_ADD_AB        1
#define MSGID_AB_REPLY      2
#define MSGID_AB_PINREQ     8
#define MSGID_AB_PINREPLY   9
#define MSGID_AB_ERROR      3
#define MSGID_ACTION        4
#define MSGID_ACTION_REPLY  5
#define MSGID_PRINT         6
#define MSGID_ADD_BOOTSTRAP 7

#define AB_ERROR_WONTTELL   0
#define AB_ERROR_HMAC       1
#define AB_ERROR_EXPIRED    2
#define AB_ERROR_TICKET     3
#define AB_ERROR_LOCKED     4

#define ACTION_MAINOPEN    0x00
#define ACTION_MAINCLOSE   0x01
#define ACTION_ADDUSER     0x10
#define ACTION_REMUSER     0x11
#define ACTION_LOCKUSER    0x12
#define ACTION_UNLOCKUSER  0x13
#define ACTION_ADDADMIN    0x14
#define ACTION_REMADMIN    0x15
#define ACTION_KEYMIGRATION 0x016

#define DONE    1
#define NOTDONE 0
#define ANON    1
#define NOTANON 0

extern uint8_t msg_wait;
extern uint16_t msg_length;
extern void* msg_data;

void init_session(void);
void submit_ab(authblock_t * ab);
void submit_pin(char* pin, uint16_t length_B);
void send_mainopen(void);
void send_mainclose(void);
void req_bootab(char* name, char* pin, uint16_t pinlen_B, uint8_t anon, uint8_t pinflags);

void freemsg(void);
uint8_t waitformessage(uint16_t timeout);
uint8_t getmsgid(void* msg);

#endif /*COMM_H_*/
