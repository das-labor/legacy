
#include <stdint.h>
#include <string.h>
#include "i2csw.h"
#include "protocol.h"

#define DB_SIZE 1024

#define DB_STATE_EMPTY 0xff
#define DB_STATE_VALID 0x42
#define DB_STATE_DEACTIVATED 0x23

//One Database entry is 64 Byte in Size.
typedef struct{
	uint8_t state;
	uint8_t permissions;
	uint16_t deactivation_id;
	char reserved[12];
	uint8_t token[8];
	char nickname[8];
	char realname[32];
}db_entry_t;

#define db ((db_entry_t *)0)

uint8_t database_check(uint16_t id, uint8_t * token){
	uint8_t state;
	uint8_t tmpbuf[8];
	
	if(id >= DB_SIZE)
		return RESULT_DENIED;
		
	i2cEeRead(&state, &db[id].state, 1);
	
	if((state != DB_STATE_VALID) && (state != DB_STATE_DEACTIVATED)){
		return RESULT_DENIED;
	}
		
	i2cEeRead(tmpbuf, db[id].token, 8);
	if( memcmp(tmpbuf, token, 8) != 0)
		return RESULT_DENIED;
	
	if(state == DB_STATE_DEACTIVATED)
		return RESULT_DEACTIVATED;
	
	return RESULT_OK;
}

void database_store_token(uint16_t id, uint8_t * token){
	i2cEeWrite(db[id].token, token, 8);
}

void database_get_nickname(uint16_t id, char * nickname){
	i2cEeRead((uint8_t*)nickname, (uint8_t*)db[id].nickname, 8);
}

uint8_t database_get_permissions(uint16_t id){
	uint8_t permissions;
	i2cEeRead(&permissions, &db[id].permissions, 1);
	return permissions;
}

void database_set_permissions(uint16_t id, uint8_t permissions){
	i2cEeWrite(&db[id].permissions, &permissions, 1);
}

uint16_t database_get_deactivation_id(uint16_t id){
	uint16_t di;
	i2cEeRead((uint8_t*)&di, (uint8_t*)&db[id].deactivation_id, 2);
	return di;
}

uint16_t database_new_entry(uint8_t permissions, char * nickname, char * realname){
	uint16_t id;
	uint8_t state;
	for(id = 0; id<DB_SIZE; id++){
		i2cEeRead(&state, &db[id].state, 1);
		if(state != DB_STATE_EMPTY)
			continue;
		
		state = DB_STATE_VALID;
		i2cEeWrite(&db[id].state, &state, 1);
		
		//don't make new Admin cards here
		permissions &= ~PERM_ADMIN;
		i2cEeWrite(&db[id].permissions, &permissions, 1);
		
		i2cEeWrite((uint8_t *)db[id].nickname, (uint8_t *)nickname, 8);
		i2cEeWrite((uint8_t *)db[id].realname, (uint8_t *)realname, 32);
				
		return id;
	}
	return 0xffff;
}
