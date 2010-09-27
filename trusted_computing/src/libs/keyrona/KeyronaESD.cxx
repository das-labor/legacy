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
 * @file        KeyronaESD.cxx
 * @brief       Implementation of Keyronas ESD class.
 * @version     $Revision: 764 $
 * @date        Last modification at $Date: 2009-07-29 15:33:10 +0200 (Wed, 29 Jul 2009) $ by $Author: selhorst $
 */

#include <KeyronaESD.hxx>
#include <KeyronaTPM.hxx>


using namespace std;
using namespace utils;


//================================================================================
//
// load ESD by string
KeyronaESD::KeyronaESD( string ESDIdentifier, KeyronaStorage &storageDB ) :
            myESDID (0),
            myESDIDString(""),
            myESDName (""),
            myESDUUID (""),
            myESDDevice (""),
            myESDStorage (storageDB)
{
    debug << "KeyronaESD|Constructor()]: Loading ESD by ESDID string '" << myESDIDString << "'." << endl;

    loadESD(ESDIdentifier);
};

//================================================================================
//
// load ESD by ESDID
KeyronaESD::KeyronaESD( UInt32 ESDID, KeyronaStorage &storageDB ) :
            myESDID (0),
            myESDIDString(""),
            myESDName (""),
            myESDUUID (""),
            myESDDevice (""),
            myESDStorage (storageDB)
{
    // convert given ESDID to a string
    string myESDIdentifierString = convertUInt32toString(ESDID);
    debug << "KeyronaESD|Constructor()]: Loading ESD by ID '" << myESDIdentifierString << "'." << endl;

    loadESD(myESDIdentifierString);
};

//================================================================================
//
// create new ESD incl. key generation
KeyronaESD::KeyronaESD( string &ESDName, string &ESDDevice, string &ESDUUID,
                            KeyronaStorage &storageDB ) :
            myESDID (0),
            myESDIDString(""),
            myESDName (ESDName),
            myESDUUID (ESDUUID),
            myESDDevice (ESDDevice),
            myESDStorage (storageDB)
{
	// Initialize RNG
    srand(time(NULL));

    // generate new random subjectID
    do
    {
        myESDID = rand();
        myESDIDString = convertUInt32toString(myESDID);
    }
    while (storageDB.findSection(myESDIDString));

    vector<StringPair> myESDs = storageDB.findAllEntries(KeyronaESD_ESDName, myESDName);
    if (myESDs.size())
        throw InvalidUsername("KeyronaSubject: The supplied ESDname is already in use!");

    debug << "KeyronaSubject|Constructor(): Creating new SubjectID '" << myESDID << "'." << endl;
	
	storeESD();
};

void KeyronaESD::deleteESD()
{
    debug << "KeyronaESD|deleteESD()]: Deleting ESD from DB..." << endl;
    myESDID = 0;
    myESDName.clear();
    myESDUUID.clear();
    myESDDevice.clear();
    myESDStorage.deleteSection(myESDIDString);
    myESDIDString.clear();
};

bool KeyronaESD::loadESD(string &ESDIdentifier)
{
    debug << "KeyronaESD|loadESD()]: Loading ESD from DB..." << endl;

    if (ESDIdentifier.empty())
        throw InvalidUsername("KeyronaESD: The supplied ESD was empty!");

    // first, try to identify ESD by name
    vector<StringPair> mySection = myESDStorage.findAllEntries(KeyronaESD_ESDName, ESDIdentifier);
    if (mySection.size())
    {
        debug << "KeyronaESD: Trying to load ESD by name: '" << ESDIdentifier << "'" << endl;
        StringPair mySectionResult;
        mySectionResult = mySection.back();
        myESDIDString = mySectionResult.first;
        myESDID = convertStringtoUInt32(myESDIDString);
    }
    // if it fails, try to load by ID
    else
    {
        debug << "KeyronaESD: Trying to load ESD by ID: '" << ESDIdentifier << "'" << endl;
        if (! myESDStorage.findSection(ESDIdentifier))
           throw UnknownESD("KeyronaESD: ESD '" + ESDIdentifier + "' not found!");
        myESDIDString = ESDIdentifier;
        // convert given ESDID to a string
        myESDID = convertStringtoUInt32(myESDIDString);
    }

    myESDStorage.selectSection(myESDIDString);
    myESDStorage.selectSubSection(KeyronaESD_ESDProperties);
    myESDName = myESDStorage.getEntry(KeyronaESD_ESDName);
    myESDUUID = myESDStorage.getEntry(KeyronaESD_ESDUUID);
    myESDDevice = myESDStorage.getEntry(KeyronaESD_ESDDevice);

    return true;
};

bool KeyronaESD::storeESD()
{
    debug << "KeyronaESD|storeESD()]: Storing ESD to DB..." << endl;    myESDStorage.selectSection(myESDIDString);
    
    myESDStorage.selectSubSection(KeyronaESD_ESDProperties);
    myESDStorage.setEntry(KeyronaESD_isESD, KeyronaESD_isESD_true);
    myESDStorage.setEntry(KeyronaESD_ESDName, myESDName);
    myESDStorage.setEntry(KeyronaESD_ESDUUID, myESDUUID);
    myESDStorage.setEntry(KeyronaESD_ESDDevice, myESDDevice);
    
    return true;
};

KeyronaESD::~KeyronaESD()
{
    debug << "KeyronaESD|Destructor()]: Destructor for ESD '" << myESDName << "' called." << endl;
};
