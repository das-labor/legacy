

#include "config.h"
#include "debug.h"
#include "uart.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h> // itoa()
#include <avr/eeprom.h>
#include <stdlib.h>
#include "24Cxxx.h"
#include "i2c_tools.h"
#include "main_test_tools.h"
#include "ticketDB.h"
#include "flmDB.h"
#include "prng.h"
#include "hmac-sha256.h"
#include "sha256.h"
#include "action.h"
#include "types.h"
#include "keys.h"
#include "reqvalidator.h"

/*
#include "mcp9800.h"
*/
/****************************************************
 ****************************************************
 *  FUNCTIONS
 ****************************************************
 ****************************************************/

/****************************************************
 *  main_tools_init()
 * **************************************************/
 

/******************************************************************************/

uint8_t getbadrandom(void){
	ADCSRA |= 0x40 | 0x80; /* start conversion */
	while(ADCSRA & 0x40)   /* wait for conversation to complete */
		;
	return (uint8_t)ADC;
}

/******************************************************************************/

void prng_init(void){
	/* here we should add some entropy to the prng */
	uint8_t i,j;
	uint8_t b[64];
	for(i=0; i<32; ++i){
		for(j=0; j<64; ++j){
			b[j] = getbadrandom();
		}
		addEntropy(64*8,&b);
	}
}

/******************************************************************************/
 
void main_tools_init(){
    i2c_init();
    E24C_init();
    prng_init();
    ticketdb_init();
}

/****************************************************
 *  dump_dbstats()
 * **************************************************/
void dump_dbstats(void){
	uint16_t i;
	uart_putstr_P(PSTR("\r\n"));
	i = ticketdb_getstatMaxUsers();
	uart_putstr_P(PSTR("MaxUsers:")); uart_putbyte(i>>8); uart_putbyte(i&0xff); uart_putstr_P(PSTR("\r\n"));
	i = ticketdb_getstatUsers();
	uart_putstr_P(PSTR("Users:")); uart_putbyte(i>>8); uart_putbyte(i&0xff); uart_putstr_P(PSTR("\r\n"));
	i = ticketdb_getstatAdmins();
	uart_putstr_P(PSTR("Admins:")); uart_putbyte(i>>8); uart_putbyte(i&0xff); uart_putstr_P(PSTR("\r\n"));
	i = ticketdb_getstatLockedUsers();
	uart_putstr_P(PSTR("Locked Users:")); uart_putbyte(i>>8); uart_putbyte(i&0xff); uart_putstr_P(PSTR("\r\n"));
	i = ticketdb_getstatLockedAdmins();
	uart_putstr_P(PSTR("Lockes Admins:")); uart_putbyte(i>>8); uart_putbyte(i&0xff); uart_putstr_P(PSTR("\r\n"));
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
			uart_putc(c);
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
			++src; ++n;
		}
		*dest  = charhex2value(*src++) << 4;
		while(*src==' '){
			++src; ++n;
		}
		*dest |= charhex2value(*src++);
		dest++;
		n -= 2;
	}
}


void console_getnick(void * data){
	char rcvbuffer[101];
	uart_putstr_P(PSTR("\r\n nickname: "));
	console_getnstr( 30, rcvbuffer);
	*((char**)data) = malloc(strlen(rcvbuffer));
	strncpy(*((char**)data), rcvbuffer, 101);
}


/****************************************************
 *  console_verifyuser()
 ****************************************************/
void console_verifyuser(void){
	char * nick;
	authblock_t ab;
	
	console_getnick(&nick);
	new_account(&ab, nick);
	free(nick);
	
}

/****************************************************
 *  console_adduser()
 * **************************************************/
void console_adduser(void){
	char * nick;
	authblock_t ab;
	uart_putstr_P(PSTR("\r\n new user"));
	console_getnick(&nick);
	new_account(&ab, nick);
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
	uart_putstr_P(PSTR("\r\n*** add user ***\r\n"));
	r = ticketdb_newuser(&mac, &uid, 0);
	uart_putstr("{"); uart_putbyte(r); uart_putstr("}\r\n");
}

/****************************************************
 *  console_getauthblock()
 * **************************************************/
