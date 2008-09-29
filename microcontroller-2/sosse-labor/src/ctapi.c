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
	\brief Virtual CT-API library.

	$Id: ctapi.c,v 1.4 2002/12/24 13:29:57 m Exp $
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ctapi.h>
#include <main.h>
#include <types.h>
#include <sw.h>

IS32	cthandle=-1;
int		fd_fromCard[2], fd_toCard[2];
pid_t	pidVirtuaCard=0;
int		loadEepromFile=FALSE;
int		saveEepromFile=FALSE;
char	eepromFile[256];
int		fdEepromFile;
char	atr[32];
int		atrlen=0;

IS8 CT_forkIcc( void )
{
	IS8 rv;
	int i, next, toread, n;

	if( (rv=CT_stopIcc())!=CTAPI_RV_OK )
		return rv;

	if( (pipe(fd_fromCard)<0) || (pipe(fd_toCard)<0) )
		return CTAPI_RV_ERR_MEMORY;

	if( (pidVirtuaCard=fork()) < 0 ) {
		return CTAPI_RV_ERR_MEMORY;
	} else if( pidVirtuaCard>0 ) {
		/* parent */
		close( fd_fromCard[1] );
	} else {
		/* child */
		close( fd_toCard[1] );
		sosse_main();
		exit( 0 );
	}

	// Read ATR
	i=0;
	if( read( fd_fromCard[0], atr+i, 1 )!=1 ) {
		CT_stopIcc();
		return CTAPI_RV_ERR_TRANS;
	}
	i++;
	if( read( fd_fromCard[0], atr+i, 1 )!=1 ) {
		CT_stopIcc();
		return CTAPI_RV_ERR_TRANS;
	}
	i++;
	do {
		next=atr[i-1]>>7;
		toread=((atr[i-1]>>7)&0x01)+((atr[i-1]>>6)&0x01)+((atr[i-1]>>5)&0x01)+
			((atr[i-1]>>4)&0x01);
		for( n=0; n<toread; n++ ) {
			if( read( fd_fromCard[0], atr+i+n, 1 )!=1 ) {
				CT_stopIcc();
				return CTAPI_RV_ERR_TRANS;
			}
		}
		i+=toread;
	} while( next );
	for( n=0; n<(atr[1]&0xF); n++ ) {
		if( read( fd_fromCard[0], atr+i+n, 1 )!=1 ) {
			CT_stopIcc();
			return CTAPI_RV_ERR_TRANS;
		}
	}
	i+=atr[1]&0xF;
	atrlen=i;

	return CTAPI_RV_OK;
}

IS8 CT_stopIcc( void )
{
	if( pidVirtuaCard ) {
		close( fd_toCard[1] );
		// TODO: Wait for VirtuaCard process to terminate.
		pidVirtuaCard=0;
	}

	return CTAPI_RV_OK;
}

IS8 CT_ifdCmdResetCT( IU16 lenc, IU8 *command, IU16 *lenr, IU8 *response )
{
	if( command[2]>1 ) {
		response[0]=0x6A;
		response[1]=0x00;
		*lenr=2;
	}

	if( command[2] ) {
		if( CT_forkIcc()!=CTAPI_RV_OK ) {
			response[0]=0x64;
			response[1]=0x00;
			*lenr=2;
		}

		if( command[3]&1 ) {
			// TODO: Check lenr
			memcpy( response, atr, atrlen );
			response[atrlen+1]=0x90;
			response[atrlen+2]=0x00;
			*lenr=atrlen+2;
		} else {
			response[0]=0x90;
			response[1]=0x00;
			*lenr=2;
		}
	} else {
		CT_stopIcc();
		response[0]=0x90;
		response[1]=0x00;
		*lenr=2;
	}

	return CTAPI_RV_OK;
}

IS8 CT_ifdCmdRequestICC( IU16 lenc, IU8 *command, IU16 *lenr, IU8 *response )
{
	if( command[2]!=1 ) {
		response[0]=0x6A;
		response[1]=0x00;
		*lenr=2;
	}

	if( CT_forkIcc()!=CTAPI_RV_OK ) {
		response[0]=0x64;
		response[1]=0x00;
		*lenr=2;
	}

	if( command[3]&1 ) {
		// TODO: Check lenr
		memcpy( response, atr, atrlen );
		response[atrlen+0]=0x90;
		response[atrlen+1]=0x00;
		*lenr=atrlen+2;
	} else {
		response[0]=0x90;
		response[1]=0x00;
		*lenr=2;
	}

	return CTAPI_RV_OK;
}

