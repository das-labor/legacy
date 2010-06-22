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
            myESDType (ESDTYPE_UNDEFINED),
            myESDID (0),
            myESDIDString(""),
            myESDName (""),
            myESDKeyUUID (""),
            myESDKeyType (""),
            myESDEMail (""),
            myESDKeyfile (""),
            myESDCountrycode (""),
            myESDOrganisation (""),
            myESDOrganisationUnit (""),
            myESDState (""),
            myESDLocality (""),
            myESDisAdmin (0),
            myESDKey(0),
            myESDStorage (storageDB)
{
    debug << "KeyronaESD|Constructor()]: Loading ESD by ESDID string '" << myESDIDString << "'." << endl;

    loadESD(ESDIdentifier);
};

//================================================================================
//
// load ESD by ESDID
KeyronaESD::KeyronaESD( UInt32 ESDID, KeyronaStorage &storageDB ) :
            myESDType (ESDTYPE_UNDEFINED),
            myESDID (0),
            myESDIDString(""),
            myESDName (""),
            myESDKeyUUID (""),
            myESDKeyType (""),
            myESDEMail (""),
            myESDKeyfile (""),
            myESDCountrycode (""),
            myESDOrganisation (""),
            myESDOrganisationUnit (""),
            myESDState (""),
            myESDLocality (""),
            myESDisAdmin (0),
            myESDKey(0),
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
KeyronaESD::KeyronaESD( UInt8 ESDType,
                            string &ESDName,
                            string &ESDEMail,
                            string &ESDCountryCode,
                            string &ESDOrganisation,
                            string &ESDOrganisationUnit,
                            string &ESDState,
                            string &ESDLocality,
                            bool   admin,
                            string &keyDirectory,
                            KeyronaStorage &storageDB ) :
            myESDType (ESDType),
            myESDID (0),
            myESDIDString(""),
            myESDName (ESDName),
            myESDKeyUUID (""),
            myESDKeyType (""),
            myESDEMail (ESDEMail),
            myESDKeyfile (""),
            myESDCountrycode (ESDCountryCode),
            myESDOrganisation (ESDOrganisation),
            myESDOrganisationUnit (ESDOrganisationUnit),
            myESDState (ESDState),
            myESDLocality (ESDLocality),
            myESDisAdmin (admin),
            myESDKey(0),
            myESDStorage (storageDB)
{
    checkUsername();
