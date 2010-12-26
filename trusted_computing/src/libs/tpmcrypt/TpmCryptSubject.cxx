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
 * @file        TpmCryptSubject.cxx
 * @brief       Implementation of TpmCrypts subject class.
 * @version     $Revision: 764 $
 * @date        Last modification at $Date: 2009-07-29 15:33:10 +0200 (Wed, 29 Jul 2009) $ by $Author: selhorst $
 */

#include <TpmCryptSubject.hxx>
#include <TpmCryptTPM.hxx>


using namespace std;
using namespace utils;


//================================================================================
//
// load subject by string
TpmCryptSubject::TpmCryptSubject( string subjectIdentifier, TpmCryptStorage &storageDB ) :
            mySubjectType (SUBJECTTYPE_UNDEFINED),
            mySubjectID (0),
            mySubjectIDString(""),
            mySubjectName (""),
            mySubjectKeyUUID (""),
            mySubjectKeyType (""),
            mySubjectAuth (""),
            mySubjectPlatformSelection (""),
            mySubjectEMail (""),
            mySubjectKeyfile (""),
            mySubjectCountrycode (""),
            mySubjectOrganisation (""),
            mySubjectOrganisationUnit (""),
            mySubjectState (""),
            mySubjectLocality (""),
            mySubjectisAdmin (0),
            mySubjectKey(0),
            mySubjectStorage (storageDB)
{
    debug << "TpmCryptSubject|Constructor()]: Loading Subject by subjectID string '" << mySubjectIDString << "'." << endl;

    loadSubject(subjectIdentifier);
};

//================================================================================
//
// load subject by subjectID
TpmCryptSubject::TpmCryptSubject( UInt32 subjectID, TpmCryptStorage &storageDB ) :
            mySubjectType (SUBJECTTYPE_UNDEFINED),
            mySubjectID (0),
            mySubjectIDString(""),
            mySubjectName (""),
            mySubjectKeyUUID (""),
            mySubjectKeyType (""),
            mySubjectAuth (""),
            mySubjectPlatformSelection (""),
            mySubjectEMail (""),
            mySubjectKeyfile (""),
            mySubjectCountrycode (""),
            mySubjectOrganisation (""),
            mySubjectOrganisationUnit (""),
            mySubjectState (""),
            mySubjectLocality (""),
            mySubjectisAdmin (0),
            mySubjectKey(0),
            mySubjectStorage (storageDB)
{
    // convert given SubjectID to a string
    string mySubjectIdentifierString = convertUInt32toString(subjectID);
    debug << "TpmCryptSubject|Constructor()]: Loading Subject by ID '" << mySubjectIdentifierString << "'." << endl;

    loadSubject(mySubjectIdentifierString);
};

