/**
 * \file	qport.c
 * \author	Daniel Otte
 * \date	29.10.2006
 * \par License:
 * GPL
 * \brief security layer for charachter based point-to-point conection based on preshared keys  
 * 
 */

#include <stdint.h>
#include <stdlib.h> /* malloc(), free() & Co.*/
#include <string.h> /* memset() */
#include <util/delay.h> /* delays for debugging */


#include "config.h"
#include "error-handling.h"
#include "sha256.h"
#include "qcrypt.h"
#include "prng.h"
#include "rtc.h"
#include "persistentCounter.h"
#include "qstructure.h"
#include "qport.h"

#include "uart.h"
#undef DEBUG
#include "debug.h"
#include <avr/io.h>

#ifdef DISPLAY
	#include "hd44780.h"
	#define display_init hd44780_init
	#define display_print hd44780_print
	#define display_print_P hd44780_print_P
	#define display_putc  hd44780_data
	#define display_set_cursor hd44780_set_cursor
	#define display_clear_line hd44780_clear_line
	#define display_command hd44780_command
	
	void display_dump(uint8_t line, void* buffer, uint8_t size){
		char hextab[]="0123456789ABCDEF";
		display_set_cursor(line,0);
		while(size){
			display_putc(hextab[(*((uint8_t*)buffer))>>4]);
			display_putc(hextab[(*((uint8_t*)buffer))&0xf]);
			++buffer;
			--size;
		}
	}
#else
	#define display_init() 
	#define display_print(s) 
	#define display_print_P(s) 
	#define display_putc(s)  
	#define display_set_cursor(a,b)
	#define display_clear_line(s) 
	#define display_command(s)
	#define display_dump(a,b,c)
#endif


/* ugly things: */
#define inc_counter percnt_inc
#define get_counter percnt_get
#define sendrawbyte(b) uart_putc(b)

/* some local prototypes */
verify_t check_counter(void* buffer); /* implemented further down */
void recv_keyack(uint8_t b);
void recv_ackack(uint8_t b);

bool escaped;

enum qp_main_state_e {unkeyed, keying, wait_keyack, keyack ,idle, msg, stream} main_state;

void (*msgrx_hook) (uint16_t length, uint8_t* buffer) = (void*)0;


/******************************************************************************/

uint8_t		msgrx_len_cnt=0; 
uint16_t	msgrx_len;
uint8_t*	msgrx_buffer=(void*)0;
uint16_t	msgrx_buffer_idx;
uint8_t		msgrx_sig_idx=0;
uint8_t		msgrx_sig_buffer[SIG_SIZE];

static uint8_t display_init_stat=0;

#ifdef QP_TERMINAL
uint8_t *ackackrx_buffer	= (void*)0;
uint8_t ackackrx_buffer_idx	= 0;

uint8_t*	keyrx_buffer	 = (void*)0;
uint8_t		keyrx_buffer_idx = 0;
#endif
#ifdef QP_SERVER
uint8_t *keyack_value_buffer	= (void*)0;
uint8_t *keyackrx_buffer		= (void*)0;
uint8_t keyackrx_idx			= 0;
#endif


/******************************************************************************/

void sendbyte(uint8_t b){
	switch (b){
	case QP_RESET_CODE:
		sendrawbyte(QP_ESC_CODE);
		sendrawbyte(QP_RESET_ESC);
		break;
	#ifdef	QP_XON_XOFF_SUPPORT
	case QP_XON_CODE:
		sendrawbyte(QP_ESC_CODE);
		sendrawbyte(QP_XON_ESC);
		break;
	case QP_XOFF_CODE:
		sendrawbyte(QP_ESC_CODE);
		sendrawbyte(QP_XOFF_ESC);
		break;
	#endif
	default:
		sendrawbyte(b);
	}
	if(b==QP_ESC_CODE)
		sendrawbyte(QP_ESC_ESC);
}


