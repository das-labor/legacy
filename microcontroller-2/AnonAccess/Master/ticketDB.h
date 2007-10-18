/***************************************
 *  Header file for interfacing the database
 * on external eeprom
 * 
 ***/

#include <stdint.h>
#include "types.h"


#define DB_ERROR_OK			0x00
#define DB_ERROR_IDSTRING	0x01
#define DB_ERROR_VERSION	0x02
#define DB_ERROR_HEADERSIZE	0x04
#define DB_ERROR_DBFULL		0x08
#define DB_ERROR_NOSUCHUSER	0x10

/*********************************************************************************************************/


typedef struct stats_st{
	uint16_t max_users;
	uint16_t users;
	uint16_t admins;
	uint16_t locked_users;
	uint16_t locked_admins;
} stats_t;


typedef struct userentry_st{
	uint8_t     flags;
	uint8_t	    reserved[7];
	ticketmac_t ticketmac;
} userentry_t; /* total size is 80 bytes */

#define DB_FLAGS_OFFSET      0
#define DB_NICKNAME_OFFSET   8
#define DB_REALNAME_OFFSET  16
#define DB_TICKETMAC_OFFSET 48

typedef struct dbheader_st{
	uint8_t id[10]; 
	uint8_t majversion;
	uint8_t minversion;
	uint16_t headersize;
	stats_t stat;
	uint8_t reserved[8];
} dbheader_t;

#define DBSTAT_OFFSET 14

/*********************************************************************************************************/


void    ticketdb_format(dbsize_t size);
uint8_t	ticketdb_init(void);
uint8_t	ticketdb_newuser(ticketmac_t*, userid_t*, uint16_t);
uint8_t	ticketdb_deluser(userid_t);

bool    ticketdb_userexists(userid_t id);
uint8_t	ticketdb_getUserTicketMac(userid_t id, ticketmac_t*);
uint8_t	ticketdb_getUserFlags(userid_t id, userflags_t*);
uint8_t	ticketdb_setUserTicketMac(userid_t id, ticketmac_t*);
uint8_t	ticketdb_setUserFlags(userid_t id, userflags_t*);

unsigned ticketdb_getstatMaxUsers(void);
unsigned ticketdb_getstatUsers(void);
unsigned ticketdb_getstatAdmins(void);
unsigned ticketdb_getstatLockedUsers(void);
unsigned ticketdb_getstatLockedAdmins(void);

