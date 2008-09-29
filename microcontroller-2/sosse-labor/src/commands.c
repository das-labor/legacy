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
	\brief Commands.

	The documentation of the commands is on the main page of the doxygen
	documentation.

	$Id: commands.c,v 1.27 2003/03/30 12:42:21 m Exp $
*/

#include <auth.h>
#include <hal.h>
#include <commands.h>
#include <config.h>
#include <crypt.h>
#include <fs.h>
#include <log.h>
#include <stddef.h>
#include <string.h>
#include <sw.h>
#include <t0.h>
#include <tools.h>
#include <types.h>

#ifdef __AVR__
#include <avr/io.h>
#endif /* __AVR__ */

/*! \brief Valid data in response array. Invalid if zero. */
uint8_t resplen;
/*! \brief Data returned by Get Response command.

	The size of this array is the max. of sizeof(S_FINFO) and CRYPT_BLOCK_LEN.
*/
uint8_t response[8];

#if CONF_WITH_TESTCMDS==1
void cmd_write( void )
{
	uint8_t b, i;

	if( t0_testP3( 0x00 ) ) {
		sw_set( SW_WRONG_LEN );
		return;
	}

	/* TODO: check for eeprom range */

	/* Receive */
	for( i=0; i<header[4]; i++ ) {
		/* ~ACK */
		t0_sendCAck();

		/* Data */
		b = hal_io_recByteT0();

		if( !hal_eeprom_write( ((uint16_t)header[2]<<8)+header[3]+i, &b, 1 ) ) return;
	}

	sw_set( SW_OK );
}
#endif /* CONF_WITH_TESTCMDS==1 */

#if CONF_WITH_TESTCMDS==1
void cmd_read( void )
{
	uint16_t i, max=header[4];
	uint8_t b;

	/* TODO: check for eeprom range */

	/* ACK */
	t0_sendAck();

	if( !max ) max=256;
	for( i=0; i<max; i++ ) {
		if( !hal_eeprom_read( &b, (header[2]<<8)+header[3]+i, 1 ) ) return;

		/* Data */
		hal_io_sendByteT0( b );
	}

	/* SW */
	sw_set( SW_OK );
}
#endif /* CONF_WITH_TESTCMDS==1 */

#if CONF_WITH_FUNNY==1
void cmd_led( void )
{
	uint8_t i;
	uint8_t j;

	switch( header[2] ) {
	case 1:
		hal_led( header[3] );
		break;
	case 2:
		for( i=0; i<header[3]; i++ ) {
			hal_led( 0x01 );
			for( j=0; j<120; j++ ) { hal_io_sendByteT0( 0x60 ); }
			hal_led( 0x02 );
			for( j=0; j<120; j++ ) { hal_io_sendByteT0( 0x60 ); }
			hal_led( 0x04 );
			for( j=0; j<120; j++ ) { hal_io_sendByteT0( 0x60 ); }
			hal_led( 0x08 );
			for( j=0; j<120; j++ ) { hal_io_sendByteT0( 0x60 ); }
			hal_led( 0x04 );
			for( j=0; j<120; j++ ) { hal_io_sendByteT0( 0x60 ); }
			hal_led( 0x02 );
			for( j=0; j<120; j++ ) { hal_io_sendByteT0( 0x60 ); }
		}
		hal_led( 0x01 );
		for( j=0; j<120; j++ ) { hal_io_sendByteT0( 0x60 ); }
		hal_led( 0x00 );
		break;
	default:
		sw_set( SW_WRONG_P1P2 );
		return;
	}

	/* SW */
	sw_set( SW_OK );
}
#endif /* CONF_WITH_FUNNY==1 */

