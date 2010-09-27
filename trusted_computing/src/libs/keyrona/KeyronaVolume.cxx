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
 * @file        KeyronaVolume.cxx
 * @brief       Implementation of the Keyrona volume class.
 * @version     $Revision: 673 $
 * @date        Last modification at $Date: 2009-06-25 15:14:43 +0200 (Thu, 25 Jun 2009) $ by $Author: selhorst $
 */

#include <KeyronaVolume.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

typedef map<string, int> t_StringMap;

//================================================================================
//
KeyronaVolume::KeyronaVolume( const string &VolumeUUID, KeyronaStorage &storageDB ) :
myVolumeUUID (VolumeUUID),
myVolumePath (""),
myVolumeType (0),
myVolumeStorage (storageDB)
{
    debug << "KeyronaVolume|Constructor(): Loading volume!" << endl;
    if (VolumeUUID.empty())
        throw InvalidUUID("KeyronaVolume|Constructor(): The supplied UUID was empty!");

    if (!myVolumeStorage.findSection(VolumeUUID))
        throw InvalidUUID("KeyronaVolume|Constructor(): Volume does not exist!");
    loadVolume();
};

//================================================================================
//
KeyronaVolume::KeyronaVolume(   const string &VolumeUUID,
                                const string &VolumePath,
                                const string &VolumePass,
                                UInt8 VolumeEncryptionSystem,
                                KeyronaGroup *AdminGroup,
                                KeyronaStorage &storageDB) :
myVolumeUUID (VolumeUUID),
myVolumePath (VolumePath),
myVolumeType (VolumeEncryptionSystem),
myVolumeStorage (storageDB)
{
    debug << "KeyronaVolume|Constructor(): Creating new volume!" << endl;

    if (myVolumeStorage.findSection(VolumeUUID))
        throw InvalidUUID("KeyronaVolume|Constructor(): Volume already exists!");

    if (VolumeUUID.empty())
        throw InvalidUUID("KeyronaVolume|Constructor(): The supplied UUID was empty!");

    if (VolumePath.empty())
        throw InvalidFilename("KeyronaVolume|Constructor(): The supplied device path was empty!");

    if ((VolumeEncryptionSystem <= VOLUMETYPE_BEGIN) | (VolumeEncryptionSystem >= VOLUMETYPE_END))
        throw InvalidEncSystem("KeyronaVolume|Constructor(): The supplied encryption system was invalid!");

    if (VolumePass.empty())
        throw InvalidKey("KeyronaVolume|Constructor(): The supplied password was invalid!");

    if (AdminGroup->getMyGroupID() != KEYRONA_ADMIN_GROUP)
        throw InvalidGroup("KeyronaVolume|Constructor(): Invalid admin group supplied!");

    // Encrypt VolumePass for admin group
    addGroup(AdminGroup, VolumePass);
    storeVolume();
};

//================================================================================
//
KeyronaVolume::~KeyronaVolume()
{
    // nothing to do...
    debug << "KeyronaVolume|Destructor(): !" << endl;
};

//================================================================================
//
void   KeyronaVolume::addSubject(KeyronaSubject *Subject, const string &VolumePassword)
{
    if (Subject->isAdmin())
        throw InvalidSubjectID("KeyronaVolume|addSubject(): You are not allowed to add an admin-subject!");
    
    debug << "KeyronaVolume|addSubject(): Adding subject '" << Subject->getMySubjectIDString() << "'" << endl;

    if (VolumePassword.empty())
        throw InvalidPassword("KeyronaVolume|addSubject(): The supplied password was empty!");
    ByteVector passwordVector = convertStringToByteVector(VolumePassword);    

    debug << "KeyronaVolume|addSubject(): Encrypting volume key for subject '" << Subject->getMySubjectIDString() << "'" << endl;
    ByteVector passwordVectorByteVector = Subject->encryptForSubject(Subject, passwordVector);
    if (!passwordVector.size())
        throw EncryptionFailed("KeyronaVolume|addSubject(): Encryption for subject '" + Subject->getMySubjectName() + "' failed!");

    debug << "KeyronaVolume|addSubject(): Storing volume key for subject '" << Subject->getMySubjectIDString() << "'" << endl;
    string myBase64EncodedPassword = EncodeByteVectorToBASE64(passwordVectorByteVector);
    debug << "KeyronaVolume|addSubject(): BASE64 encoded encrypted passphrase: " << myBase64EncodedPassword << endl;

    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(KeyronaVolume_SubjectKeyDB);
    if (myVolumeStorage.findEntry(Subject->getMySubjectIDString()))
        debug << "KeyronaVolume|addSubject(): Subject '" << Subject->getMySubjectIDString() << "' was already in volume" << endl;
        
    myVolumeStorage.setEntry(Subject->getMySubjectIDString(), myBase64EncodedPassword);
    string message = "You have been added to volume '" + myVolumeUUID + "'";
    Subject->addMessageForUser(message);
};


