
#include <inttypes.h>

#define NICKNAME_MAX_LEN 7

//requests below 0x80 don't need credentials

//requests from 0x80 need credentials
#define REQUEST_OPEN_CRED 0x80


#define REPLY_OK 0
#define REPLY_DENIED 1


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
	uint8_t type;
	uint8_t reply;
	char nickname[NICKNAME_MAX_LEN+1];
}reply_t;