#if CONF_WITH_PINCMDS==1
void cmd_changeUnblockPIN( void )
{
	uint8_t pins[max(PIN_LEN+PIN_LEN,PUK_LEN+PIN_LEN)];
	uint8_t pin1type;
/* This makes no difference with the GCC, but it may with some other compiler. */
#if ((PIN_LEN)==(PUK_LEN))
#define pinssize (PIN_LEN+PIN_LEN)
#define pin1len	PIN_LEN
#else
	uint8_t pinssize, pin1len;
#endif

	switch( header[1] ) {
	case INS_UNBLOCK_PIN:
#if PIN_LEN!=PUK_LEN
		pinssize=PUK_LEN+PIN_LEN;
		pin1len=PUK_LEN;
#endif
		pin1type=PIN_TYPE_PUK;
		break;
	default:
#if PIN_LEN!=PUK_LEN
		pinssize=PIN_LEN+PIN_LEN;
		pin1len=PIN_LEN;
#endif
		pin1type=PIN_TYPE_PIN;
		break;
	}

	if( !t0_testP3( pinssize ) ) return;

	if( !t0_testP1P2( 0x0000 ) ) return;

	/* ACK */
	t0_sendAck();

	/* Receive */
	t0_recBlock( pins, pinssize );

	if( !( auth_verifyPin( pin1type, pins, pin1len ) &&
		auth_setPin( PIN_TYPE_PIN, pins+pin1len, PIN_LEN ) ) ) return;

	sw_set( SW_OK );
}
#endif /* CONF_WITH_PINCMDS==1 */

#if CONF_WITH_CREATECMD==1
void cmd_create( void )
{
	S_FSTREAM fs;
	S_FINFO fi;

	if( !t0_testP3( sizeof(S_FINFO) ) ) return;

	if( !t0_testP1P2( 0x0000 ) ) return;

	/* Get DF data */
	if( !fs_getData( &selected, &fs, &fi ) ) return;

	/* Correct DF type? */
	if( fi.type!=FS_TYPE_DF ) {
		sw_set( SW_NOT_ALLOWED );
		return;
	}

	/* Correct permissions? */
	if( !auth_checkAc( (fi.ac[0]>>4)&0x0F ) ) return;

	/* ACK */
	t0_sendAck();

	/* Receive */
	t0_recBlock( (uint8_t *)&fi, sizeof(S_FINFO) );

	hton_us( &fi.size, 1 );
	hton_us( &fi.fid, 1 );

	if( !fs_create( &fi ) ) return;

	sw_set( SW_OK );
}
#endif /* CONF_WITH_CREATECMD==1 */

#if CONF_WITH_DELETECMD==1
void cmd_delete( void )
{
	S_FSTREAM fs;
	S_FINFO fi;
	uint16_t fid;
	uint8_t b;

	if( !t0_testP3( sizeof(uint16_t) ) ) return;

	if( !t0_testP1P2( 0x0000 ) ) return;

	/* Get DF data */
	if( !fs_getData( &selected, &fs, &fi ) ) return;

	/* Correct DF type? */
	if( fi.type!=FS_TYPE_DF ) {
		sw_set( SW_NOT_ALLOWED );
		return;
	}

	/* Correct permissions? */
	if( !auth_checkAc( fi.ac[0]&0x0F ) ) return;

	/* ACK */
	t0_sendAck();

	/* Data */
	b = hal_io_recByteT0();
	fid=b<<8;
	b = hal_io_recByteT0();
	fid+=b;

	if( !fs_delete( fid ) ) return;

	sw_set( SW_OK );
}
#endif /* CONF_WITH_DELETECMD==1 */

#if CONF_WITH_KEYCMDS==1
void cmd_extAuth( void )
{
	uint8_t cry[8];

	if( !t0_testP3( CRYPT_BLOCK_LEN ) ) return;

	if( !t0_testP1P2( 0x0000 ) ) return;

	/* ACK */
	t0_sendAck();

	/* Receive */
	t0_recBlock( cry, sizeof(cry) );

	if( !auth_createVerifyCryptogram( cry, sizeof(cry), FALSE, 0 ) ) return;

	sw_set( SW_OK );
}
#endif /* CONF_WITH_KEYCMDS==1 */