//================================================================================
//
void   KeyronaVolume::deleteSubject(KeyronaSubject *Subject)
{
    debug << "KeyronaVolume|deleteSubject(): deleting subject '" << Subject->getMySubjectIDString() << "'" << endl;
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(KeyronaVolume_SubjectKeyDB);
    myVolumeStorage.deleteEntry(Subject->getMySubjectIDString());
    string message = "You have been deleted from volume '" + myVolumeUUID + "'";
    Subject->addMessageForUser(message);
};

//================================================================================
//
void KeyronaVolume::listSubjects()
{
    debug << "KeyronaVolume|listSubjects(): Listing all subjects" << endl;
    cout << "\t" << "The following subjects are available in volume '" << myVolumeUUID << "':" << endl;
    vector<string> myAvailableSubjects = getSubjects();
    if (myAvailableSubjects.size())
        printStringVector(&myAvailableSubjects);
    else
        cout << "\tnone" << endl;
};


//================================================================================
//
vector<string> KeyronaVolume::getSubjects()
{
    debug << "KeyronaVolume|getSubjects(): Retrieving all subjects" << endl;
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(KeyronaVolume_SubjectKeyDB);
    vector<string> myAvailableSubjects = myVolumeStorage.getAllEntries();
    return myAvailableSubjects;
};


//================================================================================
//
void   KeyronaVolume::addGroup(KeyronaGroup *Group, const string &VolumePassword)
{
    debug << "KeyronaVolume|addGroup(): Adding group '" << Group->getMyGroupID() << "'" << endl;

    if (VolumePassword.empty())
        throw InvalidPassword("KeyronaVolume|addGroup(): The supplied password was empty!");
    ByteVector passwordVector = convertStringToByteVector(VolumePassword);
	cout << "top" << endl;
    debug << "KeyronaVolume|addGroup(): Encrypting volume key for group '" << Group->getMyGroupID() << "'" << endl;
    ByteVector passwordVectorByteVector = Group->encryptForGroup(Group, passwordVector);
    if (!passwordVector.size())
        throw EncryptionFailed("KeyronaVolume|addGroup(): Encryption for group '" + Group->getMyGroupID() + "' failed!");

    debug << "KeyronaVolume|addGroup(): Storing volume key for group '" << Group->getMyGroupID() << "'" << endl;
    string myBase64EncodedPassword = EncodeByteVectorToBASE64(passwordVectorByteVector);
    debug << "KeyronaVolume|addGroup(): BASE64 encoded encrypted passphrase: " << myBase64EncodedPassword << endl;

    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(KeyronaVolume_GroupKeyDB);
    if (myVolumeStorage.findEntry(Group->getMyGroupID()))
        debug << "KeyronaVolume|addGroup(): Group '" << Group->getMyGroupID() << "' was already in volume, overwriting..." << endl;

    myVolumeStorage.setEntry(Group->getMyGroupID(), myBase64EncodedPassword);
    string message = "Group '" + Group->getMyGroupID() + "' has been added to volume '" + myVolumeUUID + "'";
    Group->addMessageForGroupMembers(message);

};


//================================================================================
//
void   KeyronaVolume::deleteGroup(KeyronaGroup *Group)
{
    debug << "KeyronaVolume|deleteGroup(): deleting group '" << Group->getMyGroupID() << "'" << endl;
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(KeyronaVolume_GroupKeyDB);

    if (Group->getMyGroupID() == KEYRONA_ADMIN_GROUP)
        throw InvalidGroup("The admin group cannot be deleted!");
    else
        myVolumeStorage.deleteEntry(Group->getMyGroupID());
    string message = "Group '" + Group->getMyGroupID() + "' has been deleted from volume '" + myVolumeUUID + "'";
    Group->addMessageForGroupMembers(message);
};

