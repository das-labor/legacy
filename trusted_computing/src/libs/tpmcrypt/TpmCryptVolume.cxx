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
 * @file        TpmCryptVolume.cxx
 * @brief       Implementation of the TpmCrypt volume class.
 * @version     $Revision: 673 $
 * @date        Last modification at $Date: 2009-06-25 15:14:43 +0200 (Thu, 25 Jun 2009) $ by $Author: selhorst $
 */

#include <TpmCryptVolume.hxx>

using namespace std;
using namespace utils;
using namespace tpmcrypt;

typedef map<string, int> t_StringMap;

//================================================================================
//
TpmCryptVolume::TpmCryptVolume( const string &VolumeUUID, TpmCryptStorage &storageDB ) :
myVolumeUUID (VolumeUUID),
myVolumePath (""),
myVolumeType (0),
myVolumeStorage (storageDB)
{
    debug << "TpmCryptVolume|Constructor(): Loading volume!" << endl;
    if (VolumeUUID.empty())
        throw InvalidUUID("TpmCryptVolume|Constructor(): The supplied UUID was empty!");

    if (!TpmCryptFindVolume(storeDB,VolumeUUID))
        throw InvalidUUID("TpmCryptVolume|Constructor(): Volume does not exist!");
    loadVolume();
};

//================================================================================
//
TpmCryptVolume::TpmCryptVolume( const string &VolumeUUID,
                                const string &VolumePath,
                                const string &VolumePass,
                                UInt8 VolumeEncryptionSystem,
                                TpmCryptUser	*myAdmin,
                                TpmCryptStorage &storageDB) :
myVolumeUUID (VolumeUUID),
myVolumePath (VolumePath),
myVolumeType (VolumeEncryptionSystem),
myVolumeStorage (storageDB)
{
    debug << "TpmCryptVolume|Constructor(): Creating new volume!" << endl;

    if (TpmCryptFindVolume(storeDB,VolumeUUID))
        throw InvalidUUID("TpmCryptVolume|Constructor(): Volume already exists!");

    if (VolumeUUID.empty())
        throw InvalidUUID("TpmCryptVolume|Constructor(): The supplied UUID was empty!");

    if (VolumePath.empty())
        throw InvalidFilename("TpmCryptVolume|Constructor(): The supplied device path was empty!");

    if ((VolumeEncryptionSystem <= VOLUMETYPE_BEGIN) | (VolumeEncryptionSystem >= VOLUMETYPE_END))
        throw InvalidEncSystem("TpmCryptVolume|Constructor(): The supplied encryption system was invalid!");

    if (VolumePass.empty())
        throw InvalidKey("TpmCryptVolume|Constructor(): The supplied password was invalid!");

    // Damit encrypt mutiple volumes with admin pw TODO TODO TODO
    storeVolume();
};

//================================================================================
//
TpmCryptVolume::~TpmCryptVolume()
{
    // nothing to do...
    debug << "TpmCryptVolume|Destructor(): !" << endl;
};

