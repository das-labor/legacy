

#include "config.h"
#include "debug.h"
#include "uart.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h> // itoa()
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "24Cxxx.h"
#include "i2c_tools.h"
#include "debug_tools.h"
#include "ticketDB.h"
#include "flmDB.h"
#include "entropium.h"
#include "hmac-sha256.h"
#include "sha256.h"
#include "action.h"
#include "types.h"
#include "keys.h"
#include "reqvalidator.h"
#include "shabea.h"
#include "rtc.h"
#include "reset_counter.h"
#include "enc2E24C.h"
#include "hwrnd.h"
#include "lop.h"
#include "lop_debug.h"

extern lop_ctx_t lop0;
/*
#define DS(a)   lop_dbg_str_P(&lop0, PSTR(a))
#define DD(a,b) lop_dbg_hexdump(&lop0, (a),(b))
#define DC(a)   lop_sendstream(&lop0, a)
#define DB(a)   dbg_putbyte(a)
*/

#define DS(a)   
#define DD(a,b) 
#define DC(a)   
#define DB(a)   

/*
#include "mcp9800.h"
*/
/****************************************************
 ****************************************************
 *  FUNCTIONS
 ****************************************************
 ****************************************************/

/****************************************************
 *  uart_putbyte()
 * **************************************************/
void dbg_putbyte(uint8_t b){
	char table[]={'0','1','2','3','4','5','6','7',
				  '8','9','A','B','C','D','E','F'};
	DC(table[b>>4]);
	DC(table[b&0xF]);
}

/****************************************************
 *  dump_dbstats()
 * **************************************************/
void dump_dbstats(void){
	uint16_t i;
	DS("\r\n");
	i = ticketdb_getstatMaxUsers();
	DS("MaxUsers:"); DB(i>>8); DB(i&0xff); DS("\r\n");
	i = ticketdb_getstatUsers();
	DS("Users:"); DB(i>>8); DB(i&0xff); DS("\r\n");
	i = ticketdb_getstatAdmins();
	DS("Admins:"); DB(i>>8); DB(i&0xff); DS("\r\n");
	i = ticketdb_getstatLockedUsers();
	DS("Locked Users:"); DB(i>>8); DB(i&0xff); DS("\r\n");
	i = ticketdb_getstatLockedAdmins();
	DS("Lockes Admins:"); DB(i>>8); DB(i&0xff); DS("\r\n");
}

/****************************************************
 *  console_dumptimestamp()
 * **************************************************/

void console_dumptimestamp(void){
	timestamp_t t;
	t = gettimestamp();
	DS("\r\n timestamp (hex): ");
	DD(&t, sizeof(timestamp_t));
}

/****************************************************
 *  console_dumpresets()
 * **************************************************/

void console_dumpresets(void){
	uint64_t t;
	t = resetcnt_read();
	DS("\r\n resets (hex): ");
	DD(&t, sizeof(timestamp_t));
}

/****************************************************
 *  console_getnstr()
 * **************************************************/
#define ENTER 0x0D

void console_getnstr(uint16_t n, char* dest){
	char c;
	int16_t i=0;
		while((ENTER != (c=uart_getc())) && i<n){
			++i;
			*dest++ = c;
			DC(c);
		}
		*dest='\0';
}

/****************************************************
 *  charhex2value()
 * **************************************************/
uint8_t charhex2value(char c){
	if(('0'<=c) && ('9' >=c))
		return c - '0';
	if(('A'<=c) && ('F' >=c))
		return c - 'A' + 0x0A;
	if(('a'<=c) && ('f' >=c))
		return c - 'a' + 0x0A;
	return 0xFF;
}

/****************************************************
 *  asciihex2values()
 * **************************************************/
void asciihex2values(uint8_t* dest, char * src, int n){
	while(n>1){
		if(src=='\0')
			return;
		while(*src==' '){
			++src;
		}
		*dest  = charhex2value(*src++) << 4;
		while(*src==' '){
			++src;
		}
		*dest |= charhex2value(*src++);
		dest++;
		n -= 2;
	}
}

