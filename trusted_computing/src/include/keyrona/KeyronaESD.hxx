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
 * @file        KeyronaESD.hxx
 * @brief       Implementation of the Keyrona ESD class.
 * @version     $Revision: 790 $
 * @date        Last modification at $Date: 2009-08-04 15:07:58 +0200 (Tue, 04 Aug 2009) $ by $Author: goericke $
 */

#ifndef _KEYRONAESD_HXX
#define	_KEYRONAESD_HXX

#include <Keyrona.hxx>
#include <KeyronaStorage.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

// ESD entries
const std::string KeyronaESD_ESDProperties= "ESDProperties";
const std::string KeyronaESD_isESD        = "isESD";
const std::string KeyronaESD_isESD_true    = "true";
const std::string KeyronaESD_isESD_false   = "false";

const std::string KeyronaESD_ESDName    = "Name";
const std::string KeyronaESD_ESDUUID = "UUID";
const std::string KeyronaESD_ESDDevice = "Device";

namespace keyrona
{
    class KeyronaESD
    {
        public:
        
        KeyronaESD( string ESDIdentifier, KeyronaStorage &storageDB );
        
        KeyronaESD( UInt32 ESDID, KeyronaStorage &storageDB );
        
        KeyronaESD( string &ESDName, string &ESDDevice, string &ESDUUID, KeyronaStorage &storageDB );
        
        void deleteESD();
        
        ~KeyronaESD();
        
        UInt32 getMyESDID()     { return myESDID; };
        
        string getMyESDIDString()   { return myESDIDString; };

		string getMyESDName()   { return myESDName; };
		
		string getMyESDUUID()   { return myESDUUID; };

		string getMyESDDevice()   { return myESDDevice; };
        
        private:
	    /*!
             *  @brief stores ESD in the database
	     */
            bool storeESD();
	    
	    /*!
             *  @brief loads ESD from the database
	     */
            bool loadESD(string &ESDIdentifier);
            
			//void checkUsername();

            UInt32  myESDID;
            string  myESDIDString;
            string  myESDName;
            string	myESDUUID;
            string  myESDDevice;

            KeyronaStorage &myESDStorage;
			
            /*!
             *  @brief  disabled copy constructor
             */
            KeyronaESD( const KeyronaESD& );

            /*!
             *  @brief  disabled assignment operator
             */
            KeyronaESD& operator=( const KeyronaESD& );
    };
};
#endif	/* _KEYRONAESD_HXX */