//================================================================================
//
// create new subject incl. key generation
TpmCryptSubject::TpmCryptSubject( UInt8 subjectType,
                            string &subjectName,
                            string &pcr_string,
                            string &subjectEMail,
                            string &subjectCountryCode,
                            string &subjectOrganisation,
                            string &subjectOrganisationUnit,
                            string &subjectState,
                            string &subjectLocality,
                            bool   admin,
                            string &keyDirectory,
                            TpmCryptStorage &storageDB ) :
            mySubjectType (subjectType),
            mySubjectID (0),
            mySubjectIDString(""),
            mySubjectName (subjectName),
            mySubjectKeyUUID (""),
            mySubjectKeyType (""),
            mySubjectAuth (""),
            mySubjectPlatformSelection (pcr_string),
            mySubjectEMail (subjectEMail),
            mySubjectKeyfile (""),
            mySubjectCountrycode (subjectCountryCode),
            mySubjectOrganisation (subjectOrganisation),
            mySubjectOrganisationUnit (subjectOrganisationUnit),
            mySubjectState (subjectState),
            mySubjectLocality (subjectLocality),
            mySubjectisAdmin (admin),
            mySubjectKey(0),
            mySubjectStorage (storageDB)
{
    checkUsername();

    if ( (mySubjectType < SUBJECTTYPE_BEGIN) | (mySubjectType > SUBJECTTYPE_END) )
        throw InvalidSubjectType("TpmCryptSubject: Subject Type was invalid!");

    if (mySubjectType == SUBJECTTYPE_PLATFORM)
        mySubjectName = "Platform_" + mySubjectName;

    if (mySubjectType == SUBJECTTYPE_TOKEN)
        mySubjectName = "Token_" + mySubjectName;

    if (keyDirectory.empty())
        throw NoFilename("TpmCryptSubject: The supplied key directory was empty!");

    if (!dirExists(keyDirectory))
        throw DirNotExist("TpmCryptSubject: The supplied key directory does not exist!");

    if (!dirWritable(keyDirectory))
        throw DirNotWritable("TpmCryptSubject: The supplied key directory is not writable!");
	
	// Initialize RNG
    srand(time(NULL));

    // generate new random subjectID
    mySubjectKeyUUID = convertUInt32toString((rand() % 100000 + 1));
	if (mySubjectKeyUUID.empty()) 
		throw InvalidKeyUUID("TpmCryptSubject: The supplied Key UUID was invalid");
		
    mySubjectKeyType = "subject";

    string myKeyDirectory = keyDirectory;

    if  (
        (mySubjectEMail.empty()) |
        (mySubjectEMail.find("@") == string::npos ) |
        (mySubjectEMail.find(".") == string::npos )
        )
            throw InvalidEMail("TpmCryptSubject: The supplied eMail address was invalid!");

    if (mySubjectCountrycode.empty())
        throw InvalidCountrycode("TpmCryptSubject: The supplied countrycode was empty!");

    if (mySubjectCountrycode.length() != 2)
        throw InvalidCountrycode("TpmCryptSubject: The supplied countrycode was invalid!");

    mySubjectCountrycode[0] = toupper(mySubjectCountrycode[0]);
    mySubjectCountrycode[1] = toupper(mySubjectCountrycode[1]);

    if (mySubjectOrganisation.empty())
        throw InvalidOrganisation("TpmCryptSubject: The supplied organisation was empty!");

    if (mySubjectOrganisationUnit.empty())
        throw InvalidOrganisationUnit("TpmCryptSubject: The supplied organisation unit was empty!");

    if (mySubjectState.empty())
        throw InvalidState("TpmCryptSubject: The supplied state/province was empty!");

    if (mySubjectLocality.empty())
        throw InvalidLocality("TpmCryptSubject: The supplied locality was empty!");


    // Initialize RNG
    srand(time(NULL));

    // generate new random subjectID
    do
    {
        mySubjectID = rand();
        mySubjectIDString = convertUInt32toString(mySubjectID);
    }
    while (storageDB.findSection(mySubjectIDString));

    vector<StringPair> myUsers = storageDB.findAllEntries(TpmCryptSubject_SubjectName, mySubjectName);
    if (myUsers.size())
        throw InvalidUsername("TpmCryptSubject: The supplied subjectname is already in use!");

    debug << "TpmCryptSubject|Constructor(): Creating new SubjectID '" << mySubjectID << "'." << endl;

    string myPassword;
    switch(mySubjectType)
    {
        case SUBJECTTYPE_USER:
        {
            mySubjectKeyfile = myKeyDirectory + TpmCryptPathSeparator + "TpmCrypt" + TpmCryptSubjectType_User + TpmCryptFileSeparator + mySubjectIDString  + TpmCryptFileSeparator + mySubjectName + TpmCryptP15FileExtension;
            if (mySubjectisAdmin) 
                myPassword = getPasswordVerified("Please enter passphrase for new admin '" + mySubjectName + "': ");
            else 
                myPassword = getPasswordVerified("Please enter passphrase for new user '" + mySubjectName + "': ");
                
                mySubjectKey = new TpmCryptKey(this, myPassword);
            
            mySubjectAuth = "0";
            break;
        }
        case SUBJECTTYPE_TOKEN:
        {
            mySubjectKeyfile = myKeyDirectory + TpmCryptPathSeparator + "TpmCrypt" + TpmCryptSubjectType_Token + TpmCryptFileSeparator + mySubjectIDString  + TpmCryptFileSeparator + mySubjectName + TpmCryptP15FileExtension;
            string defaultPassword = getStringFromUser("Do you want to set a password for this token? [yes | no]: ");
            if (
        	(defaultPassword == "y") ||
        	(defaultPassword == "yes") ||
        	(defaultPassword == "Y") ||
        	(defaultPassword == "Yes") ||
        	(defaultPassword == "YES")
    	       )
                myPassword = getPasswordVerified("Please enter passphrase for new token '" + mySubjectName + "': ");
            else
                myPassword = TPMCRYPT_TOKEN_STANDARD_PASSWD;

            break;
        }
        case SUBJECTTYPE_PLATFORM:
        {
            mySubjectKeyfile = myKeyDirectory + TpmCryptPathSeparator + "TpmCrypt" + TpmCryptSubjectType_Platform + TpmCryptFileSeparator + mySubjectIDString  + TpmCryptFileSeparator + mySubjectName + TpmCryptP15FileExtension;

			if(mySubjectAuth.empty())
			{
				TpmCryptTPM myTPM;
				cout << mySubjectPlatformSelection << endl;
				vector<int> pcr = convertStringToIntVector(mySubjectPlatformSelection);
				int local = 0;
				cout << mySubjectPlatformSelection << endl;
				string test = generateRandomString(6);
				cout << "Authentifikation Data: " << test << "\tBitte merken oder notieren sie sich diese 6 Zeichen" << endl;
				ByteVector random = convertStringToByteVector(test);
				vector<ByteVector> mySealedDataWithAuth = myTPM.seal(random,local,pcr); // XXX see kyronatpm.hxx
				ByteVector AuthData = mySealedDataWithAuth.back();
				mySubjectAuth = EncodeByteVectorToBASE64(AuthData);
				mySealedDataWithAuth.pop_back();
				pcr.clear();
			}
			break;
        }
    }
    storeSubject();
};