//================================================================================
//
void KeyronaVolume::listGroups()
{
    debug << "KeyronaVolume|listGroups(): Listing all groups" << endl;
    cout << "\t" << "The following groups are available in volume '" << myVolumeUUID << "':" << endl;
    vector<string> myAvailableGroups = getGroups();
    if (myAvailableGroups.size())
        printStringVector(&myAvailableGroups);
    else
        cout << "\tnone" << endl;
};


//================================================================================
//
vector<string> KeyronaVolume::getGroups()
{
    debug << "KeyronaVolume|getGroups(): Retrieving all groups" << endl;
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(KeyronaVolume_GroupKeyDB);
    vector<string> myAvailableGroups = myVolumeStorage.getAllEntries();
    return myAvailableGroups;
};


//================================================================================
//
string KeyronaVolume::getDecryptedVolumeKey(KeyronaSubject *Subject, string &password)
{
    if (!Subject->getMySubjectID())
        throw InvalidSubjectID("KeyronaVolume|getDecryptedVolumeKey(): Invalid Subject ID supplied!");

    if (Subject->isAdmin())
        throw InvalidSubjectID("KeyronaVolume|getDecryptedVolumeKey(): As admin, you are not allowed to use this method. Please use 'getDecryptedVolumeKeyByAdmin'");

    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(KeyronaVolume_SubjectKeyDB);

    debug << "KeyronaVolume|getDecryptedVolumeKey(): receiving BASE64-encoded key" << endl;
    string myBase64EncodedPassword = myVolumeStorage.getEntry(Subject->getMySubjectIDString());

    debug << "KeyronaVolume|getDecryptedVolumeKey(): decoding BASE64-encoded key" << endl;
    ByteVector myBase64EncodedPasswordByteVector = DecodeBASE64StringToByteVector(myBase64EncodedPassword);
    debug << "KeyronaVolume|getDecryptedVolumeKey(): decrypting key" << endl;
    ByteVector myDecode = Subject->decryptBySubject(Subject, myBase64EncodedPasswordByteVector, password);

    return convertByteVector2String(myDecode);
};

//================================================================================
//
string KeyronaVolume::getDecryptedVolumeKey(KeyronaGroup *Group, string &password)
{
    if (password.empty())
        throw InvalidPassword("KeyronaVolume|getDecryptedVolumeKey(): Empty password supplied!");

    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(KeyronaVolume_GroupKeyDB);

    debug << "KeyronaVolume|getDecryptedVolumeKey(): receiving BASE64-encoded key" << endl;
    string myBase64EncodedPassword = myVolumeStorage.getEntry(Group->getMyGroupID());

    debug << "KeyronaVolume|getDecryptedVolumeKey(): decoding BASE64-encoded key" << endl;
    ByteVector myDecodeByteVector = DecodeBASE64StringToByteVector(myBase64EncodedPassword);

    debug << "KeyronaVolume|getDecryptedVolumeKey(): decrypting key" << endl;

    ByteVector myDecode = Group->decryptByGroup(Group, myDecodeByteVector, password);

    return convertByteVector2String(myDecode);
};

//================================================================================
//
void   KeyronaVolume::printVolume()
{
    debug << "KeyronaVolume|printVolume(): Printing volume information" << endl;
    cout << "Volume UUID: '" << myVolumeUUID << "'" << endl;
    cout << "\t" << "Currently attached to: '" << myVolumePath << "'" << endl;
    cout << "\t" << "Encryption system: '";
    switch(myVolumeType)
    {
        case VOLUMETYPE_DMCRYPT: cout << KeyronaVolumeType_DMCRYPT; break;
        case VOLUMETYPE_TRUECRYPT: cout << KeyronaVolumeType_TRUECRYPT; break;
        case VOLUMETYPE_ECRYPTFS: cout << KeyronaVolumeType_ECRYPTFS; break;
        case VOLUMETYPE_LUKS: cout << KeyronaVolumeType_LUKS; break;
        case VOLUMETYPE_ENCFS: cout << KeyronaVolumeType_ENCFS; break;
        default: cout << "undefined!";
    }
    cout << "'" << endl;
};

//================================================================================
//
void   KeyronaVolume::deleteVolume()
{
    debug << "KeyronaVolume|deleteVolume(): Deleting volume '" + myVolumeUUID + "'" << endl;
    myVolumeStorage.deleteSection(myVolumeUUID);
};

