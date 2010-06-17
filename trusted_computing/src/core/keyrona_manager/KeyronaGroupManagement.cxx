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
 * @file        KeyronaGroupManagement.cxx
 * @brief       Implementation of the command line tool 'keyrona_manager'.
 * @version     $Revision: 714 $
 * @date        Last modification at $Date: 2009-07-13 13:14:24 +0200 (Mon, 13 Jul 2009) $ by $Author: selhorst $
 */

#include <Keyrona.hxx>
#include <KeyronaManager.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

enum myMenu
{
    createGroup = 1,
    deleteGroup,
    listGroups,
    addSubjectToGroup,
    deleteSubjectFromGroup,
    listSubjectsInGroup
};

// mapping between strings and int values
typedef map<string, int> t_StringMap;

//================================================================================
//
bool KeyronaListGroups(KeyronaStorage &myGroupStorage, KeyronaStorage &mySubjectStorage)
{
    vector<string> myGroups = KeyronaFindAllGroups(myGroupStorage, mySubjectStorage, true);
    removeStringFromStringVector(&myGroups, KEYRONA_ADMIN_GROUP);
    vector<string>::const_iterator Iterator;
    if (myGroups.size())
    {
        cout << "We have the following groups: " << endl;

        Iterator=myGroups.begin();
        while ( Iterator != myGroups.end())
        {
            KeyronaGroup myGroup(*(Iterator), myGroupStorage, mySubjectStorage);
            if (myGroup.getMyGroupID() != KEYRONA_ADMIN_GROUP)
                myGroup.printGroup(printVerbose);
            Iterator++;
        }
        return true;
    }
    else
    {
        cout << "No groups found." << endl;
        return false;
    }
};

//================================================================================
//
void KeyronaAddSubjectToGroup(KeyronaStorage &myGroupStorage, KeyronaStorage &mySubjectStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (!KeyronaListGroups(myGroupStorage, mySubjectStorage))
        return;

    // get GroupId
    vector<string> myGroups = KeyronaFindAllGroups(myGroupStorage, mySubjectStorage, false);
    if (!myGroups.size())
    {
        cout << "No groups available!" << endl;
        return;
    }
    string groupName = selectFromStringVector(myGroups, "\nNumber:\t\tGroup:", "Please select the desired group:");
    if (!KeyronaFindGroup(myGroupStorage, groupName))
        throw UnknownGroup("Group '" + groupName + "' not found.");

    // check that this is not the admin group
    if (groupName == KEYRONA_ADMIN_GROUP)
        throw InvalidGroup("You are not allowed to add subjects to the admin group, please use '--init addAdmin' for this!");

    // load group
    KeyronaGroup myGroup(groupName, myGroupStorage, mySubjectStorage);

    // get all Users
    vector<string> myUsers = KeyronaFindAllUsers(mySubjectStorage, false);
    // get all Token
    vector<string> myToken = KeyronaFindAllToken(mySubjectStorage);

    // add Token to Userlist
    myUsers.insert(myUsers.end(), myToken.begin(), myToken.end());

    if (!myUsers.size())
    {
        cout << "No users / token available!" << endl;
        return;
    }

    // remove existing group users from user list
    vector<UInt32> myGroupUsers = myGroup.getMySubjects();
    vector<UInt32>::const_iterator Iterator;
    Iterator=myGroupUsers.begin();
    while ( Iterator != myGroupUsers.end())
    {
        KeyronaSubject *mySubject = new KeyronaSubject(*(Iterator), mySubjectStorage);
        removeStringFromStringVector(&myUsers, mySubject->getMySubjectName());
        delete mySubject;
        Iterator++;
    }

    if (!myUsers.size())
    {
        cout << "No more users / token available to be added to group '" << groupName << "'!" << endl;
        return;
    }
    string subjectID = selectFromStringVector(myUsers, "Number:\t\tSubject:", "Please select the subject to be added:");

    if (!KeyronaFindSubject(mySubjectStorage, subjectID))
        throw UnknownSubject("Subject '" + subjectID + "' not found.");

    // load subject
    KeyronaSubject mySubject(subjectID, mySubjectStorage);

    if (myGroup.isSubjectInGroup(&mySubject))
        throw InvalidSubjectType("User '" + mySubject.getMySubjectName() + "' is already in group '" + myGroup.getMyGroupID() + "'!");


    if ((mySubject.getMySubjectType() != SUBJECTTYPE_USER) && (mySubject.getMySubjectType() != SUBJECTTYPE_TOKEN))
        throw InvalidSubjectType("Only user and token are allowed to be added to groups!");

    // load admin
    KeyronaSubject *myExistingAdmin = getCurrentAdmin(mySubjectStorage);
    // retrieving admin group key from admins...
    KeyronaGroup myAdminGroup(KEYRONA_ADMIN_GROUP, myGroupStorage, mySubjectStorage);
    // get admin password
    string adminPassword = getPassword("Please enter the according password for admin '" + myExistingAdmin->getMySubjectName() + "' (" + myExistingAdmin->getMySubjectIDString() + "): ");
    // get admin key password decrypted with subject key
    string adminGroupKeyPassword = myAdminGroup.getDecryptedGroupKey(myExistingAdmin, adminPassword);
    // get group key password decrypted with admin key
    string groupKeyPassword = myGroup.getDecryptedGroupKeyByAdmin(adminGroupKeyPassword);

    // adding subject to group with plain group key
    myGroup.addSubjectToGroup(&mySubject, groupKeyPassword);
    cout << "Successfully added subject '" << subjectID << "' to group '" << groupName << "'." << endl;

    string message = "New subject '" + mySubject.getMySubjectName() + "' has been added to group '" + groupName + "'";
    myGroup.addMessageForGroupMembers(message);
};

