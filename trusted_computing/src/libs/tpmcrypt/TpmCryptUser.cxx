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
 * @file        TpmCryptSubject.cxx
 * @brief       Implementation of TpmCrypts subject class.
 * @version     $Revision: 764 $
 * @date        Last modification at $Date: 2009-07-29 15:33:10 +0200 (Wed, 29 Jul 2009) $ by $Author: selhorst $
 */

#include <TpmCryptUser.hxx>
#include <TpmCryptTPM.hxx>


using namespace std;
using namespace utils;


TpmCryptUser::TpmCryptUser(string &name, string &email, string &country, string &organization, bool isAdmin, TpmCryptStorage &storageDB) :
			myUserUUID (0),
			myUserName (name),
			myUserEmail (email),
			myUserCountry (country),
			myUserOrganization (organization),
			myUserAdminFlag (isAdmin),
			myUserStorage (storageDB)
{
			
};

TpmCryptUser::TpmCryptUser(UInt32 uuid, TpmCryptStorage &storageDB) :
			myUserUUID (uuid),
			myUserName (""),
			myUserEmail (""),
			myUserCountry (""),
			myUserOrganization (""),
			myUserAdminFlag (0),
			myUserStorage (storageDB)
{
	
};

TpmCryptUser::TpmCryptUser(string name, TpmCryptStorage &storageDB) :
			myUserUUID (0),
			myUserName (name),
			myUserEmail (""),
			myUserCountry (""),
			myUserOrganization (""),
			myUserAdminFlag (0),
			myUserStorage (storageDB)
{
	
};

TpmCryptUser::~TpmCryptUser()
{
    if (myUserKey)
        delete myUserKey;
};

bool load(string &name, UInt32 uuid)
{
	if(uuid = 0)
	{
		myUserName = name;
		myUserUUID = myUserStorage.queryDB("users","u_uuid","name",name);
		myUserEmail = myUserStorage.queryDB("users","email","name",name);
		myUserCountry = myUserStorage.queryDB("users","country","name",name);
		myUserOrganization = myUserStorage.queryDB("users","organization","name",name);
		myUserIsAdmin = myUserStorage.queryDB("users","isAdmin","name",name);
	}
	else
	{
		myUserUUID = uuid;
		myUserName = myUserStorage.queryDB("users","name","u_uuid",uuid);
		myUserEmail = myUserStorage.queryDB("users","email","u_uuid",uuid);
		myUserCountry = myUserStorage.queryDB("users","country","u_uuid",uuid);
		myUserOrganization = myUserStorage.queryDB("users","organization","u_uuid",uuid);
		myUserIsAdmin = myUserStorage.queryDB("users","isAdmin","u_uuid",uuid);
	}
	
	myUserKeysUUID = myUserStorage.queryDB("user_keys","key_uuid","u_uuid",myUserUUID);
	myUserKeyType =
	myUserKeySize =
	myUserKeyValid = 
	
	
	return true;
};

void store()
{
	myUserStorage.storeDB("users","name","u_uuid",myUserName,myUserUUID);
	myUserStorage.storeDB("users","email","u_uuid",myUserEmail,myUserUUID);
	myUserStorage.storeDB("users","country","u_uuid",myUserCountry,myUserUUID);
	myUserStorage.storeDB("users","organization","u_uuid",myUserOrganization,myUserUUID);
	myUserStorage.storeDB("users","isAdmin","u_uuid",myUserIsAdmin,myUserUUID);
};

void print()
{
	cout << "Name: '" << myUserName << "' (ID: " << myUserUUID << ")" << endl;
    cout << "KeyUUID: '" << mySubjectKeyUUID << endl;
    cout << "KeyType: '" << mySubjectKeyType << endl;
    cout << mySubjectKey->printKeyInformation(this) << endl;
    cout << "\t" << "Type: '" << TpmCryptSubjectType[mySubjectType] << "'" << endl;
    
    cout << "\t" << "EMail: '" << mySubjectEMail << "'" << endl;
    cout << "\t" << "Countrycode: '" << mySubjectCountrycode << "'" << endl;
    cout << "\t" << "Organisation: '" << mySubjectOrganisation << "'" << endl;
    cout << "\t" << "OrganisationUnit: '" << mySubjectOrganisationUnit << "'" << endl;
    cout << "\t" << "State: '" << mySubjectState << "'" << endl;
    cout << "\t" << "Locality: '" << mySubjectLocality << "'" << endl;
    
    cout << "\t" << "Keyfile: '" << mySubjectKeyfile << "'" << endl;
    if (mySubjectType == SUBJECTTYPE_USER)
        cout << "\t" << "Subject is admin: '" << (mySubjectisAdmin ? TpmCryptSubject_TRUE:TpmCryptSubject_FALSE) << "'" << endl;
	
	cout << "\t" << "Subject's last login: ";
    if (myLastLogin.isValid())
        myLastLogin.printDate();
    else
        cout << "Subject has not logged in, yet!" << endl;
};
