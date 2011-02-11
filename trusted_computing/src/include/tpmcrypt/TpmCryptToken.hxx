// Copyright (C) 2008, 2009 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
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
 * @file        TpmCryptSubject.hxx
 * @brief       Implementation of the TpmCrypt subject class.
 * @version     $Revision: 790 $
 * @date        Last modification at $Date: 2009-08-04 15:07:58 +0200 (Tue, 04 Aug 2009) $ by $Author: goericke $
 */

#ifndef _TPMCRYPTTOKEN_HXX
#define	_TPMCRYPTTOKEN_HXX

#include <TpmCrypt.hxx>
#include <TpmCryptStorage.hxx>
#include <TpmCryptDate.hxx>
#include <TpmCryptKey.hxx>

using namespace std;
using namespace utils;
using namespace tpmcrypt;

namespace tpmcrypt
{
    // required for cross-reference between KeryonaKey and TpmCryptSubject
    class TpmCryptKey;
    class TpmCryptToken
    {
       public:

       private:

    };
};

#endif	/* _TPMCRYPTTOKEN_HXX */

