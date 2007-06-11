

#define DB_STATE_EMPTY 0xff
#define DB_STATE_VALID 0x00

typedef struct{
	uint8_t state;
	uint8_t flags;
	uint8_t token[8];
	char nickname[8];
	char realname[32];
	char reserved[14];
}db_entry_t;


uint8_t database_check(){
	


}

void database_get_nickname(char * nickname){
	
}
