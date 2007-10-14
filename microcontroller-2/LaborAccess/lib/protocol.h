
#include <inttypes.h>

//how many chars can a nickname have?
#define NICKNAME_MAX_LEN 7

//how many bytes does a token have?
#define TOKEN_SIZE 32


//Requests that don't need permissions
#define REQUEST_RESET 0x01
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
#define RESULT_OK 0
#define RESULT_DENIED 1
#define RESULT_DEACTIVATED 2

//flags
#define FLAG_ADMIN 0x01
#define FLAG_DEACTIVATED 0x02


//a set bit means the door is open/unlocked
#define STATE_DOOR_UPSTAIRS   0x01
#define STATE_DOOR_DOWNSTAIRS 0x02

//this is the general structure for a request.
//It must have atleast the maximum Size used for any request.
typedef struct{
	uint8_t type;
	uint8_t data[65];
}request_t;

typedef struct{
	uint8_t type;
	uint16_t card_id;
	uint8_t token[TOKEN_SIZE];
}request_auth_t; //type + 34 bytes

//flags in permissions
#define PERM_ADMIN 0x01
#define PERM_DOOR_DOWNSTAIRS 0x02
#define PERM_DOOR_UPSTAIRS 0x04


typedef struct{
	uint8_t result;
	uint8_t permissions;
	uint8_t token[TOKEN_SIZE];
	char nickname[NICKNAME_MAX_LEN+1];
}reply_auth_t;



typedef struct{
	uint8_t type;
	//this permissions field only decides about the doors, that may be opened,
	//not abput the admin permissions.
	uint8_t permissions;
	char nickname[NICKNAME_MAX_LEN+1];
	char realname[32];
}request_new_card_t; //type + 65 bytes

typedef struct{
	uint8_t result;
	uint16_t id;
	uint8_t token[TOKEN_SIZE];
}reply_new_card_t;

typedef request_auth_t request_make_admin_t;

typedef struct{
	uint8_t result;
	uint8_t token[TOKEN_SIZE];
}reply_make_admin_t;

typedef struct{
	uint8_t type;
	uint16_t id;
}request_delete_admin_t;
