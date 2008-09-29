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

/* $Id: main.c,v 1.31 2002/12/24 13:33:11 m Exp $ */

/*! @file
	\brief main() function with command loop.
*/

#include <config.h>
#include <auth.h>
#include <commands.h>
#include <sw.h>
#include <fs.h>
#include <hal.h>
#include <t0.h>
#include <transaction.h>

/*! \brief Main function containing command interpreter loop.

	At the end of the loop, sw is sent as the status word.

	This function does never return.
*/
#if defined(CTAPI)
void sosse_main( void )
#else
int main( void )
#endif
{
	uint8_t i, len, b;

	/* TODO: On error? */
	hal_init();

	/* Send ATR */
	/* TODO: Possible from EEPROM? */
	hal_io_sendByteT0( 0x3B );

#if CONF_WITH_LGGING==1
	log_init();
#endif

	resplen = 0;

#if CONF_WITH_TRANSACTIONS==1
	/* Commit transactions not yet done. */
	/* TODO: On error? */
	ta_commit();
#endif /* CONF_WITH_TRANSACTIONS */

	/* Initialize FS state in RAM. */
	/* TODO: On error? */
	fs_init();

#if CONF_WITH_PINAUTH==1
	/* Initialize authentication state. */
	/* TODO: On error? */
	auth_init();
#endif /* CONF_WITH_PINAUTH==1 */

	if( !(hal_eeprom_read( &len, ATR_LEN_ADDR, 1 ) &&
		(len<=ATR_MAXLEN)) )
		for(;;) {}

	for( i=1; i<len; i++ ) {
		if( !hal_eeprom_read( &b, ATR_ADDR+i, 1 ) ) for(;;) {}
		hal_io_sendByteT0( b );
	}

	/* Command loop */
	for(;;) {
		for( i=0; i<5; i++ ) {
			header[i] = hal_io_recByteT0();
		}

#if CONF_WITH_KEYAUTH==1
		if( challvalidity ) challvalidity--;
#endif /* CONF_WITH_KEYAUTH==1 */

#if CONF_WITH_TRNG==1
		hal_rnd_addEntropy();
#endif

		if( (header[0]&0xFC)==CLA_PROP ) {
			switch( header[1]&0xFE ) {
#if CONF_WITH_TESTCMDS==1
			case INS_WRITE:
				cmd_write();
				break;
			case INS_READ:
				cmd_read();
				break;
#endif /* CONF_WITH_TESTCMDS==1 */
#if CONF_WITH_FUNNY==1
			case INS_LED:
				cmd_led();
				break;
#endif /* CONF_WITH_FUNNY==1 */
#if CONF_WITH_PINCMDS==1
			case INS_CHANGE_PIN:
				cmd_changeUnblockPIN();
				break;
#endif /* CONF_WITH_PINCMDS==1 */
#if CONF_WITH_CREATECMD==1
			case INS_CREATE:
				cmd_create();
				break;
#endif /* CONF_WITH_CREATECMD==1 */
#if CONF_WITH_DELETECMD==1
			case INS_DELETE:
				cmd_delete();
				break;
#endif /* CONF_WITH_DELETECMD==1 */
#if CONF_WITH_KEYCMDS==1
			case INS_EXTERNAL_AUTH:
				cmd_extAuth();
				break;
#endif /* CONF_WITH_KEYCMDS==1 */
#if CONF_WITH_KEYCMDS==1
			case INS_GET_CHALLENGE:
				cmd_getChallenge();
				break;
#endif /* CONF_WITH_KEYCMDS==1 */
			case INS_GET_RESPONSE:
				cmd_getResponse();
				break;
#if CONF_WITH_KEYCMDS==1
			case INS_INTERNAL_AUTH:
				cmd_intAuth();
				break;
#endif /* CONF_WITH_KEYCMDS==1 */
			case INS_READ_BINARY:
				cmd_readBinary();
				break;
			case INS_SELECT:
				cmd_select();
				break;
#if CONF_WITH_PINCMDS==1
			case INS_UNBLOCK_PIN:
				cmd_changeUnblockPIN();
				break;
#endif /* CONF_WITH_PINCMDS==1 */
			case INS_UPDATE_BINARY:
				cmd_updateBinary();
				break;
#if CONF_WITH_KEYCMDS==1
			case INS_VERIFY_KEY:
				cmd_verifyKeyPIN();
				break;
#endif /* CONF_WITH_KEYCMDS==1 */
#if CONF_WITH_PINCMDS==1
			case INS_VERIFY_PIN:
				cmd_verifyKeyPIN();
				break;
#endif /* CONF_WITH_PINCMDS==1 */
			default:
				t0_sendWord( SW_WRONG_INS );
			}
		} else {
			t0_sendWord( SW_WRONG_CLA );
		}

#if CONF_WITH_TRNG==1
		hal_rnd_addEntropy();
#endif

		/* Return the SW in sw */
		t0_sendSw();
	}
}

