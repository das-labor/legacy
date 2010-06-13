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
valid (false)
{

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
bool KeyronaKey::changePassword(string &oldPassword, string &newPassword)
{
   
   
   
    return true;
  
};
