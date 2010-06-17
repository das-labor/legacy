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
#ifndef CRYPTOEXCEPTIONS_HXX_
#define CRYPTOEXCEPTIONS_HXX_

#include <stdexcept>
#include <string>
#include <iostream>

namespace crypto {
    /**
     * @brief Base class of all exceptions of package @ref crypto.
     *
     * @author Christian Stueble (stueble@sirrix.com)
     */
    class CryptoException  : public std::runtime_error {
    public:
        /// Creates a new base exception of package @ref crypto.
        CryptoException( const std::string &message="" );

        /// Allow inheritance.
        virtual ~CryptoException() throw();

        /// @return The error message.
        std::string getErrorMessage() const;
    };
      
    // ------------------- functions -------------------------
    inline std::ostream& operator<<( std::ostream &ostr, const CryptoException &e ) {
        return ostr << e.getErrorMessage();
    };
}

#endif /*FRAMEWORKEXCEPTION_HXX_*/