/*******************************************************************************
*******************************************************************************/
void qp_byterx(uint8_t b){
	if (b==QP_RESET_CODE){
		// /* RESET EVRYTHING * /
		escaped=false;
		main_state=unkeyed;
		if (!display_init_stat){
			display_init();
			display_clear_line(1);
			display_clear_line(0);
			display_init_stat=1;
		}
		display_putc('-');
	} else {	// * no reset code * /
		if(escaped){
			escaped=false;
			switch(b){
				case QP_RESET_ESC:
					b=QP_RESET_CODE;
					goto data_process;
					break;
				case QP_ESC_ESC:
					b=QP_ESC_CODE;
					goto data_process;
					break;
			#ifdef QP_XON_XOFF_SUPPORT
				case QP_XON_ESC:
					b=QP_XON_CODE;
					goto data_process;
					break;
				case QP_XOFF_ESC:
					b=QP_XOFF_CODE;
					goto data_process;
					break;	
			#endif	/* XON_XOFF_SUPPORT */	
		#ifdef QP_SERVER
				case QP_TYPE_KEYREQ:
					display_putc('T');
					main_state=unkeyed;
					rekey();
					display_putc('t');
					break;
				case QP_TYPE_KEYACK:	
					if(main_state!=wait_keyack){
						display_putc('#');
						error(ERROR_INVALID_STATE_TRASITION);
					}
					main_state=keyack;
					break;
		#endif
		#ifdef QP_TERMINAL
				case QP_TYPE_KEYTX:
					display_putc('R');
			//		if(main_state!=unkeyed){
			//			display_putc('#');error(ERROR_INVALID_STATE_TRASITION);
			//		}
					main_state = keying;
					break;
				case QP_TYPE_ACKACK:	
					if(main_state!=wait_keyack){
						display_putc('#');error(ERROR_INVALID_STATE_TRASITION);
					}
					main_state=keyack;
					break;
		#endif
				case QP_TYPE_SECMSG:
					display_putc('M');
					if(main_state!=idle){
						display_putc('#');error(ERROR_INVALID_STATE_TRASITION);
					}
					main_state=msg;
					break;
				case QP_TYPE_SECSTR:
					if(main_state!=idle){
						display_putc('#');error(ERROR_INVALID_STATE_TRASITION);
					}
					break;
				case QP_TYPE_EOSTR:
					if(main_state!=stream){
						display_putc('#');error(ERROR_INVALID_STATE_TRASITION);
					}
					break;
				default: error(ERROR_ILLEGAL_SEQUENCE);
			}
		}else{ //* not escaped normal code * /
			if(b==QP_ESC_CODE){
				escaped=true;
			} else {
				//* most bytes will go here * /
			data_process:
				switch(main_state){
					case unkeyed:
					case idle:
						display_putc('\'');
					//	error(ERROR_IDLE_RECV); //* we might drop it silently or reset or ... * /
						break;
				#ifdef QP_TERMINAL			
					case keying:
						keyingrx(b);
						break;
					case keyack:
						recv_ackack(b);
						break;	
				#endif
				#ifdef QP_SERVER			
					case keyack:
						recv_keyack(b);
						break;
				#endif
					case msg: 
						msgrx(b);
						break;
					case stream:
						streamrx(b);
						break;
					default: display_putc('#');error(ERROR_INVALID_VAR);
				} //* switch main_state * /
			} //* if(b==QP_ESC_CODE) ... else ... * /
		} //* if(escaped) ... else ... * /
	} //* if (b==QP_RESET_CODE) ... else ... *  /
};

/********************************************************************************************************/

/* Message:
 * 
 * +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
 * | T=MSG |  L_0  |  L_1  |  D_0  |  D_1  |  ...  |  D_l  |  S_0  |  S_1  |  ...  |  S_sz | 
 * +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
 *     P       C       C       C       C       C       C
 * 
 */
/* 
uint8_t		msgrx_len_cnt=0; 
uint16_t	msgrx_len;
uint8_t*	msgrx_buffer=(void*)0;
uint16_t	msgrx_buffer_idx;
uint8_t		msgrx_sig_idx=0;
uint8_t		msgrx_sig_buffer[SIG_SIZE];
*/

