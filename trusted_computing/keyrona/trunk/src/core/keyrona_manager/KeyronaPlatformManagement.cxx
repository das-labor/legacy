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
 * @file        KeyronaPlatformManagement.cxx
 * @brief       Implementation of the command line tool 'keyrona_manager'.
 * @version     $Revision: 673 $
 * @date        Last modification at $Date: 2009-06-25 15:14:43 +0200 (Thu, 25 Jun 2009) $ by $Author: selhorst $
 */

#include <Keyrona.hxx>
#include <KeyronaManager.hxx>
#include <KeyronaTPM.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

enum myMenu
{
    createPlatform = 1,
    importPlatform,
    deletePlatform,
    listPlatforms,
    exportPlatform
};

//================================================================================
//
bool KeyronaListPlatforms(KeyronaStorage &mySubjectStorage)
{
    vector<StringPair> myPlatforms = mySubjectStorage.findAllEntries(KeyronaSubject_SubjectType, KeyronaSubjectType_Platform);

    vector<utils::StringPair>::const_iterator Iterator;
    if (myPlatforms.size())
    {
        cout << "We have the following Platforms: " << endl;

        Iterator=myPlatforms.begin();
        while ( Iterator != myPlatforms.end())
        {
            if (!KeyronaFindPlatform(mySubjectStorage, Iterator->first))
            {
                Iterator++;
                continue;
            }

            KeyronaSubject mySubject(Iterator->first, mySubjectStorage);
            mySubject.printSubject(printVerbose);
            Iterator++;
        }
        return true;
    }
    else
    {
        cout << "No Platforms found." << endl;
        return false;
    }
};

//================================================================================
//
void KeyronaDeletePlatform(KeyronaStorage &mySubjectStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (KeyronaListPlatforms(mySubjectStorage))
    {
        vector<string> myPlatforms = KeyronaFindAllPlatforms(mySubjectStorage);
        if (! myPlatforms.size())
        {
            cout << "No Platforms found." << endl;
            return;
        }
        string PlatformToDelete = selectFromStringVector(myPlatforms, "Number:\t\tPlatform:", "Please select the platform to be deleted:");

        if (!KeyronaFindPlatform(mySubjectStorage, PlatformToDelete))
            throw UnknownSubject("Platform '" + PlatformToDelete + "' not found.");

        KeyronaSubject *mySubject = new KeyronaSubject(PlatformToDelete, mySubjectStorage);

        if (mySubject->getMySubjectType() != SUBJECTTYPE_PLATFORM)
            throw InvalidSubjectType("You can only delete platforms!");

        if (verifySelection())
        {
            // DELETING Platform
            removeFile(mySubject->getMySubjectKeyfile());
            removeFile(mySubject->getMySubjectKeyfile() + KEYRONA_TPM_KEY_EXTENSION);
            removeFile(mySubject->getMySubjectKeyfile() + KEYRONA_TPM_DATA_EXTENSION);

            mySubject->deleteSubject();
            delete mySubject;

            if (!KeyronaFindPlatform(mySubjectStorage, PlatformToDelete))
                cout << "Successfully deleted platform '" << PlatformToDelete << "'." << endl;
            else
                cout << "Failed to delete platform '" << PlatformToDelete << "'!" << endl;
            printToSyslog("KEYRONA: Platform '" + PlatformToDelete + "' was deleted from this system");
        }
    };
};

//================================================================================
//
void KeyronaCreatePlatform(KeyronaStorage &mySubjectStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    vector<string> myPlatforms = KeyronaFindAllPlatforms(mySubjectStorage);

    string myPlatformname = getStringFromUser("Please enter name for new Platform: ");
    if (KeyronaFindPlatform(mySubjectStorage, myPlatformname))
        throw InvalidPlatform("KeyronaCreatePlatform(): Platform already exists!");

    string myEMail = KEYRONA_PRIVATE_MODE_ADMIN_MAIL;
    string myC  = KEYRONA_PRIVATE_MODE_C;
    string myO  = KeyronaSubjectType_Platform;
    string myOU = KeyronaSubjectType_Platform;
    string mySP = KeyronaSubjectType_Platform;
    string myL  = KeyronaSubjectType_Platform;

    cout << "Please wait while creating new platform." << endl;
    cout << "Note: This step might require some additional time due to TPM access. Please be patient..." << endl;
    KeyronaSubject newPlatform(SUBJECTTYPE_PLATFORM, myPlatformname, myEMail, myC, myO, myOU, mySP, myL, false, myKeyDirectory, mySubjectStorage);
    if (! newPlatform.getMySubjectID())
        throw NotCreated("Platform could not be created!");

    cout << "Successfully created new Platform with SubjectID '" << newPlatform.getMySubjectID() << "'." << endl;
    printToSyslog("KEYRONA: Platform '" + myPlatformname + "' was added to this system");
    newPlatform.printSubject(printVerbose);
};


// mapping between strings and int values
typedef map<string, int> t_StringMap;

//================================================================================
//
void KeyronaPlatformManagement     ( string platformParam, KeyronaConfigfile &myConfigfile )
{
    t_StringMap KeyronaPlatformMap;
    KeyronaPlatformMap["createPlatform"]    = createPlatform;
    KeyronaPlatformMap["cp"]                = createPlatform;
    KeyronaPlatformMap["importPlatform"]    = importPlatform;
    KeyronaPlatformMap["ip"]                = importPlatform;
    KeyronaPlatformMap["deletePlatform"]    = deletePlatform;
    KeyronaPlatformMap["dp"]                = deletePlatform;
    KeyronaPlatformMap["listPlatforms"]     = listPlatforms;
    KeyronaPlatformMap["lp"]                = listPlatforms;
    KeyronaPlatformMap["exportPlatform"]    = exportPlatform;
    KeyronaPlatformMap["ep"]                = exportPlatform;

    // Create Subject database storage object
    KeyronaStorage mySubjectStorage( "SubjectDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_SubjectDBIdentifier) );

    switch(KeyronaPlatformMap[platformParam])
    {
        case createPlatform:
            KeyronaCreatePlatform(mySubjectStorage);
            break;
        case importPlatform:
            throw NotImplemented("importPlatform");
            break;
        case deletePlatform:
            KeyronaDeletePlatform(mySubjectStorage);
            break;
        case listPlatforms:
            KeyronaListPlatforms(mySubjectStorage);
            break;
        case exportPlatform:
            throw NotImplemented("exportPlatform");
            break;
        default:
            throw utils::ParseError("invalid parameter");
    }
};
