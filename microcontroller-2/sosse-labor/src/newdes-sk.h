/*! @file
	\brief NEWDES-SK declarations.

	$Id: newdes-sk.h,v 1.1 2003/03/30 12:42:21 m Exp $
*/

#ifndef SOSSE_NEWDESSK_H
#define SOSSE_NEWDESSK_H

#include <types.h>

#define NEWDESSK_KEY_LEN	15	//!< NEWDES-SK key size.
#define NEWDESSK_BLOCK_LEN	8	//!< NEWDES-SK block length.

/*!	\brief NEWDES-SK encryption function.

	This function encrypts <EM>v</EM> with <EM>k</EM> and returns the
	encrypted data in <EM>v</EM>.

	\param v Array of eight iu8 values containing the data block.
	\param k Array of 15 iu8 values containing the key.
*/
void newdessk_enc( uint8_t* v, uint8_t* k );

/*!	\brief NEWDES-SK decryption function.

	This function decrypts <EM>v</EM> with <EM>k</EM> and returns the
	decrypted data in <EM>v</EM>.

	\param v Array of eight iu8 values containing the data block.
	\param k Array of 15 iu8 values containing the key.
*/
void newdessk_dec( uint8_t* v, uint8_t* k );

#endif

