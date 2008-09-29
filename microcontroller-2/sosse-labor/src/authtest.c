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
	\brief Test program for authentication functions.

	$Id: authtest.c,v 1.11 2002/12/22 15:42:55 m Exp $
*/

#include <stdio.h>

#include <config.h>
#include <auth.h>
#include <sw.h>
#include <fs.h>
#include <hal.h>
#include <transaction.h>

#include <netinet/in.h>

extern uint8_t authstate;
extern uint32_t challenge[2];
extern uint8_t challvalidity;

int main( int argc, char *argv[] )
{
	uint8_t buf[256];
	FILE *fh;
	int i, ret=1;

#if 0
	printf( "hal_init()\n" );
	hal_init();
#endif

	printf( "ta_commit()\n" );
	if( !ta_commit() ) goto reterror;

	printf( "fs_init()\n" );
	if( !fs_init() ) goto reterror;

	printf( "auth_init()\n" );
	if( !auth_init() ) goto reterror;

	printf( "auth_verifyPin( PIN_TYPE_PIN, \"12341234\", 8 )\n" );
	if( !auth_verifyPin( PIN_TYPE_PIN, "12341234", 8 ) ) goto reterror;
	printf( "authstate=%.2X\n", authstate );

	printf( "auth_verifyPin( PIN_TYPE_PIN, \"1234123\", 7 )\n" );
	if( auth_verifyPin( PIN_TYPE_PIN, "1234123", 7 ) ) goto reterror;
	printf( "authstate=%.2X\n", authstate );
	printf( "sw=%.4X\n", sw );

	printf( "auth_verifyPin( PIN_TYPE_PIN, \"12941234\", 8 )\n" );
	if( auth_verifyPin( PIN_TYPE_PIN, "12941234", 8 ) ) goto reterror;
	printf( "authstate=%.2X\n", authstate );
	printf( "sw=%.4X\n", sw );

	printf( "auth_verifyPin( PIN_TYPE_PIN, \"12341234\", 8 )\n" );
	if( !auth_verifyPin( PIN_TYPE_PIN, "12341234", 8 ) ) goto reterror;
	printf( "authstate=%.2X\n", authstate );

	printf( "auth_verifyPin( PIN_TYPE_PIN, \"12941234\", 8 )\n" );
	if( auth_verifyPin( PIN_TYPE_PIN, "12941234", 8 ) ) goto reterror;
	printf( "authstate=%.2X\n", authstate );
	printf( "sw=%.4X\n", sw );

	printf( "auth_verifyPin( PIN_TYPE_PIN, \"12941234\", 8 )\n" );
	if( auth_verifyPin( PIN_TYPE_PIN, "12941234", 8 ) ) goto reterror;
	printf( "authstate=%.2X\n", authstate );
	printf( "sw=%.4X\n", sw );

	printf( "auth_verifyPin( PIN_TYPE_PIN, \"12941234\", 8 )\n" );
	if( auth_verifyPin( PIN_TYPE_PIN, "12941234", 8 ) ) goto reterror;
	printf( "authstate=%.2X\n", authstate );
	printf( "sw=%.4X\n", sw );

	printf( "auth_verifyPin( PIN_TYPE_PIN, \"12941234\", 8 )\n" );
	if( auth_verifyPin( PIN_TYPE_PIN, "12941234", 8 ) ) goto reterror;
	printf( "authstate=%.2X\n", authstate );
	printf( "sw=%.4X\n", sw );

	printf( "auth_verifyPin( PIN_TYPE_PIN, \"12941234\", 8 )\n" );
	if( auth_verifyPin( PIN_TYPE_PIN, "12941234", 8 ) ) goto reterror;
	printf( "authstate=%.2X\n", authstate );
	printf( "sw=%.4X\n", sw );

	printf( "auth_setPin( PIN_TYPE_PIN, \"56781234\", 8 )\n" );
	if( !auth_setPin( PIN_TYPE_PIN, "56781234", 8 ) ) goto reterror;

	printf( "hal_rnd_getBlock( buf )\n" );
	if( !hal_rnd_getBlock( buf ) ) goto reterror;
	printf( "rnd:" );
	for( i=0; i<8; i++ ) {
		printf( " %.2X", buf[i] );
	}
	printf( "\n" );

	printf( "hal_rnd_getBlock( buf )\n" );
	if( !hal_rnd_getBlock( buf ) ) goto reterror;
	printf( "rnd:" );
	for( i=0; i<8; i++ ) {
		printf( " %.2X", buf[i] );
	}
	printf( "\n" );

	printf( "auth_verifyPin( PIN_TYPE_PUK, \"12345678\", 8 )\n" );
	if( !auth_verifyPin( PIN_TYPE_PUK, "12345678", 8 ) ) goto reterror;
	printf( "authstate=%.2X\n", authstate );
	printf( "sw=%.4X\n", sw );

	challvalidity=1;
	challenge[0]=ntohl(0x9D75A946);
	challenge[1]=ntohl(0x0156A8C1);
	sw=0x9000;
	printf( "auth_createVerifyCryptogram( ..., 8, FALSE, 0 )\n" );
	if( !auth_createVerifyCryptogram( "\x01\x01\xE2\x1F\x05\x2A\xE1\xF3", 8, FALSE, 0 ) )
		goto reterror;
	printf( "sw=%.4X\n", sw );

	printf( "auth_verifyPin( PIN_TYPE_KEY, \"...\", 8 )\n" );
	if( !auth_verifyPin( PIN_TYPE_EXT, "\x01\x23\x45\x67\x89\xAB\xCD\xEF\x01\x23\x45\x67\x89\xAB\xCD\xEF", 16 ) ) goto reterror;
	printf( "authstate=%.2X\n", authstate );
	printf( "sw=%.4X\n", sw );

	ret = 0;
	printf( "Run OK.\n" );

reterror:
	fh = fopen( "dumppin.bin", "wb" );
	i=0;
	while( hal_eeprom_read( buf, i, 128 ) ) {
		fwrite( buf, 1, 128, fh );
		i+=128;
	}
	fclose( fh );
	return ret;
}

