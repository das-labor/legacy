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
	\brief Declarations for virtual CT-API library.

	$Id: ctapi.h,v 1.2 2002/12/22 23:16:15 m Exp $
*/

#ifndef H_CTAPI
#define H_CTAPI

#include <types.h>

#define CTAPI_AD_ICC1			0
#define CTAPI_AD_CT				1
#define CTAPI_AD_HOST			2

#define CTAPI_RV_OK				   0
#define CTAPI_RV_ERR_INVALID	  -1
#define CTAPI_RV_ERR_CT			  -8
#define CTAPI_RV_ERR_TRANS		 -10
#define CTAPI_RV_ERR_MEMORY		 -11
#define CTAPI_RV_ERR_HOST		-127
#define CTAPI_RV_ERR_HTSI		-128

#define CTAPI_PORT				1

#define CTAPI_CLA				0x20
#define CTAPI_INS_RESET_CT		0x11
#define CTAPI_INS_REQUEST_ICC	0x12
#define CTAPI_INS_GET_STATUS	0x13
#define CTAPI_INS_EJECT_ICC		0x14
#define CTAPI_INS_EEPROM		0xE0

typedef unsigned char	IU8;
typedef signed char		IS8;
typedef unsigned short	IU16;
typedef signed short	IS16;
typedef unsigned long	IU32;
typedef signed long		IS32;

extern int		fd_fromCard[2], fd_toCard[2];
extern int		loadEepromFile;
extern int		saveEepromFile;
extern char		eepromFile[256];
extern int		fdEepromFile;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

IS8 CT_forkIcc( void );
IS8 CT_stopIcc( void );
IS8 CT_ifdCmdEepromFile( IU16 lenc, IU8 *command, IU16 *lenr, IU8 *response );

IS8 CT_ifdCmdResetCT( IU16 lenc, IU8 *command, IU16 *lenr, IU8 *response );
IS8 CT_ifdCmdRequestICC( IU16 lenc, IU8 *command, IU16 *lenr, IU8 *response );
IS8 CT_ifdCmdGetStatus( IU16 lenc, IU8 *command, IU16 *lenr, IU8 *response );
IS8 CT_ifdCmdEjectICC( IU16 lenc, IU8 *command, IU16 *lenr, IU8 *response );
IS8 CT_ifdCmd( IU16 lenc, IU8 *command, IU16 *lenr, IU8 *response );
IS8 CT_iccCmd( IU8 *dad, IU8 *sad, IU16 lenc, IU8 *command,
	IU16 *lenr, IU8 *response );
IS8 CT_init( IU16 ctn, IU16 pn );
IS8 CT_data( IU16 ctn, IU8 *dad, IU8 *sad, IU16 lenc, IU8 *command,
	IU16 *lenr, IU8 *response );
IS8 CT_close( IU16 ctn );

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* H_CTAPI */

