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
 * @file        KeyronaInit.cxx
 * @brief       Implementation of the command line tool 'keyrona_manager'.
 * @version     $Revision: 673 $
 * @date        Last modification at $Date: 2009-06-25 15:14:43 +0200 (Thu, 25 Jun 2009) $ by $Author: selhorst $
 */

#include <Keyrona.hxx>
#include <KeyronaManager.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;


enum myMenu
{
    createAdmin = 1,
    deleteAdmin,
    listAdmins
};

// mapping between strings and int values
typedef map<string, int> t_StringMap;

//================================================================================
//
bool KeyronaListAdmins(KeyronaStorage &mySubjectStorage)
{
    vector<StringPair> myAdmins = mySubjectStorage.findAllEntries(KeyronaSubject_SubjectAdmin, KeyronaSubject_TRUE);

    vector<utils::StringPair>::const_iterator Iterator;
    if (myAdmins.size())
    {
        cout << "We have the following admins: " << endl;

        Iterator=myAdmins.begin();
        while ( Iterator != myAdmins.end())
        {
            KeyronaSubject mySubject(Iterator->first, mySubjectStorage);
            mySubject.printSubject(printVerbose);
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
void KeyronaDeleteAdmin(KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Superuser successfully logged in!" << endl;

    if (!KeyronaListAdmins(mySubjectStorage))
        return;

    vector<string> myAdmins = KeyronaFindAllAdmins(mySubjectStorage);
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

    if (!KeyronaFindAdmin(mySubjectStorage, adminID))
        throw UnknownSubject("Subject '" + adminID + "' is not an admin.");

    if (verifySelection())
    {
        // load admin group
        KeyronaGroup myAdminGroup(KEYRONA_ADMIN_GROUP, myGroupStorage, mySubjectStorage);

        // load admin subject
        KeyronaSubject myAdmin(adminID, mySubjectStorage);

        // delete admin from admin group
        myAdminGroup.deleteSubjectFromGroup(&myAdmin);

        // delete admin
        myAdmin.deleteSubject();

        cout << "Successfully deleted admin '" << adminID << "'." << endl;
        printToSyslog("KEYRONA: Admin '" + adminID + "' was removed from this system");
    }
};


//================================================================================
//
void KeyronaCreateAdmin(KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage)
{
    if (loginSuperUser())
        cout << "Superuser successfully logged in!" << endl;

    string myUsername = getStringFromUser("Please enter username for new admin: ");

    if (KeyronaFindUser(mySubjectStorage, myUsername))
        throw InvalidUsername("KeyronaCreateAdmin(): User already exists!");

    string myEMail = KEYRONA_PRIVATE_MODE_ADMIN_MAIL;
    string myC  = KEYRONA_PRIVATE_MODE_C;
    string myO  = KEYRONA_PRIVATE_MODE;
    string myOU = KEYRONA_PRIVATE_MODE;
    string mySP = KEYRONA_PRIVATE_MODE;
    string myL  = KEYRONA_PRIVATE_MODE;
    if (EnterpriseMode)
    {
        myEMail = getStringFromUser("Please enter email address for new admin: ");
        myC = getStringFromUser("Please enter countrycode for new admin: ");
        myO = getStringFromUser("Please enter organisation for new admin: ");
        myOU= getStringFromUser("Please enter organisation unit for new admin: ");
        mySP= getStringFromUser("Please enter state/province for new admin: ");
        myL = getStringFromUser("Please enter locality for new admin: ");
    }

    // Check, whether we have an admin group
    cout << "Checking existence of admin-group '" << KEYRONA_ADMIN_GROUP << "': ";
    if (KeyronaFindGroup(myGroupStorage, KEYRONA_ADMIN_GROUP))
    {
        cout << "present!" << endl;
        // load admin group
        KeyronaGroup myAdminGroup(KEYRONA_ADMIN_GROUP, myGroupStorage, mySubjectStorage);
        // first, get the group key from an existing admin group member
        myAdminGroup.printSubjectsInGroup(printVerbose);

        // load admin
        KeyronaSubject *myExistingAdmin = getCurrentAdmin(mySubjectStorage);

        string adminPassword = getPassword("Please enter the according password for admin '" + myExistingAdmin->getMySubjectName() + "' (" + myExistingAdmin->getMySubjectIDString() + "): ");
        string groupKeyPassword = myAdminGroup.getDecryptedGroupKey(myExistingAdmin, adminPassword);

        KeyronaSubject newAdmin(SUBJECTTYPE_USER, myUsername, myEMail, myC, myO, myOU, mySP, myL, true, myKeyDirectory, mySubjectStorage);
        if (! newAdmin.getMySubjectID())
            throw NotCreated("Admin could not be created!");
        cout << "Successfully created new admin with SubjectID '" << newAdmin.getMySubjectID() << "'." << endl;
        printToSyslog("KEYRONA: Admin '" + myUsername + "' was added to this system");
        myAdminGroup.addSubjectToGroup(&newAdmin, groupKeyPassword);
        cout << "Successfully added new admin '" << newAdmin.getMySubjectName() << "' to admin group!" << endl;
    }
    else
    {
        cout << "not present!" << endl;
        cout << "Deleting already registered keys of TPM" << endl;
        cout << "Please be patient, this can take some time..." << endl;
        KeyronaKey deleteAllKeys();

        KeyronaSubject newAdmin(SUBJECTTYPE_USER, myUsername, myEMail, myC, myO, myOU, mySP, myL, true, myKeyDirectory, mySubjectStorage);
        if (! newAdmin.getMySubjectID())
            throw NotCreated("Admin could not be created!");
        cout << "Successfully created new admin with SubjectID '" << newAdmin.getMySubjectID() << "'." << endl;
        printToSyslog("KEYRONA: Admin '" + myUsername + "' was added to this system");
        cout << "Creating new admin group..." << endl;
        KeyronaGroup myAdminGroup(KEYRONA_ADMIN_GROUP, myGroupStorage, mySubjectStorage);
    }
}

//================================================================================
//
void KeyronaInit     ( string initParam, KeyronaConfigfile &myConfigfile )
{
    t_StringMap KeyronaInitMap;
    KeyronaInitMap["createAdmin"]    = createAdmin;
    KeyronaInitMap["ca"]          = createAdmin;
    KeyronaInitMap["deleteAdmin"] = deleteAdmin;
    KeyronaInitMap["da"]          = deleteAdmin;
    KeyronaInitMap["listAdmins"]  = listAdmins;
    KeyronaInitMap["la"]          = listAdmins;

    // Create User database storage object
    KeyronaStorage mySubjectStorage( "SubjectDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_SubjectDBIdentifier) );

    // Create Group database storage object
    KeyronaStorage myGroupStorage( "GroupDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_GroupDBIdentifier) );

    switch(KeyronaInitMap[initParam])
    {
        case createAdmin:
            KeyronaCreateAdmin(mySubjectStorage, myGroupStorage);
            break;
        case deleteAdmin:
            KeyronaDeleteAdmin(mySubjectStorage, myGroupStorage);
            break;
        case listAdmins:
            KeyronaListAdmins(mySubjectStorage);
            break;
        default:
            throw ParseError("invalid parameter");
    }
};
