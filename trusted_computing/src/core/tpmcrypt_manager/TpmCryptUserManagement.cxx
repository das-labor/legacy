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
#include <TpmCryptKey.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

enum myMenu
{
    createUser = 1,
    importUser,
    deleteUser,
    listUsers,
    changeUserCredential
};


// mapping between strings and int values
typedef map<string, int> t_StringMap;

//================================================================================
//
bool TpmCryptListUsers(TpmCryptStorage &mySubjectStorage, bool includeAdmins)
{
    vector<StringPair> myUsers = mySubjectStorage.findAllEntries(TpmCryptSubject_SubjectType, TpmCryptSubjectType_User);

    vector<utils::StringPair>::const_iterator Iterator;
    if (myUsers.size())
    {
        cout << "We have the following users ";
        if (includeAdmins)
            cout << "(admins included):" << endl;
        else
            cout << "(admins not included):" << endl;
        
        Iterator=myUsers.begin();
        while ( Iterator != myUsers.end())
        {
            if (!TpmCryptFindUser(mySubjectStorage, Iterator->first))
            {
                Iterator++;
                continue;
            }

            TpmCryptSubject *mySubject = new TpmCryptSubject (Iterator->first, mySubjectStorage);
            if (! mySubject->isAdmin())
            {
                mySubject->printSubject(printVerbose);
            }

            if ((mySubject->isAdmin()) && (includeAdmins))
            {
                mySubject->printSubject(printVerbose);
            }

            delete mySubject;
            Iterator++;
        }
        return true;
    }
    else
    {
        cout << "No users found." << endl;
        return false;
    }
};

//================================================================================
//
void TpmCryptDeleteUser(TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage, TpmCryptStorage &myVolumeStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (! TpmCryptListUsers(mySubjectStorage, false))
        return;

    vector<string> myUsers = TpmCryptFindAllUsers(mySubjectStorage, false);
    if (! myUsers.size())
    {
        cout << "No users found." << endl;
        return;
    }
    string userToDelete = selectFromStringVector(myUsers, "\nNumber:\t\tUser:", "Please select the user to be deleted:");

    if (!TpmCryptFindSubject(mySubjectStorage, userToDelete))
        throw UnknownSubject("User '" + userToDelete + "' not found.");

    TpmCryptSubject *mySubject = new TpmCryptSubject(userToDelete, mySubjectStorage);
    if (mySubject->isAdmin())
    {
        cout << "You are not allowed to delete an admin. Please choose --init deleteAdmin() to do so." << endl;
        cout << "Failed to delete user '" << userToDelete << "'!" << endl;
        return;
    }

    if (mySubject->getMySubjectType() != SUBJECTTYPE_USER)
        throw InvalidSubjectType("You can only delete users!");

    if (verifySelection())
    {
        string myUserSubjectID = mySubject->getMySubjectIDString();

        // DELETING USER FROM EVERY GROUP
        vector<StringPair> deleteUserFromGroups = myGroupStorage.findAllEntries(myUserSubjectID);
        // deleting user in every group
        vector<utils::StringPair>::const_iterator GroupIterator;

        if (deleteUserFromGroups.size())
        {
            cout << "Deleting user from the following groups:" << endl;
            GroupIterator=deleteUserFromGroups.begin();
            while ( GroupIterator != deleteUserFromGroups.end())
            {
                StringPair myStringpair = *(GroupIterator);
                string myGroupID = myStringpair.first;
                TpmCryptGroup *myGroup = new TpmCryptGroup(myGroupID, myGroupStorage, mySubjectStorage);
                myGroup->printGroup(false);
                myGroup->deleteSubjectFromGroup(mySubject);
                delete myGroup;
                GroupIterator++;
            }
        }

        // DELETING USER FROM EVERY VOLUME
        vector<StringPair> deleteUserFromVolume = myVolumeStorage.findAllEntries(myUserSubjectID);
        // deleting user in every volume
        vector<utils::StringPair>::const_iterator VolumeIterator;

        if (deleteUserFromVolume.size())
        {
            cout << "Deleting user from the following volumes:" << endl;
            VolumeIterator=deleteUserFromVolume.begin();
            while ( VolumeIterator != deleteUserFromVolume.end())
            {
                StringPair myStringpair = *(VolumeIterator);
                string myVolumeID = myStringpair.first;
                TpmCryptVolume *myVolume = new TpmCryptVolume(myVolumeID, myVolumeStorage);
                myVolume->printVolume();
                myVolume->deleteSubject(mySubject);
                delete myVolume;
                VolumeIterator++;
            }
        }

        // FINALLY, DELETING USER  
            for (UInt32 i = 0; i < 10000; i++) {   
    //UInt32 uuid = convertStringtoUInt32(keyuuid);  
    cout << i << endl;
    TpmCryptTPM myTPM;
    myTPM.delete_key(i);
	}          
        removeFile(mySubject->getMySubjectKeyfile());
        mySubject->deleteSubject();
        delete mySubject;

        if (!TpmCryptFindSubject(mySubjectStorage, userToDelete))
            cout << "Successfully deleted user '" << userToDelete << "'." << endl;
        else
            cout << "Failed to delete user '" << userToDelete << "'!" << endl;
        printToSyslog("TPMCRYPT: User '" + userToDelete + "' was deleted from this system");
    }
};

