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


#include <Keyrona.hxx>
#include <KeyronaManager.hxx>
#include <KeyronaKey.hxx>

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
bool KeyronaListUsers(KeyronaStorage &mySubjectStorage, bool includeAdmins)
{
    vector<StringPair> myUsers = mySubjectStorage.findAllEntries(KeyronaSubject_SubjectType, KeyronaSubjectType_User);

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
            if (!KeyronaFindUser(mySubjectStorage, Iterator->first))
            {
                Iterator++;
                continue;
            }

            KeyronaSubject *mySubject = new KeyronaSubject (Iterator->first, mySubjectStorage);
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
void KeyronaDeleteUser(KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage, KeyronaStorage &myVolumeStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (! KeyronaListUsers(mySubjectStorage, false))
        return;

    vector<string> myUsers = KeyronaFindAllUsers(mySubjectStorage, false);
    if (! myUsers.size())
    {
        cout << "No users found." << endl;
        return;
    }
    string userToDelete = selectFromStringVector(myUsers, "\nNumber:\t\tUser:", "Please select the user to be deleted:");

    if (!KeyronaFindSubject(mySubjectStorage, userToDelete))
        throw UnknownSubject("User '" + userToDelete + "' not found.");

    KeyronaSubject *mySubject = new KeyronaSubject(userToDelete, mySubjectStorage);
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
                KeyronaGroup *myGroup = new KeyronaGroup(myGroupID, myGroupStorage, mySubjectStorage);
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
                KeyronaVolume *myVolume = new KeyronaVolume(myVolumeID, myVolumeStorage);
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
    KeyronaTPM myTPM;
    myTPM.delete_key(i);
	}          
        removeFile(mySubject->getMySubjectKeyfile());
        mySubject->deleteSubject();
        delete mySubject;

        if (!KeyronaFindSubject(mySubjectStorage, userToDelete))
            cout << "Successfully deleted user '" << userToDelete << "'." << endl;
        else
            cout << "Failed to delete user '" << userToDelete << "'!" << endl;
        printToSyslog("KEYRONA: User '" + userToDelete + "' was deleted from this system");
    }
};