void msgrx(uint8_t b){
	b = q_decipher(b);
	if(msgrx_len_cnt<2){ /* reciving length */
		((uint8_t*)&msgrx_len)[msgrx_len_cnt++] = b;
		if (msgrx_len_cnt==2){
			if(msgrx_buffer){
				free(msgrx_buffer); /* free old memory */
			}
			if(!(msgrx_buffer = malloc(msgrx_len * sizeof(uint8_t)))){
				display_putc('/'); while(1);
				error(ERROR_MALLOC_FAILED);
			}
			msgrx_buffer_idx = 0;
			msgrx_sig_idx = 0;
		}
	} else {			/**/
		if(msgrx_buffer_idx<msgrx_len){
			/* load data bytes */
			msgrx_buffer[msgrx_buffer_idx++] = b;
		} else {
			/* load signature */
			if(msgrx_sig_idx<SIG_SIZE){
				msgrx_sig_buffer[msgrx_sig_idx++] = b;
			}else{
				display_putc('?'); while(1);
				error(ERROR_MSG_TOO_LONG);
			}
		}
	}
	if(msgrx_sig_idx==SIG_SIZE){
		msgrx_len_cnt=0;
		msgrx_sig_idx=0;
		
	//	display_putc('n');
		if(q_verify_std(msgrx_buffer, msgrx_len*8, msgrx_sig_buffer)==failed){
			display_putc('z'); while(1);
			error(ERROR_BAD_MSG_SIG);
		}
	/*	gotmsg(); */
		main_state=idle;
	//	display_putc('m');
		if(msgrx_hook){
			msgrx_hook(msgrx_len, msgrx_buffer);
		}
	}
}

/********************************************************************************************************/

void msgtx(uint16_t length, uint8_t *msg){
	uint8_t signature[SIG_SIZE];	
	uint8_t i;
	
	memset(signature,0, SIG_SIZE);
	if(main_state==unkeyed){
		rekey();
	}
	
	sendrawbyte(QP_ESC_CODE);
	sendrawbyte(QP_TYPE_SECMSG);
	sendbyte(q_encipher(length&0xff));
	sendbyte(q_encipher(length>>8));
	q_sign_std(signature, (void*)msg, length*8);
	for(;length!=0;--length){
		sendbyte(q_encipher(*msg++));
	}
	for(i=0; i<SIG_SIZE; ++i){
		 sendbyte(q_encipher(signature[i]));
	}
}

/********************************************************************************************************/

void streamrx(uint8_t b){
	/* still undecided what to do and how to ensure integrity */
}

/********************************************************************************************************/

#ifdef QP_TERMINAL
#ifdef QP_HASHKEYS
void setkeys_kp_terminal(uint8_t* buffer){
	sha256_hash_t hash;


	/* setup RX keys for sys A */
	sha256(&hash, buffer, (KP_KRA_OFFSET+KEYA_SIZE)*8);	
	memset(keyA_rx, 0, KEYA_SIZE);
	memxor(keyA_rx, hash, KEYA_SIZE, sizeof(hash));
	/* setup TX keys for sys A */
	sha256(&hash, buffer, (KP_KTA_OFFSET+KEYA_SIZE)*8);	
	memset(keyA_tx, 0, KEYA_SIZE);
	memxor(keyA_tx, hash, KEYA_SIZE, sizeof(hash));
	/* setup RX keys for sys B */
	sha256(&hash, buffer, (KP_KRB_OFFSET+KEYB_SIZE)*8);		
	memset(keyB_rx, 0, KEYB_SIZE);
	memxor(keyB_rx, hash, KEYB_SIZE, sizeof(hash));
	/* setup TX keys for sys B */
	sha256(&hash, buffer, (KP_KTB_OFFSET+KEYB_SIZE)*8);	
	memset(keyB_tx, 0, KEYB_SIZE);
	memxor(keyB_tx, hash, KEYB_SIZE, sizeof(hash));
	#ifdef QP_MSG_CHECK_HMAC
	/* setup TX keys for signature sys */
	sha256(&hash, buffer, (KP_KRS_OFFSET+KEYS_SIZE)*8);		
	memset(keyS_rx, 0, KEYS_SIZE);
	memxor(keyS_rx, hash, KEYS_SIZE, sizeof(hash));
	/* setup TX keys for signature sys */
	sha256(&hash, buffer, (KP_KTS_OFFSET+KEYS_SIZE)*8);		
	memset(keyS_tx, 0, KEYA_SIZE);
	memxor(keyS_tx, hash, KEYA_SIZE, sizeof(hash));
	#endif	
	q_stream_crypt_init();
}
#else
void setkeys_kp_terminal(uint8_t* buffer){
	memcpy(keyA_rx, buffer+KP_KRA_OFFSET, KEYA_SIZE);
	memcpy(keyA_tx, buffer+KP_KTA_OFFSET, KEYA_SIZE);
	memcpy(keyB_rx, buffer+KP_KRB_OFFSET, KEYB_SIZE);
	memcpy(keyB_tx, buffer+KP_KTB_OFFSET, KEYB_SIZE);
	#ifdef QP_MSG_CHECK_HMAC	
	memcpy(keyS_rx, buffer+KP_KRS_OFFSET, KEYS_SIZE);
	memcpy(keyS_tx, buffer+KP_KTS_OFFSET, KEYS_SIZE);
	#endif	
	q_stream_crypt_init();
}
#endif /* def QP_HASHKEYS ... ELSE ... */
#endif /* def QP_TERMINAL */