void test_shabea256(void){
	uint8_t b[32];
	uint8_t key[32];
	
	entropium_fillBlockRandom(key, 32);
	entropium_fillBlockRandom(b, 32);
	DS("\r\n key:   "); DD(key, 32);
	DS("\r\n plain: "); DD(b, 32);
	
	shabea256(b, key, 256, 1, 16);
	DS("\r\n crypt: "); DD(b, 32);
	
	shabea256(b, key, 256, 0, 16);
	DS("\r\n plain: "); DD(b, 32);
	
	shabea256(b, key, 256, 1, 16);
	DS("\r\n crypt: "); DD(b, 32);
}

void console_getnick(void * data){
	char rcvbuffer[31]={0};
	DS("\r\n nickname: ");
	console_getnstr( 30, rcvbuffer);
	*((char**)data) = malloc(strlen(rcvbuffer)+1);
	strncpy(*((char**)data), rcvbuffer, 30);
}


/****************************************************
 *  console_verifyuser()
 ****************************************************/
void console_verifyuser(void){
	authblock_t ab;
	authcredvalid_state_t t;
	
	console_getauthblock(&ab);
	t= check_authblock(&ab);
	if(t==invalid_cred){
		DS("\r\n invalid authblock");
		return;
	}
	if(t==valid_admin){
		DS("\r\n valid admin authblock");
	} else {
		DS("\r\n valid user authblock");
	}
	DS("\r\n your new authblock");
	console_dumpauthblock(&ab);
}

/****************************************************
 *  console_adduser()
 * **************************************************/
void console_adduser(void){
	char * nick;
	sha256_hash_t pinhash;
	authblock_t ab;
	memset(&pinhash, 0, sizeof(sha256_hash_t));
	DS("\r\n new user");
	console_getnick(&nick);
	new_account(&ab, nick, pinhash, 0,0);
	free(nick);
	console_dumpauthblock(&ab);
}

/****************************************************
 *  console_adduser_db()
 * **************************************************/
void console_adduser_db(void){
	uint8_t r;
	ticketmac_t mac = {0xde, 0xad, 0xbe, 0xef};
	uint16_t uid=1;
	DS("\r\n*** add user ***\r\n");
	r = ticketdb_newuser(&mac, &uid, 0);
	DS("{"); DB(r); DS("}\r\n");
}

/****************************************************
 *  console_getauthblock()
 * **************************************************/
void console_getauthblock(authblock_t *ab){
	char rcvbuffer[150];

	DS("\r\n uid:    ");
	console_getnstr(6, rcvbuffer);
	asciihex2values((uint8_t*)&(ab->uid),rcvbuffer, 4);

	DS("\r\n ticket: ");
	console_getnstr(110, rcvbuffer);
	asciihex2values( ab->ticket,rcvbuffer, 64);

	DS("\r\n rid:    ");
	console_getnstr(110, rcvbuffer);
	asciihex2values( ab->rid,rcvbuffer, 64);

	DS("\r\n hmac:   ");
	console_getnstr(110, rcvbuffer);
	asciihex2values( ab->hmac,rcvbuffer, 64);
}

/****************************************************
 *  console_getauthblock()
 * **************************************************/
void console_dumpauthblock(authblock_t *ab){
	DS("\r\n uid:    ");
	DD((char*)&(ab->uid), 2);
	
	DS("\r\n ticket: ");
	DD((char*)(ab->ticket), 32);
	
	DS("\r\n rkey:   ");
	DD((char*)(ab->rkey), 32);

	DS("\r\n rid:    ");
	DD((char*)(ab->rid), 32);

	DS("\r\n hmac:   ");
	DD((char*)(ab->hmac), 32);
}

/****************************************************
 *  console_getauthcredentials()
 * **************************************************/

uint8_t console_getauthblocks(authblock_t **ab){
	uint8_t n=0;
	while(n<255){
		DS("\r\n press 1 to submit additional credentials"
		   "\r\n press 2 to continue");
		switch(uart_getc()){
			case '1': break;
			case '2': return n;
			default:  continue; 
		}
		*ab=realloc(*ab,(n+1)*sizeof(authblock_t));
		if(!*ab)
			DS("\r\n BOOOM");
		console_getauthblock(ab[n]);
		n++;
		return n;
	}
	return n;
}

