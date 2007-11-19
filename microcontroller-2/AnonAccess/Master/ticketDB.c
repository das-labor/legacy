
#include <string.h>
//#include <stdio.h>
#include "ticketDB.h"
#include "24CBlockDev.h"
#include "config.h"
#include "debug.h"


#define MAJ_VERSION	0
#define MIN_VERSION 1

#define DB_IDSTRING {'A', 'n', 'o', 'n', 'A', 'c','c', 'e', 's', 's' }

/*
#define READ_BLOCK(a, d, s)  E24C_blockdev_readBlock((a),(d),(s))
#define WRITE_BLOCK(a, d, s) E24C_blockdev_writeBlock((a),(d),(s))
#define SET_BLOCK(a,v,s)     E24C_blockdev_setBlock((a),(v),(s))
*/
#include "keys.h"
#include "enc2E24C.h"

#define READ_BLOCK(a, d, s)  crypto_read_block((d),(a),(s), eeprom_key)
#define WRITE_BLOCK(a, d, s) crypto_write_block((d),(a),(s), eeprom_key)
#define SET_BLOCK(a,v,s)     crypto_set_block((v),(a),(s), eeprom_key)

/******************************************************************************/
uint16_t dbheadersize;
stats_t  dbstats;
/******************************************************************************/

void ticketdb_syncstats(void){
	WRITE_BLOCK(TICKETDB_OFFSET + DBSTAT_OFFSET ,&dbstats,sizeof(dbstats));
}

/******************************************************************************/

uint8_t	ticketdb_newuser(ticketmac_t* mac, userid_t* id, uint16_t initid){
	userflags_t flags;
	
	if (dbstats.users == dbstats.max_users)
		return DB_ERROR_DBFULL;
	
	/* find new location */
	initid %= dbstats.max_users;
//	while(ticketdb_userexists(initid))
//		initid = (initid+1)% dbstats.max_users;
	*id=initid;
	
	ticketdb_setUserTicketMac(*id, mac);
	
	flags.admin=false;
	flags.exist=true;
	flags.locked=false;
	flags.notify_lostadmin=false;
	ticketdb_setUserFlags(*id,&flags);
	return DB_ERROR_OK;
}

/******************************************************************************/

uint8_t	ticketdb_deluser(userid_t id){
	if (!ticketdb_userexists(id))
		return DB_ERROR_NOSUCHUSER;
	userentry_t d={0,{0},{0}};
	ticketdb_setUserTicketMac(id, &(d.ticketmac));
	ticketdb_setUserFlags(id,(userflags_t*)&(d.flags));
	return DB_ERROR_OK;
}

/******************************************************************************/

bool ticketdb_userexists(userid_t id){
	userflags_t flags;
	if (id>=dbstats.max_users)
		return false;
return false;
	ticketdb_getUserFlags(id,&flags);
//	uart_putstr("\r\n <"); uart_hexdump(&flags, 1); uart_putstr(">");
	return flags.exist;	
//	return (!(E24C_blockdev_readByte(dbheadersize + id*sizeof(userentry_t)) & 0x01));
}

/******************************************************************************/

uint8_t ticketdb_getUserTicketMac(userid_t id, ticketmac_t* dest){
	if (id>=dbstats.max_users)
		return DB_ERROR_NOSUCHUSER;;
	READ_BLOCK(TICKETDB_OFFSET + dbheadersize + id*sizeof(userentry_t)
	                        +DB_TICKETMAC_OFFSET, dest, 32);
	return DB_ERROR_OK;
}

/******************************************************************************/

uint8_t ticketdb_getUserFlags(userid_t id, userflags_t* dest){
	if (id>=dbstats.max_users)
		return DB_ERROR_NOSUCHUSER;
	uint32_t addr	= TICKETDB_OFFSET + dbheadersize +id*sizeof(userentry_t)
	           +DB_FLAGS_OFFSET;
	           
	READ_BLOCK(addr, dest, 1);
	return DB_ERROR_OK;
}

/******************************************************************************/

uint8_t ticketdb_setUserTicketMac(userid_t id, ticketmac_t* src){
	if (id>=dbstats.max_users)
		return DB_ERROR_NOSUCHUSER;
	WRITE_BLOCK(TICKETDB_OFFSET + dbheadersize + id*sizeof(userentry_t)
	                         +DB_TICKETMAC_OFFSET, src, 32);
	return DB_ERROR_OK;
}

