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
 * @file        KeyronaSubject.cxx
 * @brief       Implementation of Keyronas subject class.
 * @version     $Revision: 764 $
 * @date        Last modification at $Date: 2009-07-29 15:33:10 +0200 (Wed, 29 Jul 2009) $ by $Author: selhorst $
 */

#include <KeyronaSubject.hxx>
#include <KeyronaTPM.hxx>


using namespace std;
using namespace utils;


//================================================================================
//
// load subject by string
KeyronaSubject::KeyronaSubject( string subjectIdentifier, KeyronaStorage &storageDB ) :
            mySubjectType (SUBJECTTYPE_UNDEFINED),
            mySubjectID (0),
            mySubjectIDString(""),
            mySubjectName (""),
            mySubjectKeyUUID (""),
            mySubjectKeyType (""),
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
    debug << "KeyronaSubject|Constructor()]: Loading Subject by subjectID string '" << mySubjectIDString << "'." << endl;

    loadSubject(subjectIdentifier);
};

//================================================================================
//
// load subject by subjectID
KeyronaSubject::KeyronaSubject( UInt32 subjectID, KeyronaStorage &storageDB ) :
            mySubjectType (SUBJECTTYPE_UNDEFINED),
            mySubjectID (0),
            mySubjectIDString(""),
            mySubjectName (""),
            mySubjectKeyUUID (""),
            mySubjectKeyType (""),
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
    debug << "KeyronaSubject|Constructor()]: Loading Subject by ID '" << mySubjectIdentifierString << "'." << endl;

    loadSubject(mySubjectIdentifierString);
};

//================================================================================
//
// create new subject incl. key generation
KeyronaSubject::KeyronaSubject( UInt8 subjectType,
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
                            KeyronaStorage &storageDB ) :
            mySubjectType (subjectType),
            mySubjectID (0),
            mySubjectIDString(""),
            mySubjectName (subjectName),
            mySubjectKeyUUID (""),
            mySubjectKeyType (""),
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
        throw InvalidSubjectType("KeyronaSubject: Subject Type was invalid!");

    if (mySubjectType == SUBJECTTYPE_PLATFORM)
        mySubjectName = "Platform_" + mySubjectName;

    if (mySubjectType == SUBJECTTYPE_TOKEN)
        mySubjectName = "Token_" + mySubjectName;

    if (keyDirectory.empty())
        throw NoFilename("KeyronaSubject: The supplied key directory was empty!");

    if (!dirExists(keyDirectory))
        throw DirNotExist("KeyronaSubject: The supplied key directory does not exist!");

    if (!dirWritable(keyDirectory))
        throw DirNotWritable("KeyronaSubject: The supplied key directory is not writable!");
	
	// Initialize RNG
    srand(time(NULL));

    // generate new random subjectID
    mySubjectKeyUUID = convertUInt32toString((rand() % 100000 + 1));
	if (mySubjectKeyUUID.empty()) 
		throw InvalidKeyUUID("KeyronaSubject: The supplied Key UUID was invalid");
		
    mySubjectKeyType = "subject";

    string myKeyDirectory = keyDirectory;

    if  (
        (mySubjectEMail.empty()) |
        (mySubjectEMail.find("@") == string::npos ) |
        (mySubjectEMail.find(".") == string::npos )
        )
            throw InvalidEMail("KeyronaSubject: The supplied eMail address was invalid!");

    if (mySubjectCountrycode.empty())
        throw InvalidCountrycode("KeyronaSubject: The supplied countrycode was empty!");

    if (mySubjectCountrycode.length() != 2)
        throw InvalidCountrycode("KeyronaSubject: The supplied countrycode was invalid!");

    mySubjectCountrycode[0] = toupper(mySubjectCountrycode[0]);
    mySubjectCountrycode[1] = toupper(mySubjectCountrycode[1]);

    if (mySubjectOrganisation.empty())
        throw InvalidOrganisation("KeyronaSubject: The supplied organisation was empty!");

    if (mySubjectOrganisationUnit.empty())
        throw InvalidOrganisationUnit("KeyronaSubject: The supplied organisation unit was empty!");

    if (mySubjectState.empty())
        throw InvalidState("KeyronaSubject: The supplied state/province was empty!");

    if (mySubjectLocality.empty())
        throw InvalidLocality("KeyronaSubject: The supplied locality was empty!");


    // Initialize RNG
    srand(time(NULL));

    // generate new random subjectID
    do
    {
        mySubjectID = rand();
        mySubjectIDString = convertUInt32toString(mySubjectID);
    }
    while (storageDB.findSection(mySubjectIDString));

    vector<StringPair> myUsers = storageDB.findAllEntries(KeyronaSubject_SubjectName, mySubjectName);
    if (myUsers.size())
        throw InvalidUsername("KeyronaSubject: The supplied subjectname is already in use!");

    debug << "KeyronaSubject|Constructor(): Creating new SubjectID '" << mySubjectID << "'." << endl;

    string myPassword;
    switch(mySubjectType)
    {
        case SUBJECTTYPE_USER:
        {
            mySubjectKeyfile = myKeyDirectory + KeyronaPathSeparator + "Keyrona" + KeyronaSubjectType_User + KeyronaFileSeparator + mySubjectIDString  + KeyronaFileSeparator + mySubjectName + KeyronaP15FileExtension;
            if (mySubjectisAdmin) 
                myPassword = getPasswordVerified("Please enter passphrase for new admin '" + mySubjectName + "': ");
            else 
                myPassword = getPasswordVerified("Please enter passphrase for new user '" + mySubjectName + "': ");
                
                mySubjectKey = new KeyronaKey(this, myPassword);
            break;
        }
        case SUBJECTTYPE_TOKEN:
        {
            mySubjectKeyfile = myKeyDirectory + KeyronaPathSeparator + "Keyrona" + KeyronaSubjectType_Token + KeyronaFileSeparator + mySubjectIDString  + KeyronaFileSeparator + mySubjectName + KeyronaP15FileExtension;
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
                myPassword = KEYRONA_TOKEN_STANDARD_PASSWD;

            break;
        }
        case SUBJECTTYPE_PLATFORM:
        {
            mySubjectKeyfile = myKeyDirectory + KeyronaPathSeparator + "Keyrona" + KeyronaSubjectType_Platform + KeyronaFileSeparator + mySubjectIDString  + KeyronaFileSeparator + mySubjectName + KeyronaP15FileExtension;
			break;
        }
    }
    storeSubject();
};