//================================================================================
//
// import subject without key generation
TpmCryptSubject::TpmCryptSubject( UInt8 subjectType,
                            string &subjectName,
                            string &pcr_string,
                            string &subjectEMail,
                            string &subjectCountryCode,
                            string &subjectOrganisation,
                            string &subjectOrganisationUnit,
                            string &subjectState,
                            string &subjectLocality,
                            bool   admin,
                            string &keyDirectory,
                            string &subjectKeyfile,
                            bool copyKeyfile,
                            TpmCryptStorage &storageDB ) :
            mySubjectType (subjectType),
            mySubjectID (0),
            mySubjectIDString(""),
            mySubjectName (subjectName),
            mySubjectKeyUUID (""),
            mySubjectKeyType (""),
            mySubjectAuth (""),  
            mySubjectPlatformSelection (pcr_string),         
            mySubjectEMail (subjectEMail),
            mySubjectKeyfile (""),
            mySubjectCountrycode (subjectCountryCode),
            mySubjectOrganisation (subjectOrganisation),
            mySubjectOrganisationUnit (subjectOrganisationUnit),
            mySubjectState (subjectState),
            mySubjectLocality (subjectLocality),
            mySubjectisAdmin (admin),
            mySubjectKey(0),
            mySubjectStorage (storageDB)
{
    checkUsername();

    if ( (mySubjectType < SUBJECTTYPE_BEGIN) | (mySubjectType > SUBJECTTYPE_END) )
        throw InvalidSubjectType("TpmCryptSubject: Subject Type was invalid!");

    if (keyDirectory.empty())
        throw NoFilename("TpmCryptSubject: The supplied key directory was empty!");

    if (!dirExists(keyDirectory))
        throw DirNotExist("TpmCryptSubject: The supplied key directory does not exist!");

    if (!dirWritable(keyDirectory))
        throw DirNotWritable("TpmCryptSubject: The supplied key directory is not writable!");
    
    string myKeyDirectory = keyDirectory;

    if  (
        (mySubjectEMail.empty()) |
        (mySubjectEMail.find("@") == string::npos ) |
        (mySubjectEMail.find(".") == string::npos )
        )
            throw InvalidEMail("TpmCryptSubject: The supplied eMail address was invalid!");

    if (mySubjectCountrycode.empty())
        throw InvalidCountrycode("TpmCryptSubject: The supplied countrycode was empty!");

    if (mySubjectCountrycode.length() != 2)
        throw InvalidCountrycode("TpmCryptSubject: The supplied countrycode was invalid!");

    mySubjectCountrycode[0] = toupper(mySubjectCountrycode[0]);
    mySubjectCountrycode[1] = toupper(mySubjectCountrycode[1]);

    if (mySubjectOrganisation.empty())
        throw InvalidOrganisation("TpmCryptSubject: The supplied organisation was empty!");

    if (mySubjectOrganisationUnit.empty())
        throw InvalidOrganisationUnit("TpmCryptSubject: The supplied organisation unit was empty!");

    if (mySubjectState.empty())
        throw InvalidState("TpmCryptSubject: The supplied state/province was empty!");

    if (mySubjectLocality.empty())
        throw InvalidLocality("TpmCryptSubject: The supplied locality was empty!");


    // Initialize RNG
    srand(time(NULL));

    // generate new random subjectID
    do
    {
        mySubjectID = rand();
        mySubjectIDString = convertUInt32toString(mySubjectID);
    }
    while (storageDB.findSection(mySubjectIDString));

    vector<StringPair> myUsers = storageDB.findAllEntries(TpmCryptSubject_SubjectName, mySubjectName);
    if (myUsers.size())
        throw InvalidUsername("TpmCryptSubject: The supplied subjectname is already in use!");

    debug << "TpmCryptSubject|Constructor()]: Creating new SubjectID '" << mySubjectID << "'." << endl;

    if (copyKeyfile)
    {
        debug << "TpmCryptSubject|Constructor()]: Copying keyfile '" << subjectKeyfile << "' to '" << mySubjectKeyfile << "'" << endl;
        mySubjectKeyfile = myKeyDirectory + TpmCryptPathSeparator + mySubjectIDString  + TpmCryptFileSeparator + mySubjectName + TpmCryptP15FileExtension;
        string myCommand = "/bin/cp \"" + subjectKeyfile + "\" \"" + mySubjectKeyfile + "\"";
        if (system(myCommand.c_str()))
            throw RuntimeError("TpmCryptSubject(): Could not copy keyfile '" + subjectKeyfile + "' to '" + mySubjectKeyfile + "'");
    }
    else
    {
        debug << "TpmCryptSubject|Constructor()]: Not copying keyfile '" << subjectKeyfile  << endl;
        mySubjectKeyfile = subjectKeyfile;
    }

    storeSubject();
};

//================================================================================
//
TpmCryptSubject::~TpmCryptSubject()
{
    debug << "TpmCryptSubject|Destructor()]: Destructor for subject '" << mySubjectName << "' called." << endl;
    if (mySubjectKey)
        delete mySubjectKey;
};