/********************************************************************************************************/

#ifdef QP_SERVER
#ifdef QP_HASHKEYS
void setkeys_kp_server(uint8_t* buffer){
	sha256_hash_t hash;
	/* setup TX keys for sys A */
	sha256(&hash, buffer, (KP_KRA_OFFSET+KEYA_SIZE)*8);	
	memset(keyA_tx, 0, KEYA_SIZE);
	memxor(keyA_tx, hash, KEYA_SIZE, sizeof(hash));
	/* setup RX keys for sys A */
	sha256(&hash, buffer, (KP_KTA_OFFSET+KEYA_SIZE)*8);	
	memset(keyA_rx, 0, KEYA_SIZE);
	memxor(keyA_rx, hash, KEYA_SIZE, sizeof(hash));
	/* setup TX keys for sys B */
	sha256(&hash, buffer, (KP_KRB_OFFSET+KEYB_SIZE)*8);		
	memset(keyB_tx, 0, KEYB_SIZE);
	memxor(keyB_tx, hash, KEYB_SIZE, sizeof(hash));
	/* setup RX keys for sys B */
	sha256(&hash, buffer, (KP_KTB_OFFSET+KEYB_SIZE)*8);	
	memset(keyB_rx, 0, KEYB_SIZE);
	memxor(keyB_rx, hash, KEYB_SIZE, sizeof(hash));
	#ifdef QP_MSG_CHECK_HMAC
	/* setup TX keys for signature sys */
	sha256(&hash, buffer, (KP_KRS_OFFSET+KEYS_SIZE)*8);			
	memset(keyS_tx, 0, KEYS_SIZE);
	memxor(keyS_tx, hash, KEYS_SIZE, sizeof(hash));
	/* setup RX keys for signature sys */
	sha256(&hash, buffer, (KP_KTS_OFFSET+KEYS_SIZE)*8);	
	memset(keyS_rx, 0, KEYS_SIZE);
	memxor(keyS_rx, hash, KEYS_SIZE, sizeof(hash));
	#endif	
	q_stream_crypt_init();
}
#else
void setkeys_kp_server(uint8_t* buffer){
	memcpy(keyA_tx, buffer+KP_KRA_OFFSET, KEYA_SIZE);
	memcpy(keyA_rx, buffer+KP_KTA_OFFSET, KEYA_SIZE);
	memcpy(keyB_tx, buffer+KP_KRB_OFFSET, KEYB_SIZE);
	memcpy(keyB_rx, buffer+KP_KTB_OFFSET, KEYB_SIZE);
	#ifdef QP_MSG_CHECK_HMAC	
	memcpy(keyS_tx, buffer+KP_KRS_OFFSET, KEYS_SIZE);
	memcpy(keyS_rx, buffer+KP_KTS_OFFSET, KEYS_SIZE);
	#endif	
	q_stream_crypt_init();
}
#endif /* def QP_HASHKEYS ... ELSE ... */
#endif /* def QP_TERMINAL */

/********************************************************************************************************/


/*
 * +-------+
 * | T=KEY |
 * +-------+
 * #=====================================================================================================#
 * #                                                                                                     #
 * # +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+   #
 * # | RAND  | RAND  | CNT0  | CNT1  | CNT2  | CNT3  | KTA_0 |  ...  | KTA_n | KTB_0 |  ...  | KTB_m |   #
 * # +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+   #
 * #                                                                                                     #
 * # +-------+-------+-------+-------+-------+-------+-------+-------+-------++-------+-------+-------+  #
 * # | KRA_0 |  ...  | KRA_n | KRB_0 |  ...  | KRB_n |  KS_0 |  ...  |  KS_x ||  S_0  |  ...  |  S_sz |  #
 * # +-------+-------+-------+-------+-------+-------+-------+-------+-------++-------+-------+-------+  #
 * #                                                                                                     #
 * #=====================================================================================================#
 */
#ifdef QP_TERMINAL

 
void rekey(void){
	/* send rekey request */
	display_putc('r');
	sendrawbyte(QP_ESC_CODE);
	sendrawbyte(QP_TYPE_KEYREQ);
}

/******************************************************************************/

