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

typedef map<string, int> t_StringMap;

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
    //if (!KeyronaFindPlatform(mySubjectStorage, myPlatformname))
     //   throw InvalidPlatform("KeyronaCreatePlatform(): Platform already exists!");

    string myEMail = KEYRONA_PRIVATE_MODE_ADMIN_MAIL;
    string myC  = KEYRONA_PRIVATE_MODE_C;
    string myO  = KeyronaSubjectType_Platform;
    string myOU = KeyronaSubjectType_Platform;
    string mySP = KeyronaSubjectType_Platform;
    string myL  = KeyronaSubjectType_Platform;
	vector<int> pcr = KeyronaChooseAllPCR();
	string pcr_string = convertIntVectorToString(&pcr);
    
    KeyronaSubject newPlatform(SUBJECTTYPE_PLATFORM, myPlatformname, pcr_string, myEMail, myC, myO, myOU, mySP, myL, false, myKeyDirectory, mySubjectStorage);
    if (! newPlatform.getMySubjectID())
        throw NotCreated("Platform could not be created!");

    cout << "Successfully created new Platform with SubjectID '" << newPlatform.getMySubjectID() << "'." << endl;
    printToSyslog("KEYRONA: Platform '" + myPlatformname + "' was added to this system");
    newPlatform.printSubject(printVerbose);
};

//================================================================================
//
vector<int> KeyronaChooseAllPCR()
{	
	vector<int> final;
	t_StringMap KeyronaPlatformTypeMap;
    KeyronaPlatformTypeMap["PCR0"] = PLATFORMTYPE_PCR0;
    KeyronaPlatformTypeMap["PCR1"] = PLATFORMTYPE_PCR1;
    KeyronaPlatformTypeMap["PCR2"] = PLATFORMTYPE_PCR2;
    KeyronaPlatformTypeMap["PCR3"] = PLATFORMTYPE_PCR3;
    KeyronaPlatformTypeMap["PCR4"] = PLATFORMTYPE_PCR4;
    KeyronaPlatformTypeMap["PCR5"] = PLATFORMTYPE_PCR5;
    KeyronaPlatformTypeMap["PCR6"] = PLATFORMTYPE_PCR6;
    KeyronaPlatformTypeMap["PCR7"] = PLATFORMTYPE_PCR7;
    KeyronaPlatformTypeMap["PCR8"] = PLATFORMTYPE_PCR8;
    KeyronaPlatformTypeMap["PCR9"] = PLATFORMTYPE_PCR9;
    KeyronaPlatformTypeMap["PCR10"] = PLATFORMTYPE_PCR10;
    KeyronaPlatformTypeMap["PCR11"] = PLATFORMTYPE_PCR11;
    KeyronaPlatformTypeMap["PCR12"] = PLATFORMTYPE_PCR12;
    KeyronaPlatformTypeMap["PCR13"] = PLATFORMTYPE_PCR13;
    KeyronaPlatformTypeMap["PCR14"] = PLATFORMTYPE_PCR14;
    KeyronaPlatformTypeMap["PCR15"] = PLATFORMTYPE_PCR15;
    KeyronaPlatformTypeMap["PCR16"] = PLATFORMTYPE_PCR16;
    KeyronaPlatformTypeMap["PCR17"] = PLATFORMTYPE_PCR17;
    KeyronaPlatformTypeMap["PCR18"] = PLATFORMTYPE_PCR18;
    KeyronaPlatformTypeMap["PCR19"] = PLATFORMTYPE_PCR19;
    KeyronaPlatformTypeMap["PCR20"] = PLATFORMTYPE_PCR20;
    KeyronaPlatformTypeMap["PCR21"] = PLATFORMTYPE_PCR21;
    KeyronaPlatformTypeMap["PCR22"] = PLATFORMTYPE_PCR22;
    KeyronaPlatformTypeMap["PCR23"] = PLATFORMTYPE_PCR23;
    
    vector<string> AvailablePCR;
	AvailablePCR.push_back("PCR0");
	AvailablePCR.push_back("PCR1");
	AvailablePCR.push_back("PCR2");
	AvailablePCR.push_back("PCR3");
	AvailablePCR.push_back("PCR4");
	AvailablePCR.push_back("PCR5");
	AvailablePCR.push_back("PCR6");
	AvailablePCR.push_back("PCR7");
	AvailablePCR.push_back("PCR8");
	AvailablePCR.push_back("PCR9");
	AvailablePCR.push_back("PCR10");
	AvailablePCR.push_back("PCR11");
	AvailablePCR.push_back("PCR12");
	AvailablePCR.push_back("PCR13");
	AvailablePCR.push_back("PCR14");
	AvailablePCR.push_back("PCR15");
	AvailablePCR.push_back("PCR16");
	AvailablePCR.push_back("PCR17");
	AvailablePCR.push_back("PCR18");
	AvailablePCR.push_back("PCR19");
	AvailablePCR.push_back("PCR20");
	AvailablePCR.push_back("PCR21");
	AvailablePCR.push_back("PCR22");
	AvailablePCR.push_back("PCR23");

    string toStop = "-1";
    string pcrid;
    int x = 0;
    while ((pcrid.compare(0, toStop.length(), toStop)) && (AvailablePCR.size()))
    {
		pcrid = selectFromStringVector(AvailablePCR, "Please choose the Platform Configuration Register:  ", "Enter a number: ");
		if (pcrid == toStop)
            break;
		switch(KeyronaPlatformTypeMap[pcrid])
		{
				case PLATFORMTYPE_PCR0: final.push_back(x); break;
				case PLATFORMTYPE_PCR1: x = 1; final.push_back(x); break;
				case PLATFORMTYPE_PCR2: x = 2; final.push_back(x); break;
				case PLATFORMTYPE_PCR3: x = 3; final.push_back(x); break;
				case PLATFORMTYPE_PCR4: x = 4; final.push_back(x); break;
				case PLATFORMTYPE_PCR5: x = 5; final.push_back(x); break;
				case PLATFORMTYPE_PCR6: x = 6; final.push_back(x); break;
				case PLATFORMTYPE_PCR7: x = 7; final.push_back(x); break;
				case PLATFORMTYPE_PCR8: x = 8; final.push_back(x); break;
				case PLATFORMTYPE_PCR9: x = 9; final.push_back(x); break;
				case PLATFORMTYPE_PCR10: x = 10; final.push_back(x); break;
				case PLATFORMTYPE_PCR11: x = 11; final.push_back(x); break;
				case PLATFORMTYPE_PCR12: x = 12; final.push_back(x); break;
				case PLATFORMTYPE_PCR13: x = 13; final.push_back(x); break;
				case PLATFORMTYPE_PCR14: x = 14; final.push_back(x); break;
				case PLATFORMTYPE_PCR15: x = 15; final.push_back(x); break;
				case PLATFORMTYPE_PCR16: x = 16; final.push_back(x); break;
				case PLATFORMTYPE_PCR17: x = 17; final.push_back(x); break;
				case PLATFORMTYPE_PCR18: x = 18; final.push_back(x); break;
				case PLATFORMTYPE_PCR19: x = 19; final.push_back(x); break;
				case PLATFORMTYPE_PCR20: x = 20; final.push_back(x); break;
				case PLATFORMTYPE_PCR21: x = 21; final.push_back(x); break;
				case PLATFORMTYPE_PCR22: x = 22; final.push_back(x); break;
				case PLATFORMTYPE_PCR23: x = 23; final.push_back(x); break;
		}
		removeStringFromStringVector(&AvailablePCR, pcrid);
	}
	
	return final;
};

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
