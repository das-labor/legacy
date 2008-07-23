#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

#ifndef __BOOL__
#ifndef BOOL
typedef enum{false=0, true=1} bool;
	#define BOOL
	#define __BOOL__
#endif
#endif


typedef enum{
	mainopen   = 0, 
	mainclose, 
	adduser, 
	remuser,
	lockuser,
	unlockuser,
	addadmin,
	remadmin,
	keymigrate
} action_t;


typedef uint16_t userid_t;
#define NO_USER 0xFFFF
typedef uint32_t dbsize_t;
typedef uint8_t  ticketmac_t[32];


typedef uint16_t uid_t;
typedef uint8_t ticket_t[32];


typedef uint8_t token_t[32];

typedef struct{
	uint8_t users_req;
	uint8_t admins_req;
} action_requirements_t;

typedef struct userflags_st{
	unsigned exist:1;				/* this user exists */
	unsigned admin:1;				/* this user is admin */
	unsigned locked:1;				/* this account is locked */
	unsigned notify_lostadmin:1;	/* this user must be notifyed about lost admin privileges */
	unsigned anonymous:1;			/* this user is anonymous */
	unsigned force_admin_pin:1;     /* this user is allways asked for pin */
	unsigned force_normal_pin:1;    /* this user is asked for pin when logging in as admin */
	unsigned lock_nick:1;           /* the corresponding nick is locked */
} userflags_t;

typedef enum{
	invalid_cred = 0,
	valid_user   = 1,
	valid_admin  = 3,
	valid_user_lostadm  = 5,
	invalidtimeout_cred = 0x10
} authcredvalid_state_t; /* authentification credentials valid status */
/*
typedef struct{
	uid_t    uid;
	ticket_t ticket;
	uint8_t  rkey[32];
	uint8_t  rid[32];
	uint8_t  pinhmac[32];
	uint8_t  hmac[32];
} authblock_t;
*/

typedef struct{
	uid_t    uid;
	ticket_t ticket;
	uint8_t  rid[32];
	uint8_t  hmac[32];
} authblock_t;



#endif /*TYPES_H_*/