verify_t check_counter(void* buffer){
	sha256_hash_t hash;
	uint32_t counter;
	uint8_t i;
	
	counter = *((uint32_t*)(buffer+KP_CNT_OFFSET));
	
	sha256(&hash, buffer, KP_RAND_SIZE*4);
	for(i=0; i<KP_CNT_SIZE; ++i){
		((uint8_t*)&counter)[i] ^= hash[i];
	}
	/* if we would only accept equal counters, we might get a deadlock */
	return (get_counter()<=counter)?ok:failed;
}

/******************************************************************************/

void send_keyack(void* buffer){
	sha256_hash_t hash;
	uint8_t i;
	
	sha256(&hash, buffer, (KP_CNT_OFFSET+KP_CNT_SIZE)*8);
	sha256(&hash, &hash, HASH_SIZE*8); /* being quite unsure if this is good */
	sendrawbyte(QP_ESC_CODE);
	sendrawbyte(QP_TYPE_KEYACK);
	for(i=0; i<HASH_SIZE; ++i){
		sendbyte(hash[i]); /* sending UNENCRYPTED!!! */
	}
	display_putc('A');
}

/******************************************************************************/

void recv_ackack(uint8_t b){
	if(ackackrx_buffer_idx==0){
		if(!(ackackrx_buffer=malloc(2*HASH_SIZE))){
			/* brap ERROR brap */
		}
	}
	ackackrx_buffer[ackackrx_buffer_idx++]=q_decipher(b);
	
	if(ackackrx_buffer_idx==2*HASH_SIZE){
		sha256((void*)ackackrx_buffer, ackackrx_buffer, HASH_SIZE*8);
		if(!memcmp(ackackrx_buffer,ackackrx_buffer+HASH_SIZE,HASH_SIZE)){
			/* everything is fine */
			percnt_inc();
			main_state=idle;
		} else {
			/* got wrong values */
	//		PORTC = 0xF3;
			display_dump(0,ackackrx_buffer, 13);
			display_dump(1,ackackrx_buffer+HASH_SIZE, 13);
			main_state=unkeyed;
		}
		ackackrx_buffer_idx = 0;
		free(ackackrx_buffer);
		ackackrx_buffer = (void*)0;
	}
}

/******************************************************************************/

#ifdef HIGH_SECURE_KEYSETUP
 #error "HIGH SECURE is still unsuported"
#else /* normal mode, simply multiple encrypt, save memory */

#define RECV_ALGOS 3

void keyingrx(uint8_t b){
	/* TODO: protect against flooding/probing of keys */
	static uint8_t n=0;
	
	/* initialising stuff */
	if(!keyrx_buffer){
		if(!(keyrx_buffer = malloc(SIZEOF_KEYPACKET))){
			display_putc('&'); while(1);
			error(ERROR_MALLOC_FAILED);
		}
	}	
	/* load in buffer */
	if(keyrx_buffer_idx>=SIZEOF_KEYPACKET){
		display_putc('%'); while(1);
		error(STUPID_BUT_EVIL_PROGRAMMER_ERROR);
	}
		keyrx_buffer[keyrx_buffer_idx++] = b;
	if(keyrx_buffer_idx==SIZEOF_KEYPACKET){
		/* decrypt */
		q_decipher_keyblock(keyrx_buffer,0);
		q_decipher_keyblock(keyrx_buffer,1);
		q_decipher_keyblock(keyrx_buffer,2);
		/* verify */
		if (q_verify_ltk(keyrx_buffer) == failed){
			display_putc('E'); while(1);
			error (ERROR_BAD_KEYPACKET_SIG);
		}
		if (check_counter(keyrx_buffer) == failed){
			display_putc('$');
			while(1);
			error (ERROR_BAD_COUNTER);
		}
		/* set up keys */
		setkeys_kp_terminal(keyrx_buffer);
		send_keyack(keyrx_buffer);
		/* clean up */
		free(keyrx_buffer);
		keyrx_buffer = (void*)0;
		main_state = wait_keyack;
		n = 0; 
		DEBUG_S("\r\nSuccess");
		display_putc('Y');
		keyrx_buffer_idx = 0;
	}
}

#endif /* HIGH_SECURE_MODE */
#endif /* TERMINAL */

/*****************************************************************************/

#ifdef QP_SERVER

