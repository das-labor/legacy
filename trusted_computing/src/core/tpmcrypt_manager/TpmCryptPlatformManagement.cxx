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


#include <TpmCrypt.hxx>
#include <TpmCryptManager.hxx>
#include <TpmCryptTPM.hxx>

using namespace std;
using namespace utils;
using namespace tpmcrypt;

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
bool TpmCryptListPlatforms(TpmCryptStorage &myPlatformStorage)
{
    vector<string> myPlatforms = mySubjectStorage.TpmCryptFindAllPlatforms(myPlatformStorage);

    vector<string>::const_iterator Iterator;
    if (myPlatforms.size())
    {
        cout << "We have the following Platforms: " << endl;

        Iterator=myPlatforms.begin();
        while ( Iterator != myPlatforms.end())
        {
            if (!TpmCryptFindPlatform(myPlatformStorage, Iterator->first))
            {
                Iterator++;
                continue;
            }

            TpmCryptPlatform myPlatform(Iterator->first, myPlatformStorage);
            myPlatform.printPlatform(printVerbose);
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
void TpmCryptDeletePlatform(TpmCryptStorage &mySubjectStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (TpmCryptListPlatforms(mySubjectStorage))
    {
        vector<string> myPlatforms = TpmCryptFindAllPlatforms(mySubjectStorage);
        if (! myPlatforms.size())
        {
            cout << "No Platforms found." << endl;
            return;
        }
        string PlatformToDelete = selectFromStringVector(myPlatforms, "Number:\t\tPlatform:", "Please select the platform to be deleted:");

        if (!TpmCryptFindPlatform(mySubjectStorage, PlatformToDelete))
            throw UnknownSubject("Platform '" + PlatformToDelete + "' not found.");

        TpmCryptSubject *mySubject = new TpmCryptSubject(PlatformToDelete, mySubjectStorage);

        if (mySubject->getMySubjectType() != SUBJECTTYPE_PLATFORM)
            throw InvalidSubjectType("You can only delete platforms!");

        if (verifySelection())
        {
            // DELETING Platform
            removeFile(mySubject->getMySubjectKeyfile() + TPMCRYPT_TPM_KEY_EXTENSION);
            removeFile(mySubject->getMySubjectKeyfile() + TPMCRYPT_TPM_DATA_EXTENSION);

            mySubject->deleteSubject();
            delete mySubject;

            if (!TpmCryptFindPlatform(mySubjectStorage, PlatformToDelete))
                cout << "Successfully deleted platform '" << PlatformToDelete << "'." << endl;
            else
                cout << "Failed to delete platform '" << PlatformToDelete << "'!" << endl;
            printToSyslog("TPMCRYPT: Platform '" + PlatformToDelete + "' was deleted from this system");
        }
    };
};

//================================================================================
//
void TpmCryptCreatePlatform(TpmCryptStorage &mySubjectStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    vector<string> myPlatforms = TpmCryptFindAllPlatforms(mySubjectStorage);

    string myPlatformname = getStringFromUser("Please enter name for new Platform: ");
    //if (!TpmCryptFindPlatform(mySubjectStorage, myPlatformname))
      // throw InvalidPlatform("TpmCryptCreatePlatform(): Platform already exists!");

    string myEMail = TPMCRYPT_PRIVATE_MODE_ADMIN_MAIL;
    string myC  = TPMCRYPT_PRIVATE_MODE_C;
    string myO  = TpmCryptSubjectType_Platform;
    string myOU = TpmCryptSubjectType_Platform;
    string mySP = TpmCryptSubjectType_Platform;
    string myL  = TpmCryptSubjectType_Platform;
	vector<int> pcr = TpmCryptChooseAllPCR();
	string pcr_string = convertIntVectorToString(pcr);
    
    TpmCryptSubject newPlatform(SUBJECTTYPE_PLATFORM, myPlatformname, pcr_string, myEMail, myC, myO, myOU, mySP, myL, false, myKeyDirectory, mySubjectStorage);
    if (! newPlatform.getMySubjectID())
        throw NotCreated("Platform could not be created!");

    cout << "Successfully created new Platform with SubjectID '" << newPlatform.getMySubjectID() << "'." << endl;
    printToSyslog("TPMCRYPT: Platform '" + myPlatformname + "' was added to this system");
    newPlatform.printSubject(printVerbose);
};

//================================================================================
//
vector<int> TpmCryptChooseAllPCR()
{	
	vector<int> final;
	t_StringMap TpmCryptPlatformTypeMap;
    TpmCryptPlatformTypeMap["PCR00"] = PLATFORMTYPE_PCR00;
    TpmCryptPlatformTypeMap["PCR01"] = PLATFORMTYPE_PCR01;
    TpmCryptPlatformTypeMap["PCR02"] = PLATFORMTYPE_PCR02;
    TpmCryptPlatformTypeMap["PCR03"] = PLATFORMTYPE_PCR03;
    TpmCryptPlatformTypeMap["PCR04"] = PLATFORMTYPE_PCR04;
    TpmCryptPlatformTypeMap["PCR05"] = PLATFORMTYPE_PCR05;
    TpmCryptPlatformTypeMap["PCR06"] = PLATFORMTYPE_PCR06;
    TpmCryptPlatformTypeMap["PCR07"] = PLATFORMTYPE_PCR07;
    TpmCryptPlatformTypeMap["PCR08"] = PLATFORMTYPE_PCR08;
    TpmCryptPlatformTypeMap["PCR09"] = PLATFORMTYPE_PCR09;
    TpmCryptPlatformTypeMap["PCR10"] = PLATFORMTYPE_PCR10;
    TpmCryptPlatformTypeMap["PCR11"] = PLATFORMTYPE_PCR11;
    TpmCryptPlatformTypeMap["PCR12"] = PLATFORMTYPE_PCR12;
    TpmCryptPlatformTypeMap["PCR13"] = PLATFORMTYPE_PCR13;
    TpmCryptPlatformTypeMap["PCR14"] = PLATFORMTYPE_PCR14;
    TpmCryptPlatformTypeMap["PCR15"] = PLATFORMTYPE_PCR15;
    TpmCryptPlatformTypeMap["PCR16"] = PLATFORMTYPE_PCR16;
    TpmCryptPlatformTypeMap["PCR17"] = PLATFORMTYPE_PCR17;
    TpmCryptPlatformTypeMap["PCR18"] = PLATFORMTYPE_PCR18;
    TpmCryptPlatformTypeMap["PCR19"] = PLATFORMTYPE_PCR19;
    TpmCryptPlatformTypeMap["PCR20"] = PLATFORMTYPE_PCR20;
    TpmCryptPlatformTypeMap["PCR21"] = PLATFORMTYPE_PCR21;
    TpmCryptPlatformTypeMap["PCR22"] = PLATFORMTYPE_PCR22;
    TpmCryptPlatformTypeMap["PCR23"] = PLATFORMTYPE_PCR23;
    
    vector<string> AvailablePCR;
	AvailablePCR.push_back("PCR00");
	AvailablePCR.push_back("PCR01");
	AvailablePCR.push_back("PCR02");
	AvailablePCR.push_back("PCR03");
	AvailablePCR.push_back("PCR04");
	AvailablePCR.push_back("PCR05");
	AvailablePCR.push_back("PCR06");
	AvailablePCR.push_back("PCR07");
	AvailablePCR.push_back("PCR08");
	AvailablePCR.push_back("PCR09");
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
		switch(TpmCryptPlatformTypeMap[pcrid])
		{
				case PLATFORMTYPE_PCR00: final.push_back(x); break;
				case PLATFORMTYPE_PCR01: x = 1; final.push_back(x); break;
				case PLATFORMTYPE_PCR02: x = 2; final.push_back(x); break;
				case PLATFORMTYPE_PCR03: x = 3; final.push_back(x); break;
				case PLATFORMTYPE_PCR04: x = 4; final.push_back(x); break;
				case PLATFORMTYPE_PCR05: x = 5; final.push_back(x); break;
				case PLATFORMTYPE_PCR06: x = 6; final.push_back(x); break;
				case PLATFORMTYPE_PCR07: x = 7; final.push_back(x); break;
				case PLATFORMTYPE_PCR08: x = 8; final.push_back(x); break;
				case PLATFORMTYPE_PCR09: x = 9; final.push_back(x); break;
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
	vector<int>::const_iterator i;
	for(i = final.begin(); i != final.end(); i++)
	{
		cout << *(i) << endl;
		
	}
	return final;
};

//================================================================================
//
void TpmCryptPlatformManagement     ( string platformParam, TpmCryptConfigfile &myConfigfile )
{
    t_StringMap TpmCryptPlatformMap;
    TpmCryptPlatformMap["createPlatform"]    = createPlatform;
    TpmCryptPlatformMap["cp"]                = createPlatform;
    TpmCryptPlatformMap["importPlatform"]    = importPlatform;
    TpmCryptPlatformMap["ip"]                = importPlatform;
    TpmCryptPlatformMap["deletePlatform"]    = deletePlatform;
    TpmCryptPlatformMap["dp"]                = deletePlatform;
    TpmCryptPlatformMap["listPlatforms"]     = listPlatforms;
    TpmCryptPlatformMap["lp"]                = listPlatforms;
    TpmCryptPlatformMap["exportPlatform"]    = exportPlatform;
    TpmCryptPlatformMap["ep"]                = exportPlatform;

    // Create Subject database storage object
    TpmCryptStorage mySubjectStorage( "SubjectDB", myConfigfile.getConfigfileEntry(TpmCryptConfigfile_SubjectDBIdentifier) );

    switch(TpmCryptPlatformMap[platformParam])
    {
        case createPlatform:
            TpmCryptCreatePlatform(mySubjectStorage);
            break;
        case importPlatform:
            throw NotImplemented("importPlatform");
            break;
        case deletePlatform:
            TpmCryptDeletePlatform(mySubjectStorage);
            break;
        case listPlatforms:
            TpmCryptListPlatforms(mySubjectStorage);
            break;
        case exportPlatform:
            throw NotImplemented("exportPlatform");
            break;
        default:
            throw utils::ParseError("invalid parameter");
    }
};