//================================================================================
//
void KeyronaListSubjectsInGroup(KeyronaStorage &myGroupStorage, KeyronaStorage &mySubjectStorage)
{
    if (!KeyronaListGroups(myGroupStorage, mySubjectStorage))
        return;
    
    // get GroupId
    vector<string> myGroups = KeyronaFindAllGroups(myGroupStorage, mySubjectStorage, false);
    if (!myGroups.size())
    {
        cout << "No groups available!" << endl;
        return;
    }
    string groupName = selectFromStringVector(myGroups, "\nNumber:\t\tGroup:", "Please select the desired group:");
    if (!KeyronaFindGroup(myGroupStorage, groupName))
        throw UnknownGroup("Group '" + groupName + "' not found.");

    KeyronaGroup myGroup(groupName, myGroupStorage, mySubjectStorage);
    myGroup.printSubjectsInGroup(printVerbose);
};

//================================================================================
//
void KeyronaCreateGroup(KeyronaStorage &myGroupStorage, KeyronaStorage &mySubjectStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    string groupName = getStringFromUser("Please enter the group name to be created: ");

    if ((KeyronaFindGroup(myGroupStorage, "Group_" + groupName)) || (KeyronaFindGroup(myGroupStorage, groupName)))
    {
        cout << "Group '" + groupName + "' already exists." << endl;
        return;
    }
    KeyronaGroup newGroup(groupName, myGroupStorage, mySubjectStorage);
    cout << "Group '" + newGroup.getMyGroupID() + "' successfully created." << endl;
    newGroup.printGroup(true);
    printToSyslog("KEYRONA: Group '" + newGroup.getMyGroupID() + "' was added to this system");
};

//================================================================================
//
void KeyronaDeleteSubjectFromGroup(KeyronaStorage &myGroupStorage, KeyronaStorage &mySubjectStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (!KeyronaListGroups(myGroupStorage, mySubjectStorage))
        return;

    // get GroupId
    vector<string> myGroups = KeyronaFindAllGroups(myGroupStorage, mySubjectStorage, false);
    if (!myGroups.size())
    {
        cout << "No groups available!" << endl;
        return;
    }
    string groupName = selectFromStringVector(myGroups, "\nNumber:\t\tGroup:", "Please select the desired group:");
    if (!KeyronaFindGroup(myGroupStorage, groupName))
        throw UnknownGroup("Group '" + groupName + "' not found.");

    // check that this is not the admin group
    if (groupName == KEYRONA_ADMIN_GROUP)
        throw InvalidGroup("You are not allowed to delete subjects from the admin group, please use '--init deleteAdmin' for this!");

    // load group
    KeyronaGroup myGroup(groupName, myGroupStorage, mySubjectStorage);

    // get SubjectID
    myGroup.printSubjectsInGroup(printVerbose);
    vector<UInt32> myUsers = myGroup.getMySubjects();
    if (!myUsers.size())
    {
        cout << "No subjects available!" << endl;
        return;
    }

    // convert our subjectIDs into names
    vector<string> mySubjectNamesInGroup;
    vector<UInt32>::const_iterator Iterator;
    Iterator=myUsers.begin();
    while ( Iterator != myUsers.end())
    {
        KeyronaSubject *mySubject = new KeyronaSubject(*(Iterator), mySubjectStorage);
        mySubjectNamesInGroup.push_back(mySubject->getMySubjectName());
        delete mySubject;
        Iterator++;
    }
    // select the subject to be deleted
    string subjectID = selectFromStringVector(mySubjectNamesInGroup, "Number:\t\tSubject:", "Please select the subject to be deleted:");

    if (!KeyronaFindSubject(mySubjectStorage, subjectID))
        throw UnknownSubject("Subject '" + subjectID + "' not found.");

    if (verifySelection())
    {
        // load subject
        KeyronaSubject mySubject(subjectID, mySubjectStorage);

        string message = "Subject '" + mySubject.getMySubjectName() + "' has been deleted from group '" + groupName + "'";
        myGroup.addMessageForGroupMembers(message);

        // deleting subject
        myGroup.deleteSubjectFromGroup(&mySubject);
        cout << "Successfully deleted subject '" << subjectID << "' from group '" << groupName << "'." << endl;
    }
};