void make_keypacket(void *buffer){
	/* setup keys and counters and mac ... */
	fillBlockRandom(buffer, SIZEOF_KEYPACKET-SIGNATURE_SIZE); /* fill Keyspace with random, (counter will be overwitten) */
	/* copy counter value in pad_buffer */
	*((uint32_t*)(&(((uint8_t*)buffer)[KP_CNT_OFFSET]))) = get_counter();
	/* xor the counter with hash of RAND */
	{
		sha256_hash_t hash;
		uint8_t i;
		
		sha256(&hash, buffer, KP_RAND_SIZE*4);
		for(i=0; i<KP_CNT_SIZE; ++i){
			((uint8_t*)buffer)[KP_CNT_OFFSET+i] ^= hash[i];
		}
	}
	/* sign the packet */
	q_sign_ltk(buffer);
}

/******************************************************************************/

void send_keypacket_buffer(void* buffer){
	uint8_t i;
	sendrawbyte(QP_ESC_CODE);
	sendrawbyte(QP_TYPE_KEYTX);
	for(i=0; i<SIZEOF_KEYPACKET; ++i){
		sendbyte(((uint8_t*)buffer)[i]);
	}
}


/******************************************************************************/

void send_ackack(void){
	sha256_hash_t hash;
	uint8_t i;
	
	fillBlockRandom(hash, HASH_SIZE);
	sendrawbyte(QP_ESC_CODE);
	sendrawbyte(QP_TYPE_ACKACK);
	for(i=0; i<HASH_SIZE; ++i){
		sendbyte(q_encipher(hash[i]));
	}
	sha256(&hash, hash, HASH_SIZE*8);
	for(i=0; i<HASH_SIZE; ++i){
		sendbyte(q_encipher(hash[i]));
	}
}

/******************************************************************************/

void make_keyack_value(void* buffer){
	if(!keyack_value_buffer){
		if(!(keyack_value_buffer=malloc(HASH_SIZE * sizeof(uint8_t)))){
			/* brap ERROR brap */
			// PORTC=0xF1;
			while(1);
		}
	} /* make stricter typeing later ... */
	sha256((void*)keyack_value_buffer, buffer, (KP_CNT_OFFSET+KP_CNT_SIZE)*8);
	sha256((void*)keyack_value_buffer, keyack_value_buffer, HASH_SIZE*8);
}

void recv_keyack(uint8_t b){
	if (keyackrx_idx==0){
		if(!(keyackrx_buffer=malloc(HASH_SIZE *  sizeof(uint8_t)))){
			/* brap ERROR brap */
			while(1);
		}
	}
	keyackrx_buffer[keyackrx_idx++] = b;
	if(keyackrx_idx==HASH_SIZE){
		if(keyack_value_buffer && (!memcmp(keyack_value_buffer,keyackrx_buffer, HASH_SIZE))){
			/* everything is fine */
			free(keyack_value_buffer);
			keyack_value_buffer = (void*)0;
			/* we should be sure that the keys are allready set when sending ackack */
			percnt_inc();
			send_ackack();
			main_state=idle;
			// PORTC ^= 0x20;
		} else {
			/* got corrupted data 
			 * maybe something went wron,
			 * it would be a good idea, to sleep some time and then try an rekey 
			 */
			if(!keyack_value_buffer){
				// PORTC=0xF2;
				while(1);
			}
			while(1); 
			main_state=unkeyed;
		}
		/* clean up */
		free(keyackrx_buffer);
		keyackrx_idx=0;
	}
}

/******************************************************************************/

#ifdef HIGH_SECURE_KEYSETUP
 #error "HIGH_SECURE_KEYSETUP is not implemented at the moment; sorry :-("
#else /* normal mode */

void rekey(void){
	uint8_t *key_buffer;
	key_buffer = malloc(SIZEOF_KEYPACKET);
	if(!key_buffer){
		/* brap ERROR brap */
		while(1);
	}
	/* check if we should really do a rekey */
	make_keypacket(key_buffer);	
	/* TODO: copy the keys to local vars */
	setkeys_kp_server(key_buffer);	
	/* */
	make_keyack_value(key_buffer);
	q_encipher_keyblock(key_buffer, 0);
	q_encipher_keyblock(key_buffer, 1);	
	q_encipher_keyblock(key_buffer, 2);

	send_keypacket_buffer(key_buffer);
	free(key_buffer);
		DEBUG_S("\r\npacket done"); 
	main_state=wait_keyack;
}

#endif /* HIGH_SECURE_MODE ... ELSE ... */
#endif /* QP_SERVER */

