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
	\brief HAL emulation for Linux.

	$Id: hal-emu.c,v 1.18 2003/03/30 12:42:21 m Exp $
*/

#include <config.h>
#if defined(CTAPI)
#include <ctapi.h>
#endif
#include <sio/sio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <sw.h>
#include <types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define PORT "/dev/ttyS0"

uint8_t eeprom[1*1024] = {
	/* CARD_STATE_ADDR */
	0x00,
	/* ATR_LEN_ADDR */
	0x0D,
	/* ATR_ADDR */
	0x3B, 0xA8, 0x00, 0x40, 0x0A, 0x53, 0x4F, 0x53,
	0x02, 0x03, 0x25, 0x01, 0x03, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	/* SERNUM_ADDR */
	0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
	/* RAND_STATE_ADDR */
    /* Counter */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* State */
    0xF1, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xE0,
    /* Key */
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	/* TRANSAC_STATE_ADDR */
	0x00,
	/* TRANSAC_DATA_ADDR */
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
#if 0
	/* ADM_KEY_ADDR */
	0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
	0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
	/* ADM_RETCNTR_ADDR */
	0x0A,
	/* PIN_ADDR */
	0x31, 0x32, 0x33, 0x34,
	/* PIN_RETCNTR_ADDR */
	0x03,
	/* PUK_ADDR */
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
	/* PUK_RETCNTR_ADDR */
	0x0A,
#endif
	/* Beginning here is the FS data little endian */
	/* FS_START_PTR_ADDR */
    (FS_START_PTR_ADDR+2)&0xFF, (FS_START_PTR_ADDR+2)>>8,
	/* MF */
	0x00, 0x02, 0x00, 0x3F, 0x38, 0x00,
    /* Ext Auth key file */
    0x11, 0x00, 0x00, 0xFF, 0x00, 0x2F,
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
    0x0A,
    /* Int Auth key file */
    0x11, 0x00, 0x01, 0xFF, 0x00, 0x2F,
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
    0x0A,
    /* PIN file */
    0x12, 0x00, 0x02, 0xFF, 0x00, 0x2F,
    0x31, 0x32, 0x33, 0x34, 0x31, 0x32, 0x33, 0x34,
    0x03,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x0A,
    /* Empty entry */
    0x00, 0x00

};

#if defined(CTAPI)
#if defined(USE_SIO)
#undef USE_SIO
#endif
#elif defined(USE_SIO)
SIO_INFO *si;
#else
int halsend;
#endif

void hal_init( void )
{
#if defined(DEBUG) && !defined(CTAPI)
	fprintf( stderr, "hal_init()\n" );

	fprintf( stderr, "Config defines:\n" );
	fprintf( stderr, "\tCARD_STATE_ADDR: 0x%.4X\n", CARD_STATE_ADDR );
	fprintf( stderr, "\tATR_LEN_ADDR: 0x%.4X\n", ATR_LEN_ADDR );
	fprintf( stderr, "\tATR_MAXLEN: %d\n", ATR_MAXLEN );
	fprintf( stderr, "\tATR_ADDR: 0x%.4X\n", ATR_ADDR );
	fprintf( stderr, "\tSERNUM_ADDR: 0x%.4X\n", SERNUM_ADDR );
	fprintf( stderr, "\tSERNUM_LEN: %d\n", SERNUM_LEN );
	fprintf( stderr, "\tRAND_STATE_ADDR: 0x%.4X\n", RAND_STATE_ADDR );
	fprintf( stderr, "\tRAND_STATE_LEN: %d\n", RAND_STATE_LEN );
	fprintf( stderr, "\tTRANSAC_STATE_ADDR: 0x%.4X\n", TRANSAC_STATE_ADDR );
	fprintf( stderr, "\tTRANSAC_DATA_ADDR: 0x%.4X\n", TRANSAC_DATA_ADDR );
	fprintf( stderr, "\tTRANSAC_DATA_LEN: %d\n", TRANSAC_DATA_LEN );
	fprintf( stderr, "\tADM_KEY_LEN: %d\n", ADM_KEY_LEN );
	fprintf( stderr, "\tINT_KEY_LEN: %d\n", INT_KEY_LEN );
	fprintf( stderr, "\tPIN_LEN: %d\n", PIN_LEN );
	fprintf( stderr, "\tPUK_LEN: %d\n", PUK_LEN );
	fprintf( stderr, "\tFS_START_PTR_ADDR: 0x%.4X\n", FS_START_PTR_ADDR );
#endif

#if defined(CTAPI)
	if( loadEepromFile ) {
		if( (fdEepromFile=open( eepromFile, O_EXCL ))!=-1 ) {
			if( read( fdEepromFile, eeprom, sizeof(eeprom) )!=sizeof(eeprom) ) {
				close( fd_fromCard[1] );
				close( fdEepromFile );
				exit( 0 );
			}
			close( fdEepromFile );
		} else {
			close( fd_fromCard[1] );
		}
	}
#elif defined(USE_SIO)
	si = SIO_Open( PORT );
	if( si==0 ) exit( 1 );

	SIO_SetSpeed( si, 9600 );
	SIO_SetDataBits( si, 8 );
	SIO_SetParity( si, SIO_PARITY_EVEN );
	SIO_SetStopBits( si, 2 );
	SIO_SetIOMode( si, SIO_IOMODE_DIRECT );
	SIO_WriteSettings( si );
#else
	halsend=TRUE;
#endif

	return;
}

