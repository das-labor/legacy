/*
	Simple Operating System for Smartcard Education
	Copyright (C) 2002  Matthias Bruestle <m@mbsks.franken.de>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*! @file
	\brief HAL (Hardware Abstraction Layer).

	$Id: hal.c,v 1.22 2003/03/30 12:42:21 m Exp $
*/

#include <config.h>
#include <crypt.h>
#include <hal.h>
#include <avr/eeprom.h>
#include <avr/io.h>
#include <log.h>
#include <string.h>
#include <tools.h>
#include <types.h>

#if CONF_WITH_TRNG==1
/* Random number gathering */
union {
	uint8_t c[8];
	uint32_t l[2];
} randdata;
uint8_t randbytes;
#endif

void hal_init( void )
{
	ACSR = 0x80; /* outb(0x80,ACSR) */
	DDRA = 0xFF; /* outb(0xFF,DDRA) */
	DDRB = 0xFF; /* outb(0xFF,DDRB) */
	DDRC = 0xFF; /* outb(0xFF,DDRC) */
	DDRD = 0xFF; /* outb(0xFF,DDRD) */
	PORTA = 0xFF; /* outb(0xFF,PORTA) */
	PORTB = 0xFF; /* outb(0xFF,PORTB) */
	PORTC = 0xFF; /* outb(0xFF,PORTC) */
	PORTD = 0xFF; /* outb(0xFF,PORTD) */

#if CONF_WITH_TRNG==1
	/* Random number gathering */
	randbytes=0;
	TIMSK = 0;	/* Disable timer interrupts */
	TCNT0 = 0;	/* Set timer 0 value */
	TCCR0 = 1;	/* Start timer 0 */
#endif

	return;
}

/*	addr: r25:r24
	ret: r25(=0):r24
*/
extern uint8_t xeread( uint16_t addr );
/*	addr: r25:r24
	b: r22
*/
extern void xewrt( uint16_t addr, uint8_t b );

bool hal_eeprom_read( uint8_t *dst, uint16_t src, uint8_t len )
{
	while( len-- ) {
		if( src<EEPROM_SIZE ) {
			/* Internal EEPROM */
			while( !eeprom_is_ready() ) {}

			*dst++ = eeprom_read_byte((uint8_t*) src++ );
#if CONF_WITH_I2CEEPROM==1
		} else {
			/* External I2C EEPROM */
			/* Subtract the size of the internal EEPROM to get the
			   location in external EEPROM.
			*/
			*dst++ = xeread( src-EEPROM_SIZE );
			src++;
#endif /* CONF_WITH_I2CEEPROM==1 */
		}
	}

	return TRUE;
}

/* TODO: Compare written data? Return 6581 on error. */
bool hal_eeprom_write( uint16_t dst, uint8_t *src, uint8_t len )
{
	while( len-- ) {
		if( dst<EEPROM_SIZE ) {
			/* Internal EEPROM */
			while( !eeprom_is_ready() ) {}

			eeprom_write_byte( (uint8_t*)dst++, *src++ );
#if CONF_WITH_I2CEEPROM==1
		} else {
			/* External I2C EEPROM */
			/* Subtract the size of the internal EEPROM to get the
			   location in external EEPROM.
			*/
			xewrt( dst-EEPROM_SIZE, *src++ );
			dst++;
#endif /* CONF_WITH_I2CEEPROM==1 */
		}
	}

	return TRUE;
}

/*	b: r25(=0):r24
*/
extern void sendbytet0( uint8_t b );
/*	ret: r25(=0):r24
*/
extern uint8_t recbytet0( void );

void hal_io_sendByteT0( uint8_t b )
{
	sendbytet0( b );
	return;
}

uint8_t hal_io_recByteT0( void )
{
	return recbytet0();
}

#if CONF_WITH_TRNG==1
bool hal_rnd_addEntropy( void )
{
	randdata.c[(randbytes++)&0x07] = inp( TCNT0 );	/* Read timer 0 */

	if( randbytes>7 ) {
		struct {
			uint32_t state[2];
			uint32_t key[4];
		} x917;

		randbytes=0;

		log_add( LOG_TAG_RANDOM, randdata.c, sizeof(randdata.c) );

		if( !hal_eeprom_read( (uint8_t*)x917.state, RAND_STATE_ADDR+8,
			sizeof(x917) ) )
			return FALSE;

		x917.state[0]=randdata.l[0];
		x917.state[1]=randdata.l[1];

		crypt_enc( x917.state, x917.key );

		if( !hal_eeprom_write( RAND_STATE_ADDR, (uint8_t*)x917.state,
			sizeof(x917.state) ) )
			return FALSE;
	}

	return TRUE;
}
#endif

