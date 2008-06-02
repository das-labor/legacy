#ifndef COMM_H_
#define COMM_H_

#include "types.h"
#include "lop.h"
#include "qport.h"
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

#define MSGID_MAX  9
#define MSGID_CNT (MSGID_MAX+1)

#define AB_ERROR_WONTTELL   0
#define AB_ERROR_HMAC       1
#define AB_ERROR_EXPIRED    2
#define AB_ERROR_TICKET     3
#define AB_ERROR_LOCKED     4
#define AB_ERROR_ALLREADY_PARTICIPATING 5
#define AB_ERROR_TOO_MANY_PARTICIPANTS  6
#define AB_ERROR_TIMEOUT    7
#define AB_ERROR_AUTHBLOCK  8
#define AB_ERROR_PINTIMEOUT 9
#define AB_ERROR_PIN       10


#define ACTION_MAINOPEN    0x00
#define ACTION_MAINCLOSE   0x01
#define ACTION_ADDUSER     0x10
#define ACTION_REMUSER     0x11
#define ACTION_LOCKUSER    0x12
#define ACTION_UNLOCKUSER  0x13
#define ACTION_ADDADMIN    0x14
#define ACTION_REMADMIN    0x15
#define ACTION_KEYMIGRATION 0x016

#define STR_CLASS_NO    0x00
#define STR_CLASS_INFO  0x01
#define STR_CLASS_WARN  0x02
#define STR_CLASS_ERROR 0x03
#define STR_CLASS_SEC   0x04
#define STR_CLASS_MAX   0x04

#define DONE    1
#define NOTDONE 0
#define ANON    1
#define NOTANON 0

/* these are defined in main.c */
extern lop_ctx_t lop0;
extern qport_ctx_t qp0;
extern lop_ctx_t lop1;



void messagerx(uint16_t length, void* msg);
void send_str(uint8_t terminal_id, char* str, uint8_t str_class);

void freemsg(void);
uint8_t waitformessage(uint32_t timeout);
uint8_t getmsgid(const void* msg);

#endif /*COMM_H_*/