//================================================================================
//
void KeyronaImportUser(KeyronaStorage &mySubjectStorage)
{
    bool keyFileValid = false;
    bool storeFile = false;
    bool validChoice = false;
    string keyFileToImport;
    string myCryptlibKeyfile;
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
    if (KeyronaFindUser(mySubjectStorage, myUsername))
        throw InvalidUsername("KeyronaImportUser(): User already exists!");

    myEMail = getStringFromUser("Please enter email address for user '" + myUsername + "': ");

    cout << "1) OpenSSL public/private keypair" << endl;
    cout << "2) Cryptlib .p15-keyfile" << endl;
    string myKeyChoice = getStringFromUser("What type of user do you want to import?: ");

    if (! myKeyChoice.compare(0,1,"1"))
    {
        validChoice = true;
        cout << "Importing OpenSSL PEM-files..." << endl;
        string myPrivateKeyfile = getStringFromUser("Please enter the path to your private key keyfile (e.g., mykey.key):\n");
        if (! verifyFilename(myPrivateKeyfile, false))
            throw InvalidFilename("KeyronaImportUser(): invalid file given!");
        string myPublicKeyfile = getStringFromUser("Please enter the path to your public key keyfile (e.g., mykey.crt):\n");
        if (! verifyFilename(myPublicKeyfile, false))
            throw InvalidFilename("KeyronaImportUser(): invalid file given!");
        myCryptlibKeyfile = getStringFromUser("Please enter the path where to store the resulting .p15-file:\n");
        if (verifyFilename(myCryptlibKeyfile, false))
            throw InvalidFilename("KeyronaImportUser(): File '" + myCryptlibKeyfile + "' already exists!");
        string password = getPasswordVerified("Please enter password to protect the keyfile: ");
        if (! convertPEM2p15(myPublicKeyfile, myPrivateKeyfile, password, myUsername, myCryptlibKeyfile))
                throw EncryptionFailed("KeyronaImportUser(): Could not convert PEM-files into .p15-file");
        keyFileValid = true;
    }

    if (! myKeyChoice.compare(0,1,"2"))
    {
        validChoice = true;
        cout << "Importing Cryptlib .p15-file..." << endl;
        myCryptlibKeyfile = getStringFromUser("Please enter the path to your private key keyfile (e.g., mykey.p15):\n");
        if (! verifyFilename(myCryptlibKeyfile, false))
            throw InvalidFilename("KeyronaImportUser(): invalid file given!");
        cout << "Verifying .p15-file: ";
        CRYPT_KEYSET tempKeyset;
        cryptlibCheck(cryptInit(), "Initialization failed!");
        if (cryptKeysetOpen( &tempKeyset, CRYPT_UNUSED, CRYPT_KEYSET_FILE, myCryptlibKeyfile.c_str(), CRYPT_KEYOPT_NONE))
        {
            cout << "failed!" << endl;
            throw InvalidFilename("KeyronaImportUser(): File '" + myCryptlibKeyfile + "' is not a valid .p15-file");
        }
        cout << "success!" << endl;
        cryptlibCheck(cryptKeysetClose (tempKeyset), "Closing keyset");
        keyFileValid = true;
    }

    if (!validChoice)
        throw InvalidEncSystem("KeyronaImportUser(): Invalid selection!");

    if (keyFileValid)
    {
        CRYPT_CONTEXT   cryptContext;
        CRYPT_KEYSET    cryptKeyset;

        char myString[MAX_CRYPT_BUFFER_SIZE];
        int length;

        cout << "Retrieving attributes from keyfile... " << endl;
        cryptlibCheck(cryptCreateContext( &cryptContext, CRYPT_UNUSED, CRYPT_ALGO_RSA ), "Creating crypto context");
        cryptlibCheck(cryptKeysetOpen( &cryptKeyset, CRYPT_UNUSED, CRYPT_KEYSET_FILE, myCryptlibKeyfile.c_str(), CRYPT_KEYOPT_READONLY ), "Opening keyset with file '" + myCryptlibKeyfile + "'");

        try
        {
            cryptlibCheck(cryptGetPublicKey(cryptKeyset, &cryptContext, CRYPT_KEYID_NAME, myUsername.c_str()), "Retrieving public key from keyset with label '" + myUsername + "'");
        }
        catch (Libcrypt &e)
        {
            cout << "ERROR: Could not retrieve public key from keyfile!" << endl;
            cout << "Make sure, that the username you entered is the common name (CN) in the certificate" << endl;
            cout << "resp. its label in the keyfile!" << endl;
            return;
        }

        if (cryptGetAttributeString(cryptContext, CRYPT_CERTINFO_COMMONNAME, myString, &length) == CRYPT_OK)
        {
            string myCommonName(myString, length);
            cout << "\tUser's common name: " << myCommonName << endl;
            if (myUsername != myCommonName)
            {
                cout << "ERROR: The common name in the certificate does not match the entered username '" << myUsername << "'!" << endl;
                return;
            }
        }

        if (cryptGetAttributeString(cryptContext, CRYPT_CERTINFO_COUNTRYNAME, myString, &length) == CRYPT_OK)
        {
            myC = string(myString, length);
            cout << "\tUser's country code: " << myC << endl;
        }
        else
            myC = getStringFromUser("Please enter countrycode for new user: ");

        if (cryptGetAttributeString(cryptContext, CRYPT_CERTINFO_ORGANISATIONNAME, myString, &length) == CRYPT_OK)
        {
            myO = string(myString, length);
            cout << "\tUser's organisation: " << myO << endl;
        }
        else
            myO = getStringFromUser("Please enter organisation for user: ");

        if (cryptGetAttributeString(cryptContext, CRYPT_CERTINFO_ORGANISATIONALUNITNAME, myString, &length) == CRYPT_OK)
        {
            myOU = string(myString, length);
            cout << "\tUser's organisation unit: " << myOU << endl;
        }
        else
            myOU = getStringFromUser("Please enter organisation unit for user: ");

        if (cryptGetAttributeString(cryptContext, CRYPT_CERTINFO_STATEORPROVINCENAME, myString, &length) == CRYPT_OK)
        {
            mySP = string(myString, length);
            cout << "\tUser's state: " << mySP << endl;
        }
        else
            mySP= getStringFromUser("Please enter state/province for user: ");

        if (cryptGetAttributeString(cryptContext, CRYPT_CERTINFO_LOCALITYNAME, myString, &length) == CRYPT_OK)
        {
            myL = string(myString, length);
            cout << "\tUser's locality: " << myL << endl;
        }
        else
            myL = getStringFromUser("Please enter locality for user: ");

        cryptlibCheck(cryptKeysetClose( cryptKeyset ), "Closing keyfile");
        cryptlibCheck(cryptDestroyContext( cryptContext ), "Destroying context");

        string store = getStringFromUser("Shall I store the keyfile in my database? [yes|no]: ");
        cout << "Good, we have a valid Cryptlib-keyfile!" << endl;
        if ((store == "yes") | (store == "YES"))
        {
            cout << "Storing Cryptlib-keyfile in our database!" << endl;
            storeFile = true;
        }
        else
        {
            cout << "Not storing Cryptlib-keyfile in our database!" << endl;
        }
    }
    else
        cout << "No valid keyfile given, giving up!" << endl;


    KeyronaSubject newUser(SUBJECTTYPE_USER, myUsername, myEMail, myC, myO, myOU, mySP, myL, false, myKeyDirectory, myCryptlibKeyfile, storeFile, mySubjectStorage);
    if (! newUser.getMySubjectID())
        throw NotCreated("User could not be imported!");

    cout << "Successfully imported new user with SubjectID '" << newUser.getMySubjectID() << "'." << endl;
    printToSyslog("KEYRONA: User '" + myUsername + "' was imported to this system");
    newUser.printSubject(true);
};

