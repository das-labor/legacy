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
        
    UInt32 uuid = convertStringtoUInt32(keynum);
        	
	KeyronaTPM myTPM;
	vector<ByteVector> keyinfo = myTPM.create_key(password, uuid, type);
	
    // data
    ByteVector Data = keyinfo.back();
    keyinfo.pop_back();
    string DataFile = keyfile; //+ keynum + KEYRONA_TPM_KEY_EXTENSION;
    storeByteVectorInFile(DataFile, Data);
    
    valid = true;
    KeyCounter++;
};

//================================================================================
//
KeyronaKey::KeyronaKey(KeyronaGroup *Group, string &groupKeyPassword) :
valid (false),
keyfile (""),
keynum (""),
type ("")
{
	if (!Group)
        throw InvalidSubjectID("KeyronaKey|Constructor(): Invalid Subject pointer given!");

    if (Group->getMyGroupKeyfile().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied keyss filename was empty!");
    else
        keyfile = Group->getMyGroupKeyfile();
        
    if (Group->getMyGroupKeyUUID().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied keys filename was empty!");
    else
        keynum = Group->getMyGroupKeyUUID();
       
    if (Group->getMyGroupKeyType().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied keysss filename was empty!");
    else
        type = Group->getMyGroupKeyType();
        
    UInt32 uuid = convertStringtoUInt32(keynum);
        	
	KeyronaTPM myTPM;
	vector<ByteVector> keyinfo = myTPM.create_key(groupKeyPassword, uuid, type);

    // data
    ByteVector Data = keyinfo.back();
    keyinfo.pop_back();
    string DataFile = keyfile ; //+ keynum + KEYRONA_TPM_KEY_EXTENSION
    storeByteVectorInFile(DataFile, Data);
    	
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
ByteVector  KeyronaKey::encrypt(KeyronaSubject *Subject, KeyronaGroup *Group, ByteVector &toEncrypt) 
{
	if (Group == NULL) {					
		if (Subject->getMySubjectKeyUUID().empty())
			throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
		else
			keyuuid = Subject->getMySubjectKeyUUID();
	}
	if(Subject == NULL) {
		if (Group->getMyGroupKeyUUID().empty())
			throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
		else
			keyuuid = Group->getMyGroupKeyUUID();
    }
        
    UInt32 uuid = convertStringtoUInt32(keyuuid);    

   	KeyronaTPM myTPM;
    ByteVector result = myTPM.bind(toEncrypt, uuid);
	
	keyuuid.clear();
	
    return result;
};

//================================================================================
//
ByteVector  KeyronaKey::decrypt(KeyronaSubject *Subject, KeyronaGroup *Group, ByteVector &toDecrypt, string &myPassword) 
{
    if (Group == NULL) {					
		if (Subject->getMySubjectKeyUUID().empty())
			throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
		else
			keyuuid = Subject->getMySubjectKeyUUID();
	}
	if(Subject == NULL) {
		if (Group->getMyGroupKeyUUID().empty())
			throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
		else
			keyuuid = Group->getMyGroupKeyUUID();
    }
    	
	UInt32 uuid = convertStringtoUInt32(keyuuid);
	
	KeyronaTPM myTPM;
    ByteVector result = myTPM.unbind(toDecrypt, uuid, myPassword);
	
	keyuuid.clear();
	
    return result;
};

//================================================================================
//
void KeyronaKey::printKeyInformation(KeyronaSubject *Subject) 
{
    if (Subject->getMySubjectKeyfile().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
    else
        keyfile = Subject->getMySubjectKeyfile();	
	
	if (Subject->getMySubjectKeyUUID().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
    else
        keynum = Subject->getMySubjectKeyUUID();
	
	 string KeyFile = keyfile; // + keynum + KEYRONA_TPM_KEY_EXTENSION;
     ByteVector Key = loadByteVectorFromFile(KeyFile);
     
	 string result = convertByteVector2String(Key);
     
     cout << result << endl;
};

//================================================================================
//
void KeyronaKey::deleteKey(KeyronaSubject *Subject, KeyronaGroup *Group)
{
	if (Group == NULL) {					
		if (Subject->getMySubjectKeyUUID().empty())
			throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
		else
			keyuuid = Subject->getMySubjectKeyUUID();
	}
	else {
		if (Group->getMyGroupKeyUUID().empty())
			throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
		else
			keyuuid = Group->getMyGroupKeyUUID();
    }			
        
    UInt32 uuid = convertStringtoUInt32(keyuuid);  
    
    KeyronaTPM myTPM;
    myTPM.delete_key(uuid);    
};

//================================================================================
//
bool KeyronaKey::changePassword(KeyronaSubject *Subject, string &oldPassword, string &newPassword) 
{
    if (Subject->getMySubjectKeyUUID().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
    else
        keynum = Subject->getMySubjectKeyUUID();
   
	UInt32 uuid = convertStringtoUInt32(keynum);
   
	KeyronaTPM myTPM;
	myTPM.change_key_auth(newPassword, oldPassword, uuid);
   
    return true;
};