//================================================================================
//
// import subject without key generation
KeyronaSubject::KeyronaSubject( UInt8 subjectType,
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
                            KeyronaStorage &storageDB ) :
            mySubjectType (subjectType),
            mySubjectID (0),
            mySubjectIDString(""),
            mySubjectName (subjectName),
            mySubjectKeyUUID (""),
            mySubjectKeyType (""),  
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
        throw InvalidSubjectType("KeyronaSubject: Subject Type was invalid!");

    if (keyDirectory.empty())
        throw NoFilename("KeyronaSubject: The supplied key directory was empty!");

    if (!dirExists(keyDirectory))
        throw DirNotExist("KeyronaSubject: The supplied key directory does not exist!");

    if (!dirWritable(keyDirectory))
        throw DirNotWritable("KeyronaSubject: The supplied key directory is not writable!");
    
    string myKeyDirectory = keyDirectory;

    if  (
        (mySubjectEMail.empty()) |
        (mySubjectEMail.find("@") == string::npos ) |
        (mySubjectEMail.find(".") == string::npos )
        )
            throw InvalidEMail("KeyronaSubject: The supplied eMail address was invalid!");

    if (mySubjectCountrycode.empty())
        throw InvalidCountrycode("KeyronaSubject: The supplied countrycode was empty!");

    if (mySubjectCountrycode.length() != 2)
        throw InvalidCountrycode("KeyronaSubject: The supplied countrycode was invalid!");

    mySubjectCountrycode[0] = toupper(mySubjectCountrycode[0]);
    mySubjectCountrycode[1] = toupper(mySubjectCountrycode[1]);

    if (mySubjectOrganisation.empty())
        throw InvalidOrganisation("KeyronaSubject: The supplied organisation was empty!");

    if (mySubjectOrganisationUnit.empty())
        throw InvalidOrganisationUnit("KeyronaSubject: The supplied organisation unit was empty!");

    if (mySubjectState.empty())
        throw InvalidState("KeyronaSubject: The supplied state/province was empty!");

    if (mySubjectLocality.empty())
        throw InvalidLocality("KeyronaSubject: The supplied locality was empty!");


    // Initialize RNG
    srand(time(NULL));

    // generate new random subjectID
    do
    {
        mySubjectID = rand();
        mySubjectIDString = convertUInt32toString(mySubjectID);
    }
    while (storageDB.findSection(mySubjectIDString));

    vector<StringPair> myUsers = storageDB.findAllEntries(KeyronaSubject_SubjectName, mySubjectName);
    if (myUsers.size())
        throw InvalidUsername("KeyronaSubject: The supplied subjectname is already in use!");

    debug << "KeyronaSubject|Constructor()]: Creating new SubjectID '" << mySubjectID << "'." << endl;

    if (copyKeyfile)
    {
        debug << "KeyronaSubject|Constructor()]: Copying keyfile '" << subjectKeyfile << "' to '" << mySubjectKeyfile << "'" << endl;
        mySubjectKeyfile = myKeyDirectory + KeyronaPathSeparator + mySubjectIDString  + KeyronaFileSeparator + mySubjectName + KeyronaP15FileExtension;
        string myCommand = "/bin/cp \"" + subjectKeyfile + "\" \"" + mySubjectKeyfile + "\"";
        if (system(myCommand.c_str()))
            throw RuntimeError("KeyronaSubject(): Could not copy keyfile '" + subjectKeyfile + "' to '" + mySubjectKeyfile + "'");
    }
    else
    {
        debug << "KeyronaSubject|Constructor()]: Not copying keyfile '" << subjectKeyfile  << endl;
        mySubjectKeyfile = subjectKeyfile;
    }

    storeSubject();
};