#if CONF_WITH_KEYCMDS==1
void cmd_getChallenge( void )
{
	uint8_t i, buf[CRYPT_BLOCK_LEN];

	if( !t0_testP3( CRYPT_BLOCK_LEN ) ) return;

	if( !t0_testP1P2( 0x0000 ) ) return;

	auth_getChallenge( buf, sizeof(buf) );

	/* ACK */
	t0_sendAck();

	for( i=0; i<sizeof(buf); i++ ) {
		/* Data */
		hal_io_sendByteT0( buf[i] );
	}

	/* SW */
	sw_set( SW_OK );
}
#endif /* CONF_WITH_KEYCMDS==1 */

void cmd_getResponse( void )
{
	uint8_t i;

	if( resplen==0 ) {
		sw_set( SW_WRONG_CONDITION );
		return;
	}

	if( !t0_testP3( resplen ) ) {
		if( (header[4]>resplen) || (!header[4]) ) {
			sw_set( SW_WRONG_LE|resplen );
			return;
		}
		/* User want's not all data */
		resplen=header[4];
	}

	if( !t0_testP1P2( 0x0000 ) ) return;

	/* ACK */
	t0_sendAck();

	/* Data */
	for( i=0; i<resplen; i++ ) {
		hal_io_sendByteT0( response[i] );
	}

	sw_set( SW_OK );
}

#if CONF_WITH_KEYCMDS==1
void cmd_intAuth( void )
{
	bool enc=TRUE;

	if( !t0_testP3( CRYPT_BLOCK_LEN ) ) return;

#if CONF_WITH_DECRYPT==1
	if( !t0_testP1P2( 0x0000 ) && !t0_testP1P2( 0x0001 ) ) return;

	if( header[3]&0x01 ) enc=FALSE;
#else /* CONF_WITH_DECRYPT!=1 */
	if( !t0_testP1P2( 0x0000 ) ) return;
#endif /* CONF_WITH_DECRYPT!=1 */

	/* ACK */
	t0_sendAck();

	/* Receive */
	t0_recBlock( response, CRYPT_BLOCK_LEN );

	if( !auth_createVerifyCryptogram( response, CRYPT_BLOCK_LEN, TRUE, enc ) )
	{
		resplen=0;
		return;
	}

	resplen=CRYPT_BLOCK_LEN;

	sw_set( SW_AVAILABLE|CRYPT_BLOCK_LEN );
}
#endif /* CONF_WITH_KEYCMDS==1 */

void cmd_readBinary( void )
{
	S_FSTREAM fs;
	S_FINFO fi;
	uint16_t i, max=header[4];
	uint8_t b;

	/* Get file data */
	if( !fs_getData( &selected, &fs, &fi ) ) return;

	/* Correct file type? */
	if( fi.type!=FS_TYPE_EF ) {
		sw_set( SW_NOT_ALLOWED );
		return;
	}

	/* Correct permissions? */
	if( !auth_checkAc( fi.ac[0]&0x0F ) ) return;

	if( !max ) max=256;

	/* Test file size */
	fs.pos=((uint16_t)header[2]<<8)|header[3];
	if(fs.pos>fs.size) {
		sw_set( SW_WRONG_REFERENCE );
		return;
	}
	if( !fstream_test( &fs, max ) ) return;

	/* ACK */
	t0_sendAck();

	for( i=0; i<max; i++ ) {
		if( !fstream_read( &fs, &b, 1 ) ) return;

		/* Data */
		hal_io_sendByteT0( b );
	}

	sw_set( SW_OK );
}

