// Copyright (C) 2008, 2009 by Sirrix AG security technologies#
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
#include <KeyronaTPM.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

enum myMenu
{
    createToken = 1,
    deleteToken,
    listToken,
};

//================================================================================
//
bool KeyronaListToken(KeyronaStorage &mySubjectStorage)
{
    vector<StringPair> myToken = mySubjectStorage.findAllEntries(KeyronaSubject_SubjectType, KeyronaSubjectType_Token);

    vector<utils::StringPair>::const_iterator Iterator;
    if (myToken.size())
    {
        cout << "We have the following Token: " << endl;

        Iterator=myToken.begin();
        while ( Iterator != myToken.end())
        {
            if (!KeyronaFindToken(mySubjectStorage, Iterator->first))
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
        cout << "No Token found." << endl;
        return false;
    }
};

//================================================================================
//
void KeyronaDeleteToken(KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage, KeyronaStorage &myVolumeStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (KeyronaListToken(mySubjectStorage))
    {
        vector<string> myToken = KeyronaFindAllToken(mySubjectStorage);
        if (! myToken.size())
        {
            cout << "No Token found." << endl;
            return;
        }
        string TokenToDelete = selectFromStringVector(myToken, "Number:\t\tToken:", "Please select the token to be deleted:");

        if (!KeyronaFindToken(mySubjectStorage, TokenToDelete))
            throw UnknownSubject("Token '" + TokenToDelete + "' not found.");

        KeyronaSubject *mySubject = new KeyronaSubject(TokenToDelete, mySubjectStorage);

        if (mySubject->getMySubjectType() != SUBJECTTYPE_TOKEN)
            throw InvalidSubjectType("You can only delete token!");

        if (verifySelection())
        {
            string myTokenSubjectID = mySubject->getMySubjectIDString();

            // DELETING TOKEN FROM EVERY GROUP
            vector<StringPair> deleteTokenFromGroups = myGroupStorage.findAllEntries(myTokenSubjectID);

            // deleting token in every group
            vector<utils::StringPair>::const_iterator GroupIterator;

            if (deleteTokenFromGroups.size())
            {
                cout << "Deleting token from the following groups:" << endl;
                GroupIterator=deleteTokenFromGroups.begin();
                while ( GroupIterator != deleteTokenFromGroups.end())
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

            // DELETING TOKEN FROM EVERY VOLUME
            vector<StringPair> deleteTokenFromVolume = myVolumeStorage.findAllEntries(myTokenSubjectID);
            // deleting token in every volume
            vector<utils::StringPair>::const_iterator VolumeIterator;

            if (deleteTokenFromVolume.size())
            {
                cout << "Deleting token from the following volumes:" << endl;
                VolumeIterator=deleteTokenFromVolume.begin();
                while ( VolumeIterator != deleteTokenFromVolume.end())
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

            // FINALLY, DELETING TOKEN
            try
            {
        	removeFile(mySubject->getMySubjectKeyfile());
	    }
	    catch ( FileNotExists &e )
	    {
		cout << e.what() << endl;
	    }
            mySubject->deleteSubject();
            delete mySubject;

            if (!KeyronaFindSubject(mySubjectStorage, TokenToDelete))
                cout << "Successfully deleted token '" << TokenToDelete << "'." << endl;
            else
                cout << "Failed to delete token '" << TokenToDelete << "'!" << endl;
            printToSyslog("KEYRONA: Token '" + TokenToDelete + "' was deleted from this system");
        }
    }
};

//================================================================================
//
void KeyronaCreateToken(KeyronaStorage &mySubjectStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    string myTokenname = getStringFromUser("Please enter name for new Token: ");
    if (KeyronaFindToken(mySubjectStorage, myTokenname))
        throw InvalidToken("KeyronaCreateToken(): Token already exists!");

    string myEMail = KEYRONA_PRIVATE_MODE_ADMIN_MAIL;
    string myC  = KEYRONA_PRIVATE_MODE_C;
    string myO  = KeyronaSubjectType_Token;
    string myOU = KeyronaSubjectType_Token;
    string mySP = KeyronaSubjectType_Token;
    string myL  = KeyronaSubjectType_Token;
    
    string myTokenDirectory = getStringFromUser("Please specify the path of the mounted Token (e.g. /media/USB-Stick/): ");

    cout << "Please wait while creating new token." << endl;
    myTokenDirectory = removeDelimiter(myTokenDirectory);
    KeyronaSubject newToken(SUBJECTTYPE_TOKEN, myTokenname, myEMail, myC, myO, myOU, mySP, myL, false, myTokenDirectory, mySubjectStorage);
    if (! newToken.getMySubjectID())
        throw NotCreated("Token could not be created!");

    cout << "Successfully created new Token with SubjectID '" << newToken.getMySubjectID() << "'." << endl;
    printToSyslog("KEYRONA: Token '" + myTokenname + "' was added to this system");
    newToken.printSubject(printVerbose);
};


// mapping between strings and int values
typedef map<string, int> t_StringMap;

//================================================================================
//
void KeyronaTokenManagement     ( string tokenParam, KeyronaConfigfile &myConfigfile )
{
    t_StringMap KeyronaTokenMap;
    KeyronaTokenMap["createToken"]    = createToken;
    KeyronaTokenMap["ct"]                = createToken;
    KeyronaTokenMap["deleteToken"]    = deleteToken;
    KeyronaTokenMap["dt"]                = deleteToken;
    KeyronaTokenMap["listToken"]     = listToken;
    KeyronaTokenMap["lt"]                = listToken;

    // Create User database storage object
    KeyronaStorage mySubjectStorage( "SubjectDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_SubjectDBIdentifier) );

    // Create Group database storage object
    KeyronaStorage myGroupStorage( "GroupDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_GroupDBIdentifier) );

    // Create Volume database storage object
    KeyronaStorage myVolumeStorage( "VolumeDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_VolumeDBIdentifier) );
    
    switch(KeyronaTokenMap[tokenParam])
    {
        case createToken:
            KeyronaCreateToken(mySubjectStorage);
            break;
        case deleteToken:
            KeyronaDeleteToken(mySubjectStorage, myGroupStorage, myVolumeStorage);
            break;
        case listToken:
            KeyronaListToken(mySubjectStorage);
            break;
        default:
            throw utils::ParseError("invalid parameter");
    }
};
