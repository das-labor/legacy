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

    if (!myVolumeStorage.findSection(VolumeUUID))
        throw InvalidUUID("TpmCryptVolume|Constructor(): Volume does not exist!");
    loadVolume();
};

//================================================================================
//
TpmCryptVolume::TpmCryptVolume(   const string &VolumeUUID,
                                const string &VolumePath,
                                const string &VolumePass,
                                UInt8 VolumeEncryptionSystem,
                                TpmCryptGroup *AdminGroup,
                                TpmCryptStorage &storageDB) :
myVolumeUUID (VolumeUUID),
myVolumePath (VolumePath),
myVolumeType (VolumeEncryptionSystem),
myVolumeStorage (storageDB)
{
    debug << "TpmCryptVolume|Constructor(): Creating new volume!" << endl;

    if (myVolumeStorage.findSection(VolumeUUID))
        throw InvalidUUID("TpmCryptVolume|Constructor(): Volume already exists!");

    if (VolumeUUID.empty())
        throw InvalidUUID("TpmCryptVolume|Constructor(): The supplied UUID was empty!");

    if (VolumePath.empty())
        throw InvalidFilename("TpmCryptVolume|Constructor(): The supplied device path was empty!");

    if ((VolumeEncryptionSystem <= VOLUMETYPE_BEGIN) | (VolumeEncryptionSystem >= VOLUMETYPE_END))
        throw InvalidEncSystem("TpmCryptVolume|Constructor(): The supplied encryption system was invalid!");

    if (VolumePass.empty())
        throw InvalidKey("TpmCryptVolume|Constructor(): The supplied password was invalid!");

    if (AdminGroup->getMyGroupID() != TPMCRYPT_ADMIN_GROUP)
        throw InvalidGroup("TpmCryptVolume|Constructor(): Invalid admin group supplied!");

    // Encrypt VolumePass for admin group
    addGroup(AdminGroup, VolumePass);
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
void   TpmCryptVolume::addSubject(TpmCryptSubject *Subject, const string &VolumePassword)
{
    if (Subject->isAdmin())
        throw InvalidSubjectID("TpmCryptVolume|addSubject(): You are not allowed to add an admin-subject!");
    
    debug << "TpmCryptVolume|addSubject(): Adding subject '" << Subject->getMySubjectIDString() << "'" << endl;

    if (VolumePassword.empty())
        throw InvalidPassword("TpmCryptVolume|addSubject(): The supplied password was empty!");
    ByteVector passwordVector = convertStringToByteVector(VolumePassword);    

    debug << "TpmCryptVolume|addSubject(): Encrypting volume key for subject '" << Subject->getMySubjectIDString() << "'" << endl;
    ByteVector passwordVectorByteVector = Subject->encryptForSubject(Subject, passwordVector);
    if (!passwordVector.size())
        throw EncryptionFailed("TpmCryptVolume|addSubject(): Encryption for subject '" + Subject->getMySubjectName() + "' failed!");

    debug << "TpmCryptVolume|addSubject(): Storing volume key for subject '" << Subject->getMySubjectIDString() << "'" << endl;
    string myBase64EncodedPassword = EncodeByteVectorToBASE64(passwordVectorByteVector);
    debug << "TpmCryptVolume|addSubject(): BASE64 encoded encrypted passphrase: " << myBase64EncodedPassword << endl;

    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(TpmCryptVolume_SubjectKeyDB);
    if (myVolumeStorage.findEntry(Subject->getMySubjectIDString()))
        debug << "TpmCryptVolume|addSubject(): Subject '" << Subject->getMySubjectIDString() << "' was already in volume" << endl;
        
    myVolumeStorage.setEntry(Subject->getMySubjectIDString(), myBase64EncodedPassword);
    string message = "You have been added to volume '" + myVolumeUUID + "'";
    Subject->addMessageForUser(message);
};


//================================================================================
//
void   TpmCryptVolume::deleteSubject(TpmCryptSubject *Subject)
{
    debug << "TpmCryptVolume|deleteSubject(): deleting subject '" << Subject->getMySubjectIDString() << "'" << endl;
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(TpmCryptVolume_SubjectKeyDB);
    myVolumeStorage.deleteEntry(Subject->getMySubjectIDString());
    string message = "You have been deleted from volume '" + myVolumeUUID + "'";
    Subject->addMessageForUser(message);
};

//================================================================================
//
void TpmCryptVolume::listSubjects()
{
    debug << "TpmCryptVolume|listSubjects(): Listing all subjects" << endl;
    cout << "\t" << "The following subjects are available in volume '" << myVolumeUUID << "':" << endl;
    vector<string> myAvailableSubjects = getSubjects();
    if (myAvailableSubjects.size())
        printStringVector(&myAvailableSubjects);
    else
        cout << "\tnone" << endl;
};


//================================================================================
//
vector<string> TpmCryptVolume::getSubjects()
{
    debug << "TpmCryptVolume|getSubjects(): Retrieving all subjects" << endl;
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(TpmCryptVolume_SubjectKeyDB);
    vector<string> myAvailableSubjects = myVolumeStorage.getAllEntries();
    return myAvailableSubjects;
};


//================================================================================
//
void   TpmCryptVolume::addGroup(TpmCryptGroup *Group, const string &VolumePassword)
{
    debug << "TpmCryptVolume|addGroup(): Adding group '" << Group->getMyGroupID() << "'" << endl;

    if (VolumePassword.empty())
        throw InvalidPassword("TpmCryptVolume|addGroup(): The supplied password was empty!");
    ByteVector passwordVector = convertStringToByteVector(VolumePassword);
	cout << "top" << endl;
    debug << "TpmCryptVolume|addGroup(): Encrypting volume key for group '" << Group->getMyGroupID() << "'" << endl;
    ByteVector passwordVectorByteVector = Group->encryptForGroup(Group, passwordVector);
    if (!passwordVector.size())
        throw EncryptionFailed("TpmCryptVolume|addGroup(): Encryption for group '" + Group->getMyGroupID() + "' failed!");

    debug << "TpmCryptVolume|addGroup(): Storing volume key for group '" << Group->getMyGroupID() << "'" << endl;
    string myBase64EncodedPassword = EncodeByteVectorToBASE64(passwordVectorByteVector);
    debug << "TpmCryptVolume|addGroup(): BASE64 encoded encrypted passphrase: " << myBase64EncodedPassword << endl;

    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(TpmCryptVolume_GroupKeyDB);
    if (myVolumeStorage.findEntry(Group->getMyGroupID()))
        debug << "TpmCryptVolume|addGroup(): Group '" << Group->getMyGroupID() << "' was already in volume, overwriting..." << endl;

    myVolumeStorage.setEntry(Group->getMyGroupID(), myBase64EncodedPassword);
    string message = "Group '" + Group->getMyGroupID() + "' has been added to volume '" + myVolumeUUID + "'";
    Group->addMessageForGroupMembers(message);

};


//================================================================================
//
void   TpmCryptVolume::deleteGroup(TpmCryptGroup *Group)
{
    debug << "TpmCryptVolume|deleteGroup(): deleting group '" << Group->getMyGroupID() << "'" << endl;
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(TpmCryptVolume_GroupKeyDB);

    if (Group->getMyGroupID() == TPMCRYPT_ADMIN_GROUP)
        throw InvalidGroup("The admin group cannot be deleted!");
    else
        myVolumeStorage.deleteEntry(Group->getMyGroupID());
    string message = "Group '" + Group->getMyGroupID() + "' has been deleted from volume '" + myVolumeUUID + "'";
    Group->addMessageForGroupMembers(message);
};

//================================================================================
//
void TpmCryptVolume::listGroups()
{
    debug << "TpmCryptVolume|listGroups(): Listing all groups" << endl;
    cout << "\t" << "The following groups are available in volume '" << myVolumeUUID << "':" << endl;
    vector<string> myAvailableGroups = getGroups();
    if (myAvailableGroups.size())
        printStringVector(&myAvailableGroups);
    else
        cout << "\tnone" << endl;
};


//================================================================================
//
vector<string> TpmCryptVolume::getGroups()
{
    debug << "TpmCryptVolume|getGroups(): Retrieving all groups" << endl;
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(TpmCryptVolume_GroupKeyDB);
    vector<string> myAvailableGroups = myVolumeStorage.getAllEntries();
    return myAvailableGroups;
};


//================================================================================
//
string TpmCryptVolume::getDecryptedVolumeKey(TpmCryptSubject *Subject, string &password)
{
    if (!Subject->getMySubjectID())
        throw InvalidSubjectID("TpmCryptVolume|getDecryptedVolumeKey(): Invalid Subject ID supplied!");

    if (Subject->isAdmin())
        throw InvalidSubjectID("TpmCryptVolume|getDecryptedVolumeKey(): As admin, you are not allowed to use this method. Please use 'getDecryptedVolumeKeyByAdmin'");

    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(TpmCryptVolume_SubjectKeyDB);

    debug << "TpmCryptVolume|getDecryptedVolumeKey(): receiving BASE64-encoded key" << endl;
    string myBase64EncodedPassword = myVolumeStorage.getEntry(Subject->getMySubjectIDString());

    debug << "TpmCryptVolume|getDecryptedVolumeKey(): decoding BASE64-encoded key" << endl;
    ByteVector myBase64EncodedPasswordByteVector = DecodeBASE64StringToByteVector(myBase64EncodedPassword);
    debug << "TpmCryptVolume|getDecryptedVolumeKey(): decrypting key" << endl;
    ByteVector myDecode = Subject->decryptBySubject(Subject, myBase64EncodedPasswordByteVector, password);

    return convertByteVector2String(myDecode);
};

//================================================================================
//
string TpmCryptVolume::getDecryptedVolumeKey(TpmCryptGroup *Group, string &password)
{
    if (password.empty())
        throw InvalidPassword("TpmCryptVolume|getDecryptedVolumeKey(): Empty password supplied!");

    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(TpmCryptVolume_GroupKeyDB);

    debug << "TpmCryptVolume|getDecryptedVolumeKey(): receiving BASE64-encoded key" << endl;
    string myBase64EncodedPassword = myVolumeStorage.getEntry(Group->getMyGroupID());

    debug << "TpmCryptVolume|getDecryptedVolumeKey(): decoding BASE64-encoded key" << endl;
    ByteVector myDecodeByteVector = DecodeBASE64StringToByteVector(myBase64EncodedPassword);

    debug << "TpmCryptVolume|getDecryptedVolumeKey(): decrypting key" << endl;

    ByteVector myDecode = Group->decryptByGroup(Group, myDecodeByteVector, password);

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
    myVolumeStorage.deleteSection(myVolumeUUID);
};

//================================================================================
//
void TpmCryptVolume::storeVolume()
{
    debug << "TpmCryptVolume|storeVolume(): Storing volume '" + myVolumeUUID + "'" << endl;
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(TpmCryptVolume_GenericDB);
    myVolumeStorage.setEntry(TpmCryptVolume_isVolume, TpmCryptVolume_isVolume_true);
    myVolumeStorage.setEntry(TpmCryptVolume_VolumeUUID, myVolumeUUID);
    myVolumeStorage.setEntry(TpmCryptVolume_VolumePATH, myVolumePath);

    switch(myVolumeType)
    {
        case VOLUMETYPE_DMCRYPT: myVolumeStorage.setEntry(TpmCryptVolume_VolumeType, TpmCryptVolumeType_DMCRYPT); break;
        case VOLUMETYPE_TRUECRYPT: myVolumeStorage.setEntry(TpmCryptVolume_VolumeType, TpmCryptVolumeType_TRUECRYPT); break;
        case VOLUMETYPE_ECRYPTFS: myVolumeStorage.setEntry(TpmCryptVolume_VolumeType, TpmCryptVolumeType_ECRYPTFS); break;
        case VOLUMETYPE_LUKS: myVolumeStorage.setEntry(TpmCryptVolume_VolumeType, TpmCryptVolumeType_LUKS); break;
        case VOLUMETYPE_ENCFS: myVolumeStorage.setEntry(TpmCryptVolume_VolumeType, TpmCryptVolumeType_ENCFS); break;

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
    
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(TpmCryptVolume_GenericDB);
    myVolumeTypeString = myVolumeStorage.getEntry(TpmCryptVolume_VolumeType);
    myVolumePath = myVolumeStorage.getEntry(TpmCryptVolume_VolumePATH);

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
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(TpmCryptVolume_SSSDB);
    myVolumeStorage.setEntry(SSSID, TpmCryptVolume_SSSDB_valid);
};

//================================================================================
//
vector<string> TpmCryptVolume::getAvailableSSS()
{
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(TpmCryptVolume_SSSDB);
    return myVolumeStorage.getAllEntries();
};

//================================================================================
//
void TpmCryptVolume::deleteSSS(string &SSSID)
{
    if (SSSID.empty())
        throw InvalidSSSID("TpmCryptVolume|deleteSSS(): Empty SSSID supplied!");
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(TpmCryptVolume_SSSDB);
    myVolumeStorage.deleteEntry(SSSID);
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
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(TpmCryptVolume_GenericDB);
    myVolumeStorage.setEntry(TpmCryptVolume_VolumePATH, myVolumePath);
};

//================================================================================
//
bool TpmCryptVolume::isSubjectInVolume(TpmCryptSubject *Subject)
{
    // load all subjects already in volume
    vector<string> mySubjectsInVolume = getSubjects();

    vector<string>::const_iterator Iterator;
    Iterator=mySubjectsInVolume.begin();
    while ( Iterator != mySubjectsInVolume.end())
    {
        if (*(Iterator) == Subject->getMySubjectIDString())
            return true;
        Iterator++;
    }
    return false;
};

//================================================================================
//
bool TpmCryptVolume::isGroupInVolume(TpmCryptGroup *Group)
{
    // load all groups already in volume
    vector<string> myGroupsInVolume = getGroups();

    vector<string>::const_iterator Iterator;
    Iterator=myGroupsInVolume.begin();
    while ( Iterator != myGroupsInVolume.end())
    {
        if (*(Iterator) == Group->getMyGroupID())
            return true;
        Iterator++;
    }
    return false;
};