//================================================================================
//
void   TpmCryptVolume::addUser(TpmCryptUser *User, const string &VolumePassword)
{
    if (TpmCryptFindAdmin(storeDB,User->getMyUserUUID())
        throw InvalidSubjectID("TpmCryptVolume|addSubject(): You are not allowed to add an admin-subject!");
        
    if (TpmCryptFindUserInVolume(storeDB,User->getMyUserUUID(),myVolumeUUID)
        throw InvalidSubjectID("TpmCryptVolume|addSubject(): You are not allowed to add an admin-subject!");
    
    debug << "TpmCryptVolume|addSubject(): Adding subject '" << User->getMyUserIDString() << "'" << endl;

    if (VolumePassword.empty())
        throw InvalidPassword("TpmCryptVolume|addSubject(): The supplied password was empty!");
    ByteVector passwordVector = convertStringToByteVector(VolumePassword);    

    debug << "TpmCryptVolume|addSubject(): Encrypting volume key for subject '" << User->getMyUserUUID() << "'" << endl;
    ByteVector passwordVectorByteVector = User->encryptForUser(Subject, passwordVector);
    if (!passwordVector.size())
        throw EncryptionFailed("TpmCryptVolume|addSubject(): Encryption for subject '" + User->getMyUserName() + "' failed!");

    debug << "TpmCryptVolume|addSubject(): Storing volume key for subject '" << User->getMyUserUUID() << "'" << endl;
    string myBase64EncodedPassword = EncodeByteVectorToBASE64(passwordVectorByteVector);
    debug << "TpmCryptVolume|addSubject(): BASE64 encoded encrypted passphrase: " << myBase64EncodedPassword << endl;

    if (TpmCryptFindUserInVolume(myVolumeStorage,User->getMyUserUUID(),myVolumeUUID))
        debug << "TpmCryptVolume|addSubject(): Subject '" << User->getMyUserUUID() << "' was already in volume" << endl;
        
    myVolumeStorage.storeDB("user_keys","key_blob","u_uuid",myBase64EncodedPassword,User->getMyUserUUID());
    string message = "You have been added to volume '" + myVolumeUUID + "'";
    Subject->addMessageForUser(message);
};


//================================================================================
//
void TpmCryptVolume::deleteUser(TpmCryptUser *User)
{
    debug << "TpmCryptVolume|deleteSubject(): deleting subject '" << User->getMyUserUUID() << "'" << endl;
    myVolumeStorage.deleteDB("volume_users","u_uuid",User->getMyUserUUID());
    string message = "You have been deleted from volume '" + myVolumeUUID + "'";
    User->addMessageForUser(message);
};

//================================================================================
//
void TpmCryptVolume::listUsers()
{
    debug << "TpmCryptVolume|listSubjects(): Listing all subjects" << endl;
    cout << "\t" << "The following subjects are available in volume '" << myVolumeUUID << "':" << endl;
    vector<string> myAvailableUsers = TpmCryptFindAllUsersInVolume(myVolumeStorage,VolumeUUID);
    if (myAvailableUsers.size())
        printStringVector(&myAvailableUsers);
    else
        cout << "\tnone" << endl;
};

//================================================================================
//
string TpmCryptVolume::getDecryptedVolumeKey(TpmCryptSubject *User, string &password)
{
    if (!User->getMyUserUUID)
        throw InvalidSubjectID("TpmCryptVolume|getDecryptedVolumeKey(): Invalid Subject ID supplied!");

    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(TpmCryptVolume_SubjectKeyDB);

    debug << "TpmCryptVolume|getDecryptedVolumeKey(): receiving BASE64-encoded key" << endl;
    string myBase64EncodedPassword = myVolumeStorage.queryDoubleDB("user_keys","key_blob","u_uuid",User->getMyUserUUID,"v_uuid",myVolumeUUID);
    debug << "TpmCryptVolume|getDecryptedVolumeKey(): decoding BASE64-encoded key" << endl;
    ByteVector myBase64EncodedPasswordByteVector = DecodeBASE64StringToByteVector(myBase64EncodedPassword);
    debug << "TpmCryptVolume|getDecryptedVolumeKey(): decrypting key" << endl;
    ByteVector myDecode = User->decryptByUser(User, myBase64EncodedPasswordByteVector, password);

    return convertByteVector2String(myDecode);
};

//================================================================================
//
void   TpmCryptVolume::printVolume()
{
    debug << "TpmCryptVolume|printVolume(): Printing volume information" << endl;
    cout << "Volume UUID: '" << myVolumeUUID << "'" << endl;
    cout << "\t" << "Currently attached to: '" << myVolumePath << "'" << endl;
    cout << "\t" << "Encryption system: '";
    switch(myVolumeType)
    {
        case VOLUMETYPE_DMCRYPT: cout << TpmCryptVolumeType_DMCRYPT; break;
        case VOLUMETYPE_TRUECRYPT: cout << TpmCryptVolumeType_TRUECRYPT; break;
        case VOLUMETYPE_ECRYPTFS: cout << TpmCryptVolumeType_ECRYPTFS; break;
        case VOLUMETYPE_LUKS: cout << TpmCryptVolumeType_LUKS; break;
        case VOLUMETYPE_ENCFS: cout << TpmCryptVolumeType_ENCFS; break;
        default: cout << "undefined!";
    }
    cout << "'" << endl;
};

//================================================================================
//
void   TpmCryptVolume::deleteVolume()
{
    debug << "TpmCryptVolume|deleteVolume(): Deleting volume '" + myVolumeUUID + "'" << endl;
    myVolumeStorage.deleteDB("volumes","v_uuid",myVolumeUUID);
    myVolumeStorage.deleteDB("volumes_users","v_uuid",myVolumeUUID);
    myVolumeStorage.deleteDB("volumes_sss","v_uuid",myVolumeUUID);
    myVolumeStorage.deleteDB("user_keys","v_uuid",myVolumeUUID);
};

//================================================================================
//
void TpmCryptVolume::storeVolume()
{
    debug << "TpmCryptVolume|storeVolume(): Storing volume '" + myVolumeUUID + "'" << endl;

    myVolumeStorage.storeDB("volumes","path","v_uuid",myVolumePath,myVolumeUUID);

    switch(myVolumeType)
    {
        case VOLUMETYPE_DMCRYPT: myVolumeStorage.storeDB("volumes","type","v_uuid",TpmCryptVolumeType_DMCRYPT,myVolumeUUID); break;
        case VOLUMETYPE_TRUECRYPT: myVolumeStorage.storeDB("volumes","type","v_uuid",TpmCryptVolumeType_TRUECRYPT,myVolumeUUID); break;
        case VOLUMETYPE_ECRYPTFS: myVolumeStorage.storeDB("volumes","type","v_uuid",TpmCryptVolumeType_ECRYPTFS,myVolumeUUID); break;
        case VOLUMETYPE_LUKS: myVolumeStorage.storeDB("volumes","type","v_uuid",TpmCryptVolumeType_LUKS,myVolumeUUID); break;
        case VOLUMETYPE_ENCFS: myVolumeStorage.storeDB("volumes","type","v_uuid",TpmCryptVolumeType_ENCFS,myVolumeUUID); break;

    }
};

//================================================================================
//
void TpmCryptVolume::loadVolume()
{
    debug << "TpmCryptVolume|loadVolume(): Loading volume '" + myVolumeUUID + "'" << endl;
    t_StringMap TpmCryptVolumeTypeMap;
    string myVolumeTypeString;
    TpmCryptVolumeTypeMap["DMCRYPT"] = VOLUMETYPE_DMCRYPT;
    TpmCryptVolumeTypeMap["TRUECRYPT"] = VOLUMETYPE_TRUECRYPT;
    TpmCryptVolumeTypeMap["ECRYPTFS"] = VOLUMETYPE_ECRYPTFS;
    TpmCryptVolumeTypeMap["LUKS"] = VOLUMETYPE_LUKS;
    TpmCryptVolumeTypeMap["ENCFS"] = VOLUMETYPE_ENCFS;

    myVolumeTypeString = myVolumeStorage.queryDB("volumes","type","v_uuid",myVolumeUUID);
    myVolumePath = myVolumeStorage.queryDB("volumes","path","v_uuid",myVolumeUUID);
    switch(TpmCryptVolumeTypeMap[myVolumeTypeString])
    {
        case VOLUMETYPE_DMCRYPT: myVolumeType = VOLUMETYPE_DMCRYPT; break;
        case VOLUMETYPE_TRUECRYPT: myVolumeType = VOLUMETYPE_TRUECRYPT; break;
        case VOLUMETYPE_ECRYPTFS: myVolumeType = VOLUMETYPE_ECRYPTFS; break;
        case VOLUMETYPE_LUKS: myVolumeType = VOLUMETYPE_LUKS; break;
        case VOLUMETYPE_ENCFS: myVolumeType = VOLUMETYPE_ENCFS; break;
    }
};
// Secret-Sharing-Scheme
//================================================================================
//
void TpmCryptVolume::addSSS(string &SSSID)
{
    if (SSSID.empty())
        throw InvalidSSSID("TpmCryptVolume|addSSS(): Empty SSSID supplied!");
	
	if(TpmCryptFindSSS(myVolumeStorage,SSSID);
		throw InvalidSSSID("Already in Volume");
	
    myVolumeStorage.volumes_sssDB(myVolumeUUID,SSSID);
};

//================================================================================
//
vector<string> TpmCryptVolume::getAvailableSSS()
{
    return TpmCryptFindAllSSSInVolume(myVolumeStorage,myVolumeUUID);
};

//================================================================================
//
void TpmCryptVolume::deleteSSS(string &SSSID)
{
    if (SSSID.empty())
        throw InvalidSSSID("TpmCryptVolume|deleteSSS(): Empty SSSID supplied!");
        
	myVolumeStorage.deleteDB("volumes_sss","sss_uuid",SSSID);
};

//================================================================================
//
void TpmCryptVolume::updateDevicePath(string &newDevicePath)
{
    if (!(fileExists(newDevicePath)))
        throw InvalidFilename("TpmCryptVolume|updateDevicePath(): The supplied device name does not exist!");

    // set internal variable
    myVolumePath = newDevicePath;

    // store new value to database
    myVolumeStorage.storeDB("volumes","path","v_uuid",myVolumePath,myVolumeUUID);
};


