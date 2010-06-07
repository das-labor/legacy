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
#include "HMAC.hxx"
#include <openssl/hmac.h>
#include <Types.hxx>

using namespace utils;

crypto::HMAC::HMAC( const ByteVector& bytes ) :
    myHMAC( new HMAC_CTX )
{
	HMAC_CTX_init( myHMAC );
    HMAC_Init( myHMAC, bytes.toCArray(), bytes.size(), EVP_sha1() );
}

crypto::HMAC::~HMAC() {
	cleanUp();
	delete myHMAC;
}

void crypto::HMAC::hash( const utils::ByteVector &message ) {
	HMAC_Update( myHMAC, message.toCArray(), message.size() );
}

const ByteVector crypto::HMAC::getDigest() {
	UInt8 digest[ 20 ];
    unsigned int dlen;

    HMAC_Final( myHMAC, digest, &dlen ); /// @warning Can dlen be larger than TPM_HASH_SIZE?
    cleanUp();

    return ByteVector( digest, dlen );
}

void crypto::HMAC::cleanUp() {
    #ifdef HAVE_HMAC_CTX_CLEANUP
        HMAC_CTX_cleanup( myHMAC );
    #else
        HMAC_cleanup( myHMAC );
    #endif
}
