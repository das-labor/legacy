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
	\brief Authentication functions.

	$Id: auth.c,v 1.21 2003/03/30 19:43:15 m Exp $
*/

#include <config.h>
#include <auth.h>
#include <crypt.h>
#include <fs.h>
#include <fstream.h>
#include <hal.h>
#include <string.h>
#include <sw.h>
#include <tea.h>
#include <tools.h>
#include <transaction.h>
#include <types.h>

#if CONF_WITH_KEYAUTH==1
/*! \brief Value of the last with Get Challenge fetched random challenge. */
uint8_t challenge[CRYPT_BLOCK_LEN];
uint8_t challvalidity;
#endif /* CONF_WITH_KEYAUTH */

#if (CONF_WITH_PINAUTH==1) || (CONF_WITH_KEYAUTH==1)

/*! \brief Variable indicating current authentication state.

	Single bit flags indicate, which authorization process has been
	successful. The flag is cleared, at the reset of the card and after
	unsuccessful authorization processes.

	Valid flags are:

	- AUTH_FLAG_PIN
	- AUTH_FLAG_KEY

*/
uint8_t authstate;

bool auth_init( void )
{
	authstate=0;
	challvalidity=0;

	return TRUE;
}

bool auth_checkAc( uint8_t ac )
{
	sw_set( SW_ACCESS_DENIED );

	switch( ac ) {
	case AUTH_AC_NEV:
	default:
		return FALSE;
	case AUTH_AC_PIN:
		if( !(authstate&AUTH_FLAG_PIN) ) return FALSE;
		break;
	case AUTH_AC_ADM:
		if( !(authstate&AUTH_FLAG_KEY) ) return FALSE;
		break;
	case AUTH_AC_POA:
		if( !(authstate&(AUTH_FLAG_PIN|AUTH_FLAG_KEY)) ) return FALSE;
		break;
	case AUTH_AC_PAA:
		if( (authstate&(AUTH_FLAG_PIN|AUTH_FLAG_KEY)) !=
			(AUTH_FLAG_PIN|AUTH_FLAG_KEY) )
			return FALSE;
		break;
	case AUTH_AC_ALW:
		break;
	}

	return TRUE;
}

