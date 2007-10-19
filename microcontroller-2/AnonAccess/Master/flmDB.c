/*******************************************************************************
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

#include <string.h>
#include <stdint.h>
#include "config.h"
#include "types.h"
#include "flmDB.h"
#include "24CBlockDev.h"
#include "ticketDB.h"
#include "uart.h"

#define FLMDB_MAXID (FLMDB_SIZE/sizeof(flmdb_entry_t)-1)


void flmdb_loadentry(flmdb_entry_t * entry, entryid_t id){
	if(id>FLMDB_MAXID)
		return;
	E24C_blockdev_readBlock(FLMDB_OFFSET + id*sizeof(flmdb_entry_t),
	                        entry, sizeof(flmdb_entry_t));
	return;
}

void flmdb_setentry(flmdb_entry_t * entry, entryid_t id){
	if(id>FLMDB_MAXID)
		return;
	E24C_blockdev_writeBlock(FLMDB_OFFSET + id*sizeof(flmdb_entry_t), 
	                         entry, sizeof(flmdb_entry_t));
	return;
}

void flmdb_process(uint8_t * searchmac, userid_t uid, userflags_t * flags){
	entryid_t i;
	flmdb_entry_t entry;
	for(i=0; i<= FLMDB_MAXID; ++i){
		flmdb_loadentry(&entry, i);
		
		uart_putc('.');
		if(entry.active && !memcmp(entry.hnick, searchmac, 32)){
			ticketdb_getUserFlags(uid, flags);
			/* apply flag modifiers */
			flags->admin |= entry.setflags.admin;
			flags->exist |= entry.setflags.exist;
			flags->locked|= entry.setflags.locked;
			flags->notify_lostadmin |= entry.setflags.notify_lostadmin;
			flags->admin &= ~entry.clearflags.admin;
			flags->exist &= ~entry.clearflags.exist;
			flags->locked&= ~entry.clearflags.locked;
			flags->notify_lostadmin &= ~entry.clearflags.notify_lostadmin;
			flags->reserved = 0;
			
			/* sync with db */
			ticketdb_setUserFlags(uid, flags);
			// entry.active = 0;
			memset(&entry, 0, sizeof(flmdb_entry_t));
			flmdb_setentry(&entry, i);
		}
	}
}

void flmdb_makeentry(uint8_t * mac, userflags_t setflags, userflags_t clearflags){
	entryid_t i;
	flmdb_entry_t entry;
	uint8_t t;
	
	memcpy(entry.hnick, mac, 32);
	memset(entry.reserved, 0, 5);
	entry.active = 1;
	entry.setflags = setflags;
	entry.clearflags = clearflags;
	
	for(i=0; i<=FLMDB_MAXID; ++i){
		t = E24C_blockdev_readByte(FLMDB_OFFSET + i*sizeof(flmdb_entry_t) + FLMDB_ACTIVE_OFFSET);
		if(!t){
			flmdb_setentry(&entry, i);
			return;
		}
	}
}




