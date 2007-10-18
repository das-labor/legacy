#ifndef REQVALIDATOR_H_
#define REQVALIDATOR_H_

bool valid_authreq(action_t action, uint8_t n, authblock_t * authblock);
authcredvalid_state_t check_authblock(authblock_t * ab);
void new_account(authblock_t * ab, char* nickname);
void modify_account(char * nickname, userflags_t setflags, userflags_t clearflags);


#endif /*REQVALIDATOR_H_*/
