#ifndef REQVALIDATOR_H_
#define REQVALIDATOR_H_

bool check_permissions(uint8_t users, uint8_t admins, action_t action);
bool final_action(action_t action);
bool check_pin(authblock_t * ab, sha256_hash_t pinhash);
void change_pin(authblock_t * ab, sha256_hash_t pinhash);

authcredvalid_state_t check_authblock(authblock_t * ab);
void new_account(authblock_t * ab, char* nickname, sha256_hash_t pinhash,uint8_t anon, uint8_t pinflags);

void modify_account(char * nickname, userflags_t setflags, userflags_t clearflags);
void modify_account_byuid(uid_t uid, userflags_t setflags, userflags_t clearflags);

uint8_t pin_required(uid_t uid, uint8_t admin);
bool final_action(action_t action);

#endif /*REQVALIDATOR_H_*/
