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
	\brief File system.

	$Id: fs.c,v 1.23 2002/12/22 15:42:55 m Exp $
*/

#include <config.h>
#include <sw.h>
#include <fs.h>
#include <hal.h>
#include <log.h>
#include <string.h>
#include <transaction.h>
#include <types.h>

//! Start address of the MF file header.
uint16_t fsstart;
//! Size of the content area of the MF.
uint16_t fssize;

S_FPATH selected;

bool fs_init( void )
{
	if( !hal_eeprom_read( (uint8_t*)&fsstart, FS_START_PTR_ADDR,
		sizeof(fsstart) ) )
		return FALSE;

	if( !hal_eeprom_read( (uint8_t*)&fssize, fsstart, sizeof(fssize) ) )
		return FALSE;

	memset( &selected, 0xFF, sizeof(selected) );

	return TRUE;
}

bool fs_seek( uint16_t fid, S_FSTREAM *fs, S_FINFO *fi, uint8_t type )
{
	/* We have nothing to do */
	if( fid==0xFFFF ) return TRUE;

	/* We want MF */
	if( fid==0x3F00 ) {
		fs->start=fsstart;
		fs->size=fssize;
		fs->pos=0;
		return TRUE;
	}

	while( fstream_read( fs, (uint8_t *)fi, sizeof(uint16_t) ) ) {
		//fs->pos-=sizeof(iu16);
		if( !fi->size ) {
			sw_set( SW_FILE_NOT_FOUND );
			return FALSE;
		}
		if( !fstream_read( fs, (uint8_t *)fi+sizeof(uint16_t),
			sizeof(S_FINFO)-sizeof(uint16_t) ) ) break;

		if( fid==fi->fid ) {
			if( fi->type!=type ) {
				sw_set( SW_FILE_NOT_FOUND );
				return FALSE;
			}
			fs->start+=fs->pos;
			fs->size=fi->size;
			fs->pos=0;
			return TRUE;
		} else {
			fs->pos+=fi->size;
		}
	}

	sw_set( SW_FILE_NOT_FOUND );

	return FALSE;
}

bool fs_seekEnd( S_FSTREAM *fs )
{
	S_FINFO fi;

	while( fstream_read( fs, (uint8_t *)&fi, sizeof(uint16_t) ) ) {
		fs->pos-=sizeof(uint16_t);
		/* Test also for erased cells. */
		if( (!fi.size) || (fi.size==0xFFFF) ) {
			return TRUE;
		}
		if( !fstream_read( fs, (uint8_t *)&fi, sizeof(S_FINFO) ) ) break;

		fs->pos+=fi.size;
	}

	return FALSE;
}

bool fs_getData( S_FPATH *fp, S_FSTREAM *fs, S_FINFO *fi )
{
	fs->start=fsstart;
	fs->size=fssize+6;
	fs->pos=0;

	if( !fstream_read( fs, (uint8_t *)fi, sizeof(S_FINFO) ) ) return FALSE;

	fs->start=fsstart+FS_HEADER_SIZE;
	fs->size=fssize;
	fs->pos=0;

	if( !fs_seek( fp->df, fs, fi, FS_TYPE_DF ) ) return FALSE;

	if( !fs_seek( fp->ef, fs, fi, FS_TYPE_EF ) ) return FALSE;

	return TRUE;
}

bool fs_getFPath( uint16_t fid, S_FPATH *fp, S_FSTREAM *fs, S_FINFO *fi )
{
	S_FINFO fitmp;
	S_FSTREAM fstmp;

	if( fid==0xFFFF ) {
		sw_set( SW_WRONG_DATA );
		return FALSE;
	}

	fp->df=selected.df;
	fp->ef=fid;

	if( !fs_getData( fp, &fstmp, &fitmp ) ) {
		fp->df=fid;
		fp->ef=0xFFFF;
		if( !fs_getData( fp, &fstmp, &fitmp ) ) {
			sw_set( SW_FILE_NOT_FOUND );
			return FALSE;
		}
	}

	if( fp->df==0x3F00 ) fp->df=0xFFFF;
	if( fs!=NULL ) memcpy( fs, &fstmp, sizeof(fstmp) );
	if( fi!=NULL ) memcpy( fi, &fitmp, sizeof(fitmp) );

	return TRUE;
}

