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
#ifndef crypto_OPENSSL_SHA1_HXX_
#define crypto_OPENSSL_SHA1_HXX_

#include <Hash.hxx>

struct SHAstate_st;  // predefinition

namespace crypto {

      /**
       * @brief This class is an implementation of the SHA1 algorithm based on the Open SSL library.
       *
       * @note Update the digest via hash() and retrieve the resulting digest through the getDigest() interface
       *
       * @author Patrick McKelvy (mckelvy@sirrix.com)
       * @author Christian Stueble (stueble@sirrix.com)
       */
      class SHA1 : public Hash {
         public:
            /// Creates a new SHA1 object
            SHA1();

            /// Clear resources dedicated to the instance
            ~SHA1();

            /// @see Hash
            virtual void                        hash( const utils::ByteVector &message );

            /// @see Hash
            virtual const utils::ByteVector     getDigest();

            /// Calculate the digest of the given message
            static const utils::ByteVector      calculateDigest( const utils::ByteVector &message );

         private:
            SHA1( const SHA1 & );	       ///< disable assignment (or implement it)
            SHA1& operator=(const SHA1 &); ///< diable assignment (or implement it)

            SHAstate_st    *mySHA;
      };

}   // namespace crypto

#endif   // crypto_OPENSSL_SHA1_HXX_
