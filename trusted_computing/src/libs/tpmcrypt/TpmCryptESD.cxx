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
 * @file        TpmCryptESD.cxx
 * @brief       Implementation of TpmCrypts ESD class.
 * @version     $Revision: 764 $
 * @date        Last modification at $Date: 2009-07-29 15:33:10 +0200 (Wed, 29 Jul 2009) $ by $Author: selhorst $
 */

#include <TpmCryptESD.hxx>
#include <TpmCryptTPM.hxx>


using namespace std;
using namespace utils;


//================================================================================
//
// load ESD by string
TpmCryptESD::TpmCryptESD( string ESDIdentifier, TpmCryptStorage &storageDB ) :
            myESDID (0),
            myESDIDString(""),
            myESDName (""),
            myESDUUID (""),
            myESDDevice (""),
            myESDStorage (storageDB)
{
    debug << "TpmCryptESD|Constructor()]: Loading ESD by ESDID string '" << myESDIDString << "'." << endl;

    loadESD(ESDIdentifier);
};

//================================================================================
//
// load ESD by ESDID
TpmCryptESD::TpmCryptESD( UInt32 ESDID, TpmCryptStorage &storageDB ) :
            myESDID (0),
            myESDIDString(""),
            myESDName (""),
            myESDUUID (""),
            myESDDevice (""),
            myESDStorage (storageDB)
{
    // convert given ESDID to a string
    string myESDIdentifierString = convertUInt32toString(ESDID);
    debug << "TpmCryptESD|Constructor()]: Loading ESD by ID '" << myESDIdentifierString << "'." << endl;

    loadESD(myESDIdentifierString);
};

//================================================================================
//
// create new ESD incl. key generation
TpmCryptESD::TpmCryptESD( string &ESDName, string &ESDDevice, string &ESDUUID,
                            TpmCryptStorage &storageDB ) :
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

    vector<StringPair> myESDs = storageDB.findAllEntries(TpmCryptESD_ESDName, myESDName);
    if (myESDs.size())
        throw InvalidUsername("TpmCryptSubject: The supplied ESDname is already in use!");

    debug << "TpmCryptSubject|Constructor(): Creating new SubjectID '" << myESDID << "'." << endl;
	
	storeESD();
};

void TpmCryptESD::deleteESD()
{
    debug << "TpmCryptESD|deleteESD()]: Deleting ESD from DB..." << endl;
    myESDID = 0;
    myESDName.clear();
    myESDUUID.clear();
    myESDDevice.clear();
    myESDStorage.deleteSection(myESDIDString);
    myESDIDString.clear();
};

bool TpmCryptESD::loadESD(string &ESDIdentifier)
{
    debug << "TpmCryptESD|loadESD()]: Loading ESD from DB..." << endl;

    if (ESDIdentifier.empty())
        throw InvalidUsername("TpmCryptESD: The supplied ESD was empty!");

    // first, try to identify ESD by name
    vector<StringPair> mySection = myESDStorage.findAllEntries(TpmCryptESD_ESDName, ESDIdentifier);
    if (mySection.size())
    {
        debug << "TpmCryptESD: Trying to load ESD by name: '" << ESDIdentifier << "'" << endl;
        StringPair mySectionResult;
        mySectionResult = mySection.back();
        myESDIDString = mySectionResult.first;
        myESDID = convertStringtoUInt32(myESDIDString);
    }
    // if it fails, try to load by ID
    else
    {
        debug << "TpmCryptESD: Trying to load ESD by ID: '" << ESDIdentifier << "'" << endl;
        if (! myESDStorage.findSection(ESDIdentifier))
           throw UnknownESD("TpmCryptESD: ESD '" + ESDIdentifier + "' not found!");
        myESDIDString = ESDIdentifier;
        // convert given ESDID to a string
        myESDID = convertStringtoUInt32(myESDIDString);
    }

    myESDStorage.selectSection(myESDIDString);
    myESDStorage.selectSubSection(TpmCryptESD_ESDProperties);
    myESDName = myESDStorage.getEntry(TpmCryptESD_ESDName);
    myESDUUID = myESDStorage.getEntry(TpmCryptESD_ESDUUID);
    myESDDevice = myESDStorage.getEntry(TpmCryptESD_ESDDevice);

    return true;
};

bool TpmCryptESD::storeESD()
{
    debug << "TpmCryptESD|storeESD()]: Storing ESD to DB..." << endl;    myESDStorage.selectSection(myESDIDString);
    
    myESDStorage.selectSubSection(TpmCryptESD_ESDProperties);
    myESDStorage.setEntry(TpmCryptESD_isESD, TpmCryptESD_isESD_true);
    myESDStorage.setEntry(TpmCryptESD_ESDName, myESDName);
    myESDStorage.setEntry(TpmCryptESD_ESDUUID, myESDUUID);
    myESDStorage.setEntry(TpmCryptESD_ESDDevice, myESDDevice);
    
    return true;
};

TpmCryptESD::~TpmCryptESD()
{
    debug << "TpmCryptESD|Destructor()]: Destructor for ESD '" << myESDName << "' called." << endl;
};