/****************************************************
 *  console_dbg()
 * **************************************************/
void console_dbg(){
	char c;
	DS("\r\n*** console debug ***\r\npress 'q' to leave\r\n");
	do {
		c = uart_getc();
		DS("Got char: 0x");
		DB(c);
		DS(" (");
		if(c>=32){
			DC(c);
		} else {
			DS("<32");
		}
		DS(" )\r\n");
	} while (c != 'q');
}

/****************************************************
 *  i2c_detect()
 * **************************************************/
void i2c_detect(i2c_detect_t table){
	uint8_t j;
	for(j=0; j<32; ++j)
		table[j]=0;
	DS("Detecting devices:\r\n");
	for (j=0; j<254; j+=2){
		
		DS("Checking: ");
		DB(j);
		
		uint8_t state;
		if ((state=i2c_start(j+I2C_READ)&0xF8) == 0x40){
			DS("\t");
			DB(j);
			DS(" READ");
			table[(j+I2C_READ)>>3] |= 1<<((j+I2C_READ)&0x7);
		}
		if ((state=i2c_start(j+I2C_WRITE)&0xF8) == 0x18){
			DS("\t");
			DB(j);
			DS(" WRITE");
			table[(j+I2C_WRITE)>>3] |= 1<<((j+I2C_WRITE)&0x7);
		}
		
		if(!((table[(j+I2C_READ)>>3]&(1<<((j+I2C_READ)&0x7)))||
			(table[(j+I2C_WRITE)>>3]&(1<<((j+I2C_WRITE)&0x7)))) ){
			DS(TERM_LN_ERASE"\r");
		} else {
       		DS("\r\n");  
		}
	}	
	i2c_stop();
}

/****************************************************
 *  eeprom_dump_page()
 * **************************************************/
void eeprom_dump_page(i2c_addr_t dev, uint32_t start, uint16_t length){
	uint32_t i=0;
	uint8_t buffer[ROW_SIZE];
    uint8_t j=0;
	DS("EEPROM-Dump (Page-Mode):\r\n"); 
	for (i=start; i<(start+length-ROW_SIZE); i+=ROW_SIZE){
		E24C_block_read(dev, i, buffer, ROW_SIZE);
		DS("0x");
		DB((i>>24)&0xff);
		DB((i>>16)&0xff);
		DB((i>> 8)&0xff);
		DB((i>> 0)&0xff);
		DS(":");
		for (j=0; j<ROW_SIZE; ++j){
			DC(' ');
			DB(buffer[j]);
		}
		DC('\t');
		for (j=0; j<ROW_SIZE; ++j){
			DC((buffer[j]<32)?'.':buffer[j]);
		}
       DS("\r\n");  
	}
	E24C_block_read(dev, i, buffer, (start+length-i));
	DS("0x");
	DB((i>>24)&0xff);
	DB((i>>16)&0xff);
	DB((i>> 8)&0xff);
	DB((i>> 0)&0xff);
	DC(':');
	for (j=0; j<ROW_SIZE; ++j){
		DC(' ');
		if (j<(start+length-i)){
			DB(buffer[j]);
		} else {
			DC(' ');
			DC(' ');
		} 
	}
	DC('\t');
	for (j=0; j<start+length-i; ++j){
		DC((buffer[j]<32)?'.':buffer[j]);
	}
    DS("\r\n");  
	
}


/****************************************************
 *  crypto_eeprom_dump()
 * **************************************************/