//================================================================================
//
void KeyronaCreateUser(KeyronaStorage &mySubjectStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;
    string myUsername = getStringFromUser("Please enter username for new user: ");
    if (KeyronaFindUser(mySubjectStorage, myUsername))
        throw InvalidUsername("KeyronaCreateUser(): User already exists!");

    string myEMail = KEYRONA_PRIVATE_MODE_USER_MAIL;
    string myC  = KEYRONA_PRIVATE_MODE_C;
    string myO  = KEYRONA_PRIVATE_MODE;
    string myOU = KEYRONA_PRIVATE_MODE;
    string mySP = KEYRONA_PRIVATE_MODE;
    string myL  = KEYRONA_PRIVATE_MODE;
    if (EnterpriseMode)
    {
        myEMail = getStringFromUser("Please enter email address for new user: ");
        myC = getStringFromUser("Please enter countrycode for new user: ");
        myO = getStringFromUser("Please enter organisation for new user: ");
        myOU= getStringFromUser("Please enter organisation unit for new user: ");
        mySP= getStringFromUser("Please enter state/province for new user: ");
        myL = getStringFromUser("Please enter locality for new user: ");
    }

    KeyronaSubject newUser(SUBJECTTYPE_USER, myUsername, myEMail, myC, myO, myOU, mySP, myL, false, myKeyDirectory, mySubjectStorage);
    if (! newUser.getMySubjectID())
        throw NotCreated("User could not be created!");

    cout << "Successfully created new user with SubjectID '" << newUser.getMySubjectID() << "'." << endl;
    printToSyslog("KEYRONA: User '" + myUsername + "' was added to this system");
    newUser.printSubject(true);
};

//================================================================================
//
void KeyronaChangeUserCredential(KeyronaStorage &mySubjectStorage)
{
   if (! KeyronaListUsers(mySubjectStorage, true))
       return;

    vector<string> myUsers = KeyronaFindAllUsers(mySubjectStorage, true);
    if (! myUsers.size())
    {
        cout << "No users found." << endl;
        return;
    }
    string userToChange = selectFromStringVector(myUsers, "\nNumber:\t\tUser:", "Please select the user that intends to change the password:");

    if (!KeyronaFindSubject(mySubjectStorage, userToChange))
        throw UnknownSubject("User '" + userToChange + "' not found.");

    KeyronaSubject *mySubject = new KeyronaSubject(userToChange, mySubjectStorage);

    if (mySubject->getMySubjectType() != SUBJECTTYPE_USER)
        throw InvalidSubjectType("Only users can change their credentials!");

    string currentSubject = mySubject->getMySubjectName();
    string oldPassword = getPassword("Please enter current password for user '" + currentSubject + "': ");
    //if (!mySubject->verifyPassword(oldPassword))
      //  throw InvalidPassword("Invalid password for user '" + currentSubject + "'!");
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
    printToSyslog("KEYRONA: User '" + currentSubject + "' changed his password");
};

//================================================================================
//
void KeyronaUserManagement     ( string userParam, KeyronaConfigfile &myConfigfile )
{
    t_StringMap KeyronaUserMap;
    KeyronaUserMap["createUser"]            = createUser;
    KeyronaUserMap["cu"]                    = createUser;
    KeyronaUserMap["importUser"]            = importUser;
    KeyronaUserMap["iu"]                    = importUser;
    KeyronaUserMap["deleteUser"]            = deleteUser;
    KeyronaUserMap["du"]                    = deleteUser;
    KeyronaUserMap["listUsers"]             = listUsers;
    KeyronaUserMap["lu"]                    = listUsers;
    KeyronaUserMap["changeUserCredential"]  = changeUserCredential;
    KeyronaUserMap["cuc"]                   = changeUserCredential;

    // Create User database storage object
    KeyronaStorage mySubjectStorage( "SubjectDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_SubjectDBIdentifier) );

    // Create Group database storage object
    KeyronaStorage myGroupStorage( "GroupDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_GroupDBIdentifier) );

    // Create Volume database storage object
    KeyronaStorage myVolumeStorage( "VolumeDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_VolumeDBIdentifier) );

    switch(KeyronaUserMap[userParam])
    {
        case createUser:
            KeyronaCreateUser(mySubjectStorage);
            break;
        case importUser:
            KeyronaImportUser(mySubjectStorage);
            break;
        case deleteUser:
            KeyronaDeleteUser(mySubjectStorage, myGroupStorage, myVolumeStorage);
            break;
        case listUsers:
            KeyronaListUsers(mySubjectStorage, false);
            break;
        case changeUserCredential:
            KeyronaChangeUserCredential(mySubjectStorage);
            break;
        default:
            throw ParseError("invalid parameter");
    }
};
