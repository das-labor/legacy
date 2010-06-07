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
#ifndef HMAC_HXX_
#define HMAC_HXX_

#include <Hash.hxx>

struct hmac_ctx_st; // predefinition

namespace crypto {

    /**
     * @brief Calculates HMAC digests.
     *
     * Initialize with a streamable object, continue to update the digest via
     * the update() interface, and retrieve the resulting digest through the
     * getDigest() interface.
     *
     * @author Patrick McKelvy, Christian Stueble
     */
    class HMAC : public Hash {
    public:
        HMAC( const utils::ByteVector& obj );
        ~HMAC();

		/// @see Hash
		virtual void hash( const utils::ByteVector &message );

		/// @see Hash
        virtual const utils::ByteVector getDigest();

    private:
    	HMAC( const HMAC & );	       ///< disable assignment (or implement it).
    	HMAC& operator=(const HMAC &); ///< diable assignment (or implement it).

        hmac_ctx_st    *myHMAC;

        void cleanUp();
    };
}   // namespace crypto

#endif     // HMAC_DIGEST_HXX_
