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
	\brief Transaction declarations.

	\note
	Is the current transaction implementation secure against tearing?
	IMO it should be nearly be. Let's consider following scenarios:

	1) Card is removed while writing the data for the transaction:
	This is no problem, because the state is still TRANSAC_STATE_CLEAN.

	2) Card is removed while writing the status byte:
	This is most likely no problem. If the state is anything other than
	TRANSAC_STATE_DATA, it is regarded as clean, which it is. The only
	problem is, when it accidently is TRANSAC_STATE_DATA, which is IMO
	very unlikely, because the erased state is totally different. But on
	the other side, if the card tearing occured shortly after the state
	update, the state really is TRANSAC_STATE_DATA.

	3) Card is removed while doing a commit and writing the data therein:
	This is no problem, because after the next reset the commit is tried
	again.

	4) Card is removed after a commit, while reseting the status byte:
	This is no problem, because most likely the new state is interpreted
	as TRANSAC_STATE_CLEAN, which it is, and if the new state is really
	TRANSAC_STATE_DATA, it is just a second time written.

	5) Card is removed while writing status byte in a rollback:
	This might be a problem, because the rollback might file, when
	the unlikely event occures, that the state is TRANSAC_STATE_DATA.
	But this can also happen, if the card is removed before the rollback,
	because the state is there already TRANSAC_STATE_DATA.

	$Id: transaction.h,v 1.9 2002/12/22 15:42:56 m Exp $
*/

#ifndef SOSSE_TRANSACTION_H
#define SOSSE_TRANSACTION_H

#include <types.h>

/*! Transaction buffer is clean. (Also all other values not being
	0xA5 are interpreted as clean. */
#define TRANSAC_STATE_CLEAN	0x00
//! Transaction buffer contains data.
#define TRANSAC_STATE_DATA	0xA5

/*! \brief Begins a transaction and sets the data.

	<EM>data/len</EM> bytes are written to the transaction buffer.
	The destination address is also stored there. Then the state is
	set to TRANSAC_STATE_DATA.

	\param data Pointer to the data to write into the transaction buffer.
	\param len Length of the data. (Max.: TRANSAC_DATA_LEN)
	\param dst Destination of the data in EEPROM.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.
*/
bool ta_setdata( uint8_t *data, uint8_t len, uint16_t dst );
/*! \brief Commits the data in the transaction buffer.

	Data is written to the destination specified in ta_setdata().
	The state is then set to clean.

	This function must be called after each reset to commit pending
	transactions.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.
*/
bool ta_commit( void );
/*! \brief Do a rollback and discard the data in the transaction buffer.

	The state is set to clean.

	\retval TRUE on success.
	\retval FALSE on failure. Error code given in sw.
*/
bool ta_rollback( void );

#endif /* SOSSE_TRANSACTION_H */
