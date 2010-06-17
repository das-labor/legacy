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
#ifndef crypto_OPENSSL_XOR_HXX_
#define crypto_OPENSSL_XOR_HXX_

#include <SHA1.hxx>

namespace crypto {

      /**
       * @brief This class implements the XOR encryption algorithm.
       *
       * @note Update the digest via hash() and retrieve the resulting digest through the getDigest() interface
       *
       * @author Patrick McKelvy (mckelvy@sirrix.com)
       */
      class XOR : public SHA1 {
         public:
            /// Creates a new XOR object
            XOR( const utils::ByteVector& secret );

            /// Destructor
            ~XOR();

            /// @see Hash
            virtual const utils::ByteVector  getDigest();

            /// XOR encryption to the given messages
            static const utils::ByteVector   xorMessages( const utils::ByteVector &msg1, const utils::ByteVector &msg2 );

         private:
            utils::ByteVector   mySecret;
      };

}   // namespace crypto

#endif   // crypto_OPENSSL_XOR_HXX_
