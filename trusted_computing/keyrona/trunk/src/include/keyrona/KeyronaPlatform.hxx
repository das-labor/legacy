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
 * @file        KeyronaPlatform.hxx
 * @brief       Implementation of the Keyrona Key class.
 * @version     $Revision: 800 $
 * @date        Last modification at $Date: 2009-08-11 18:44:01 +0200 (Tue, 11 Aug 2009) $ by $Author: selhorst $
 */

#ifndef _KEYRONAPLATFORM_HXX
#define	_KEYRONAPLATFORM_HXX

#include <Keyrona.hxx>
#include <KeyronaDate.hxx>
#include <KeyronaLogin.hxx>
#include <KeyronaSubject.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

namespace keyrona
{
    // required for cross-reference between KeyronaPlatform and KeyronaSubject
    class KeyronaSubject;
    class KeyronaPlatform
    {
        private:
            bool            valid;

        public:

			KeyronaPlatform();

            ~KeyronaPlatform();

            /*!
	     *  @brief checks if a key is valid
             *  @return, boolean, determines whether or not the key is valid
             */
	    bool            isValid()         { return valid; };


        private:
            /*!
             *  @brief  disabled copy constructor
             */
            KeyronaPlatform ( const KeyronaPlatform& );

            /*!
             *  @brief  disabled assignment operator
             */
            KeyronaPlatform& operator=( const KeyronaPlatform& );
    };
};


#endif	/* _KeyronaPlatform_HXX */
