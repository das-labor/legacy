
#include <inttypes.h>

#define NICKNAME_MAX_LEN 7

//requests below 0x80 don't need credentials

#define REQUEST_GET_DOORSTATE 0x20

#define REQUEST_UNLOCK_DOWNSTAIRS 0x40
#define REQUEST_LOCK_DOWNSTAIRS 0x41

//requests from 0x80 need credentials
#define REQUEST_UNLOCK_UPSTAIRS 0x80
#define REQUEST_LOCK_UPSTAIRS 0x81

#define REQUEST_OPEN_DOWNSTAIRS 0x82

#define REQUEST_AUTH 0x90

//requests from 0xc0 need an authenticated session




#define RESULT_OK 0
#define RESULT_DENIED 1

#define STATE_DOOR_DOWNSTAIRS 0x01


typedef struct{
	uint8_t type;
	uint8_t data[10];
}request_t;

typedef struct{
	uint16_t index;
	uint8_t key[8];	
}credentials_t;

typedef struct{
	uint8_t type;
	credentials_t credentials;
}request_cred_t;

typedef struct{
	uint8_t result;
	credentials_t credentials;
	char nickname[NICKNAME_MAX_LEN+1];
}reply_auth_t;

