#ifndef REQVALIDATOR_H_
#define REQVALIDATOR_H_

bool check_permissions(uint8_t users, uint8_t admins, action_t action);
authcredvalid_state_t check_authblock(authblock_t * ab);
void new_account(authblock_t * ab, char* nickname, uint8_t anon);
void modify_account(char * nickname, userflags_t setflags, userflags_t clearflags);


#endif /*REQVALIDATOR_H_*/
