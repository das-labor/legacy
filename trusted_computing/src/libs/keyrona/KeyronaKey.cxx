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
#include <trousers/tss.h>
#include <trousers/trousers.h>

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
	ByteVector Data = myTPM.create_key(password, uuid, type);
	
   
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
	ByteVector Data = myTPM.create_key(groupKeyPassword, uuid, type);

   
    string DataFile = keyfile ; //+ keynum + KEYRONA_TPM_KEY_EXTENSION
    storeByteVectorInFile(DataFile, Data);
   
    valid = true;
    KeyCounter++;
};


//================================================================================
//
KeyronaKey::~KeyronaKey()
{
    KeyCounter--;
};

//================================================================================
//
ByteVector  KeyronaKey::encrypt(KeyronaSubject *Subject, KeyronaGroup *Group, ByteVector &toEncrypt) 
{
	ByteVector result;
	string keyuuid;
	UInt32 uuid;
	UInt8 mySubjectType;

	KeyronaTPM myTPM;

	if (Group == NULL) {	
			if (Subject->getMySubjectKeyUUID().empty())
				throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
			else
				keyuuid = Subject->getMySubjectKeyUUID();
			    
			    uuid = convertStringtoUInt32(keyuuid);    

				result = myTPM.bind(toEncrypt, uuid);
	}
	
	if(Subject == NULL) {
		if (Group->getMyGroupKeyUUID().empty())
			throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
		else
			keyuuid = Group->getMyGroupKeyUUID();
		
		    uuid = convertStringtoUInt32(keyuuid);    

			result = myTPM.bind(toEncrypt, uuid);
    }
	
    return result;
};

//================================================================================
//
ByteVector  KeyronaKey::decrypt(KeyronaSubject *Subject, KeyronaGroup *Group, ByteVector &toDecrypt, string &myPassword) 
{
	ByteVector result;
	string keyuuid;
	UInt32 uuid;
	UInt8 mySubjectType;

	KeyronaTPM myTPM;
	
	if (Group == NULL) {	
			if (Subject->getMySubjectKeyUUID().empty())
				throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
			else
				keyuuid = Subject->getMySubjectKeyUUID();
			    
			    uuid = convertStringtoUInt32(keyuuid);    

				result = myTPM.unbind(toDecrypt, uuid, myPassword);
	}
	
	if(Subject == NULL) {
		if (Group->getMyGroupKeyUUID().empty())
			throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
		else
			keyuuid = Group->getMyGroupKeyUUID();
		
		    uuid = convertStringtoUInt32(keyuuid);    

			result = myTPM.unbind(toDecrypt, uuid, myPassword);
    }
    
    return result;
};

//================================================================================
//
string KeyronaKey::printKeyInformation(KeyronaSubject *Subject) 
{
	string keyfile, keynum;
    if (Subject->getMySubjectKeyfile().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
    else
        keyfile = Subject->getMySubjectKeyfile();	

	 string KeyFile = keyfile; // + keynum + KEYRONA_TPM_KEY_EXTENSION;
     ByteVector Key = loadByteVectorFromFile(KeyFile);
     
	 string result = convertByteVector2String(Key);
     
     return result;
};

//================================================================================
//
void KeyronaKey::deleteKey(KeyronaSubject *Subject, KeyronaGroup *Group)
{
	string keyuuid;

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
void KeyronaKey::getKeysUUID()
{
	KeyronaTPM myTPM;
	
	ByteVector keys = myTPM.get_keys_by_uuid();
	TSS_KM_KEYINFO2 k((const tdTSS_KM_KEYINFO2&) keys);
	UInt32 keysslength = keys.size();
	for( UInt32 i = 0; i<keysslength; i++) {
	printf("UUID         : %08x-%04hx-%04hx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx\n",
	       k.keyUUID.ulTimeLow, k.keyUUID.usTimeMid, k.keyUUID.usTimeHigh,
	       k.keyUUID.bClockSeqHigh, k.keyUUID.bClockSeqLow,
	       k.keyUUID.rgbNode[0] & 0xff, k.keyUUID.rgbNode[1] & 0xff,
	       k.keyUUID.rgbNode[2] & 0xff, k.keyUUID.rgbNode[3] & 0xff,
	       k.keyUUID.rgbNode[4] & 0xff, k.keyUUID.rgbNode[5] & 0xff);
	       	}
	
};

//================================================================================
//
void KeyronaKey::deleteAllKeys()
{
	KeyronaTPM myTPM;
	
	for(UInt32 i = 0; i < 1000; i++)
	{
		myTPM.delete_key(i);
		cout << "Deleting Key: " << i << endl;
	}
};
//================================================================================
//
bool KeyronaKey::changePassword(KeyronaSubject *Subject, string &oldPassword, string &newPassword) 
{
	string keynum;
    if (Subject->getMySubjectKeyUUID().empty())
        throw NoFilename("KeyronaKey|Constructor(): The supplied key filename was empty!");
    else
        keynum = Subject->getMySubjectKeyUUID();
   
	UInt32 uuid = convertStringtoUInt32(keynum);
   
	KeyronaTPM myTPM;
	
	myTPM.change_key_auth(newPassword, oldPassword, uuid);
   
    return true;
};