void console_getauthblock(authblock_t *ab){
	char rcvbuffer[101];

	uart_putstr_P(PSTR("\r\n uid :"));
	console_getnstr(6, rcvbuffer);
	asciihex2values((uint8_t*)&(ab->uid),rcvbuffer, 4);
	uart_putstr(" {"); uart_hexdump((char*)&(ab->uid),2);uart_putstr("}");

	uart_putstr_P(PSTR("\r\n ticket :"));
	console_getnstr(100, rcvbuffer);
	asciihex2values( ab->ticket,rcvbuffer, 64);
	uart_putstr("\r\n {"); uart_hexdump((char*)(ab->ticket),32);uart_putstr("}");

	uart_putstr_P(PSTR("\r\n rkey :"));
	console_getnstr(100, rcvbuffer);
	asciihex2values( ab->rkey,rcvbuffer, 64);
	uart_putstr("\r\n {"); uart_hexdump((char*)(ab->rkey),32);uart_putstr("}");

	uart_putstr_P(PSTR("\r\n rid :"));
	console_getnstr(100, rcvbuffer);
	asciihex2values( ab->rid,rcvbuffer, 64);
	uart_putstr("\r\n {"); uart_hexdump((char*)(ab->rid),32);uart_putstr("}");

	uart_putstr_P(PSTR("\r\n hmac :"));
	console_getnstr(100, rcvbuffer);
	asciihex2values( ab->hmac,rcvbuffer, 64);
	uart_putstr("\r\n {"); uart_hexdump((char*)(ab->hmac),32);uart_putstr("}");
}

/****************************************************
 *  console_getauthblock()
 * **************************************************/
void console_dumpauthblock(authblock_t *ab){
	char rcvbuffer[101];

	uart_putstr_P(PSTR("\r\n uid:    "));
	uart_hexdump(&(ab->uid), 2);
	
	uart_putstr_P(PSTR("\r\n ticket: "));
	uart_hexdump(ab->ticket, 32);
	
	uart_putstr_P(PSTR("\r\n rkey:   "));
	uart_hexdump(ab->rkey, 32);

	uart_putstr_P(PSTR("\r\n rid:    "));
	uart_hexdump(ab->rid, 32);

	uart_putstr_P(PSTR("\r\n hmac:   "));
	uart_hexdump(ab->hmac, 32);
}

/****************************************************
 *  console_getauthcredentials()
 * **************************************************/