//================================================================================
//
void TpmCryptSubject::printSubject(bool verbose)
{
    typedef map<int, string> t_StringMap;
    t_StringMap TpmCryptSubjectType;
    TpmCryptSubjectType[SUBJECTTYPE_UNDEFINED]       = TpmCryptSubjectType_Undefined;
    TpmCryptSubjectType[SUBJECTTYPE_USER]            = TpmCryptSubjectType_User;
    TpmCryptSubjectType[SUBJECTTYPE_PLATFORM]        = TpmCryptSubjectType_Platform;
    TpmCryptSubjectType[SUBJECTTYPE_TOKEN]           = TpmCryptSubjectType_Token;
    TpmCryptDate myLastLogin = getMyLastLogin();

    // and here the full info
    cout << "Name: '" << mySubjectName << "' (ID: " << mySubjectID << ")" << endl;
    cout << "KeyUUID: '" << mySubjectKeyUUID << endl;
    cout << "KeyType: '" << mySubjectKeyType << endl;
    cout << mySubjectKey->printKeyInformation(this) << endl;
    cout << "\t" << "Type: '" << TpmCryptSubjectType[mySubjectType] << "'" << endl;
	
	if( mySubjectType == SUBJECTTYPE_PLATFORM )
	{
		vector<int>::iterator it;
		vector<int> pcr_info = convertStringToIntVector(mySubjectPlatformSelection);
		cout << "Platform Configuration register:" << endl;
		for (it=pcr_info.begin(); it!=pcr_info.end(); ++it)
		{
			cout << "PCR: " << *it << endl;
		}
	}
	
    if ((EnterpriseMode) && (mySubjectType == SUBJECTTYPE_USER))
    {
        cout << "\t" << "EMail: '" << mySubjectEMail << "'" << endl;
        cout << "\t" << "Countrycode: '" << mySubjectCountrycode << "'" << endl;
        cout << "\t" << "Organisation: '" << mySubjectOrganisation << "'" << endl;
        cout << "\t" << "OrganisationUnit: '" << mySubjectOrganisationUnit << "'" << endl;
        cout << "\t" << "State: '" << mySubjectState << "'" << endl;
        cout << "\t" << "Locality: '" << mySubjectLocality << "'" << endl;
    }
    cout << "\t" << "Keyfile: '" << mySubjectKeyfile << "'" << endl;
    if (mySubjectType == SUBJECTTYPE_USER)
        cout << "\t" << "Subject is admin: '" << (mySubjectisAdmin ? TpmCryptSubject_TRUE:TpmCryptSubject_FALSE) << "'" << endl;

    cout << "\t" << "Subject's last login: ";
    if (myLastLogin.isValid())
        myLastLogin.printDate();
    else
        cout << "Subject has not logged in, yet!" << endl;
        
    if (mySubjectType == SUBJECTTYPE_USER)
        printCurrentMessagesForUser();

};

//================================================================================
//
bool TpmCryptSubject::loadSubject(string &subjectIdentifier)
{
    debug << "TpmCryptSubject|loadSubject()]: Loading Subject from DB..." << endl;

    typedef map<string, int> t_StringMap;
    t_StringMap TpmCryptSubjectType;
    TpmCryptSubjectType[TpmCryptSubjectType_Undefined]       = SUBJECTTYPE_UNDEFINED;
    TpmCryptSubjectType[TpmCryptSubjectType_User]            = SUBJECTTYPE_USER;
    TpmCryptSubjectType[TpmCryptSubjectType_Platform]        = SUBJECTTYPE_PLATFORM;
    TpmCryptSubjectType[TpmCryptSubjectType_Token]           = SUBJECTTYPE_TOKEN;

    if (subjectIdentifier.empty())
        throw InvalidUsername("TpmCryptSubject: The supplied subjectname was empty!");

    // first, try to identify subject by name
    vector<StringPair> mySection = mySubjectStorage.findAllEntries(TpmCryptSubject_SubjectName, subjectIdentifier);
    if (mySection.size())
    {
        debug << "TpmCryptSubject: Trying to load subject by name: '" << subjectIdentifier << "'" << endl;
        StringPair mySectionResult;
        mySectionResult = mySection.back();
        mySubjectIDString = mySectionResult.first;
        mySubjectID = convertStringtoUInt32(mySubjectIDString);
    }
    // if it fails, try to load by ID
    else
    {
        debug << "TpmCryptSubject: Trying to load subject by ID: '" << subjectIdentifier << "'" << endl;
        if (! mySubjectStorage.findSection(subjectIdentifier))
            throw UnknownSubject("TpmCryptSubject: Subject '" + subjectIdentifier + "' not found!");
        mySubjectIDString = subjectIdentifier;
        // convert given SubjectID to a string
        mySubjectID = convertStringtoUInt32(mySubjectIDString);
    }

    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.selectSubSection(TpmCryptSubject_SubjectProperties);
    mySubjectName = mySubjectStorage.getEntry(TpmCryptSubject_SubjectName);
    mySubjectKeyUUID = mySubjectStorage.getEntry(TpmCryptSubject_SubjectKeyUUID);
    mySubjectKeyType = mySubjectStorage.getEntry(TpmCryptSubject_SubjectKeyType);
    mySubjectAuth = mySubjectStorage.getEntry(TpmCryptSubject_SubjectAuth);
    mySubjectType = TpmCryptSubjectType[mySubjectStorage.getEntry(TpmCryptSubject_SubjectType)];
    mySubjectPlatformSelection = mySubjectStorage.getEntry(TpmCryptSubject_SubjectPlatformSelection);
    mySubjectEMail = mySubjectStorage.getEntry(TpmCryptSubject_SubjectEMail);
    mySubjectCountrycode = mySubjectStorage.getEntry(TpmCryptSubject_SubjectCountrycode);
    mySubjectOrganisation = mySubjectStorage.getEntry(TpmCryptSubject_SubjectOrganisation);
    mySubjectOrganisationUnit = mySubjectStorage.getEntry(TpmCryptSubject_SubjectOrganisationUnit);
    mySubjectState = mySubjectStorage.getEntry(TpmCryptSubject_SubjectState);
    mySubjectLocality = mySubjectStorage.getEntry(TpmCryptSubject_SubjectLocality);
    mySubjectKeyfile = mySubjectStorage.getEntry(TpmCryptSubject_SubjectKeyfile);
    mySubjectisAdmin = ! mySubjectStorage.getEntry(TpmCryptSubject_SubjectAdmin).compare(TpmCryptSubject_TRUE);

    return true;
};

