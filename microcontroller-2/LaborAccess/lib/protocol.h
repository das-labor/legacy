
#include <inttypes.h>

#define NICKNAME_MAX_LEN 7

//requests below 0x80 don't need credentials

#define REQUEST_GET_DOORSTATE 0x20

#define REQUEST_UNLOCK_DOWNSTAIRS 0x40
#define REQUEST_LOCK_DOWNSTAIRS 0x41

#define REQUEST_AUTH 0x50

//requests from 0x80 need authenticated session
#define REQUEST_UNLOCK_UPSTAIRS 0x80
#define REQUEST_LOCK_UPSTAIRS 0x81

#define REQUEST_OPEN_DOWNSTAIRS 0x82



//values for the result var in replys
#define RESULT_OK 0
#define RESULT_DENIED 1
#define RESULT_DEACTIVATED 2

//flags
#define FLAG_ADMIN 0x01



//a set bit means the door is open/unlocked
#define STATE_DOOR_UPSTAIRS   0x01
#define STATE_DOOR_DOWNSTAIRS 0x02


typedef struct{
	uint8_t type;
	uint8_t data[10];
}request_t;

typedef struct{
	uint8_t type;
	uint16_t card_id;
	uint8_t token[8];
}request_cred_t;

typedef struct{
	uint8_t result;
	uint8_t flags;
	uint8_t token[8];
	char nickname[NICKNAME_MAX_LEN+1];
}reply_auth_t;
