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
#include "SHA1.hxx"

#include <Types.hxx>
#include <openssl/sha.h>
#include <iostream>
#include <Debugging.hxx>

using namespace std;
using namespace utils;

const UInt32    TPM_HASH_SIZE    = 20;

crypto::SHA1::SHA1() :
      mySHA( new SHA_CTX ) 
{
      SHA_Init( mySHA );
}

crypto::SHA1::~SHA1() {
      delete mySHA;
}

void crypto::SHA1::hash( const ByteVector &message ) {
      debug << "   SHA1: Updating with the value: " << hex << message << endl;

      SHA1_Update( mySHA, message.toCArray(), message.size() );
      debug << "   SHA1: After updating: " << hex << message << endl;
}

const ByteVector crypto::SHA1::getDigest() {
      UInt8 digest[ TPM_HASH_SIZE ];

      SHA1_Final( digest, mySHA );

      return ByteVector( digest, TPM_HASH_SIZE );
}

const ByteVector crypto::SHA1::calculateDigest( const utils::ByteVector &message ) {
      SHA1  shaMsg;

      return shaMsg.calcDigest( message );
}
