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
 * @file        KeyronaKey.cxx
 * @brief       Implementation of the Keyrona Key class.
 * @version     $Revision: 449 $
 * @date        Last modification at $Date: 2009-04-06 00:16:17 +0200 (Mon, 06 Apr 2009) $ by $Author: selhorst $
 */

#include <KeyronaKey.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

static UInt32 KeyCounter = 0;

// create a new key
//================================================================================
//
KeyronaKey::KeyronaKey(KeyronaSubject *Subject, string &password) :
valid (false),
keyfile (""),
keynum (""),
type ("")
{
	if (!Subject)
        throw InvalidSubjectID("KeyronaKey|Constructor(): Invalid Subject pointer given!");

    if (Subject->getMySubjectKeyfile().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
    else
        keyfile = Subject->getMySubjectKeyfile();
        
    if (Subject->getMySubjectKeyUUID().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
    else
        keynum = Subject->getMySubjectKeyUUID();
        
    if (Subject->getMySubjectKeyType().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
    else
        type = Subject->getMySubjectKeyType();
        	
	KeyronaTPM;
	vector<ByteVector> keyinfo = myTPM.create_key(password, keynum, type);
	
    // data
    ByteVector Data = keyinfo.back();
    keyinfo.pop_back();
    string DataFile = keyfile + keynum + KEYRONA_TPM_KEY_EXTENSION;
    storeByteVectorInFile(DataFile, Data);
    
    valid = true;
    KeyCounter++;
};

//================================================================================
//
KeyronaKey::KeyronaKey(KeyronaGroup *Group, string &groupKeyPassword) :
valid (false),
myCryptKeysetFile (""),
myCryptKeyLabel ("")
{
    CRYPT_CONTEXT   cryptContext;
    CRYPT_KEYSET    cryptKeyset;

    if (!Group)
        throw InvalidGroup("KeyronaKey|Group-Constructor(): Invalid Group pointer given!");

    if (!KeyCounter)
    {
        // initialize cryptlib
        debug << "KeyronaKey|Group-Constructor(): Initializing cryptlib" << endl;
        cryptlibCheck(cryptInit(), "Initialization failed!");
    }

    if (groupKeyPassword.empty())
        throw InvalidPassword("KeyronaKey|Group-Constructor(): Invalid password supplied!");

    if (Group->getMyGroupKeyfile().empty())
        throw NoFilename("KeyronaKey|Group-Constructor(): The supplied key filename was empty!");
    else
        myCryptKeysetFile = Group->getMyGroupKeyfile();

    if (Group->getMyGroupID().empty())
        throw InvalidGroup("KeyronaKey|Group-Constructor(): The supplied groupID was empty!");
    else
        myCryptKeyLabel = Group->getMyGroupID();

    cout << "Creating new key with label '" << Group->getMyGroupID() << "'" << endl;

    debug << "KeyronaKey|Group-Constructor(): Creating crypto context" << endl;
    cryptlibCheck(cryptCreateContext( &cryptContext, CRYPT_UNUSED, CRYPT_ALGO_RSA ), "Creating crypto context");

    debug << "KeyronaKey|Group-Constructor(): Setting crypto label to '" << myCryptKeyLabel << "'" << endl;
    cryptlibCheck(cryptSetAttributeString( cryptContext, CRYPT_CTXINFO_LABEL, myCryptKeyLabel.c_str(), myCryptKeyLabel.length()), "Setting crypto label to '" + myCryptKeyLabel + "'");

    debug << "KeyronaKey|Group-Constructor(): Setting keylength to '" << RSA_2048_BIT << "'" << endl;
    cryptlibCheck(cryptSetAttribute( cryptContext, CRYPT_CTXINFO_KEYSIZE, RSA_2048_BIT), "Setting keylength to '" + convertUInt32toString(RSA_2048_BIT) + "'");

    debug << "KeyronaKey|Group-Constructor(): Generating new RSA key" << endl;
    cryptlibCheck(cryptGenerateKey( cryptContext ), "Generating new RSA key");

    // Storing Key 
    debug << "KeyronaKey|Group-Constructor(): Opening keyset with file '" << myCryptKeysetFile << "'" << endl;
    cryptlibCheck(cryptKeysetOpen( &cryptKeyset, CRYPT_UNUSED, CRYPT_KEYSET_FILE, myCryptKeysetFile.c_str(), CRYPT_KEYOPT_CREATE ), "Opening keyset with file '" + myCryptKeysetFile + "'");

    debug << "KeyronaKey|Group-Constructor(): Adding private key to keyset" << endl;
    cryptlibCheck(cryptAddPrivateKey( cryptKeyset, cryptContext, groupKeyPassword.c_str() ), "Adding private key to keyset");

    debug << "KeyronaKey|Group-Constructor(): Closing keyfile" << endl;
    cryptlibCheck(cryptKeysetClose( cryptKeyset ), "Closing keyfile");

    debug << "KeyronaKey|Group-Constructor(): Destroying context" << endl;
    cryptlibCheck(cryptDestroyContext( cryptContext ), "Destroying context");

    valid = true;
    KeyCounter++;
};


//================================================================================
//
KeyronaKey::~KeyronaKey()
{
    if (KeyCounter == 1)
    {
        // garbage collection and destroying of cryptlib objects
        debug << "KeyronaKey|Destructor(): Cleaning up cryptlib..." << endl;
//        cryptlibCheck(cryptEnd(), "Cryptlib cleanup process failed!");
    }
    else
        debug << "KeyronaKey|Destructor(): Not finishing cryptlib, since we still have " << KeyCounter << " open objects!" << endl;

    KeyCounter--;
};

//================================================================================
//
vector<ByteVector>  KeyronaKey::encrypt(KeyronaSubject *Subject, ByteVector &toEncrypt) :
keynum ("")
{
	if (Subject->getMySubjectKeyUUID().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
    else
        keynum = Subject->getMySubjectKeyUUID();
        
   	KeyronaTPM;
	vector<ByteVector> mySealedDataWithKey = myTPM.bind(toEncrypt, keynum);
    ByteVector result = mySealedDataWithKey.back();
    mySealedDataWithKey.pop_back();
    
    return result;
};

//================================================================================
//
ByteVector  KeyronaKey::decrypt(KeyronaSubject *Subject, vector<ByteVector> &toDecrypt, string &myPassword) :
keynum ("")
{
	if (Subject->getMySubjectKeyUUID().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
    else
        keynum = Subject->getMySubjectKeyUUID();	
	
	KeyronaTPM;
	vector<ByteVector> mySealedDataWithKey = myTPM.unbind(toDecrypt, keynum, myPassword);
    ByteVector result = mySealedDataWithKey.back();
    mySealedDataWithKey.pop_back();

    return result;
};

//================================================================================
//
void KeyronaKey::printKeyInformation(KeyronaSubject *Subject) :
keyfile (""),
keynum (""),
{
    if (Subject->getMySubjectKeyfile().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
    else
        keyfile = Subject->getMySubjectKeyfile();	
	
	if (Subject->getMySubjectKeyUUID().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
    else
        keynum = Subject->getMySubjectKeyUUID();
	
	 string KeyFile = keyfile + keynum + KEYRONA_TPM_KEY_EXTENSION;
     ByteVector Key = loadByteVectorFromFile(KeyFile);
     mySealedDataWithKey.push_back(Key);
     
     UInt8 Key_conv = convertByteVector2UInt8Vector(Key);
     string result = convertUInt8VectorToString(Key_conv);
     
     cout << result << endl;
};

//================================================================================
//
bool KeyronaKey::changePassword(KeyronaSubject *Subject, string &oldPassword, string &newPassword) :
keynum ("")
{
    if (Subject->getMySubjectKeyUUID().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
    else
        keynum = Subject->getMySubjectKeyUUID();
   
	KeyronaTPM;
	myTPM.change_key_auth(newPassword, oldPassword, keynum);
   
    return true;
};