bool fs_delete( uint16_t fid )
{
	S_FPATH fp;
	S_FSTREAM fs;
	S_FINFO fi;
	uint16_t size, dfend;

	if( (selected.ef!=0xFFFF) || (fid==0xFFFF) ) {
		sw_set( SW_NOT_ALLOWED );
		return FALSE;
	}

	/* Get end of DF */
	memcpy( &fp, &selected, sizeof(selected) );
	if( !fs_getData( &fp, &fs, &fi ) ) return FALSE;
	dfend = fs.start+fs.size;

	fp.ef=fid;

	if( !fs_getData( &fp, &fs, &fi ) ) {
		/* Because we have only one DF layer, the file must be a EF,
		   when we are already in a DF. */
		if( fp.df!=0xFFFF ) return FALSE;

		/* Look for DF */
		fp.df=fid;
		fp.ef=0xFFFF;
		if( !fs_getData( &fp, &fs, &fi ) ) return FALSE;
	}

	/* Less then 6 bytes empty after file? Then this must be the last file. */
	if( fs.start+fs.size+FS_HEADER_SIZE<=dfend ) {
		fs.pos=fs.size;
		fs.size+=sizeof(uint16_t);
		if( !fstream_read( &fs, (uint8_t *)&size, sizeof(uint16_t) ) ) return FALSE;
		fs.size-=sizeof(uint16_t);

		if( size ) {
			sw_set( SW_INCOMPATIBLE_FILE );
			return FALSE;
		}
	}

	fs.pos=0;

	/* Clear content */
	memset( &fi, 0xFF, sizeof(fi) );
	while( fs.pos<fs.size ) {
		size=(fs.size-fs.pos)<sizeof(fi) ? fs.size-fs.pos : sizeof(fi);

		if( !fstream_write( &fs, (uint8_t*)&fi, size ) ) return FALSE;
	}

	/*  file header */
#if CONF_WITH_TRANSACTIONS==1
	fs.start-=FS_HEADER_SIZE;
	fi.size=0;
	if( !( ta_setdata( (uint8_t*)&fi, sizeof(S_FINFO), fs.start ) &&
		ta_commit() ) ) return FALSE;
#else /* !CONF_WITH_TRANSACTIONS==1 */
	fs.start-=FS_HEADER_SIZE;
	fi.size=0;
	fs.pos=0;
	if( !fstream_write( &fs, (uint8_t *)&fi, sizeof(S_FINFO) ) ) return FALSE;
#endif /* !CONF_WITH_TRANSACTIONS==1 */

	return TRUE;
}

bool fs_create( S_FINFO *fi )
{
	S_FPATH fp;
	S_FSTREAM fs;
	S_FINFO fitmp;
	uint8_t size, dfend;

	if( fi->fid==0x3F00 ) {
		sw_set( SW_FILE_EXISTS );
		return FALSE;
	}

	if( (!fi->size) || (fi->size>0x7FFF) || (fi->fid==0xFFFF) ||
		((fi->type!=FS_TYPE_DF) && (fi->type!=FS_TYPE_EF)) ) {
		sw_set( SW_WRONG_DATA );
		return FALSE;
	}

	if( selected.ef!=0xFFFF ) {
		selected.ef=0xFFFF;
	}

	if( (fi->type==FS_TYPE_DF) && (selected.df!=0xFFFF) ) {
		sw_set( SW_WRONG_DATA );
		return FALSE;
	}

	/* Get end of DF */
	memcpy( &fp, &selected, sizeof(selected) );
	if( !fs_getData( &fp, &fs, &fitmp ) ) return FALSE;
	dfend = fs.start+fs.size;

	/* Look if file exists already */
	if( fi->type==FS_TYPE_DF )
		fp.df=fi->fid;
	else
		fp.ef=fi->fid;
	if( fs_getData( &fp, &fs, &fitmp ) ) {
		sw_set( SW_FILE_EXISTS );
		return FALSE;
	}

	/* Begin with MF */
	fs.start=fsstart+FS_HEADER_SIZE;
	fs.size=fssize;
	fs.pos=0;

	if( selected.df!=0xFFFF ) {
		if( !fs_seek( selected.df, &fs, &fitmp, FS_TYPE_DF ) ) return FALSE;
	}

	/* Seek to free data */
	if( !fs_seekEnd( &fs ) ) return FALSE;

	if( (fs.pos+FS_HEADER_SIZE+fi->size+sizeof(uint16_t))>fs.size ) {
		sw_set( SW_FILE_TO_SHORT );
		return FALSE;
	}

	/* Set fs to file dimensions. */
	fs.start+=fs.pos;
	fs.size=FS_HEADER_SIZE+fi->size+sizeof(uint16_t);
	fs.pos=sizeof(uint16_t);

	/* Clear all */
	memset( &fitmp, 0, sizeof(fitmp) );
	while( fs.pos<fs.size ) {
		size=(fs.size-fs.pos)<sizeof(fitmp) ? fs.size-fs.pos : sizeof(fitmp);

		if( !fstream_write( &fs, (uint8_t*)&fitmp, size ) ) return FALSE;
	}

	/* Write file header */
#if CONF_WITH_TRANSACTIONS==1
	if( !( ta_setdata( (uint8_t*)fi, sizeof(S_FINFO), fs.start ) &&
		ta_commit() ) ) return FALSE;
#else /* !CONF_WITH_TRANSACTIONS==1 */
	fs.pos=0;
	if( !fstream_write( &fs, (uint8_t*)fi, sizeof(S_FINFO) ) ) return FALSE;
#endif /* !CONF_WITH_TRANSACTIONS==1 */

	return TRUE;
}

