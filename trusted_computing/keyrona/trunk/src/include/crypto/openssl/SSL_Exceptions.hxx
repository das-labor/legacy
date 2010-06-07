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
#ifndef SSL_EXCEPTIONS_HXX_
#define SSL_EXCEPTIONS_HXX_

#include <Types.hxx>
#include <CryptoExceptions.hxx>

namespace crypto {
    /**
     * @brief Helper class to automatically load the openssl error strings.
     * 
     * If used as a static member, this class automatically loads the SSL error strings. 
     */
    class StringLoader {
    public:
        StringLoader();
    };
        
    /**
     * @brief Base class of all exceptions of package @ref crypto.
     *
     * On creation, this class automatically reads the current OpenSSL error string using
     * ERR_get_error() and ERR_reason_error_string().
     * 
     * @author Christian Stueble (stueble@sirrix.com)
     */
    class SSL_Exception : public CryptoException {
    public:
        /// Creates a new SSL-based exception
        SSL_Exception( const std::string &message );

        /// Allow inheritance.
        virtual ~SSL_Exception() throw();
        
    protected:
        static StringLoader myStringLoader; ///< Automatically load the SSL error strings.
    };
}

#endif /*FRAMEWORKEXCEPTION_HXX_*/