//================================================================================
//
bool TpmCryptSubject::storeSubject()
{
    debug << "TpmCryptSubject|storeSubject()]: Storing Subject to DB..." << endl;

    typedef map<int, string> t_StringMap;
    t_StringMap TpmCryptSubjectType;
    TpmCryptSubjectType[SUBJECTTYPE_UNDEFINED]       = TpmCryptSubjectType_Undefined;
    TpmCryptSubjectType[SUBJECTTYPE_USER]            = TpmCryptSubjectType_User;
    TpmCryptSubjectType[SUBJECTTYPE_PLATFORM]        = TpmCryptSubjectType_Platform;
    TpmCryptSubjectType[SUBJECTTYPE_TOKEN]           = TpmCryptSubjectType_Token;

    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.selectSubSection(TpmCryptSubject_SubjectProperties);
    mySubjectStorage.setEntry(TpmCryptSubject_isSubject, TpmCryptSubject_isSubject_true);
    mySubjectStorage.setEntry(TpmCryptSubject_SubjectName, mySubjectName);
    mySubjectStorage.setEntry(TpmCryptSubject_SubjectKeyUUID, mySubjectKeyUUID);
    mySubjectStorage.setEntry(TpmCryptSubject_SubjectKeyType, mySubjectKeyType);
    mySubjectStorage.setEntry(TpmCryptSubject_SubjectAuth, mySubjectAuth);
    mySubjectStorage.setEntry(TpmCryptSubject_SubjectPlatformSelection, mySubjectPlatformSelection);
    mySubjectStorage.setEntry(TpmCryptSubject_SubjectType, TpmCryptSubjectType[mySubjectType]);
    mySubjectStorage.setEntry(TpmCryptSubject_SubjectEMail, mySubjectEMail);
    mySubjectStorage.setEntry(TpmCryptSubject_SubjectKeyfile, mySubjectKeyfile);
    mySubjectStorage.setEntry(TpmCryptSubject_SubjectCountrycode, mySubjectCountrycode);
    mySubjectStorage.setEntry(TpmCryptSubject_SubjectOrganisation, mySubjectOrganisation);
    mySubjectStorage.setEntry(TpmCryptSubject_SubjectOrganisationUnit, mySubjectOrganisationUnit);
    mySubjectStorage.setEntry(TpmCryptSubject_SubjectState, mySubjectState);
    mySubjectStorage.setEntry(TpmCryptSubject_SubjectLocality, mySubjectLocality);
    mySubjectStorage.setEntry(TpmCryptSubject_SubjectAdmin, mySubjectisAdmin ? TpmCryptSubject_TRUE : TpmCryptSubject_FALSE);

    mySubjectStorage.selectSubSection(TpmCryptSubject_PasswordSection);
    if (mySubjectType == SUBJECTTYPE_USER) {
	ByteVector passwordMagicVector;
    // creating password magic test vector
    passwordMagicVector = convertStringToByteVector(mySubjectIDString);
    ByteVector passwordMagicVectorByteVector = encryptForSubject(this, passwordMagicVector);
    string myBase64EncodedPasswordMagicVector = EncodeByteVectorToBASE64(passwordMagicVectorByteVector);
    mySubjectStorage.setEntry(TpmCryptSubject_PasswordMagicEntry, myBase64EncodedPasswordMagicVector);
	}
    
    // assigning group and file permission
    try
    {
		if (mySubjectType == SUBJECTTYPE_PLATFORM) {
	    }
		else {
        assignFilePermission(mySubjectKeyfile, KEYFILEPERMISSION);
		}	
    }
    catch ( std::exception &e )
    {
        cout << "WARNING: " << e.what() << endl;
        cout << "Note: Permissions can not be applied correctly, if you are using a FAT32/VFAT filesystem." << endl;
    }
    return true;
};

//================================================================================
//
void TpmCryptSubject::deleteSubject()
{
    debug << "TpmCryptSubject|deleteSubject()]: Deleting Subject from DB..." << endl;
    mySubjectType = SUBJECTTYPE_UNDEFINED;
    mySubjectID = 0;
    mySubjectName.clear();
    mySubjectKeyUUID.clear();
    mySubjectKeyType.clear();
    mySubjectAuth.clear();
    mySubjectEMail.clear();
    mySubjectKeyfile.clear();
    mySubjectCountrycode.clear();
    mySubjectisAdmin = false;
    mySubjectStorage.deleteSection(mySubjectIDString);
    mySubjectIDString.clear();
};