//================================================================================
//
void TpmCryptImportUser(TpmCryptStorage &mySubjectStorage)
{
    bool keyFileValid = false;
    bool storeFile = false;
    bool validChoice = false;
    string keyFileToImport;
    string myUsername;
    string myC;
    string myEMail;
    string myO;
    string myOU;
    string mySP;
    string myL;

    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    myUsername = getStringFromUser("Please enter username for the user to be imported\n(NOTE: This has to be the common name (CN) in the certificate\nresp. its label in the keyfile!): ");
    if (TpmCryptFindUser(mySubjectStorage, myUsername))
        throw InvalidUsername("TpmCryptImportUser(): User already exists!");

    myEMail = getStringFromUser("Please enter email address for user '" + myUsername + "': ");


};

//================================================================================
//
void TpmCryptCreateUser(TpmCryptStorage &mySubjectStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;
    string myUsername = getStringFromUser("Please enter username for new user: ");
    if (TpmCryptFindUser(mySubjectStorage, myUsername))
        throw InvalidUsername("TpmCryptCreateUser(): User already exists!");

    string myEMail = TPMCRYPT_PRIVATE_MODE_USER_MAIL;
    string myC  = TPMCRYPT_PRIVATE_MODE_C;
    string myO  = TPMCRYPT_PRIVATE_MODE;
    string myOU = TPMCRYPT_PRIVATE_MODE;
    string mySP = TPMCRYPT_PRIVATE_MODE;
    string myL  = TPMCRYPT_PRIVATE_MODE;
    if (EnterpriseMode)
    {
        myEMail = getStringFromUser("Please enter email address for new user: ");
        myC = getStringFromUser("Please enter countrycode for new user: ");
        myO = getStringFromUser("Please enter organisation for new user: ");
        myOU= getStringFromUser("Please enter organisation unit for new user: ");
        mySP= getStringFromUser("Please enter state/province for new user: ");
        myL = getStringFromUser("Please enter locality for new user: ");
    }
	string null;
    TpmCryptSubject newUser(SUBJECTTYPE_USER, myUsername, null, myEMail, myC, myO, myOU, mySP, myL, false, myKeyDirectory, mySubjectStorage);
    if (! newUser.getMySubjectID())
        throw NotCreated("User could not be created!");

    cout << "Successfully created new user with SubjectID '" << newUser.getMySubjectID() << "'." << endl;
    printToSyslog("TPMCRYPT: User '" + myUsername + "' was added to this system");
    newUser.printSubject(true);
};