uint8_t console_getauthcredentials(authblock_t **ab){
	uint8_t n=0;
	while(n<255){
		uart_putstr_P(PSTR("\r\n press 1 to submit additional credentials"
		                   "\r\n press 2 to continue"));
		switch(uart_getc()){
			case '1': break;
			case '2': return n;
			default:  continue; 
		}
		*ab=realloc(*ab,(n+1)*sizeof(authblock_t));
		if(!*ab)
			uart_putstr_P(PSTR("\r\n BOOOM"));
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
	uart_putstr_P(PSTR("\r\n*** console debug ***\r\npress 'q' to leave\r\n"));
	do {
		c = uart_getc();
		uart_putstr_P(PSTR("Got char: 0x"));
		uart_putbyte(c);
		uart_putstr_P(PSTR(" ("));
		if(c>=32){
			uart_putc(c);
		} else {
			uart_putstr_P(PSTR("<32"));
		}
		uart_putstr_P(PSTR(" )\r\n"));
	} while (c != 'q');
}


/****************************************************
 *  uart_putbyte()
 * **************************************************/
void uart_putbyte(uint8_t b){
	char table[]={'0','1','2','3','4','5','6','7',
				  '8','9','A','B','C','D','E','F'};
	uart_putc(table[b>>4]);
	uart_putc(table[b&0xF]);
}


/****************************************************
 *  i2c_detect()
 * **************************************************/
void i2c_detect(i2c_detect_t table){
	uint8_t j;
	for(j=0; j<32; ++j)
		table[j]=0;
	uart_putstr_P(PSTR("Detecting devices:\r\n"));
	for (j=0; j<254; j+=2){
		
		uart_putstr("Checking: ");
		uart_putbyte(j);
		
		uint8_t state;
		if ((state=i2c_start(j+I2C_READ)&0xF8) == 0x40){
			uart_putstr("\t");
			uart_putbyte(j);
			uart_putstr(" READ");
			table[(j+I2C_READ)>>3] |= 1<<((j+I2C_READ)&0x7);
		}
		if ((state=i2c_start(j+I2C_WRITE)&0xF8) == 0x18){
			uart_putstr("\t");
			uart_putbyte(j);
			uart_putstr(" WRITE");
			table[(j+I2C_WRITE)>>3] |= 1<<((j+I2C_WRITE)&0x7);
		}
		
		if(!((table[(j+I2C_READ)>>3]&(1<<((j+I2C_READ)&0x7)))||
			(table[(j+I2C_WRITE)>>3]&(1<<((j+I2C_WRITE)&0x7)))) ){
			uart_putstr(TERM_LN_ERASE"\r");
		} else {
       		uart_putstr_P(PSTR("\r\n"));  
		}
	}	
	i2c_stop();
}


/****************************************************
 *  eeprom_dump_byte()
 * **************************************************/
void eeprom_dump_byte(i2c_addr_t dev, uint16_t start, uint16_t length){
	uint16_t i=0;
    uint8_t j=0;
    uart_putstr_P(PSTR("EEPROM-Dump (Byte-Mode):\r\n"));
    for (i=start; i<(start+length); i+=ROW_SIZE){
	    uart_putstr("0x");
	    uart_putbyte(HIGH(i));
	    uart_putbyte(LOW(i));
	    uart_putstr(":");
	    for (j=0; j<ROW_SIZE; ++j){
	   // fix this
	    		uart_putc(' ');
			uart_putbyte(E24C_byte_read(dev, i+j));
	    }
        uart_putstr_P(PSTR("\r\n"));  
    } 
}


/****************************************************
 *  eeprom_dump_page()
 * **************************************************/
void eeprom_dump_page(i2c_addr_t dev, uint16_t start, uint16_t length){
	uint16_t i=0;
	uint8_t buffer[ROW_SIZE];
    uint8_t j=0;
	uart_putstr_P(PSTR("EEPROM-Dump (Page-Mode):\r\n")); 
	for (i=start; i<(start+length-ROW_SIZE); i+=ROW_SIZE){
		E24C_block_read(dev, i, buffer, ROW_SIZE);
		uart_putstr("0x");
		uart_putbyte(HIGH(i));
		uart_putbyte(LOW(i));
		uart_putstr(":");
		for (j=0; j<ROW_SIZE; ++j){
			uart_putc(' ');
			uart_putbyte(buffer[j]);
		}
		uart_putc('\t');
		for (j=0; j<ROW_SIZE; ++j){
			uart_putc((buffer[j]<32)?'.':buffer[j]);
		}
        uart_putstr_P(PSTR("\r\n"));  
	}
	E24C_block_read(dev, i, buffer, (start+length-i));
	uart_putstr_P(PSTR("0x"));
	uart_putbyte(HIGH(i));
	uart_putbyte(LOW(i));
	uart_putc(':');
	for (j=0; j<ROW_SIZE; ++j){
		uart_putc(' ');
		if (j<(start+length-i)){
			uart_putbyte(buffer[j]);
		} else {
			uart_putc(' ');
			uart_putc(' ');
		} 
	}
	uart_putc('\t');
	for (j=0; j<start+length-i; ++j){
		uart_putc((buffer[j]<32)?'.':buffer[j]);
	}
    uart_putstr_P(PSTR("\r\n"));  
	
}


/****************************************************
 *  eeprom_set_byte()
 * **************************************************/
void eeprom_set_byte(i2c_addr_t dev, uint16_t start, uint16_t length){
	uint16_t i=0;
	uint8_t buffer[ROW_SIZE];
    uint8_t j=0;	
	uart_putstr_P(PSTR("EEPROM-Setting:\r\n"));
	for (i=start; i<start+length; i+=ROW_SIZE){
		for (j=0; j<ROW_SIZE; ++j){
			uart_putbyte(buffer[j]=LOW((i+j)));
            	uart_putc(' ');	
        		E24C_byte_write(dev, i+j, buffer[j]);
         	i2c_ack_poll(dev+I2C_WRITE);
            }
        uart_putstr_P(PSTR("\r\n"));  
    }
}


/****************************************************
 *  eeprom_set_page()
 * **************************************************/
void eeprom_set_page(i2c_addr_t dev, uint16_t start, uint16_t length){
	uint16_t i=0;
	uint8_t buffer[ROW_SIZE];
    uint8_t j=0;
	uart_putstr_P(("EEPROM-Setting:\r\n"));
	for (i=start; i<start+length; i+=ROW_SIZE){
		for (j=0; j<ROW_SIZE; ++j){
			uart_putbyte(buffer[j]=LOW((i+j)));
            	uart_putc(' ');	
        		}
        uart_putstr_P(PSTR("\r\n"));  
        E24C_page_write(dev, i, buffer, ROW_SIZE);
		i2c_ack_poll(dev+I2C_WRITE);
    }
}




/****************************************************
 *  thermo_dump()
 * **************************************************/
/*
void thermo_dump(i2c_addr_t dev){
	char c, tempstr[20];
	mcp9800_temp_t temp, temp_old;
	uart_putstr("\r\n*** Thermo dump ***\n\r");
	uart_putstr("Config-Reg: 0x");
	uart_putbyte(i2c_get_8bit_reg(dev,conf_reg));
	uart_putstr("\r\n");
	uart_putstr("Stetting 12-bit resolution.\r\n");
	mcp9800_set_resolution(dev,mcp9800_12bit);
	while (!uart_getc_nb(&c)){	
		temp_old = mcp9800_get_temp(dev);
		uart_putstr("temperature: 0x");
		uart_putbyte(HIGH(temp));
		uart_putstr(".0x");
		uart_putbyte(LOW(temp));
		uart_putstr(" (");
		uart_putstr(itoa((int)HIGH(temp),tempstr, 10));
		uart_putstr(".");
		if (LOW(temp)<100)
			uart_putc('0');
		if (LOW(temp)<10)
			uart_putc('0');
		uart_putstr(utoa((int)LOW(temp),tempstr, 10));
		uart_putstr(")");
		do{
			temp = mcp9800_get_temp(dev);
		}while(temp==temp_old);
		//uart_putstr(TERM_LN_ERASE"\r");
		uart_putc('\r');
	}
	
	uart_putstr("\r\n");
}
*/

/******************************************************************************/
void getnames(void ** data){
	*data = malloc(2*sizeof(void*));
	((char**)*data)[0] = malloc(8+1);
	((char**)*data)[1] = malloc(32+1);
	uart_putstr_P(PSTR("\r\n nickname (max. 8 chars): "));
	console_getnstr( 8, ((char**)*data)[0]);
	uart_putstr_P(PSTR("\r\n realname (max. 32 chars): "));
	console_getnstr(32, ((char**)*data)[1]);
}

void getuid(void ** data){
	char str[6];
	*data = malloc(2);
	uart_putstr_P(PSTR("\r\n uid (hex, lsb first): "));
	console_getnstr( 5, str);
	asciihex2values((uint8_t*)(*data), str, 4);
}

void console_toggleadmin(){
	uid_t *uid;
	userflags_t flags;
	
	getuid(&uid);
	ticketdb_getUserFlags(*uid,&flags);
	flags.admin ^= 1;
	uart_hexdump(uid, 2);
	uart_putstr_P(PSTR("\r\n now admin flag is "));
	if(!flags.admin)
		 uart_putstr_P(PSTR("not "));
	uart_putstr_P(PSTR("set"));
	ticketdb_setUserFlags(*uid, &flags);
	free(uid);
}

/****************************************************
 *  console_lasim()
 * **************************************************/
void console_lasim(void){
	void * data;
	action_t action=mainopen;
	authcredentials_t * authcreds=0;
	uint8_t n;
	
	switch(uart_getc()){
		case '1': action = mainopen;   break;
		case '2': action = mainclose;  break;
		case '3': action = adduser;    console_getnick(&data); break;
		case '4': action = remuser;    console_getnick(&data); break;
		case '5': action = lockuser;   console_getnick(&data); break;
		case '6': action = unlockuser; console_getnick(&data); break;
		case '7': action = addadmin;   console_getnick(&data); break;
		case '8': action = remadmin;   console_getnick(&data); break;
		case '9': action = keymigrate; break;		
		default: break;
	}
	
	n = console_getauthcredentials(&authcreds);
	//authcreds[0].uid = 0x0000;
	uart_putstr_P(PSTR("\r\n[0].uid = "));
	uart_hexdump(&(authcreds[0].uid), 2);
	
	if(valid_authreq(action,n,authcreds)){
		/* give new credetials!!! */
		uart_putstr_P(PSTR("\r\n data = "));
		uart_hexdump(&data, 2);
		uart_putstr_P(PSTR("\r\n data[0] = "));
		uart_hexdump(((uint8_t*)data)[0], 2);
		perform_action(action, data);
	}else{
		uart_putstr_P(PSTR("\r\n*** verification failed ***"));
	}
	free(data);
	
}