//================================================================================
//
TpmCryptDate TpmCryptSubject::getMyLastLogin()
{
    string tempString;
    vector<UInt32> myTempDate;
    myTempDate.clear();

    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.selectSubSection(TpmCryptSubjectLastLogin);

    tempString = mySubjectStorage.getEntry(TpmCryptSubjectLastLogin_Year);
    myTempDate.push_back(convertStringtoUInt32(tempString));
    tempString = mySubjectStorage.getEntry(TpmCryptSubjectLastLogin_Month);
    myTempDate.push_back(convertStringtoUInt32(tempString));
    tempString = mySubjectStorage.getEntry(TpmCryptSubjectLastLogin_Day);
    myTempDate.push_back(convertStringtoUInt32(tempString));
    tempString = mySubjectStorage.getEntry(TpmCryptSubjectLastLogin_Hour);
    myTempDate.push_back(convertStringtoUInt32(tempString));
    tempString = mySubjectStorage.getEntry(TpmCryptSubjectLastLogin_Minute);
    myTempDate.push_back(convertStringtoUInt32(tempString));
    tempString = mySubjectStorage.getEntry(TpmCryptSubjectLastLogin_Second);
    myTempDate.push_back(convertStringtoUInt32(tempString));

    try
    {
        TpmCryptDate myDate(myTempDate);
        debug << "TpmCryptSubject|getMyLastLogin(): Successfully read last login time" << endl;
        return myDate;
    }
    catch ( std::exception &e )
    {
        debug << "TpmCryptSubject|getMyLastLogin(): User has not logged in, yet." << endl;
        TpmCryptDate myDate(false);
        return myDate;
    }
};

//================================================================================
//
void TpmCryptSubject::setLastLogin(TpmCryptDate &LoginDate)
{
    vector<UInt32> myDate = LoginDate.getDate();
    UInt32 temp;
    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.selectSubSection(TpmCryptSubjectLastLogin);

    // store second
    temp = myDate.back();
    myDate.pop_back();
    mySubjectStorage.setEntry(TpmCryptSubjectLastLogin_Second, convertUInt32toString(temp));

    // store minute
    temp = myDate.back();
    myDate.pop_back();
    mySubjectStorage.setEntry(TpmCryptSubjectLastLogin_Minute, convertUInt32toString(temp));

    // store hour
    temp = myDate.back();
    myDate.pop_back();
    mySubjectStorage.setEntry(TpmCryptSubjectLastLogin_Hour, convertUInt32toString(temp));

    // store day
    temp = myDate.back();
    myDate.pop_back();
    mySubjectStorage.setEntry(TpmCryptSubjectLastLogin_Day, convertUInt32toString(temp));

    // store month
    temp = myDate.back();
    myDate.pop_back();
    mySubjectStorage.setEntry(TpmCryptSubjectLastLogin_Month, convertUInt32toString(temp));

    // store year
    temp = myDate.back();
    myDate.pop_back();
    mySubjectStorage.setEntry(TpmCryptSubjectLastLogin_Year, convertUInt32toString(temp));
};

//================================================================================
//   
ByteVector  TpmCryptSubject::encryptForSubject(TpmCryptSubject *Subject, ByteVector &toEncrypt)
{
    if (! toEncrypt.size())
       throw DecryptionFailed("KeyonaSubject|encryptForSubject(): No data supplied to be encrypted!");
	
	if (mySubjectType == SUBJECTTYPE_PLATFORM)
	{	
			loadSubject(mySubjectIDString);
			cout << mySubjectPlatformSelection << endl;
			vector<int> pcr = convertStringToIntVector(mySubjectPlatformSelection);
			int local=0;
            // sealing it to current platform
            TpmCryptTPM myTPM;
            vector<ByteVector> mySealedDataWithKey = myTPM.seal(toEncrypt,local,pcr); // XXX see kyronatpm.hxx
			
            // storing sealed data
            ByteVector Data = mySealedDataWithKey.back();
            mySealedDataWithKey.pop_back();
            pcr.clear();

            /*string DataFile = mySubjectKeyfile + TPMCRYPT_TPM_DATA_EXTENSION;
            storeByteVectorInFile(DataFile, Data);

            // storing sealing key
            ByteVector Key = mySealedDataWithKey.back();
            mySealedDataWithKey.pop_back();
            string KeyFile = mySubjectKeyfile + TPMCRYPT_TPM_KEY_EXTENSION;
            storeByteVectorInFile(KeyFile, Key);
            */
            return Data;
    }
	else {
				
    //if((mySubjectKey) && (mySubjectKey->isValid()))
	
    return mySubjectKey->encrypt(this, NULL, toEncrypt);
	//	throw InvalidKey("KeyonaSubject|encryptForSubject(): Invalid key for subject '" + mySubjectName + "'");
	}
};

