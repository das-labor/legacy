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
#include "SSL_Exceptions.hxx"
#include <openssl/err.h>
#include <openssl/ssl.h>

using namespace std;
using namespace crypto;

StringLoader SSL_Exception::myStringLoader;

StringLoader::StringLoader() { 
    SSL_load_error_strings(); 
}

SSL_Exception::SSL_Exception( const std::string &message ) :
    CryptoException( "SSL_Exception -> " + message + " -> " + ERR_reason_error_string( ERR_get_error() ) )
{}

SSL_Exception::~SSL_Exception() throw()
{}