//================================================================================
//
void TpmCryptChangeUserCredential(TpmCryptStorage &mySubjectStorage)
{
   if (! TpmCryptListUsers(mySubjectStorage, true))
       return;

    vector<string> myUsers = TpmCryptFindAllUsers(mySubjectStorage, true);
    if (! myUsers.size())
    {
        cout << "No users found." << endl;
        return;
    }
    string userToChange = selectFromStringVector(myUsers, "\nNumber:\t\tUser:", "Please select the user that intends to change the password:");

    if (!TpmCryptFindSubject(mySubjectStorage, userToChange))
        throw UnknownSubject("User '" + userToChange + "' not found.");

    TpmCryptSubject *mySubject = new TpmCryptSubject(userToChange, mySubjectStorage);

    if (mySubject->getMySubjectType() != SUBJECTTYPE_USER)
        throw InvalidSubjectType("Only users can change their credentials!");

    string currentSubject = mySubject->getMySubjectName();
    string oldPassword = getPassword("Please enter current password for user '" + currentSubject + "': ");
    string newPassword = getPasswordVerified("Please enter new password for user '" + currentSubject + "': ");
    if (!mySubject->changePassword(oldPassword, newPassword))
    {
        cout << "Failed to changed credential for user '" << currentSubject << "'!" << endl;
        return;
    }
    else
        cout << "Successfully changed credential for user '" << currentSubject << "'" << endl;


    if (mySubject->verifyPassword(newPassword))
        cout << "Successfully verified credential for user '" << currentSubject << "'" << endl;
    else
        cout << "Failed to verify credential for user '" << currentSubject << "'!" << endl;

    delete mySubject;
    printToSyslog("TPMCRYPT: User '" + currentSubject + "' changed his password");
};

//================================================================================
//
void TpmCryptUserManagement     ( string userParam, TpmCryptConfigfile &myConfigfile )
{
    t_StringMap TpmCryptUserMap;
    TpmCryptUserMap["createUser"]            = createUser;
    TpmCryptUserMap["cu"]                    = createUser;
    TpmCryptUserMap["importUser"]            = importUser;
    TpmCryptUserMap["iu"]                    = importUser;
    TpmCryptUserMap["deleteUser"]            = deleteUser;
    TpmCryptUserMap["du"]                    = deleteUser;
    TpmCryptUserMap["listUsers"]             = listUsers;
    TpmCryptUserMap["lu"]                    = listUsers;
    TpmCryptUserMap["changeUserCredential"]  = changeUserCredential;
    TpmCryptUserMap["cuc"]                   = changeUserCredential;

    // Create User database storage object
    TpmCryptStorage mySubjectStorage( "SubjectDB", myConfigfile.getConfigfileEntry(TpmCryptConfigfile_SubjectDBIdentifier) );

    // Create Group database storage object
    TpmCryptStorage myGroupStorage( "GroupDB", myConfigfile.getConfigfileEntry(TpmCryptConfigfile_GroupDBIdentifier) );

    // Create Volume database storage object
    TpmCryptStorage myVolumeStorage( "VolumeDB", myConfigfile.getConfigfileEntry(TpmCryptConfigfile_VolumeDBIdentifier) );

    switch(TpmCryptUserMap[userParam])
    {
        case createUser:
            TpmCryptCreateUser(mySubjectStorage);
            break;
        case importUser:
            TpmCryptImportUser(mySubjectStorage);
            break;
        case deleteUser:
            TpmCryptDeleteUser(mySubjectStorage, myGroupStorage, myVolumeStorage);
            break;
        case listUsers:
            TpmCryptListUsers(mySubjectStorage, false);
            break;
        case changeUserCredential:
            TpmCryptChangeUserCredential(mySubjectStorage);
            break;
        default:
            throw ParseError("invalid parameter");
    }
};