/******************************************************************************/
/* dest must have place for at least 8 characters
 */
uint8_t ticketdb_getUserNickname(userid_t id, char* dest){
	if (id>=dbstats.max_users)
		return DB_ERROR_NOSUCHUSER;;
	READ_BLOCK(TICKETDB_OFFSET + dbheadersize + id*sizeof(userentry_t)
	                        +DB_NICKNAME_OFFSET, dest, 7);
	dest[7] = '\0';
	return DB_ERROR_OK;
}
/******************************************************************************/

uint8_t ticketdb_setUserNickname(userid_t id, char* dest){
	if (id>=dbstats.max_users)
		return DB_ERROR_NOSUCHUSER;;
	WRITE_BLOCK(TICKETDB_OFFSET + dbheadersize + id*sizeof(userentry_t)
	                        +DB_NICKNAME_OFFSET, dest, 7);
	return DB_ERROR_OK;
}
/******************************************************************************/

uint8_t ticketdb_setUserFlags(userid_t id, userflags_t* src){
	userflags_t oflags;
	if (id>=dbstats.max_users)
		return DB_ERROR_NOSUCHUSER;
	
	ticketdb_getUserFlags(id, &oflags);
	dbstats.users  += src->exist - oflags.exist;	/* someone deleted the user */
	dbstats.admins += (src->exist & src->admin) - (oflags.exist & oflags.admin);
	dbstats.locked_users += (src->exist & src->locked) - (oflags.exist & oflags.locked);
	dbstats.locked_admins += (src->exist & src->admin & src->locked)
						 - (oflags.exist & oflags.admin & oflags.locked);
	ticketdb_syncstats();
	WRITE_BLOCK(TICKETDB_OFFSET + dbheadersize + id*sizeof(userentry_t)
	                        +DB_FLAGS_OFFSET, src, 1);
	return DB_ERROR_OK;
}

/******************************************************************************/

uint16_t ticketdb_getstatMaxUsers(void){
	return dbstats.max_users;
}

/******************************************************************************/

uint16_t ticketdb_getstatUsers(void){
	return dbstats.users;
}

/******************************************************************************/

uint16_t ticketdb_getstatAdmins(void){
	return dbstats.admins;
}

/******************************************************************************/

uint16_t ticketdb_getstatLockedUsers(void){
	return dbstats.locked_users;
}

/******************************************************************************/

uint16_t ticketdb_getstatLockedAdmins(void){
	return dbstats.locked_admins;
}

/******************************************************************************/

void ticketdb_format(dbsize_t size){
	SET_BLOCK(TICKETDB_OFFSET,0,size); /* set all bytes to zeo */
	
	dbheader_t header={DB_IDSTRING, MAJ_VERSION, MIN_VERSION, sizeof(dbheader_t),
						{(size - sizeof(dbheader_t))/sizeof(userentry_t), /* max users */
						  0,  /* users */
						  0,  /* admins */
						  0,  /* locked users */
						  0}};/* locked addmins */ 
	WRITE_BLOCK(TICKETDB_OFFSET,&header, sizeof(dbheader_t));
}

/******************************************************************************/

uint8_t ticketdb_init(void){ /* initDB */
	dbheader_t header;
	uint8_t ret=0;
	READ_BLOCK(TICKETDB_OFFSET,&header,sizeof(dbheader_t));
	/* check if this is a valid header */
	if (!(   header.id[0] == 'A' 
	      && header.id[1] == 'n' 
	      && header.id[2] == 'o' 
	      && header.id[3] == 'n' 
	      && header.id[4] == 'A' 
	      && header.id[5] == 'c' 
	      && header.id[6] == 'c' 
	      && header.id[7] == 'e' 
	      && header.id[8] == 's' 
	      && header.id[9] == 's' ))
		ret |= DB_ERROR_IDSTRING;
	if (header.majversion != MAJ_VERSION || header.minversion != MIN_VERSION)
		ret |= DB_ERROR_VERSION;
	if (header.headersize != sizeof(dbheader_t))
		ret |= DB_ERROR_HEADERSIZE;
	if (!ret){
		dbheadersize = header.headersize;
		dbstats = header.stat;
	}
	return ret;
}

/******************************************************************************/
