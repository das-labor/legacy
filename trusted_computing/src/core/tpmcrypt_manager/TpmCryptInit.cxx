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


#include <TpmCrypt.hxx>
#include <TpmCryptManager.hxx>

using namespace std;
using namespace utils;
using namespace tpmcrypt;

enum myMenu
{
    createAdmin = 1,
    deleteAdmin,
    listAdmins,
    destroyKeys
};

// mapping between strings and int values
typedef map<string, int> t_StringMap;

void TpmCryptDestroyKeys()
{
	TpmCryptTPM myTPM;
	
	myTPM.remove_all_keys_by_uuid();
};

//================================================================================
//
bool TpmCryptListAdmins(TpmCryptStorage &myUserStorage)
{
    vector<string> myAdmins = mySubjectStorage.TpmCryptFindAllAdmins(myUserStorage);

    vector<string>::const_iterator Iterator;
    if (myAdmins.size())
    {
        cout << "We have the following admins: " << endl;

        Iterator=myAdmins.begin();
        while ( Iterator != myAdmins.end())
        {
            TpmCryptUser myUser(Iterator->first, myUserStorage);
            myUser.printUser(printVerbose);
            Iterator++;
        }
        return true;
    }
    else
    {
        cout << "No admins found." << endl;
        return false;
    }
};

//================================================================================
//
void TpmCryptDeleteAdmin(TpmCryptStorage &mySubjectStorage)
{
    if (loginAdmin(myUserStorage))
        cout << "Superuser successfully logged in!" << endl;

    if (!TpmCryptListAdmins(myUserStorage))
        return;

    vector<string> myAdmins = TpmCryptFindAllAdmins(myUserStorage);
    if (! myAdmins.size())
    {
        throw NoAdminAvailable("No admins found, please add an administrator first!");
    }

    // check existence of at least one more admin
    if (myAdmins.size() == MIN_NUMBER_OF_ADMINS)
    {
        cout << "Only '" << MIN_NUMBER_OF_ADMINS << "' admin(s) exist, can't offer any admins to delete!" << endl;
        return;
    }

    // get admin SubjectID
    string adminID;
    adminID = selectFromStringVector(myAdmins, "\nNumber:\t\tAdmin:", "Please select the administrator to be deleted:");

    if (!TpmCryptFindAdmin(myUserStorage, adminID))
        throw UnknownSubject("Subject '" + adminID + "' is not an admin.");

    if (verifySelection())
    {
        // load admin subject
        TpmCryptSubject myAdmin(adminID, myUserStorage);

        // delete admin
        myAdmin.deleteUser();

        cout << "Successfully deleted admin '" << adminID << "'." << endl;
        printToSyslog("TPMCRYPT: Admin '" + adminID + "' was removed from this system");
    }
};


//================================================================================
//
void TpmCryptCreateAdmin(TpmCryptStorage &myUserStorage)
{
    if (loginSuperUser())
        cout << "Superuser successfully logged in!" << endl;

    string myUsername = getStringFromUser("Please enter username for new admin: ");

    if (TpmCryptFindUser(mySubjectStorage, myUsername))
        throw InvalidUsername("TpmCryptCreateAdmin(): User already exists!");

    string myEMail = TPMCRYPT_PRIVATE_MODE_ADMIN_MAIL;
    string myC  = TPMCRYPT_PRIVATE_MODE_C;
    string myO  = TPMCRYPT_PRIVATE_MODE;
    string myOU = TPMCRYPT_PRIVATE_MODE;
    string mySP = TPMCRYPT_PRIVATE_MODE;
    string myL  = TPMCRYPT_PRIVATE_MODE;
    if (EnterpriseMode)
    {
        myEMail = getStringFromUser("Please enter email address for new admin: ");
        myC = getStringFromUser("Please enter countrycode for new admin: ");
        myO = getStringFromUser("Please enter organisation for new admin: ");
        myOU= getStringFromUser("Please enter organisation unit for new admin: ");
        mySP= getStringFromUser("Please enter state/province for new admin: ");
        myL = getStringFromUser("Please enter locality for new admin: ");
    }
	
	vector<string> possibleAdmins = TpmCryptFindAllAdmins(myUserStorage);
	
	if(possibleAdmins.size() = 0)
	{
		string del = getStringFromUser("Do you want delete all TPM Keys (Yes/No): ");
		if(del = "Yes")
			TpmCryptDestroyKeys();
	}

    TpmCryptUser newAdmin(SUBJECTTYPE_USER, myUsername, "0", myEMail, myC, myO, myOU, mySP, myL, true, myKeyDirectory, mySubjectStorage);
    if (! newAdmin.getMyUserUUID())
		throw NotCreated("Admin could not be created!");
    cout << "Successfully created new admin with SubjectID '" << newAdmin.getMySubjectID() << "'." << endl;
    printToSyslog("TPMCRYPT: Admin '" + myUsername + "' was added to this system");
    cout << "Creating new admin group..." << endl;    
}

//================================================================================
//
void TpmCryptInit     ( string initParam, TpmCryptConfigfile &myConfigfile )
{
    t_StringMap TpmCryptInitMap;
    TpmCryptInitMap["createAdmin"]    = createAdmin;
    TpmCryptInitMap["ca"]          = createAdmin;
    TpmCryptInitMap["deleteAdmin"] = deleteAdmin;
    TpmCryptInitMap["da"]          = deleteAdmin;
    TpmCryptInitMap["listAdmins"]  = listAdmins;
    TpmCryptInitMap["la"]          = listAdmins;
    TpmCryptInitMap["destroyKeys"] = destroyKeys;
	TpmCryptInitMap["dk"] 		   = destroyKeys;

    // Create User database storage object
    TpmCryptStorage myUserStorage();

    switch(TpmCryptInitMap[initParam])
    {
        case createAdmin:
            TpmCryptCreateAdmin(myUserStorage);
            break;
        case deleteAdmin:
            TpmCryptDeleteAdmin(myUserStorage);
            break;
        case listAdmins:
            TpmCryptListAdmins(myUserStorage);
            break;
        case destroyKeys:
            TpmCryptDestroyKeys();
            break;
        default:
            throw ParseError("invalid parameter");
    }
};
