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
	\brief Test program for file system functions.

	$Id: fstest.c,v 1.11 2002/12/22 15:42:55 m Exp $
*/

#include <stdio.h>

#include <auth.h>
#include <fs.h>
#include <fstream.h>
#include <hal.h>

extern uint16_t fsstart, fssize;
extern uint8_t authstate;

int main( int argc, char *argv[] )
{
	uint8_t buf[256];
	FILE *fh;
	S_FINFO ef = { 20, 0x1234, FS_TYPE_EF, {0x00} };
	S_FSTREAM fs;
	S_FPATH fp;
	S_FINFO fi;
	int i, ret=1;

#if 0
	printf( "hal_init()\n" );
	hal_init();
#endif

	printf( "fs_init()\n" );
	if( !fs_init() ) goto reterror;

	printf( "fsstart=%.4X\n", fsstart );
	printf( "fssize=%.4X\n", fssize );

	printf( "fs_getFPath( 0x3F00, &fp, &fs, &fi )\n" );
	if( !fs_getFPath( 0x3F00, &fp, &fs, &fi ) ) goto reterror;

	printf( "fp.df=%.4X\n", fp.df );
	printf( "fp.ef=%.4X\n", fp.ef );
	printf( "fi.size=%.4X\n", fi.size );
	printf( "fi.fid=%.4X\n", fi.fid );
	printf( "fi.type=%.2X\n", fi.type );
	printf( "fi.ac[0]=%.2X\n", fi.ac[0] );
	printf( "fs.start=%.4X\n", fs.start );
	printf( "fs.size=%.4X\n", fs.size );
	printf( "fs.pos=%.4X\n", fs.pos );

	printf( "fs_create( {20, 0x1234, FS_TYPE_EF, 0x00} )\n" );
	if( !fs_create( &ef ) ) goto reterror;

	printf( "fs_getFPath( 0x1234, &fp, &fs, &fi )\n" );
	if( !fs_getFPath( 0x1234, &fp, &fs, &fi ) ) goto reterror;

	printf( "fp.df=%.4X\n", fp.df );
	printf( "fp.ef=%.4X\n", fp.ef );
	printf( "fi.size=%.4X\n", fi.size );
	printf( "fi.fid=%.4X\n", fi.fid );
	printf( "fi.type=%.2X\n", fi.type );
	printf( "fi.ac[0]=%.2X\n", fi.ac[0] );
	printf( "fs.start=%.4X\n", fs.start );
	printf( "fs.size=%.4X\n", fs.size );
	printf( "fs.pos=%.4X\n", fs.pos );

	printf( "fstream_write( &fs, {0x12,0x34,0x56} , 3 )\n" );
	if( !fstream_write( &fs, "\x12\x34\x56" , 3 ) ) goto reterror;

	fs.pos-=2;

	printf( "fstream_read( &fs, buf , 3 )\n" );
	if( !fstream_read( &fs, buf , 3 ) ) goto reterror;
	printf( "%.2X %.2X %.2X\n", buf[0], buf[1], buf[2] );

	printf( "fs.start=%.4X\n", fs.start );
	printf( "fs.size=%.4X\n", fs.size );
	printf( "fs.pos=%.4X\n", fs.pos );

	printf( "fstream_read( &fs, buf , 17 )\n" );
	if( fstream_read( &fs, buf , 17 ) ) goto reterror;

	printf( "fs.start=%.4X\n", fs.start );
	printf( "fs.size=%.4X\n", fs.size );
	printf( "fs.pos=%.4X\n", fs.pos );

	selected.ef=0xFFFF;
	printf( "fs_create( {20, 0x1235, FS_TYPE_EF, 0x00} )\n" );
	ef.fid=0x1235;
	if( !fs_create( &ef ) ) goto reterror;

	printf( "fs_create( {600, 0x1236, FS_TYPE_EF, 0x00} )\n" );
	ef.fid=0x1236;
	ef.size=600;
	if( fs_create( &ef ) ) goto reterror;

	printf( "fs_delete( 0x1235 )\n" );
	if( !fs_delete( 0x1235 ) ) goto reterror;

	printf( "fs_delete( 0x1236 )\n" );
	if( fs_delete( 0x1236 ) ) goto reterror;

	printf( "fs_delete( 0x1234 )\n" );
	if( !fs_delete( 0x1234 ) ) goto reterror;

	printf( "fs_create( {60, 0xDF01, FS_TYPE_DF, 0x00} )\n" );
	ef.fid=0xDF01;
	ef.size=60;
	ef.type=FS_TYPE_DF;
	if( !fs_create( &ef ) ) goto reterror;

	authstate=0;
	printf( "auth_checkAc(AUTH_AC_ALW)\n" );
	if( !auth_checkAc(AUTH_AC_ALW) ) goto reterror;

	authstate=1;
	printf( "auth_checkAc(AUTH_AC_PIN)\n" );
	if( !auth_checkAc(AUTH_AC_PIN) ) goto reterror;

	authstate=2;
	printf( "auth_checkAc(AUTH_AC_ADM)\n" );
	if( !auth_checkAc(AUTH_AC_ADM) ) goto reterror;

	authstate=0;
	printf( "auth_checkAc(AUTH_AC_ADM)\n" );
	if( auth_checkAc(AUTH_AC_ADM) ) goto reterror;

	printf( "fs_getData( &fp, &fs, &fi )\n" );
	fp.df=fp.ef=0xFFFF;
	memset( &fi, 0, sizeof(fi) );
	if( !fs_getData( &fp, &fs, &fi ) ) goto reterror;
	printf( "fp.df=%.4X\n", fp.df );
	printf( "fp.ef=%.4X\n", fp.ef );
	printf( "fi.size=%.4X\n", fi.size );
	printf( "fi.fid=%.4X\n", fi.fid );
	printf( "fi.type=%.2X\n", fi.type );
	printf( "fi.ac[0]=%.2X\n", fi.ac[0] );
	printf( "fs.start=%.4X\n", fs.start );
	printf( "fs.size=%.4X\n", fs.size );
	printf( "fs.pos=%.4X\n", fs.pos );

	ret = 0;

	printf( "Run OK.\n" );

reterror:
	fh = fopen( "dumpfs.bin", "wb" );
	i=0;
	while( hal_eeprom_read( buf, i, 128 ) ) {
		fwrite( buf, 1, 128, fh );
		i+=128;
	}
	fclose( fh );
	return ret;
}

