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
 * @file        TpmCryptKey.cxx
 * @brief       Implementation of the TpmCrypt Key class.
 * @version     $Revision: 449 $
 * @date        Last modification at $Date: 2009-04-06 00:16:17 +0200 (Mon, 06 Apr 2009) $ by $Author: selhorst $
 */

#include <TpmCryptKey.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

// create a new key
//================================================================================
//
TpmCryptKey::TpmCryptKey(TpmCryptSubject *Subject, string &password) :
valid (false),
keyfile (""),
keynum (""),
type ("")
{
	if (!Subject)
        throw InvalidSubjectID("TpmCryptKey|Constructor(): Invalid Subject pointer given!");

    if (Subject->getMySubjectKeyfile().empty())
        throw NoFilename("TpmCryptKey|Constructor(): The supplied key filename was empty!");
    else
        keyfile = Subject->getMySubjectKeyfile();
        
    if (Subject->getMySubjectKeyUUID().empty())
        throw NoFilename("TpmCryptKey|Constructor(): The supplied key filename was empty!");
    else
        keynum = Subject->getMySubjectKeyUUID();
        
    if (Subject->getMySubjectKeyType().empty())
        throw NoFilename("TpmCryptKey|Constructor(): The supplied key filename was empty!");
    else
        type = Subject->getMySubjectKeyType();
        
    UInt32 uuid = convertStringtoUInt32(keynum);
        	
	TpmCryptTPM myTPM;
	ByteVector Data = myTPM.create_key(password, uuid, type);
	
   
    string DataFile = keyfile;
    storeByteVectorInFile(DataFile, Data);
   
    valid = true;
};

//================================================================================
//
TpmCryptKey::TpmCryptKey(TpmCryptGroup *Group, string &groupKeyPassword) :
valid (false),
keyfile (""),
keynum (""),
type ("")
{
	if (!Group)
        throw InvalidSubjectID("TpmCryptKey|Constructor(): Invalid Subject pointer given!");

    if (Group->getMyGroupKeyfile().empty())
        throw NoFilename("TpmCryptKey|Constructor(): The supplied keyss filename was empty!");
    else
        keyfile = Group->getMyGroupKeyfile();
        
    if (Group->getMyGroupKeyUUID().empty())
        throw NoFilename("TpmCryptKey|Constructor(): The supplied keys filename was empty!");
    else
        keynum = Group->getMyGroupKeyUUID();
       
    if (Group->getMyGroupKeyType().empty())
        throw NoFilename("TpmCryptKey|Constructor(): The supplied keysss filename was empty!");
    else
        type = Group->getMyGroupKeyType();
        
    UInt32 uuid = convertStringtoUInt32(keynum);
        	
	TpmCryptTPM myTPM;
	ByteVector Data = myTPM.create_key(groupKeyPassword, uuid, type);

   
    string DataFile = keyfile ; 
    storeByteVectorInFile(DataFile, Data);
   
    valid = true;
};


//================================================================================
//
TpmCryptKey::~TpmCryptKey()
{
};

//================================================================================
//
ByteVector  TpmCryptKey::encrypt(TpmCryptSubject *Subject, TpmCryptGroup *Group, ByteVector &toEncrypt) 
{
	ByteVector result;
	string keyuuid;
	UInt32 uuid;
	UInt8 mySubjectType;

	TpmCryptTPM myTPM;

	if (Group == NULL) {	
			if (Subject->getMySubjectKeyUUID().empty())
				throw NoFilename("TpmCryptKey|Constructor(): The supplied key filename was empty!");
			else
				keyuuid = Subject->getMySubjectKeyUUID();
			    
			    uuid = convertStringtoUInt32(keyuuid);    

				result = myTPM.bind(toEncrypt, uuid);
	}
	
	if(Subject == NULL) {
		if (Group->getMyGroupKeyUUID().empty())
			throw NoFilename("TpmCryptKey|Constructor(): The supplied key filename was empty!");
		else
			keyuuid = Group->getMyGroupKeyUUID();
		
		    uuid = convertStringtoUInt32(keyuuid);    

			result = myTPM.bind(toEncrypt, uuid);
    }
	
    return result;
};

//================================================================================
//
ByteVector  TpmCryptKey::decrypt(TpmCryptSubject *Subject, TpmCryptGroup *Group, ByteVector &toDecrypt, string &myPassword) 
{
	ByteVector result;
	string keyuuid;
	UInt32 uuid;
	UInt8 mySubjectType;

	TpmCryptTPM myTPM;
	
	if (Group == NULL) {	
			if (Subject->getMySubjectKeyUUID().empty())
				throw NoFilename("TpmCryptKey|Constructor(): The supplied key filename was empty!");
			else
				keyuuid = Subject->getMySubjectKeyUUID();
			    
			    uuid = convertStringtoUInt32(keyuuid);    

				result = myTPM.unbind(toDecrypt, uuid, myPassword);
	}
	
	if(Subject == NULL) {
		if (Group->getMyGroupKeyUUID().empty())
			throw NoFilename("TpmCryptKey|Constructor(): The supplied key filename was empty!");
		else
			keyuuid = Group->getMyGroupKeyUUID();
		
		    uuid = convertStringtoUInt32(keyuuid);    

			result = myTPM.unbind(toDecrypt, uuid, myPassword);
    }
    
    return result;
};

//================================================================================
//
string TpmCryptKey::printKeyInformation(TpmCryptSubject *Subject) 
{
	string keyfile, keynum;
    if (Subject->getMySubjectKeyfile().empty())
        throw NoFilename("TpmCryptKey|Constructor(): The supplied key filename was empty!");
    else
        keyfile = Subject->getMySubjectKeyfile();	

	 string KeyFile = keyfile; // + keynum + TPMCRYPT_TPM_KEY_EXTENSION;
     ByteVector Key = loadByteVectorFromFile(KeyFile);
     
	 string result = convertByteVector2String(Key);
     
     return result;
};

//================================================================================
//
void TpmCryptKey::deleteKey(TpmCryptSubject *Subject, TpmCryptGroup *Group)
{
	string keyuuid;

	if (Group == NULL) {					
		if (Subject->getMySubjectKeyUUID().empty())
			throw NoFilename("TpmCryptKey|Constructor(): The supplied key filename was empty!");
		else
			keyuuid = Subject->getMySubjectKeyUUID();
	}
	else {
		if (Group->getMyGroupKeyUUID().empty())
			throw NoFilename("TpmCryptKey|Constructor(): The supplied key filename was empty!");
		else
			keyuuid = Group->getMyGroupKeyUUID();
    }
    			
    UInt32 uuid = convertStringtoUInt32(keyuuid);  

    TpmCryptTPM myTPM;

    myTPM.delete_key(uuid);

};
//================================================================================
//
void TpmCryptKey::getKeysUUID()
{

	
};

//================================================================================
//
bool TpmCryptKey::changePassword(TpmCryptSubject *Subject, string &oldPassword, string &newPassword) 
{
	string keynum;
    if (Subject->getMySubjectKeyUUID().empty())
        throw NoFilename("TpmCryptKey|Constructor(): The supplied key filename was empty!");
    else
        keynum = Subject->getMySubjectKeyUUID();
   
	UInt32 uuid = convertStringtoUInt32(keynum);
   
	TpmCryptTPM myTPM;
	
	myTPM.change_key_auth(newPassword, oldPassword, uuid);
   
    return true;
};