//================================================================================
//
string  TpmCryptSubject::verifyAuth(string &top)
{
	loadSubject(top);
	TpmCryptTPM myTPM;
	ByteVector test2 = DecodeBASE64StringToByteVector(mySubjectAuth);
	vector<ByteVector> mySealedDataWithAuth;
	mySealedDataWithAuth.push_back(test2);
	ByteVector mhh = myTPM.unseal(mySealedDataWithAuth);
	mySealedDataWithAuth.pop_back();
	string test3 = convertByteVector2String(mhh);

	return test3;
};

//================================================================================
//
ByteVector  TpmCryptSubject::decryptBySubject(TpmCryptSubject *Subject, ByteVector &toDecrypt, string &myPassword)
{
  string password;
    if (mySubjectType == SUBJECTTYPE_PLATFORM)
    {
        TpmCryptTPM myTPM;
        vector<ByteVector> mySealedDataWithKey;
		mySealedDataWithKey.push_back(toDecrypt);
		/*// Loading sealing key
            string KeyFile = mySubjectKeyfile + TPMCRYPT_TPM_KEY_EXTENSION;
            ByteVector Key = loadByteVectorFromFile(KeyFile);
            mySealedDataWithKey.push_back(Key);

            // Loading sealed data
            string DataFile = mySubjectKeyfile + TPMCRYPT_TPM_DATA_EXTENSION;
            ByteVector Data = loadByteVectorFromFile(DataFile);
            mySealedDataWithKey.push_back(Data);*/

            ByteVector myPlainPassword = myTPM.unseal(mySealedDataWithKey);
            mySealedDataWithKey.pop_back();
            
            
            /*if(mySubjectAuth != 0)
            {
     
			}*/
			
            return myPlainPassword;
            
        
/*
            string x( (const char*) myPlainPassword.toCArray(), myPlainPassword.size() );
            //if (!verifyPassword(x))
             //  throw InvalidPassword("TpmCryptSubject|decryptBySubject(): Could not retrieve the password from TPM!");
            password = x;
            cout << password << endl;*/
    }
    else    
    {
        if (myPassword.empty())
            throw InvalidPassword("KeyonaSubject|decryptBySubject(): Invalid password for subject '" + mySubjectName + "'");
        password = myPassword;
    
   // if (!(verifyPassword(password)))
    //   throw InvalidPassword("KeyonaSubject|decryptBySubject(): Invalid password for subject '" + mySubjectName + "'");
	
   //if (! toDecrypt.size())
     //   throw DecryptionFailed("KeyonaSubject|decryptBySubject(): No data supplied to be decrypted!");

    debug << "KeyonaSubject|decryptBySubject(): Updating subject's last login with current system time" << endl;
    TpmCryptDate myCurrentSystemTime;
    setLastLogin(myCurrentSystemTime);

    //if ((mySubjectKey) && (mySubjectKey->isValid()))
        return mySubjectKey->decrypt(Subject , NULL, toDecrypt, password);
    //throw InvalidKey("KeyonaSubject|decryptBySubject(): Invalid key for subject '" + mySubjectName + "'");
}

};

//================================================================================
//
bool TpmCryptSubject::verifyPassword(string &myPassword)
{
    debug << "TpmCryptSubject["+mySubjectIDString+"|verifyPassword()]: verifying password..." << endl;
    if (myPassword.empty())
        throw InvalidPassword("KeyonaSubject|verifyPassword(): Invalid password for subject '" + mySubjectName + "'");
    
    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.selectSubSection(TpmCryptSubject_PasswordSection);
    string myBase64EncodedPasswordMagicVector = mySubjectStorage.getEntry(TpmCryptSubject_PasswordMagicEntry);

    if (myBase64EncodedPasswordMagicVector.empty())
        throw NoPasswordMagic("KeyonaSubject|verifyPassword(): No password magic available for subject '" + mySubjectName + "'. Please delete and re-create user!");

    string myPasswordMagic;
    ByteVector myPasswordMagicVectorByteVector;
    // decode password magic
    ByteVector myPasswordMagicVector = DecodeBASE64StringToByteVector(myBase64EncodedPasswordMagicVector);

    debug << "TpmCryptSubject["+mySubjectIDString+"|verifyPassword()]: decrypting testvector from database with given password" << endl;
    // decrypted password magic
    try
    {
        myPasswordMagicVectorByteVector = mySubjectKey->decrypt(this, NULL, myPasswordMagicVector, myPassword);
    }
    catch ( std::exception &e )
    {
        throw DecryptionFailed("TpmCryptSubject|verifyPassword(): Invalid Password!");
    }
    myPasswordMagic = convertByteVector2String(myPasswordMagicVectorByteVector);
    if (myPasswordMagic == mySubjectIDString)
        return true;
    else
        return false;
};

//================================================================================
//
bool TpmCryptSubject::changePassword( string &oldPassword, string &newPassword)
{
    if (! newPassword.size())
        throw InvalidPassword("TpmCryptSubject|changePassword(): Empty (new) password supplied!");

        return mySubjectKey->changePassword(this, oldPassword, newPassword);
};