//================================================================================
//
void KeyronaDeleteGroup(KeyronaStorage &myGroupStorage, KeyronaStorage &mySubjectStorage, KeyronaStorage &myVolumeStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (!KeyronaListGroups(myGroupStorage, mySubjectStorage))
        return;

    // get GroupId
    vector<string> myGroups = KeyronaFindAllGroups(myGroupStorage, mySubjectStorage, false);
    if (!myGroups.size())
    {
        cout << "No groups available!" << endl;
        return;
    }
    string groupName = selectFromStringVector(myGroups, "\nNumber:\t\tGroup:", "Please select the desired group:");
    if (!KeyronaFindGroup(myGroupStorage, groupName))
        throw UnknownGroup("Group '" + groupName + "' not found.");

    // check that this is not the admin group
    if (groupName == KEYRONA_ADMIN_GROUP)
        throw InvalidGroup("You are not allowed to delete the admin group!");

    if (verifySelection())
    {
        // load group
        KeyronaGroup * myGroup = new KeyronaGroup(groupName, myGroupStorage, mySubjectStorage);

        string message = "Group '" + groupName + "' has been deleted!";
        myGroup->addMessageForGroupMembers(message);

        // DELETING GROUP FROM EVERY VOLUME
        // try to find groups in volumes
        vector<StringPair> deleteGroupFromVolume = myVolumeStorage.findAllEntries(groupName);
        vector<utils::StringPair>::const_iterator VolumeIterator;
        if (deleteGroupFromVolume.size())
        {
            cout << "Deleting group from the following volumes:" << endl;
            VolumeIterator=deleteGroupFromVolume.begin();
            while ( VolumeIterator != deleteGroupFromVolume.end())
            {
                StringPair myStringpair = *(VolumeIterator);
                string myVolumeID = myStringpair.first;
                KeyronaVolume *myVolume = new KeyronaVolume(myVolumeID, myVolumeStorage);
                myVolume->printVolume();
                myVolume->deleteGroup(myGroup);
                delete myVolume;
                VolumeIterator++;
            }
        }
        removeFile(myGroup->getMyGroupKeyfile());
        myGroup->deleteGroup();
        delete myGroup;

        if (!KeyronaFindGroup(myGroupStorage, groupName))
            cout << "Successfully deleted group '" << groupName << "'." << endl;
        else
            cout << "Failed to delete group '" << groupName << "'!" << endl;

        printToSyslog("KEYRONA: Group '" + groupName + "' was removed from this system");
    }
};

//================================================================================
//
void KeyronaGroupManagement     ( string groupParam, KeyronaConfigfile &myConfigfile )
{
    t_StringMap KeyronaGroupMap;
    KeyronaGroupMap["createGroup"]            = createGroup;
    KeyronaGroupMap["cg"]                     = createGroup;
    KeyronaGroupMap["deleteGroup"]            = deleteGroup;
    KeyronaGroupMap["dg"]                     = deleteGroup;
    KeyronaGroupMap["listGroups"]             = listGroups;
    KeyronaGroupMap["lg"]                     = listGroups;
    KeyronaGroupMap["addSubjectToGroup"]      = addSubjectToGroup;
    KeyronaGroupMap["astg"]                   = addSubjectToGroup;
    KeyronaGroupMap["deleteSubjectFromGroup"] = deleteSubjectFromGroup;
    KeyronaGroupMap["dsfg"]                   = deleteSubjectFromGroup;
    KeyronaGroupMap["listSubjectsInGroup"]    = listSubjectsInGroup;
    KeyronaGroupMap["lsig"]                   = listSubjectsInGroup;

    // Create User database storage object
    KeyronaStorage mySubjectStorage( "SubjectDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_SubjectDBIdentifier) );

    // Create Group database storage object
    KeyronaStorage myGroupStorage( "GroupDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_GroupDBIdentifier) );

    // Create Volume database storage object
    KeyronaStorage myVolumeStorage( "VolumeDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_VolumeDBIdentifier) );

    switch(KeyronaGroupMap[groupParam])
    {
        case createGroup:
            KeyronaCreateGroup(myGroupStorage, mySubjectStorage);
            break;
        case deleteGroup:
            KeyronaDeleteGroup(myGroupStorage, mySubjectStorage, myVolumeStorage);
            break;
        case listGroups:
            KeyronaListGroups(myGroupStorage, mySubjectStorage);
            break;
        case addSubjectToGroup:
            KeyronaAddSubjectToGroup(myGroupStorage, mySubjectStorage);
            break;
        case deleteSubjectFromGroup:
            KeyronaDeleteSubjectFromGroup(myGroupStorage, mySubjectStorage);
            break;
        case listSubjectsInGroup:
            KeyronaListSubjectsInGroup(myGroupStorage, mySubjectStorage);
            break;
        default:
            throw ParseError("invalid parameter");
    }
};