IS8 CT_ifdCmdGetStatus( IU16 lenc, IU8 *command, IU16 *lenr, IU8 *response )
{
	response[0]=0x90;
	response[1]=0x00;
	*lenr=2;

	if( command[3]==0x80 ) {
		response[0]=0x80;
		response[1]=0x01;
		response[2]=0x01;
		if( pidVirtuaCard ) response[2]=0x05;
		response[3]=0x90;
		response[4]=0x00;
		*lenr=5;
	}

	return CTAPI_RV_OK;
}

IS8 CT_ifdCmdEjectICC( IU16 lenc, IU8 *command, IU16 *lenr, IU8 *response )
{
	if( command[2]==1 ) {
		response[0]=0x90;
		response[1]=0x00;
		*lenr=2;
		return CT_stopIcc();
	}

	response[0]=0x6A;
	response[1]=0x00;
	*lenr=2;

	return CTAPI_RV_OK;
}

/*! \brief Parses EEPROM FILE command.

	- CLA: '20'
	- INS: 'E0'
	- P1:
		- bit 1: Save EEPROM to file at eject or reset.
		- bit 2: Load EEPROM from file at reset.
	- P2: RFU
	- Lc: Length of file name
	- Data: File name (possibly including path) without null-termination
*/
IS8 CT_ifdCmdEepromFile( IU16 lenc, IU8 *command, IU16 *lenr, IU8 *response )
{
	if( command[2]==2 )
		loadEepromFile=TRUE;
	else
		loadEepromFile=FALSE;

	if( command[2]==1 )
		saveEepromFile=TRUE;
	else
		saveEepromFile=FALSE;

	memset( eepromFile, 0, sizeof(eepromFile) );
	memcpy( eepromFile, command+5, command[4] );

	response[0]=0x90;
	response[1]=0x00;
	*lenr=2;

	return CTAPI_RV_OK;
}

IS8 CT_ifdCmd( IU16 lenc, IU8 *command, IU16 *lenr, IU8 *response )
{
	switch( command[1] ) {
	case CTAPI_INS_RESET_CT:
		return CT_ifdCmdResetCT( lenc, command, lenr, response );
	case CTAPI_INS_REQUEST_ICC:
		return CT_ifdCmdRequestICC( lenc, command, lenr, response );
	case CTAPI_INS_GET_STATUS:
		return CT_ifdCmdGetStatus( lenc, command, lenr, response );
	case CTAPI_INS_EJECT_ICC:
		return CT_ifdCmdEjectICC( lenc, command, lenr, response );
	case CTAPI_INS_EEPROM:
		return CT_ifdCmdEepromFile( lenc, command, lenr, response );
	default:
		response[0]=0x6D;
		response[1]=0x00;
		*lenr=2;
	}

	return CTAPI_RV_OK;
}

