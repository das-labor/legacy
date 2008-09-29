/*! @file
	\brief TEA declarations.

	Documentation for TEA is available at
	http://www.cl.cam.ac.uk/ftp/users/djw3/tea.ps.

	$Id: tea.h,v 1.5 2002/12/22 15:42:55 m Exp $
*/

#ifndef SOSSE_TEA_H
#define SOSSE_TEA_H

#include <types.h>

#define TEA_KEY_LEN		16	//!< TEA key size.
#define TEA_BLOCK_LEN	8	//!< TEA block length.

#define DELTA	0x9E3779B9	//!< Magic value. (Golden number * 2^31)
#define ROUNDS	32			//!< Number of rounds.

/*! \brief TEA encryption function.

	This function encrypts <EM>v</EM> with <EM>k</EM> and returns the
	encrypted data in <EM>v</EM>.

	\param v Array of two long values containing the data block.
	\param k Array of four long values containing the key.
*/
void tea_enc( uint32_t *v, uint32_t *k );

/*! \brief TEA decryption function.

	This function decrypts <EM>v</EM> with <EM>k</EM> and returns the
	decrypted data in <EM>v</EM>.

	\param v Array of two long values containing the data block.
	\param k Array of four long values containing the key.
*/
void tea_dec( uint32_t *v, uint32_t *k );

#endif /* SOSSE_TEA_H */

