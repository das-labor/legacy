
#include <stdint.h>
#include "i2csw.h"

#define DB_STATE_EMPTY 0xff
#define DB_STATE_VALID 0x23

#define FLAG_DEACTIVATED 

//One Database entry is 64 Byte in Size.
typedef struct{
	uint8_t state;
	uint8_t flags;
	uint16_t deactivation_id;
	uint8_t token[8];
	char nickname[8];
	char realname[32];
	char reserved[12];
}db_entry_t;

#define db ((db_entry_t *)0)

uint8_t database_check(uint16_t id, uint8_t * token){
	uint8_t state, flags;
	uint8_t tmpbuf[8];
	
	i2cEeRead(&state, &db[id].state, 1);
	i2cEeRead(&flags, &db[id].flags, 1);
	
	if(state != DB_STATE_VALID)
		return RESULT_DENIED;
		
	i2cEeRead(tmpbuf, db[id].token, 8);
	if( memcmp(tmpbuf, token, 8) != 0)
		return RESULT_DENIED;
	
	if(flags & FLAG_DEACTIVATED)
		return RESULT_DEACTIVATED;
	
	return RESULT_OK;
}

void database_store_token(uint16_t id, uint8_t * token){
	i2cEeWrite(db[id].token, token, 8);
}

void database_get_nickname(uint16_t id, char * nickname){
	i2cEeRead((uint8_t*)nickname, db[id].nickname, 8);
}

uint16_t database_get_deactivation_id(uint16_t id){
	uint16_t di;
	i2cEeRead(&di, &db[id].deactivation_id, 1);
	return di;
}