//================================================================================
//
void KeyronaVolume::storeVolume()
{
    debug << "KeyronaVolume|storeVolume(): Storing volume '" + myVolumeUUID + "'" << endl;
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(KeyronaVolume_GenericDB);
    myVolumeStorage.setEntry(KeyronaVolume_isVolume, KeyronaVolume_isVolume_true);
    myVolumeStorage.setEntry(KeyronaVolume_VolumeUUID, myVolumeUUID);
    myVolumeStorage.setEntry(KeyronaVolume_VolumePATH, myVolumePath);

    switch(myVolumeType)
    {
        case VOLUMETYPE_DMCRYPT: myVolumeStorage.setEntry(KeyronaVolume_VolumeType, KeyronaVolumeType_DMCRYPT); break;
        case VOLUMETYPE_TRUECRYPT: myVolumeStorage.setEntry(KeyronaVolume_VolumeType, KeyronaVolumeType_TRUECRYPT); break;
        case VOLUMETYPE_ECRYPTFS: myVolumeStorage.setEntry(KeyronaVolume_VolumeType, KeyronaVolumeType_ECRYPTFS); break;
        case VOLUMETYPE_LUKS: myVolumeStorage.setEntry(KeyronaVolume_VolumeType, KeyronaVolumeType_LUKS); break;
        case VOLUMETYPE_ENCFS: myVolumeStorage.setEntry(KeyronaVolume_VolumeType, KeyronaVolumeType_ENCFS); break;

    }
};

//================================================================================
//
void KeyronaVolume::loadVolume()
{
    debug << "KeyronaVolume|loadVolume(): Loading volume '" + myVolumeUUID + "'" << endl;
    t_StringMap KeyronaVolumeTypeMap;
    string myVolumeTypeString;
    KeyronaVolumeTypeMap["DMCRYPT"] = VOLUMETYPE_DMCRYPT;
    KeyronaVolumeTypeMap["TRUECRYPT"] = VOLUMETYPE_TRUECRYPT;
    KeyronaVolumeTypeMap["ECRYPTFS"] = VOLUMETYPE_ECRYPTFS;
    KeyronaVolumeTypeMap["LUKS"] = VOLUMETYPE_LUKS;
    KeyronaVolumeTypeMap["ENCFS"] = VOLUMETYPE_ENCFS;
    
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(KeyronaVolume_GenericDB);
    myVolumeTypeString = myVolumeStorage.getEntry(KeyronaVolume_VolumeType);
    myVolumePath = myVolumeStorage.getEntry(KeyronaVolume_VolumePATH);

    switch(KeyronaVolumeTypeMap[myVolumeTypeString])
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
void KeyronaVolume::addSSS(string &SSSID)
{
    if (SSSID.empty())
        throw InvalidSSSID("KeyronaVolume|addSSS(): Empty SSSID supplied!");
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(KeyronaVolume_SSSDB);
    myVolumeStorage.setEntry(SSSID, KeyronaVolume_SSSDB_valid);
};

//================================================================================
//
vector<string> KeyronaVolume::getAvailableSSS()
{
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(KeyronaVolume_SSSDB);
    return myVolumeStorage.getAllEntries();
};

//================================================================================
//
void KeyronaVolume::deleteSSS(string &SSSID)
{
    if (SSSID.empty())
        throw InvalidSSSID("KeyronaVolume|deleteSSS(): Empty SSSID supplied!");
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(KeyronaVolume_SSSDB);
    myVolumeStorage.deleteEntry(SSSID);
};

//================================================================================
//
void KeyronaVolume::updateDevicePath(string &newDevicePath)
{
    if (!(fileExists(newDevicePath)))
        throw InvalidFilename("KeyronaVolume|updateDevicePath(): The supplied device name does not exist!");

    // set internal variable
    myVolumePath = newDevicePath;

    // store new value to database
    myVolumeStorage.selectSection(myVolumeUUID);
    myVolumeStorage.selectSubSection(KeyronaVolume_GenericDB);
    myVolumeStorage.setEntry(KeyronaVolume_VolumePATH, myVolumePath);
};

//================================================================================
//
bool KeyronaVolume::isSubjectInVolume(KeyronaSubject *Subject)
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
bool KeyronaVolume::isGroupInVolume(KeyronaGroup *Group)
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