#if CONF_WITH_FILESYSTEM==1
bool auth_getCheckPinLen( uint8_t pintype, uint8_t pinlen, uint16_t *addr,
	uint8_t *len, uint16_t *rcaddr, uint8_t *maxret )
{
    S_FPATH fp;
    S_FSTREAM fs;
    S_FINFO fi;
	uint16_t fsize;

	fp.df=0xFFFF;

	/* Get FID depending on pin type */
	switch( pintype ) {
	case PIN_TYPE_PIN:
	case PIN_TYPE_PUK:
		fp.ef=AUTH_FID_PINPUK;
		fsize=PIN_LEN+1+PUK_LEN+1; /* PIN+CNTR+PUK+CNTR */
		break;
	case PIN_TYPE_EXT:
		fp.ef=AUTH_FID_EXTAUTH;
		fsize=ADM_KEY_LEN+1; /* KEY+CNTR */
		break;
	case PIN_TYPE_INT:
		fp.ef=AUTH_FID_INTAUTH;
		fsize=INT_KEY_LEN+1; /* KEY+Dummy CNTR */
		break;
	default:
		sw_set( SW_OTHER );
		return FALSE;
	}

    /* Get file data */
    if( !fs_getData( &fp, &fs, &fi ) ) {
		if( sw==SW_FILE_NOT_FOUND ) sw_set( SW_REF_DATA_NOT_FOUND );
		return FALSE;
	}

    /* Correct file type? */
    if( (fi.type!=FS_TYPE_EF) || (fs.size!=fsize) ) {
		sw_set( SW_REF_DATA_NOT_FOUND );
		return FALSE;
    }

	/* Get address and length depending on pin type */
	switch( pintype ) {
	case PIN_TYPE_PIN:
		*addr=fs.start;
		*len=PIN_LEN;
		*rcaddr=fs.start+PIN_LEN;
		*maxret=PIN_MAX_RETRIES;
		break;
	case PIN_TYPE_PUK:
		*addr=fs.start+PIN_LEN+1;
		*len=PUK_LEN;
		*rcaddr=fs.start+PIN_LEN+1+PUK_LEN;
		*maxret=PUK_MAX_RETRIES;
		break;
	case PIN_TYPE_EXT:
	case PIN_TYPE_INT:
		*addr=fs.start;
		*len=ADM_KEY_LEN;
		*rcaddr=fs.start+ADM_KEY_LEN;
		*maxret=KEY_MAX_RETRIES;
		break;
	default:
		sw_set( SW_OTHER );
		return FALSE;
	}

	/* Check with provided length */
	if( pinlen!=*len ) {
		sw_set( SW_WRONG_LEN );
		return FALSE;
	}

	return TRUE;
}
#else /* CONF_WITH_FILESYSTEM!=1 */
bool auth_getCheckPinLen( uint8_t pintype, uint8_t pinlen, uint16_t *addr,
	uint8_t *len, uint16_t *rcaddr, uint8_t *maxret )
{
	/* Get address and length depending on pin type */
	switch( pintype ) {
	case PIN_TYPE_PIN:
		*addr=PIN_ADDR;
		*len=PIN_LEN;
		*rcaddr=PIN_RETCNTR_ADDR;
		*maxret=PIN_MAX_RETRIES;
		break;
	case PIN_TYPE_PUK:
		*addr=PUK_ADDR;
		*len=PUK_LEN;
		*rcaddr=PUK_RETCNTR_ADDR;
		*maxret=PUK_MAX_RETRIES;
		break;
	case PIN_TYPE_EXT:
		*addr=ADM_KEY_ADDR;
		*len=ADM_KEY_LEN;
		*rcaddr=ADM_RETCNTR_ADDR;
		*maxret=KEY_MAX_RETRIES;
		break;
	case PIN_TYPE_KEY:
		*addr=INT_KEY_ADDR;
		*len=INT_KEY_LEN;
		*rcaddr=INT_RETCNTR_ADDR;	/* Dummy */
		*maxret=0;
		break;
	default:
		sw=SW_OTHER;
		return FALSE;
	}

	/* Check with provided length */
	if( pinlen!=*len ) {
		sw=SW_WRONG_LEN;
		return FALSE;
	}

	return TRUE;
}
#endif /* CONF_WITH_FILESYSTEM!=1 */

bool auth_setPin( uint8_t pintype, uint8_t *pin, uint8_t pinlen )
{
	uint16_t addr, rcaddr;
	uint8_t len, maxret;

	if( !auth_getCheckPinLen( pintype, pinlen, &addr, &len, &rcaddr, &maxret ) )
		return FALSE;

#if CONF_WITH_TRANSACTIONS==1
	if( !( ta_setdata( &maxret, sizeof(maxret), rcaddr ) &&
		ta_commit() &&
		ta_setdata( pin, len, addr ) &&
		ta_commit() ) )
		return FALSE;
#else /* CONF_WITH_TRANSACTIONS!=1 */
	if( !( hal_eeprom_write( rcaddr, &maxret, sizeof(maxret) ) &&
		hal_eeprom_write( addr, pin, len ) ) )
		return FALSE;
#endif /* CONF_WITH_TRANSACTIONS!=1 */

	return TRUE;
}

bool auth_verifyPin( uint8_t pintype, uint8_t *pin, uint8_t pinlen )
{
	uint16_t addr;
	uint16_t rcaddr;
	uint8_t len;
	uint8_t retrcntr;
	uint8_t maxret;
	uint8_t diff=0;
	uint8_t b;

	if( pintype==PIN_TYPE_PIN ) authstate&=~AUTH_FLAG_PIN;
	if( pintype==PIN_TYPE_EXT ) authstate&=~AUTH_FLAG_KEY;

	if( !auth_getCheckPinLen( pintype, pinlen, &addr, &len, &rcaddr,
		&maxret ) )
		return FALSE;

	if( !hal_eeprom_read( &retrcntr, rcaddr, sizeof(retrcntr) ) )
		return FALSE;
	if( (!retrcntr) || (retrcntr&0xF0) ) {
		sw_set( SW_AUTH_BLOCKED );
		return FALSE;
	}

	/* Compare PIN - Important to compare all bytes */
	while( len-- ) {
		if( !hal_eeprom_read( &b, addr++, 1 ) ) return FALSE;

		/* Compare single byte */
		diff|=b^*pin++;
	}

	if( diff ) {
		if(retrcntr) retrcntr--;
	} else {
		retrcntr=maxret;
	}

	/* Retry counter update.
	 * WARNING: This scheme is NOT secure !!!! */
#if CONF_WITH_TRANSACTIONS==1
	/* Transaction for retry counter update. */
	if( !( ta_setdata( &retrcntr, sizeof(retrcntr), rcaddr ) &&
		ta_commit() ) )
		return FALSE;
#else /* CONF_WITH_TRANSACTIONS!=1 */
	if( hal_eeprom_write( rcaddr, &retrcntr, sizeof(retrcntr) ) )
		return FALSE;
#endif /* CONF_WITH_TRANSACTIONS!=1 */

	if( diff ) {
		sw_set( SW_COUNTER|(retrcntr&0xF) );
		return FALSE;
	}

	if( pintype==PIN_TYPE_PIN ) authstate|=AUTH_FLAG_PIN;
	else if( pintype==PIN_TYPE_EXT ) authstate|=AUTH_FLAG_KEY;

	return TRUE;
}
#endif /* (CONF_WITH_PINAUTH==1) || (CONF_WITH_KEYAUTH==1) */

