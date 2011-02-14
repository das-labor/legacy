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

#ifndef _TPMCRYPTPLATFORM_HXX
#define	_TPMCRYPTPLATFORM_HXX

#include <string>
#include <vector>

#include <TpmCryptObject.hxx>

#include <TpmCrypt.hxx>
#include <TpmCryptStorage.hxx>
#include <TpmCryptDate.hxx>
#include <TpmCryptKey.hxx>


namespace tpmcrypt
{
    // required for cross-reference between KeryonaKey and TpmCryptSubject
    class TpmCryptKey;
    
    class TpmCryptPlatform : public TpmCryptObject
    {
       public:
			TpmCryptPlatform();
			TpmCryptPlatform(std::string &name);
			TpmCryptPlatform(UInt32 uuid);
			TpmCryptPlatform(std::string &name, std::vector<UInt8> pcrs);
			TpmCryptPlatform(cpnst TpmCryptPlatform &copy);
			
			virtual void load(std::string &name, UInt32 uuid);
			virtual void store();
			virtual void print();
			virtual void remove();
			virtual bool check();
			
			

       private:
			std::vector<UInt8> m_pcrs;
			std::string m_authSecret;

    };
};

#endif	/* _TPMCRYPTPLATFORM_HXX */

