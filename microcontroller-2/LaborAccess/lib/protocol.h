
#include <inttypes.h>

//how many chars can a nickname have?
#define NICKNAME_MAX_LEN 7

//how many bytes does a token have?
#define TOKEN_SIZE 8


//Requests that don't need permissions
#define REQUEST_RESET 0xff
#define REQUEST_GET_DOORSTATE 0x20

#define REQUEST_UNLOCK_DOWNSTAIRS 0x40
#define REQUEST_LOCK_DOWNSTAIRS 0x41

#define REQUEST_AUTH 0x50

//requests that need a valid id/token
#define REQUEST_UNLOCK_UPSTAIRS 0x80
#define REQUEST_LOCK_UPSTAIRS 0x81

#define REQUEST_OPEN_DOWNSTAIRS 0x82

//requests that need admin permissions
#define REQUEST_NEW_CARD 0x90

#define REQUEST_DELETE_ADMIN 0xa0

//requests that need permissions from 2 admins:
#define REQUEST_MAKE_ADMIN 0xb0



//values for the result var in replys
#define RESULT_OK 				0x00
#define RESULT_DENIED 			0x01
#define RESULT_DEACTIVATED 		0x02
#define RESULT_RESET 			0xff

//flags
#define FLAG_ADMIN 				0x01
#define FLAG_DEACTIVATED 		0x02


//a set bit means the door is open/unlocked
#define STATE_DOOR_UPSTAIRS   0x01
#define STATE_DOOR_DOWNSTAIRS 0x02

//this is the general structure for a request. The server uses it 
//It must have atleast the maximum Size used for any request.

typedef struct{
	uint8_t type;
	uint8_t data[42];
}request_t;


typedef struct{
	uint8_t type;
	uint16_t card_id;
	uint8_t token[TOKEN_SIZE];
}request_auth_t; //type + 10 bytes

//flags in permissions
#define PERM_ADMIN 0x01
#define PERM_DOOR_DOWNSTAIRS 0x02
#define PERM_DOOR_UPSTAIRS 0x04


typedef struct{
	uint8_t result;
	uint8_t permissions;
	uint16_t id;
	uint8_t token[TOKEN_SIZE];
	char nickname[NICKNAME_MAX_LEN+1];
}reply_auth_t;

typedef struct{
	uint8_t type;
	//this permissions field only decides about the doors, that may be opened,
	//not abput the admin permissions.
	uint8_t permissions;
	char nickname[NICKNAME_MAX_LEN+1];
#ifdef USE_REALNAME
	char realname[32];
#endif
}request_new_card_t; //type + 1 + 8 + 32 = type+41 bytes

typedef struct{
	u08 type;
	u16 id;
}request_laboranten_info_t;

typedef struct{
	u08 result;
	uint8_t permissions;
	u16 last_id;
	uint16_t next_id;
	char nickname[NICKNAME_MAX_LEN+1];
#ifdef USE_REALNAME
	char realname[32];
#endif
}reply_laboranten_info_t;
