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
#ifndef crypto_RANDOM_HXX_
#define crypto_RANDOM_HXX_

#include <ByteVector.hxx>


namespace crypto {
	/**
	 * @brief Interface of all random generators
	 *
	 * @author Anoosheh Zaerin (zaerin@sirrix.com)
	 */
    class Random {
        public:
            /**
             * @brief general random method that returns ByteArray
             * @param intended length of the mask, at most 2^32
             * @return random byte values
             */
            virtual utils::ByteVector getRandomBytes( const long int &length ) = 0;

        protected:

            /**
             *  @brief   virtual destructor to allow inheritance
             */
            virtual ~Random()
            {};

    }; // class Random
}

#endif /* crypto_RANDOM_HXX_ */