//================================================================================
//
KeyronaSubject::~KeyronaSubject()
{
    debug << "KeyronaSubject|Destructor()]: Destructor for subject '" << mySubjectName << "' called." << endl;
    if (mySubjectKey)
        delete mySubjectKey;
};

//================================================================================
//
void KeyronaSubject::printSubject(bool verbose)
{
    typedef map<int, string> t_StringMap;
    t_StringMap KeyronaSubjectType;
    KeyronaSubjectType[SUBJECTTYPE_UNDEFINED]       = KeyronaSubjectType_Undefined;
    KeyronaSubjectType[SUBJECTTYPE_USER]            = KeyronaSubjectType_User;
    KeyronaSubjectType[SUBJECTTYPE_PLATFORM]        = KeyronaSubjectType_Platform;
    KeyronaSubjectType[SUBJECTTYPE_TOKEN]           = KeyronaSubjectType_Token;
    KeyronaDate myLastLogin = getMyLastLogin();

    // and here the full info
    cout << "Name: '" << mySubjectName << "' (ID: " << mySubjectID << ")" << endl;
    cout << "KeyUUID: '" << mySubjectKeyUUID << endl;
    cout << "KeyType: '" << mySubjectKeyType << endl;
    cout << mySubjectKey->printKeyInformation(this) << endl;
    cout << "\t" << "Type: '" << KeyronaSubjectType[mySubjectType] << "'" << endl;
	
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
        cout << "\t" << "Subject is admin: '" << (mySubjectisAdmin ? KeyronaSubject_TRUE:KeyronaSubject_FALSE) << "'" << endl;

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
bool KeyronaSubject::loadSubject(string &subjectIdentifier)
{
    debug << "KeyronaSubject|loadSubject()]: Loading Subject from DB..." << endl;

    typedef map<string, int> t_StringMap;
    t_StringMap KeyronaSubjectType;
    KeyronaSubjectType[KeyronaSubjectType_Undefined]       = SUBJECTTYPE_UNDEFINED;
    KeyronaSubjectType[KeyronaSubjectType_User]            = SUBJECTTYPE_USER;
    KeyronaSubjectType[KeyronaSubjectType_Platform]        = SUBJECTTYPE_PLATFORM;
    KeyronaSubjectType[KeyronaSubjectType_Token]           = SUBJECTTYPE_TOKEN;

    if (subjectIdentifier.empty())
        throw InvalidUsername("KeyronaSubject: The supplied subjectname was empty!");

    // first, try to identify subject by name
    vector<StringPair> mySection = mySubjectStorage.findAllEntries(KeyronaSubject_SubjectName, subjectIdentifier);
    if (mySection.size())
    {
        debug << "KeyronaSubject: Trying to load subject by name: '" << subjectIdentifier << "'" << endl;
        StringPair mySectionResult;
        mySectionResult = mySection.back();
        mySubjectIDString = mySectionResult.first;
        mySubjectID = convertStringtoUInt32(mySubjectIDString);
    }
    // if it fails, try to load by ID
    else
    {
        debug << "KeyronaSubject: Trying to load subject by ID: '" << subjectIdentifier << "'" << endl;
        if (! mySubjectStorage.findSection(subjectIdentifier))
            throw UnknownSubject("KeyronaSubject: Subject '" + subjectIdentifier + "' not found!");
        mySubjectIDString = subjectIdentifier;
        // convert given SubjectID to a string
        mySubjectID = convertStringtoUInt32(mySubjectIDString);
    }

    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.selectSubSection(KeyronaSubject_SubjectProperties);
    mySubjectName = mySubjectStorage.getEntry(KeyronaSubject_SubjectName);
    mySubjectKeyUUID = mySubjectStorage.getEntry(KeyronaSubject_SubjectKeyUUID);
    mySubjectKeyType = mySubjectStorage.getEntry(KeyronaSubject_SubjectKeyType);
    mySubjectType = KeyronaSubjectType[mySubjectStorage.getEntry(KeyronaSubject_SubjectType)];
    mySubjectPlatformSelection = mySubjectStorage.getEntry(KeyronaSubject_SubjectPlatformSelection);
    mySubjectEMail = mySubjectStorage.getEntry(KeyronaSubject_SubjectEMail);
    mySubjectCountrycode = mySubjectStorage.getEntry(KeyronaSubject_SubjectCountrycode);
    mySubjectOrganisation = mySubjectStorage.getEntry(KeyronaSubject_SubjectOrganisation);
    mySubjectOrganisationUnit = mySubjectStorage.getEntry(KeyronaSubject_SubjectOrganisationUnit);
    mySubjectState = mySubjectStorage.getEntry(KeyronaSubject_SubjectState);
    mySubjectLocality = mySubjectStorage.getEntry(KeyronaSubject_SubjectLocality);
    mySubjectKeyfile = mySubjectStorage.getEntry(KeyronaSubject_SubjectKeyfile);
    mySubjectisAdmin = ! mySubjectStorage.getEntry(KeyronaSubject_SubjectAdmin).compare(KeyronaSubject_TRUE);

    return true;
};

//================================================================================
//
bool KeyronaSubject::storeSubject()
{
    debug << "KeyronaSubject|storeSubject()]: Storing Subject to DB..." << endl;

    typedef map<int, string> t_StringMap;
    t_StringMap KeyronaSubjectType;
    KeyronaSubjectType[SUBJECTTYPE_UNDEFINED]       = KeyronaSubjectType_Undefined;
    KeyronaSubjectType[SUBJECTTYPE_USER]            = KeyronaSubjectType_User;
    KeyronaSubjectType[SUBJECTTYPE_PLATFORM]        = KeyronaSubjectType_Platform;
    KeyronaSubjectType[SUBJECTTYPE_TOKEN]           = KeyronaSubjectType_Token;

    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.selectSubSection(KeyronaSubject_SubjectProperties);
    mySubjectStorage.setEntry(KeyronaSubject_isSubject, KeyronaSubject_isSubject_true);
    mySubjectStorage.setEntry(KeyronaSubject_SubjectName, mySubjectName);
    mySubjectStorage.setEntry(KeyronaSubject_SubjectKeyUUID, mySubjectKeyUUID);
    mySubjectStorage.setEntry(KeyronaSubject_SubjectKeyType, mySubjectKeyType);
    mySubjectStorage.setEntry(KeyronaSubject_SubjectPlatformSelection, mySubjectPlatformSelection);
    mySubjectStorage.setEntry(KeyronaSubject_SubjectType, KeyronaSubjectType[mySubjectType]);
    mySubjectStorage.setEntry(KeyronaSubject_SubjectEMail, mySubjectEMail);
    mySubjectStorage.setEntry(KeyronaSubject_SubjectKeyfile, mySubjectKeyfile);
    mySubjectStorage.setEntry(KeyronaSubject_SubjectCountrycode, mySubjectCountrycode);
    mySubjectStorage.setEntry(KeyronaSubject_SubjectOrganisation, mySubjectOrganisation);
    mySubjectStorage.setEntry(KeyronaSubject_SubjectOrganisationUnit, mySubjectOrganisationUnit);
    mySubjectStorage.setEntry(KeyronaSubject_SubjectState, mySubjectState);
    mySubjectStorage.setEntry(KeyronaSubject_SubjectLocality, mySubjectLocality);
    mySubjectStorage.setEntry(KeyronaSubject_SubjectAdmin, mySubjectisAdmin ? KeyronaSubject_TRUE : KeyronaSubject_FALSE);

    mySubjectStorage.selectSubSection(KeyronaSubject_PasswordSection);
    if (mySubjectType == SUBJECTTYPE_USER) {
	ByteVector passwordMagicVector;
    // creating password magic test vector
    passwordMagicVector = convertStringToByteVector(mySubjectIDString);
    ByteVector passwordMagicVectorByteVector = encryptForSubject(this, passwordMagicVector);
    string myBase64EncodedPasswordMagicVector = EncodeByteVectorToBASE64(passwordMagicVectorByteVector);
    mySubjectStorage.setEntry(KeyronaSubject_PasswordMagicEntry, myBase64EncodedPasswordMagicVector);
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
void KeyronaSubject::deleteSubject()
{
    debug << "KeyronaSubject|deleteSubject()]: Deleting Subject from DB..." << endl;
    mySubjectType = SUBJECTTYPE_UNDEFINED;
    mySubjectID = 0;
    mySubjectName.clear();
    mySubjectKeyUUID.clear();
    mySubjectKeyType.clear();
    mySubjectEMail.clear();
    mySubjectKeyfile.clear();
    mySubjectCountrycode.clear();
    mySubjectisAdmin = false;
    mySubjectStorage.deleteSection(mySubjectIDString);
    mySubjectIDString.clear();
};

//================================================================================
//
KeyronaDate KeyronaSubject::getMyLastLogin()
{
    string tempString;
    vector<UInt32> myTempDate;
    myTempDate.clear();

    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.selectSubSection(KeyronaSubjectLastLogin);

    tempString = mySubjectStorage.getEntry(KeyronaSubjectLastLogin_Year);
    myTempDate.push_back(convertStringtoUInt32(tempString));
    tempString = mySubjectStorage.getEntry(KeyronaSubjectLastLogin_Month);
    myTempDate.push_back(convertStringtoUInt32(tempString));
    tempString = mySubjectStorage.getEntry(KeyronaSubjectLastLogin_Day);
    myTempDate.push_back(convertStringtoUInt32(tempString));
    tempString = mySubjectStorage.getEntry(KeyronaSubjectLastLogin_Hour);
    myTempDate.push_back(convertStringtoUInt32(tempString));
    tempString = mySubjectStorage.getEntry(KeyronaSubjectLastLogin_Minute);
    myTempDate.push_back(convertStringtoUInt32(tempString));
    tempString = mySubjectStorage.getEntry(KeyronaSubjectLastLogin_Second);
    myTempDate.push_back(convertStringtoUInt32(tempString));

    try
    {
        KeyronaDate myDate(myTempDate);
        debug << "KeyronaSubject|getMyLastLogin(): Successfully read last login time" << endl;
        return myDate;
    }
    catch ( std::exception &e )
    {
        debug << "KeyronaSubject|getMyLastLogin(): User has not logged in, yet." << endl;
        KeyronaDate myDate(false);
        return myDate;
    }
};

//================================================================================
//
void KeyronaSubject::setLastLogin(KeyronaDate &LoginDate)
{
    vector<UInt32> myDate = LoginDate.getDate();
    UInt32 temp;
    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.selectSubSection(KeyronaSubjectLastLogin);

    // store second
    temp = myDate.back();
    myDate.pop_back();
    mySubjectStorage.setEntry(KeyronaSubjectLastLogin_Second, convertUInt32toString(temp));

    // store minute
    temp = myDate.back();
    myDate.pop_back();
    mySubjectStorage.setEntry(KeyronaSubjectLastLogin_Minute, convertUInt32toString(temp));

    // store hour
    temp = myDate.back();
    myDate.pop_back();
    mySubjectStorage.setEntry(KeyronaSubjectLastLogin_Hour, convertUInt32toString(temp));

    // store day
    temp = myDate.back();
    myDate.pop_back();
    mySubjectStorage.setEntry(KeyronaSubjectLastLogin_Day, convertUInt32toString(temp));

    // store month
    temp = myDate.back();
    myDate.pop_back();
    mySubjectStorage.setEntry(KeyronaSubjectLastLogin_Month, convertUInt32toString(temp));

    // store year
    temp = myDate.back();
    myDate.pop_back();
    mySubjectStorage.setEntry(KeyronaSubjectLastLogin_Year, convertUInt32toString(temp));
};

//================================================================================
//   
ByteVector  KeyronaSubject::encryptForSubject(KeyronaSubject *Subject, ByteVector &toEncrypt)
{
    if (! toEncrypt.size())
       throw DecryptionFailed("KeyonaSubject|encryptForSubject(): No data supplied to be encrypted!");
	
	if (mySubjectType == SUBJECTTYPE_PLATFORM)
			{	
			vector<int> pcr = convertStringToIntVector(mySubjectPlatformSelection);
			int local=0;
            // sealing it to current platform
            KeyronaTPM myTPM;
            vector<ByteVector> mySealedDataWithKey = myTPM.seal(toEncrypt,local,pcr); // XXX see kyronatpm.hxx
			
            // storing sealed data
            ByteVector Data = mySealedDataWithKey.back();
            mySealedDataWithKey.pop_back();
            /*string DataFile = mySubjectKeyfile + KEYRONA_TPM_DATA_EXTENSION;
            storeByteVectorInFile(DataFile, Data);

            // storing sealing key
            ByteVector Key = mySealedDataWithKey.back();
            mySealedDataWithKey.pop_back();
            string KeyFile = mySubjectKeyfile + KEYRONA_TPM_KEY_EXTENSION;
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
ByteVector  KeyronaSubject::decryptBySubject(KeyronaSubject *Subject, ByteVector &toDecrypt, string &myPassword)
{
  string password;
    if (mySubjectType == SUBJECTTYPE_PLATFORM)
    {
        KeyronaTPM myTPM;
        vector<ByteVector> mySealedDataWithKey;
		mySealedDataWithKey.push_back(toDecrypt);
		/*// Loading sealing key
            string KeyFile = mySubjectKeyfile + KEYRONA_TPM_KEY_EXTENSION;
            ByteVector Key = loadByteVectorFromFile(KeyFile);
            mySealedDataWithKey.push_back(Key);

            // Loading sealed data
            string DataFile = mySubjectKeyfile + KEYRONA_TPM_DATA_EXTENSION;
            ByteVector Data = loadByteVectorFromFile(DataFile);
            mySealedDataWithKey.push_back(Data);*/

            ByteVector myPlainPassword = myTPM.unseal(mySealedDataWithKey);
            mySealedDataWithKey.pop_back();

            return myPlainPassword;
/*
            string x( (const char*) myPlainPassword.toCArray(), myPlainPassword.size() );
            //if (!verifyPassword(x))
             //  throw InvalidPassword("KeyronaSubject|decryptBySubject(): Could not retrieve the password from TPM!");
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
    KeyronaDate myCurrentSystemTime;
    setLastLogin(myCurrentSystemTime);

    //if ((mySubjectKey) && (mySubjectKey->isValid()))
        return mySubjectKey->decrypt(Subject , NULL, toDecrypt, password);
    //throw InvalidKey("KeyonaSubject|decryptBySubject(): Invalid key for subject '" + mySubjectName + "'");
}

};

//================================================================================
//
bool KeyronaSubject::verifyPassword(string &myPassword)
{
    debug << "KeyronaSubject["+mySubjectIDString+"|verifyPassword()]: verifying password..." << endl;
    if (myPassword.empty())
        throw InvalidPassword("KeyonaSubject|verifyPassword(): Invalid password for subject '" + mySubjectName + "'");
    
    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.selectSubSection(KeyronaSubject_PasswordSection);
    string myBase64EncodedPasswordMagicVector = mySubjectStorage.getEntry(KeyronaSubject_PasswordMagicEntry);

    if (myBase64EncodedPasswordMagicVector.empty())
        throw NoPasswordMagic("KeyonaSubject|verifyPassword(): No password magic available for subject '" + mySubjectName + "'. Please delete and re-create user!");

    string myPasswordMagic;
    ByteVector myPasswordMagicVectorByteVector;
    // decode password magic
    ByteVector myPasswordMagicVector = DecodeBASE64StringToByteVector(myBase64EncodedPasswordMagicVector);

    debug << "KeyronaSubject["+mySubjectIDString+"|verifyPassword()]: decrypting testvector from database with given password" << endl;
    // decrypted password magic
    try
    {
        myPasswordMagicVectorByteVector = mySubjectKey->decrypt(this, NULL, myPasswordMagicVector, myPassword);
    }
    catch ( std::exception &e )
    {
        throw DecryptionFailed("KeyronaSubject|verifyPassword(): Invalid Password!");
    }
    myPasswordMagic = convertByteVector2String(myPasswordMagicVectorByteVector);
    if (myPasswordMagic == mySubjectIDString)
        return true;
    else
        return false;
};

//================================================================================
//
bool KeyronaSubject::changePassword( string &oldPassword, string &newPassword)
{
    //if (!verifyPassword(oldPassword))
     //   throw InvalidPassword("KeyronaSubject|changePassword(): Invalid (old) password supplied!");

    if (! newPassword.size())
        throw InvalidPassword("KeyronaSubject|changePassword(): Empty (new) password supplied!");

    //if ((mySubjectKey) && (mySubjectKey->isValid()))
        return mySubjectKey->changePassword(this, oldPassword, newPassword);

    //throw InvalidKey("KeyonaSubject|changePassword(): Invalid key for subject '" + mySubjectName + "'");
};

//================================================================================
//
void KeyronaSubject::addMessageForUser(string &message)
{
    if (message.empty())
        throw NoMessage("KeyronaSubject|addMessageForUser(): No message supplied!");

    if (mySubjectType != SUBJECTTYPE_USER)
    {
        debug << "KeyronaSubject|addMessageForUser(): Messages can only be added for users!";
        printToSyslog("KEYRONA: Subject '" + mySubjectName + "': Received message => '" + message + "'");
        return;
    }

    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.selectSubSection(KeyronaSubject_Messages);
    KeyronaDate currentDate;
    string currentTime = currentDate.getDateString();
    mySubjectStorage.setEntry(currentTime, message);
    printToSyslog("KEYRONA: Subject '" + mySubjectName + "': Received message => '" + message + "'");
};

//================================================================================
//
void KeyronaSubject::printCurrentMessagesForUser()
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
vector<StringPair> KeyronaSubject::getCurrentMessagesForUser()
{
    if (mySubjectType != SUBJECTTYPE_USER)
    {
        debug << "KeyronaSubject|printCurrentMessagesForUser(): Messages are only available for users!" << endl;
        vector<StringPair> nothing;
        return nothing;
    }
    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.selectSubSection(KeyronaSubject_Messages);
    return mySubjectStorage.getAllEntriesWithValues();
};

//================================================================================
//
void KeyronaSubject::deleteUserMessages()
{
    mySubjectStorage.selectSection(mySubjectIDString);
    mySubjectStorage.deleteSubSection(KeyronaSubject_Messages);
};

//================================================================================
//
void KeyronaSubject::checkUsername()
{
    std::vector<std::string> KeyronaSubject_invalidUsernames;
    KeyronaSubject_invalidUsernames.push_back("async");
    KeyronaSubject_invalidUsernames.push_back("atime");
    KeyronaSubject_invalidUsernames.push_back("noatime");
    KeyronaSubject_invalidUsernames.push_back("auto");
    KeyronaSubject_invalidUsernames.push_back("noauto");
    KeyronaSubject_invalidUsernames.push_back("context");
    KeyronaSubject_invalidUsernames.push_back("defaults");
    KeyronaSubject_invalidUsernames.push_back("dev");
    KeyronaSubject_invalidUsernames.push_back("nodev");
    KeyronaSubject_invalidUsernames.push_back("diratime");
    KeyronaSubject_invalidUsernames.push_back("nodiratime");
    KeyronaSubject_invalidUsernames.push_back("dirsync");
    KeyronaSubject_invalidUsernames.push_back("exec");
    KeyronaSubject_invalidUsernames.push_back("noexec");
    KeyronaSubject_invalidUsernames.push_back("group");
    KeyronaSubject_invalidUsernames.push_back("iversion");
    KeyronaSubject_invalidUsernames.push_back("noiversion");
    KeyronaSubject_invalidUsernames.push_back("mand");
    KeyronaSubject_invalidUsernames.push_back("nomand");
    KeyronaSubject_invalidUsernames.push_back("_netdev");
    KeyronaSubject_invalidUsernames.push_back("nofail");
    KeyronaSubject_invalidUsernames.push_back("relatime");
    KeyronaSubject_invalidUsernames.push_back("norelatime");
    KeyronaSubject_invalidUsernames.push_back("strictatime");
    KeyronaSubject_invalidUsernames.push_back("nostrictatime");
    KeyronaSubject_invalidUsernames.push_back("suid");
    KeyronaSubject_invalidUsernames.push_back("nosuid");
    KeyronaSubject_invalidUsernames.push_back("owner");
    KeyronaSubject_invalidUsernames.push_back("remount");
    KeyronaSubject_invalidUsernames.push_back("ro");
    KeyronaSubject_invalidUsernames.push_back("rw");
    KeyronaSubject_invalidUsernames.push_back("sync");
    KeyronaSubject_invalidUsernames.push_back("user");
    KeyronaSubject_invalidUsernames.push_back("nouser");
    KeyronaSubject_invalidUsernames.push_back("users");

    if (mySubjectName.empty())
        throw InvalidUsername("KeyronaSubject: The supplied subjectname was empty!");
    if (mySubjectName.rfind("Group_") != string::npos)
        throw InvalidUsername("KeyronaSubject: The supplied subjectname contained invalid substring 'Group_'!");
    if (mySubjectName.rfind("Platform_") != string::npos)
        throw InvalidUsername("KeyronaSubject: The supplied subjectname contained invalid substring 'Platform_'!");
    if (mySubjectName.rfind("Token_") != string::npos)
        throw InvalidUsername("KeyronaSubject: The supplied subjectname contained invalid substring 'Token_'!");

    vector<string>::const_iterator Iterator;
    if (KeyronaSubject_invalidUsernames.size())
    {
        Iterator=KeyronaSubject_invalidUsernames.begin();
        while ( Iterator != KeyronaSubject_invalidUsernames.end())
        {
            if (mySubjectName == *(Iterator))
                throw InvalidUsername("KeyronaSubject: The supplied subjectname was not allowed, since it is used by the Linux 'mount'-command!");
            else
                Iterator++;
        }
    }
};