void hal_destroy( void )
{
#ifdef DEBUG
	fprintf( stderr, "hal_destroy()\n" );
#endif
}

bool hal_eeprom_read( uint8_t *dst, uint16_t src, uint8_t len )
{
#ifdef DEBUG
	fprintf( stderr, "hal_eeprom_read( %.8X, %.4X, %d )=", (int)dst, (int)src, len );
#endif

	while( len-- ) {
		if( (unsigned int)src>=sizeof(eeprom) ) {
			sw_set( 0x6F00 );
			return FALSE;
		}
#ifdef DEBUG
	fprintf( stderr, " %.2X", eeprom[(unsigned int)src] );
#endif
		*dst++ = eeprom[(unsigned int)src++];
	}

#ifdef DEBUG
	fprintf( stderr, "\n" );
#endif

	return TRUE;
}

bool hal_eeprom_write( uint16_t dst, uint8_t *src, uint8_t len )
{
#ifdef DEBUG
	fprintf( stderr, "hal_eeprom_write( %.4X, %.8X, %d )=", (int)dst, (int)src, len );
#endif

	while( len-- ) {
		if( (unsigned int)dst>=sizeof(eeprom) ) {
			sw_set( 0x6F00 );
			return FALSE;
		}
#ifdef DEBUG
	fprintf( stderr, " %.2X", *src );
#endif
		eeprom[(unsigned int)dst++]=*src++;
	}

#ifdef DEBUG
	fprintf( stderr, "\n" );
#endif

	return TRUE;
}

void hal_io_sendByteT0( uint8_t b )
{
#if defined(CTAPI)
	write( fd_fromCard[1], &b, 1 );
#elif defined(USE_SIO)
#ifdef DEBUG
	fprintf( stderr, "hal_io_sendByteT0( %.2X )\n", b );
#endif
	SIO_WriteChar( si, b );
#else
	if( !halsend ) {
		fprintf( stderr, "\nSending:" );
	}
	fprintf( stderr, " %.2X", b );
	halsend=TRUE;
#endif
}

uint8_t hal_io_recByteT0( void )
{
	uint8_t b;
#if !defined(USE_SIO) && !defined(CTAPI)
	int i;
#endif

#if defined(CTAPI)
	if( read( fd_toCard[0], &b, 1 )!=1 ) {
		if( saveEepromFile ) {
			if( (fdEepromFile=open( eepromFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR ))!=-1 ) {
				write( fdEepromFile, eeprom, sizeof(eeprom) );
				close( fdEepromFile );
			}
		}
		close( fd_fromCard[1] );
		exit( 0 );
	}
#elif defined(USE_SIO)
#ifdef DEBUG
	fprintf( stderr, "hal_io_recByteT0( void )" );
	fflush( stderr );
#endif
	SIO_SetReadTimeout( si, SIO_READ_WAIT_FOREVER );
	b=SIO_ReadChar( si );
#ifdef DEBUG
	fprintf( stderr, "=%.2X\n", b );
	fflush( stderr );
#endif
#else
	if( halsend ) {
		fprintf( stderr, "\nReceiving: " );
	}
	while( fscanf( stdin, "%x", &i )!=1 ) {
		fprintf( stderr, "\nError parsing byte.\nRetry: " );
	}
	halsend=FALSE;
	b=i&0xFF;
	fprintf( stderr, " %.2X", b );
	fflush( stderr );
#endif

	return b;
}

#if CONF_WITH_STATIC_RANDOM==1
bool hal_rnd_getBlock( uint8_t *dst )
{
	memcpy( dst, "\x01\x02\x03\x04\x05\x06\x07\x08", 8 );
	return TRUE;
}
#else
/* We don't need to swap here bytes, because we just need random. */
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
#endif

