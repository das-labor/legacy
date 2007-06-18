
uint8_t database_check(uint16_t id, uint8_t * token);
void database_store_token(uint16_t id, uint8_t * token);
void database_get_nickname(uint16_t id, char * nickname);
uint16_t database_get_deactivation_id(uint16_t id);
uint8_t database_get_flags(uint16_t id);
uint16_t database_new_entry(uint8_t permissions, char * nickname, char * realname);