void crypto_eeprom_dump(uint32_t start, uint16_t length){
	uint32_t i=0;
	uint8_t buffer[ROW_SIZE];
    uint8_t j=0;
    uint8_t crypt_key[32];
    
    load_eeprom_crypt_key(crypt_key);
    DS("EEPROM-Dump (Crypto-Mode):\r\n"); 
	for (i=start; i<(start+length-ROW_SIZE); i+=ROW_SIZE){
		crypto_read_block(buffer, i, ROW_SIZE, crypt_key);
		DS("0x");
		DB((i>>24)&0xff);
		DB((i>>16)&0xff);
		DB((i>> 8)&0xff);
		DB((i>> 0)&0xff);
		DS(":");
		for (j=0; j<ROW_SIZE; ++j){
			DC(' ');
			DB(buffer[j]);
		}
		DC('\t');
		for (j=0; j<ROW_SIZE; ++j){
			DC((buffer[j]<32)?'.':buffer[j]);
		}
        DS("\r\n");  
	}
	crypto_read_block(buffer, i, (start+length-i), crypt_key);
	DS("0x");
	DB((i>>24)&0xff);
	DB((i>>16)&0xff);
	DB((i>> 8)&0xff);
	DB((i>> 0)&0xff);
	DC(':');
	for (j=0; j<ROW_SIZE; ++j){
		DC(' ');
		if (j<(start+length-i)){
			DB(buffer[j]);
		} else {
			DC(' ');
			DC(' ');
		} 
	}
	DC('\t');
	for (j=0; j<start+length-i; ++j){
		DC((buffer[j]<32)?'.':buffer[j]);
	}
    DS("\r\n");  
	
}

/****************************************************
 *  eeprom_set_byte()
 * **************************************************/
void eeprom_set_byte(i2c_addr_t dev, uint16_t start, uint16_t length){
	uint16_t i=0;
	uint8_t buffer[ROW_SIZE];
    uint8_t j=0;	
	DS("EEPROM-Setting:\r\n");
	for (i=start; i<start+length; i+=ROW_SIZE){
		for (j=0; j<ROW_SIZE; ++j){
			DB(buffer[j]=LOW((i+j)));
            	DC(' ');	
        		E24C_byte_write(dev, i+j, buffer[j]);
         	i2c_ack_poll(dev+I2C_WRITE);
            }
        DS("\r\n");  
    }
}


/****************************************************
 *  eeprom_set_page()
 * **************************************************/
void eeprom_set_page(i2c_addr_t dev, uint16_t start, uint16_t length){
	uint16_t i=0;
	uint8_t buffer[ROW_SIZE];
    uint8_t j=0;
	DS("EEPROM-Setting:\r\n");
	for (i=start; i<start+length; i+=ROW_SIZE){
		for (j=0; j<ROW_SIZE; ++j){
			DB(buffer[j]=LOW((i+j)));
            	DC(' ');	
        		}
        DS("\r\n");  
        E24C_page_write(dev, i, buffer, ROW_SIZE);
		i2c_ack_poll(dev+I2C_WRITE);
    }
}



/******************************************************************************/
void getnames(void ** data){
	*data = malloc(2*sizeof(void*));
	((char**)*data)[0] = malloc(8+1);
	((char**)*data)[1] = malloc(32+1);
	DS("\r\n nickname (max. 8 chars): ");
	console_getnstr( 8, ((char**)*data)[0]);
	DS("\r\n realname (max. 32 chars): ");
	console_getnstr(32, ((char**)*data)[1]);
}

void getuid(void ** data){
	char str[6];
	*data = malloc(2);
	DS("\r\n uid (hex, lsb first): ");
	console_getnstr( 5, str);
	asciihex2values((uint8_t*)(*data), str, 4);
}


void console_setadmin(void){
	userflags_t sf,cf;
	char * nick;
	
	sf.admin = sf.exist = sf.locked = sf.notify_lostadmin = sf.reserved = 0;
	cf.admin = cf.exist = cf.locked = cf.notify_lostadmin = cf.reserved = 0;
	sf.admin = 1;
	console_getnick(&nick);
	modify_account(nick, sf, cf);
	free(nick);
	DS("\r\n now admin flag is set");
}

void console_clearadmin(void){
	userflags_t sf,cf;
	char * nick;
	uint8_t key[32];
	uint8_t hmac[32];
	
	sf.admin = sf.exist = sf.locked = sf.notify_lostadmin = sf.reserved = 0;
	cf.admin = cf.exist = cf.locked = cf.notify_lostadmin = cf.reserved = 0;
	cf.admin = 1;
	console_getnick(&nick);
	load_nickkey(key);
	hmac_sha256(hmac, key, 256, nick, strlen(nick));
	delete_key(key, 32);
	free(nick);
	flmdb_makeentry(hmac, sf, cf, 0);
	
	DS("\r\n now admin flag is cleared");
}