#include <log.h>

#if 0
/* We don't need to swap here bytes, because we just need random data. */
bool hal_rnd_getBlock( uint8_t *dst )
{
	struct {
		uint32_t counter[2];
		uint32_t state[2];
		uint32_t key[4];
	} x917;

uint16_t data;
log_enable=TRUE;

	if( ! hal_eeprom_read( (uint8_t*)&x917, RAND_STATE_ADDR, sizeof(x917) ) )
		return FALSE;

	/* Overflow is probably not possible. Before that, the EEPROM dies. */
	x917.counter[0]++;

	if( !hal_eeprom_write( RAND_STATE_ADDR, (uint8_t*)x917.counter,
		sizeof(x917.counter) ) )
		return FALSE;

data = dst;
log_add( LOG_TAG_JOKER, &data, 2 );
data = &(x917);
log_add( LOG_TAG_JOKER, &data, 2 );
data = &(x917.counter[0]);
log_add( LOG_TAG_JOKER, &data, 2 );
data = &(x917.key[0]);
log_add( LOG_TAG_JOKER, &data, 2 );
	crypt_enc( &(x917.counter[0]), &(x917.key[0]) );

log_add( LOG_TAG_JOKER, NULL, 0 );
	((uint32_t*)dst)[0]=x917.counter[0]^x917.state[0];
	((uint32_t*)dst)[1]=x917.counter[1]^x917.state[1];
	crypt_enc( dst, x917.key );

	x917.state[0]=x917.counter[0]^((uint32_t*)dst)[0];
	x917.state[1]=x917.counter[1]^((uint32_t*)dst)[1];
	crypt_enc( x917.state, x917.key );

#if 0
data = RAND_STATE_ADDR+sizeof(x917.counter);
log_add( LOG_TAG_EEWRITE_DST, &data, 2 );
data = &x917+sizeof(x917.counter);
log_add( LOG_TAG_EEWRITE_SRC, &data, 2 );
data = RAND_STATE_LEN+sizeof(x917.counter);
log_add( LOG_TAG_EEWRITE_LEN, &data, 2 );
#endif
	if( !hal_eeprom_write( RAND_STATE_ADDR+sizeof(x917.counter), (uint8_t*)&x917
		+sizeof(x917.counter), RAND_STATE_LEN+sizeof(x917.counter) ) )
		return FALSE;
log_add( LOG_TAG_JOKER, "DONE", 4 );
log_enable=FALSE;

	return TRUE;
}
#endif

//#if 0
/* This is left here in case more stringend memory limitations are present. */
/* We don't need to swap here bytes, because we just need random data. */
bool hal_rnd_getBlock( uint8_t *dst )
{
	uint32_t block[2], key[4];

	if( !( hal_eeprom_read( (uint8_t*)block, SERNUM_ADDR, SERNUM_LEN ) &&
		hal_eeprom_read( (uint8_t*)key, RAND_STATE_ADDR, sizeof(key) ) ) )
		return FALSE;

	key[2]=key[1];
	key[3]=key[0];

	crypt_enc( block, key );

	if( !hal_eeprom_write( RAND_STATE_ADDR, (uint8_t*)block, RAND_STATE_LEN ) )
		return FALSE;

	crypt_enc( block, key );

	memcpy( dst, block, sizeof(block) );

	return TRUE;
}
//#endif

#if defined(__AVR_AT90S8535__)
bool hal_led( char set )
{
	char bank1, bank2;

	bank1 = set&0x0F;
	bank2 = (set>>2) & 0x1C;

	 DDRD  =  bank1;  /* outb( bank1, DDRD ) */
	 PORTD =  ~bank1; /* outb( ~bank1, PORTD ) */

	 DDRB  =  bank2;  /* outb( bank2, DDRB ) */
	 PORTB =  ~bank2; /* outb( ~bank2, PORTB ) */

	return TRUE;
}
#endif /* __AVR_AT90S8535__ */

