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

/**
 * @file        KeyronaCryptlib.hxx
 * @brief       Helper methods for cryptlib
 * @version     $Revision: 449 $
 * @date        Last modification at $Date: 2009-04-06 00:16:17 +0200 (Mon, 06 Apr 2009) $ by $Author: selhorst $
 */

#ifndef _KEYRONACRYPTLIB_HXX
#define	_KEYRONACRYPTLIB_HXX

#include <Keyrona.hxx>
#include <keyrona_cryptlib.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define MAX_CRYPT_BUFFER_SIZE   4096
#define CRYPTLIB_MAX_TRIES      10
#define RSA_1024_BIT            128
#define RSA_2048_BIT            256

void cryptlibCheck(int result, const std::string &what);
std::string cryptlibResultToString(int result);
bool convertPEM2p15(std::string &myPublicKey, std::string &myPrivateKey, std::string &myPassword, std::string &myLabel, std::string &myP15File);
void printCryptlibAlgo(int cryptAlgo);


#endif	/* _KEYRONACRYPTLIB_HXX */