//================================================================================
//
void TpmCryptSubject::addMessageForUser(string &message)
{
    if (message.empty())
        throw NoMessage("TpmCryptSubject|addMessageForUser(): No message supplied!");

    if (mySubjectType != SUBJECTTYPE_USER)
    {
        debug << "TpmCryptSubject|addMessageForUser(): Messages can only be added for users!";
        printToSyslog("TPMCRYPT: Subject '" + mySubjectName + "': Received message => '" + message + "'");
        return;
    }

    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.selectSubSection(TpmCryptSubject_Messages);
    TpmCryptDate currentDate;
    string currentTime = currentDate.getDateString();
    mySubjectStorage.setEntry(currentTime, message);
    printToSyslog("TPMCRYPT: Subject '" + mySubjectName + "': Received message => '" + message + "'");
};

//================================================================================
//
void TpmCryptSubject::printCurrentMessagesForUser()
{
    vector<StringPair> currentMessages = getCurrentMessagesForUser();
    if (currentMessages.size())
    {
        cout << "\tCurrent system messages for user '" << mySubjectName << "': " << endl;
        printStringPairVector(&currentMessages);
    }
};

//================================================================================
//
vector<StringPair> TpmCryptSubject::getCurrentMessagesForUser()
{
    if (mySubjectType != SUBJECTTYPE_USER)
    {
        debug << "TpmCryptSubject|printCurrentMessagesForUser(): Messages are only available for users!" << endl;
        vector<StringPair> nothing;
        return nothing;
    }
    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.selectSubSection(TpmCryptSubject_Messages);
    return mySubjectStorage.getAllEntriesWithValues();
};

//================================================================================
//
void TpmCryptSubject::deleteUserMessages()
{
    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.deleteSubSection(TpmCryptSubject_Messages);
};

//================================================================================
//
void TpmCryptSubject::checkUsername()
{
    std::vector<std::string> TpmCryptSubject_invalidUsernames;
    TpmCryptSubject_invalidUsernames.push_back("async");
    TpmCryptSubject_invalidUsernames.push_back("atime");
    TpmCryptSubject_invalidUsernames.push_back("noatime");
    TpmCryptSubject_invalidUsernames.push_back("auto");
    TpmCryptSubject_invalidUsernames.push_back("noauto");
    TpmCryptSubject_invalidUsernames.push_back("context");
    TpmCryptSubject_invalidUsernames.push_back("defaults");
    TpmCryptSubject_invalidUsernames.push_back("dev");
    TpmCryptSubject_invalidUsernames.push_back("nodev");
    TpmCryptSubject_invalidUsernames.push_back("diratime");
    TpmCryptSubject_invalidUsernames.push_back("nodiratime");
    TpmCryptSubject_invalidUsernames.push_back("dirsync");
    TpmCryptSubject_invalidUsernames.push_back("exec");
    TpmCryptSubject_invalidUsernames.push_back("noexec");
    TpmCryptSubject_invalidUsernames.push_back("group");
    TpmCryptSubject_invalidUsernames.push_back("iversion");
    TpmCryptSubject_invalidUsernames.push_back("noiversion");
    TpmCryptSubject_invalidUsernames.push_back("mand");
    TpmCryptSubject_invalidUsernames.push_back("nomand");
    TpmCryptSubject_invalidUsernames.push_back("_netdev");
    TpmCryptSubject_invalidUsernames.push_back("nofail");
    TpmCryptSubject_invalidUsernames.push_back("relatime");
    TpmCryptSubject_invalidUsernames.push_back("norelatime");
    TpmCryptSubject_invalidUsernames.push_back("strictatime");
    TpmCryptSubject_invalidUsernames.push_back("nostrictatime");
    TpmCryptSubject_invalidUsernames.push_back("suid");
    TpmCryptSubject_invalidUsernames.push_back("nosuid");
    TpmCryptSubject_invalidUsernames.push_back("owner");
    TpmCryptSubject_invalidUsernames.push_back("remount");
    TpmCryptSubject_invalidUsernames.push_back("ro");
    TpmCryptSubject_invalidUsernames.push_back("rw");
    TpmCryptSubject_invalidUsernames.push_back("sync");
    TpmCryptSubject_invalidUsernames.push_back("user");
    TpmCryptSubject_invalidUsernames.push_back("nouser");
    TpmCryptSubject_invalidUsernames.push_back("users");

    if (mySubjectName.empty())
        throw InvalidUsername("TpmCryptSubject: The supplied subjectname was empty!");
    if (mySubjectName.rfind("Group_") != string::npos)
        throw InvalidUsername("TpmCryptSubject: The supplied subjectname contained invalid substring 'Group_'!");
    if (mySubjectName.rfind("Platform_") != string::npos)
        throw InvalidUsername("TpmCryptSubject: The supplied subjectname contained invalid substring 'Platform_'!");
    if (mySubjectName.rfind("Token_") != string::npos)
        throw InvalidUsername("TpmCryptSubject: The supplied subjectname contained invalid substring 'Token_'!");

    vector<string>::const_iterator Iterator;
    if (TpmCryptSubject_invalidUsernames.size())
    {
        Iterator=TpmCryptSubject_invalidUsernames.begin();
        while ( Iterator != TpmCryptSubject_invalidUsernames.end())
        {
            if (mySubjectName == *(Iterator))
                throw InvalidUsername("TpmCryptSubject: The supplied subjectname was not allowed, since it is used by the Linux 'mount'-command!");
            else
                Iterator++;
        }
    }
};
