// Copyright (C) 2008, 2009 by Sirrix AG security technologies
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
/*!
 *  @file       Hash.hxx
 *  @brief      Generic (root) interface for all cryptographic hash algorithms
 *  @author     Christian Stueble (stueble@acm.org)
 *  @version    $Revision: 449 $
 *  @date       Last modification at $Date: 2009-04-06 00:16:17 +0200 (Mon, 06 Apr 2009) $ by $Author: selhorst $
 *  @note       None
 *  @reviews
 */

#ifndef Hash_hxx_included
#define Hash_hxx_included

#include <ByteVector.hxx>
#include <iostream>

namespace crypto {
	/**
	 *  @brief      Generic interface of all cryptographic keyed and keyless hash algorithms.
	 *  @author     Christian Stueble (stueble@acm.org)
	 *
	 *  To hash byte strings, the method hash() can be invoked 0 to n times. To get
	 *  the resulting hash value, the method getDigest() has to be invoked the last time.
	 *
	 *  After invocation of getDigest() the hashobject will be reinitialized, i.e., it can be
	 *  reused for another calculation.
	 */
	class Hash {
	public:
		/// Virtual destructor to allow inheritance.
		virtual ~Hash();

	   /**
	    *  @brief  Operates on a chunk of bytes to be hashed.
	    *    @param  message message that will be hashed
	    */
      virtual void hash( const utils::ByteVector &message ) = 0;

	   /// @brief  Returns the resulting digest.
	   virtual const utils::ByteVector getDigest() = 0;

	   /**
	    *  @brief  Hashes a byte string and returns the resulting digest.
	    *    @param  message message that will be hashed
	    *  @return const ByteString hash value of message
	    *  @note   This functions hashes a ByteString at once, i.e.,
       *          no separate invocation of hash() necessary!
       */
      inline const utils::ByteVector calcDigest( const utils::ByteVector &message );
	}; // class Hash

	// ------------------------ inline implementation -------------------------------
	const utils::ByteVector Hash::calcDigest( const utils::ByteVector &message ) {
		hash( message );
		return getDigest();
	}
} // namespace crypto

#endif
