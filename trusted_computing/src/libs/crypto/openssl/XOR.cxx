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
#include "XOR.hxx"

#include <CryptoExceptions.hxx>

using namespace crypto;
using namespace std;
using namespace utils;

XOR::XOR( const ByteVector& secret ) :
      SHA1(),
      mySecret( secret )
{}

XOR::~XOR()
{}

const ByteVector XOR::getDigest() {
      ByteVector  shaDigest = SHA1::getDigest();

      return xorMessages( shaDigest, mySecret );
}

const ByteVector XOR::xorMessages( const ByteVector &msg1, const ByteVector &msg2 ) {
      if ( msg1.size() != msg2.size() ) {
            ostringstream  ostr;
            ostr << "XOR Error: The messages to apply XOR encryption to in the function XOR::xorMessages() do not have the same size."
                     << " The first message is " << dec << msg1.size() << " bytes long while the second message is "
                  << dec << msg2.size() << " bytes long.";
            throw CryptoException( ostr.str() );

            return ByteVector();
      }

      ByteVector  result( msg1.size() );

      for( unsigned int byteNo = 0; byteNo < msg1.size(); byteNo++ )
            result[ byteNo ] = msg1[ byteNo ] ^ msg2[ byteNo ];

      return result;
}