int CT_t0( IU16 lenc, IU8 *command, IU16 *lenr, IU8 *response )
{
	char rsp[ 256+2 ];
	char ins = command[1];
	int	lc=0;
	int le=0;
	int	rsplen=0;
	char byte;
	int pointer=0;
	int spointer=0;
	int cse;
	int i;

	if( lenc==5 ) {
		cse=2;
		le=command[4];
	} else if( lenc>5 ) {
		cse=3;
		lc=command[4];
	} else {
		cse=1;
	}

	/* Send header */
	if( cse==1 ) {
		write( fd_toCard[1], command, 4 );
		byte=0;
		write( fd_toCard[1], &byte, 1 );
	} else {
		write( fd_toCard[1], command, 5 );
	}

	while( TRUE ) {

		/* Read byte */
		if( read( fd_fromCard[0], &byte, 1 )!=1 ) {
			return( CTAPI_RV_ERR_TRANS );
		}

		/* ACK */
		if( (byte&0xFE)==(ins&0xFE) ) {
			/* Direction: To Card */
			if( cse==3 ) {
				/* Send data to card */
				if( write( fd_toCard[1], command+5+spointer, lc-spointer )!=
					lc-spointer ) {
					return( CTAPI_RV_ERR_TRANS );
				}
				continue;
			} else
			/* Direction: From Card */
			if( cse==2 ) {

				/* Receive data from card */
				for( i=0; i<(le+2-pointer); i++ ) {
					if( read( fd_fromCard[0], rsp+pointer+i, 1 )!=1 ) {
						return( CTAPI_RV_ERR_TRANS );
					}
				}

				rsplen += le+2-pointer;

				/* Don't copy more than requested */
				if( rsplen>*lenr ) {
					memcpy( response, rsp, *lenr-2 );
					memcpy( response+*lenr-2, rsp+rsplen-2, 2 );
				} else {
					memcpy( response, rsp, rsplen );
					*lenr=rsplen;
				}

				return( CTAPI_RV_OK );
			} else {
				return( CTAPI_RV_ERR_TRANS );
			}
		}

		/* ~ACK */
		if( ((~byte)&0xFE)==(ins&0xFE) ) {
			/* Direction: To Card */
			if( cse==3 ) {
				/* Send data to card */
				if( write( fd_toCard[1], command+5+spointer, 1 )!=1 ) {
					return CTAPI_RV_ERR_TRANS;
				}

				spointer++;

				continue;
			} else
			/* Direction: From Card */
			if( cse==2 ) {
				/* Receive data from card */
				if( read( fd_fromCard[0], &byte, 1 )!=1 ) {
					return CTAPI_RV_ERR_TRANS;
				}

				if( (sizeof(rsp)-pointer)>0 ) {
					rsp[pointer] = byte;
					rsplen++;
					pointer++;
				}

				continue;
			} else {
				return( CTAPI_RV_ERR_TRANS );
			}
		}

		/* NULL */
		if( byte==0x60 ) {
			/* Do nothing */
			continue;
		}

		/* SW1 */
		if( ((byte&0xF0)==0x60) || ((byte&0xF0)==0x90) ) {
			if( (sizeof(rsp)-pointer)>0 ) {
				rsp[pointer]=byte;
				pointer++;
				rsplen++;
			}

			/* Read SW2 */
			if( read( fd_fromCard[0], &byte, 1 )!=1 ) {
				return CTAPI_RV_ERR_TRANS;
			}

			if( (sizeof(rsp)-pointer)>0 ) {
				rsp[pointer]=byte;
				pointer++;
				rsplen++;
			}

			/* Don't copy more than requested */
			if( rsplen>*lenr ) {
				memcpy( response, rsp, *lenr-2 );
				memcpy( response+*lenr-2, rsp+rsplen-2, 2 );
			} else {
				memcpy( response, rsp, rsplen );
				*lenr=rsplen;
			}

			return( CTAPI_RV_OK );
		}

	}

	return( CTAPI_RV_ERR_TRANS );
}

IS8 CT_iccCmd( IU8 *dad, IU8 *sad, IU16 lenc, IU8 *command,
	IU16 *lenr, IU8 *response )
{
	*dad=CTAPI_AD_HOST;
	*sad=CTAPI_AD_ICC1;

	if( !pidVirtuaCard ) {
		*sad=CTAPI_AD_CT;
		response[0]=0x6E;
		response[1]=0x00;
		*lenr=2;
		return CTAPI_RV_OK;
	}

	return CT_t0( lenc, command, lenr, response );
}

IS8 CT_init( IU16 ctn, IU16 pn )
{
	if( (cthandle>=0) || (pn!=CTAPI_PORT) ) return CTAPI_RV_ERR_INVALID;

	cthandle=ctn;
	pidVirtuaCard=0;

	return CTAPI_RV_OK;
}

IS8 CT_data( IU16 ctn, IU8 *dad, IU8 *sad, IU16 lenc, IU8 *command,
	IU16 *lenr, IU8 *response )
{
	if( (cthandle<0) ||
		(ctn!=cthandle) ||
		(dad==NULL) ||
		(sad==NULL) ||
		(command==NULL) ||
		(lenr==NULL) ||
		(response==NULL) ||
		(lenc<4) ||
		(*lenr<2) ||
		(*sad!=CTAPI_AD_HOST) ) return CTAPI_RV_ERR_INVALID;

	// TODO: Check for well formed APDUs

	switch( *dad ) {
	case CTAPI_AD_CT:
		*dad = CTAPI_AD_HOST;
		*sad = CTAPI_AD_CT;
		if( command[0]!=0x20 ) {
			response[0]=0x6E;
			response[1]=0x00;
			*lenr=2;
			return CTAPI_RV_OK;
		}
		return CT_ifdCmd( lenc, command, lenr, response );
	case CTAPI_AD_ICC1:
		return CT_iccCmd( dad, sad, lenc, command, lenr, response );
	default:
		return CTAPI_RV_ERR_INVALID;
	}
}

IS8 CT_close( IU16 ctn )
{
	IS8 rv;

	if( (cthandle<0) || (ctn!=cthandle) ) return CTAPI_RV_ERR_INVALID;
	cthandle=-1;

	if( (rv=CT_stopIcc())!=CTAPI_RV_OK )
		return rv;

	return CTAPI_RV_OK;
}

