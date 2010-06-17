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
#include <MGF1.hxx>
#include <openssl/rsa.h>
#include <openssl/evp.h>

using namespace crypto;
using namespace utils;

MGF1::MGF1( const ByteVector &seed ) :
    mySeed( seed )
{}

ByteVector MGF1::getRandomBytes( const long int &length ) {
    ByteVector mask( length );

    PKCS1_MGF1( mask.toCArray(), length, mySeed.toCArray(), mySeed.size(), EVP_sha1() );

    return mask;
}