#if CONF_WITH_KEYAUTH==1
bool auth_getChallenge( uint8_t *dst, uint8_t rndlen )
{
	if( rndlen!=CRYPT_BLOCK_LEN ) {
		sw_set( SW_WRONG_LEN );
		return FALSE;
	}

	hal_rnd_getBlock( dst );

	memcpy( challenge, dst, CRYPT_BLOCK_LEN );

	challvalidity=2;

	return TRUE;
}

bool auth_createVerifyCryptogram( uint8_t *cry, uint8_t crylen, bool create,
	bool enc )
{
	uint16_t addr, rcaddr;
	uint8_t len, maxret;
	uint32_t key[4];
	uint8_t retcntr;

	if( crylen!=CRYPT_BLOCK_LEN ) {
		sw_set( SW_WRONG_LEN );
		return FALSE;
	}

	if( create ) {
		if( !auth_checkAc( AUTH_AC_POA ) ) return FALSE;

		if( !auth_getCheckPinLen( PIN_TYPE_INT, ADM_KEY_LEN, &addr, &len,
			&rcaddr, &maxret ) ) return FALSE;
	} else {
		if( !challvalidity ) {
			sw_set( SW_WRONG_CONDITION );
			return FALSE;
		}

		authstate&=~AUTH_FLAG_KEY;

		if( !auth_getCheckPinLen( PIN_TYPE_EXT, ADM_KEY_LEN, &addr, &len,
			&rcaddr, &maxret ) ) return FALSE;

		/* Read retry counter */
		if( !hal_eeprom_read( &retcntr, rcaddr, 1 ) )
			return FALSE;

		if( !retcntr ) {
			sw_set( SW_AUTH_BLOCKED );
			return FALSE;
		}
	}

	/* Read key */
	if( !hal_eeprom_read( (uint8_t*)key, addr, len ) ) return FALSE;

	if( create ) {
#if CONF_WITH_DECRYPT==1
		if( enc ) crypt_enc( cry, key );
		else crypt_dec( cry, key );
#else /* CONF_WITH_DECRYPT!=1 */
		crypt_enc( cry, key );
#endif /* CONF_WITH_DECRYPT!=1 */

		return TRUE;
	}

	/* Encrypt challenge */
	crypt_enc( challenge, key );

	/* Compare result */
	if( memcmp( cry, challenge, crylen ) ) {
		/* Retry counter update.
		 * WARNING: This scheme is NOT secure !!!! */
		if( retcntr ) retcntr--;
#if CONF_WITH_TRANSACTIONS==1
		/* Transaction for retry counter update. */
		if( ta_setdata( &retcntr, 1, rcaddr ) &&
			ta_commit() ) {}
#else /* CONF_WITH_TRANSACTIONS!=1 */
		hal_eeprom_write( rcaddr, &retcntr, 1 );
#endif /* CONF_WITH_TRANSACTIONS!=1 */
		sw_set( SW_COUNTER|(retcntr&0x0F) );
		return FALSE;
	}

	authstate|=AUTH_FLAG_KEY;

	return TRUE;
}
#endif /*CONF_WITH_KEYAUTH==1*/

