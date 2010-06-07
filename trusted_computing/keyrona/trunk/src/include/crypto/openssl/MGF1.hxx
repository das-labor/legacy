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
#ifndef crypto_OPENSSL_MGF1_HXX_
#define crypto_OPENSSL_MGF1_HXX_

#include <Random.hxx>
#include <ByteVector.hxx>


namespace crypto {
	/**
	 * @brief This class is an implementation of the MGF1 random generator based on the Open SSL library.
	 *
	 * @author Anoosheh Zaerin (zaerin@sirrix.com)
	 */

    class MGF1 : public Random {
        public:
            MGF1( const utils::ByteVector &seed );

            /**
             * @brief Returns the indicated number of bytes generated in a pseudo-random fashion from the seed
             * @param intended length of the mask, at most 2^32
             * @return The indicated number of bytes generated in a pseudo-random fashion from the seed
             */
            utils::ByteVector getRandomBytes( const long int &length );

        private:
            utils::ByteVector mySeed;
    };

}

#endif /* crypto_OPENSSL_MGF1_HXX_ */