void cmd_select( void )
{
	S_FPATH fp;
	S_FINFO *fi=(S_FINFO*)response;
	uint16_t fid;
	uint8_t b;

	if( !t0_testP3( sizeof(uint16_t) ) ) return;

	if( !t0_testP1P2( 0x0000 ) ) return;

	/* ACK */
	t0_sendAck();

	/* Data */
	b = hal_io_recByteT0();
	fid=b<<8;
	b = hal_io_recByteT0();
	fid+=b;

	if( !fs_getFPath( fid, &fp, NULL, fi ) ) return;

	memcpy( &selected, &fp, sizeof(S_FPATH) );

	resplen=sizeof(S_FINFO);
	hton_us( &(fi->size), 1 );
	hton_us( &(fi->fid), 1 );

	sw_set( SW_AVAILABLE|sizeof(S_FINFO) );
}

void cmd_updateBinary( void )
{
	S_FSTREAM fs;
	S_FINFO fi;
	uint16_t i;
	uint8_t b;

	if( t0_testP3( 0x00 ) ) {
		sw_set( SW_WRONG_LEN );
		return;
	}

	/* Get file data */
	if( !fs_getData( &selected, &fs, &fi ) ) return;

	/* Correct file type? */
	if( fi.type!=FS_TYPE_EF ) {
		sw_set( SW_NOT_ALLOWED );
		return;
	}

	/* Correct permissions? */
	if( !auth_checkAc( (fi.ac[0]>>4)&0x0F ) ) return;

	/* Test file size */
	fs.pos=((uint16_t)header[2]<<8)|header[3];
	if(fs.pos>fs.size) {
		sw_set( SW_WRONG_REFERENCE );
		return;
	}
	if( !fstream_test( &fs, header[4] ) ) return;

	/* Receive */
	for( i=0; i<header[4]; i++ ) {
		/* ~ACK */
		t0_sendCAck();

		/* Data */
		b = hal_io_recByteT0();

		if( !fstream_write( &fs, &b, 1 ) ) return;
	}

	sw_set( SW_OK );
}

#if 0
#if CONF_WITH_KEYCMDS==1
void cmd_verifyKey( void )
{
	uint8_t key[ADM_KEY_LEN];

	if( !t0_testP3( sizeof(key) ) ) return;

	if( !t0_testP1P2( 0x0000 ) ) return;

	/* ACK */
	t0_sendAck();

	/* Receive */
	t0_recBlock( key, sizeof(key) );

	if( !auth_verifyPin( PIN_TYPE_EXT, key, sizeof(key) ) ) return;

	sw_set( SW_OK );
}
#endif /* CONF_WITH_KEYCMDS==1 */

#if CONF_WITH_PINCMDS==1
void cmd_verifyPIN( void )
{
	uint8_t pin[PIN_LEN];

	if( !t0_testP3( sizeof(pin) ) ) return;

	if( !t0_testP1P2( 0x0000 ) ) return;

	/* ACK */
	t0_sendAck();

	/* Receive */
	t0_recBlock( pin, sizeof(pin) );

	if( !auth_verifyPin( PIN_TYPE_PIN, pin, sizeof(pin) ) ) return;

	sw_set( SW_OK );
}
#endif /* CONF_WITH_PINCMDS==1 */
#endif

#if (CONF_WITH_KEYCMDS==1) || (CONF_WITH_PINCMDS==1)
void cmd_verifyKeyPIN( void )
{
	uint8_t buf[(ADM_KEY_LEN>PIN_LEN)?ADM_KEY_LEN:PIN_LEN];
	uint8_t len, type;

	switch( header[1] ) {
	case INS_VERIFY_KEY:
		type=PIN_TYPE_EXT;
		len=16;
		break;
	default:
		type=PIN_TYPE_PIN;
		len=PIN_LEN;
	}

	if( !t0_testP3( len ) ) return;

	if( !t0_testP1P2( 0x0000 ) ) return;

	/* ACK */
	t0_sendAck();

	/* Receive */
	t0_recBlock( buf, len );

	if( !auth_verifyPin( type, buf, len ) ) return;

	sw_set( SW_OK );
}
#endif /* (CONF_WITH_KEYCMDS==1) || (CONF_WITH_PINCMDS==1) */

