#ifndef ACTION_H_
#define ACTION_H_

#include "types.h"

void perform_action(action_t action, void * data);

void door_init(void);
void main_open(void);
void main_close(void);
void dump_authblock(authblock_t * ab);
void add_user(char * nickname, uint8_t anon);
void rem_user(char * nickname);
void lock_user(char * nickname);
void unlock_user(char * nickname);
void add_admin(char * nickname);
void rem_admin(char * nickname);
void keymigration(void);

#endif /*ACTION_H_*/
