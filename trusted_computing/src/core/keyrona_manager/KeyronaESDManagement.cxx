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
    createESD,
    showESD
};

// mapping between strings and int values
typedef map<string, int> t_StringMap;

//================================================================================
//
void KeyronaCreateESD(KeyronaStorage &myESDStorage)
{
	if (loginSuperUser())
        cout << "Superuser successfully logged in!" << endl;

    string myESD = getStringFromUser("Please enter name for new ESD: ");

    /*if (KeyronaFindESD(mySubjectStorage, myESD))
        throw InvalidUsername("KeyronaCreateESD(): ESD already exists!");
        
    KeyronaSubject newESD(, myESD, myEMail, myC, myO, myOU, mySP, myL, false, myKeyDirectory, mySubjectStorage);
        if (! newESD.getMyESDID())
            throw NotCreated("ESD could not be created!");
        cout << "Successfully created new admin with SubjectID '" << newAdmin.getMySubjectID() << "'." << endl;
        printToSyslog("KEYRONA: Admin '" + myUsername + "' was added to this system");
        cout << "Creating new admin group..." << endl;
*/
};

void KeyronaShowESD(KeyronaStorage &myESDStorage)
{
	
};

void KeyronaESDManagement( string esdParam, KeyronaConfigfile &myConfigfile )
{
    t_StringMap KeyronaESDMap;
    KeyronaInitMap["createESD"]   = createESD;
    KeyronaInitMap["ce"]          = createESD;
    KeyronaInitMap["showESD"]     = showESD;
    KeyronaInitMap["se"]          = showESD;

    // Create User database storage object
    KeyronaStorage mySubjectStorage( "ESDDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_ESDDBIdentifier) );
    
    switch(KeyronaESDMap[esdParam])
    {
        case createESD:
            KeyronaCreateESD(myESDStorage);
            break;
        case showESD:
            KeyronaShowESD(myESDStorage);
            break;
        default:
            throw ParseError("invalid parameter");
    }
};
