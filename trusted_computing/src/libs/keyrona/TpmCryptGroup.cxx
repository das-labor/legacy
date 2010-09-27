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
 * @file        TpmCryptGroup.cxx
 * @brief       Implementation of the TpmCrypt group class.
 * @version     $Revision: 670 $
 * @date        Last modification at $Date: 2009-06-24 16:32:59 +0200 (Wed, 24 Jun 2009) $ by $Author: selhorst $
 */

#include <TpmCryptGroup.hxx>

using namespace std;
using namespace utils;
using namespace tpmcrypt;

//================================================================================
//
TpmCryptGroup::TpmCryptGroup(const string GroupID, TpmCryptStorage &GroupStorage, TpmCryptStorage &UserStorage) :
            myGroupSubjectIDs (NULL),
            myGroupID (GroupID),
            myGroupKeyfile (""),
            myGroupKeyUUID (""),
            myGroupKeyType (""),
            myGroupKey (NULL),
            myGroupStorage (GroupStorage),
            mySubjectStorage  (UserStorage)
{
    if (myGroupID.empty())
        throw InvalidGroup("TpmCryptGroup: The supplied GroupID was empty!");

    // do we have such a group already?
    if (myGroupStorage.findSection(myGroupID))
    {
        debug << "TpmCryptGroup[" + myGroupID + "|Constructor()]: Group '" + myGroupID + "' already exists, using that group." << endl;
        loadGroup();
        return;
    }

    // if not, create a new one
    if (myGroupID != TPMCRYPT_ADMIN_GROUP)
        myGroupID = "Group_" + myGroupID;
    debug << "TpmCryptGroup[" + myGroupID + "|Constructor()]: Group '" + myGroupID + "' does not exist, creating new group." << endl;
    myGroupKeyfile = myKeyDirectory + TpmCryptPathSeparator + TpmCryptGroup_GroupKeyIdentifier  + TpmCryptFileSeparator + myGroupID + TpmCryptP15FileExtension;
        
    // Initialize RNG
    srand(time(NULL));

    // generate new random subjectID
    myGroupKeyUUID = convertUInt32toString((rand() % 100000 + 1));
    myGroupKeyType = "group";
        
    // generating random password for new key
    string myGroupKeyPassword = generateRandomString(TPMCRYPT_MINIMUM_PASSWORD_LENGTH);

    // creating new group key
    myGroupKey = new TpmCryptKey(this, myGroupKeyPassword);
    
    // creating test vector
    ByteVector testVector = convertStringToByteVector(myGroupID);
    ByteVector testVectorByteVector = encryptForGroup(this, testVector);
    string myBase64EncodedTestVector = EncodeByteVectorToBASE64(testVectorByteVector);

    // storing group
    storeGroup();

    // storing test vector
    myGroupStorage.selectSection(myGroupID);
    myGroupStorage.selectSubSection(TpmCryptGroup_GroupProperties);
    myGroupStorage.setEntry(TpmCryptGroup_TestVector, myBase64EncodedTestVector);

    // check if this group is the admin group
    if (myGroupID == TPMCRYPT_ADMIN_GROUP)
    {
        debug << "TpmCryptGroup[" + myGroupID + "|Constructor()]: Adminstrator group selected, encrypting group password for all available admins..." << endl;
        vector<StringPair> myAdmins = mySubjectStorage.findAllEntries(TpmCryptSubject_SubjectAdmin, TpmCryptSubject_TRUE);

        vector<utils::StringPair>::const_iterator Iterator;
        if (myAdmins.size())
        {
            Iterator=myAdmins.begin();
            while ( Iterator != myAdmins.end())
            {
                TpmCryptSubject *mySubject = new TpmCryptSubject (Iterator->first, mySubjectStorage);
                cout << "\tEncrypting group password for admin: " << mySubject->getMySubjectName() << " (" << mySubject->getMySubjectIDString() << ")" << endl;
                addSubjectToGroup(mySubject, myGroupKeyPassword);
                delete mySubject;
                Iterator++;
            }
        }
        else
        {
            deleteGroupByAdmin();
            throw InconsistentDatabase("TpmCryptGroup[" + myGroupID + "|Constructor()]: no admins found! Please create an administrator first!");
        }
    }
    else
    {
        // ENCRYPT GROUPKEY PASSWORD FOR ADMINS!
        debug << "TpmCryptGroup[" + myGroupID + "|Constructor()]: Encrypting group password for the admin group" << endl;
        TpmCryptGroup myAdminGroup(TPMCRYPT_ADMIN_GROUP, GroupStorage, UserStorage);
        ByteVector passwordVector = convertStringToByteVector(myGroupKeyPassword);
        ByteVector passwordVectorByteVector = myAdminGroup.encryptForGroup(this, passwordVector);

        if (!passwordVector.size())
            throw EncryptionFailed("TpmCryptGroup[" + myGroupID + "|Constructor()]: Encryption failed for admin group '" + TPMCRYPT_ADMIN_GROUP + "'!");

        // convert it into Base64
        string myBase64EncodedPassword = EncodeByteVectorToBASE64(passwordVectorByteVector);

        // select group keys section
        myGroupStorage.selectSection(myGroupID);
        myGroupStorage.selectSubSection(TpmCryptGroup_GroupKeys);
        myGroupStorage.setEntry(TPMCRYPT_ADMIN_GROUP, myBase64EncodedPassword);
    }
        
};


//================================================================================
//
TpmCryptGroup::~TpmCryptGroup()
{
    
};

//================================================================================
//
void    TpmCryptGroup::printGroup(bool verbose)
{
    cout << "GroupID: '" << myGroupID << "'" << endl;
    if (verbose)
    {
        cout << "\tGroupKeyfile: '" << myGroupKeyfile << "'" << endl;
        cout << "\tKey details:" << endl;
        myGroupKey->printKeyInformation();
    }
};

//================================================================================
//
void TpmCryptGroup::printSubjectsInGroup(bool verbose)
{
    vector<UInt32>::const_iterator Iterator;
    if (myGroupSubjectIDs.size())
    {
        cout << "Subjects in group: '" << myGroupID << "':" << endl;

        Iterator=myGroupSubjectIDs.begin();
        while ( Iterator != myGroupSubjectIDs.end())
        {
            TpmCryptSubject *mySubject = new TpmCryptSubject (*(Iterator), mySubjectStorage);
            mySubject->printSubject(verbose);
            delete mySubject;
            Iterator++;
        }
    }
    else
        cout << "No subjects in group '" << myGroupID << "'." << endl;
};

//================================================================================
//
void    TpmCryptGroup::deleteGroup()
{
    debug << "TpmCryptGroup[" << myGroupID << "|deleteGroup()] called!" << endl;
    if (myGroupID == TPMCRYPT_ADMIN_GROUP)
        throw InvalidGroup("The administrator group can not be deleted!");
    myGroupSubjectIDs.clear();
    myGroupStorage.deleteSection(myGroupID);
    myGroupID.clear();
};

//================================================================================
//
void    TpmCryptGroup::deleteGroupByAdmin()
{
    debug << "TpmCryptGroup[" << myGroupID << "|deleteGroupByAdmin()] called!" << endl;
    myGroupSubjectIDs.clear();
    myGroupStorage.deleteSection(myGroupID);
    myGroupID.clear();
};

//================================================================================
//
void    TpmCryptGroup::addSubjectToGroup(TpmCryptSubject *Subject, string &groupKeyPassword)
{
    debug << "TpmCryptGroup[" << myGroupID << "|addSubjectToGroup()] called!" << endl;
    if (! Subject->getMySubjectID())
        throw InvalidSubjectID("TpmCryptGroup["+myGroupID+"|addSubjectToGroup()]: Invalid SubjectID supplied!");

    // check if we are admin group and if subject is admin
    if ((myGroupID == TPMCRYPT_ADMIN_GROUP) && (! Subject->isAdmin()))
            throw InvalidSubjectID("TpmCryptGroup["+myGroupID+"|addSubjectToGroup()]: You are not allowed to add non-admin subjects to the admin group!");
    
    // add subject to group
    UInt32 mySID = Subject->getMySubjectID();
    vector<UInt32>::const_iterator Iterator;
    Iterator=myGroupSubjectIDs.begin();
    while ( Iterator != myGroupSubjectIDs.end())
    {
        if (*(Iterator) == mySID)
        {
            debug << "TpmCryptGroup["+myGroupID+"|addSubjectToGroup()]: Subject already in group!" << endl;
            return;
        }
        Iterator++;
    }

    debug << "TpmCryptGroup["+myGroupID+"|addSubjectToGroup()]: generating testvector" << endl;
    // generate reference testVector
    vector<UInt8> referenceTestVector = convertStringToUInt8Vector(myGroupID);

    debug << "TpmCryptGroup["+myGroupID+"|addSubjectToGroup()]: reading testvector from database" << endl;
    // get testVector from database
    myGroupStorage.selectSection(myGroupID);
    myGroupStorage.selectSubSection(TpmCryptGroup_GroupProperties);
    string encryptedTestVector = myGroupStorage.getEntry(TpmCryptGroup_TestVector);

    // decode testVector
    ByteVector decodedTestVector = DecodeBASE64StringToByteVector(encryptedTestVector);
	ByteVector decodedTestVectorByteVector;
	
    debug << "TpmCryptGroup["+myGroupID+"|addSubjectToGroup()]: decrypting testvector from database with given password" << endl;
    // decrypted testVector
    try
    {
        decodedTestVectorByteVector = decryptByGroup(this, decodedTestVector, groupKeyPassword);
    }
    catch ( std::exception &e )
    {
        cout << "Error: could not add subject to group, since the given group key password was invalid!" << endl;
        throw DecryptionFailed("TpmCryptGroup|addSubjectToGroup(): decryption of testvector failed!");
    }

    debug << "TpmCryptGroup["+myGroupID+"|addSubjectToGroup()]: verifying decrypted testvector with reference testvector" << endl;
    if (decodedTestVectorByteVector != referenceTestVector)
        throw DecryptionFailed("TpmCryptGroup|addSubjectToGroup(): decryption of testvector failed with given passphrase!");

    debug << "TpmCryptGroup["+myGroupID+"|addSubjectToGroup()]: verification successful!" << endl;

    debug << "TpmCryptGroup["+myGroupID+"|addSubjectToGroup()]: encrypting group key for new subject: '" << Subject->getMySubjectName() << "'" << endl;

    // encrypt group key password for subject
    ByteVector passwordVector = convertStringToByteVector(groupKeyPassword);
    ByteVector passwordVectorByteVector = Subject->encryptForSubject(Subject, passwordVector);
    if (!passwordVector.size())
        throw EncryptionFailed("TpmCryptGroup[" + myGroupID + "|Constructor()]: Encryption failed for admin '" + Subject->getMySubjectName() + "'!");

    // convert it into Base64
    string myBase64EncodedPassword = EncodeByteVectorToBASE64(passwordVectorByteVector);

    // store the encrypted key password for subejct
    // select keys section
    myGroupStorage.selectSection(myGroupID);
    myGroupStorage.selectSubSection(TpmCryptGroup_SubjectKeys);
    myGroupStorage.setEntry(Subject->getMySubjectIDString(), myBase64EncodedPassword);

    // add subject to groups subject list
    myGroupSubjectIDs.push_back(mySID);

    // store subject List
    storeSubjectList();
};

//================================================================================
//
void    TpmCryptGroup::deleteSubjectFromGroup(TpmCryptSubject *Subject)
{
    debug << "TpmCryptGroup[" << myGroupID << "|deleteSubjectFromGroup()] called!" << endl;

    if (! Subject->getMySubjectID())
        throw InvalidSubjectID("TpmCryptGroup["+myGroupID+"|addSubjectToGroup()]: Invalid SubjectID supplied!");

    UInt32 mySubjectToDelete = Subject->getMySubjectID();

    vector<UInt32>::iterator Iterator;
    Iterator=myGroupSubjectIDs.begin();
    while ( Iterator != myGroupSubjectIDs.end())
    {
        if (*(Iterator) == mySubjectToDelete)
        {
            debug << "TpmCryptGroup[" << myGroupID << "|deleteSubjectFromGroup()] Deleting subject '" << mySubjectToDelete << "'!" << endl;

            // deleting subject-encrypted groupkey
            myGroupStorage.selectSection(myGroupID);
            myGroupStorage.selectSubSection(TpmCryptGroup_SubjectKeys);
            
            // delete encrypted group key for subject
            myGroupStorage.deleteEntry(Subject->getMySubjectIDString());
            
            // deleting subject from groups subject list
            myGroupSubjectIDs.erase(Iterator);
            storeSubjectList();
            return;
        }
        Iterator++;
    }
};

//================================================================================
//
bool    TpmCryptGroup::isSubjectInGroup(TpmCryptSubject *Subject)
{
    debug << "TpmCryptGroup[" << myGroupID << "|isSubjectInGroup()] called!" << endl;

    if (! Subject->getMySubjectID())
        throw InvalidSubjectID("TpmCryptGroup["+myGroupID+"|isSubjectInGroup()]: Invalid SubjectID supplied!");
    
    vector<UInt32>::const_iterator Iterator;
    if (myGroupSubjectIDs.size())
    {
        UInt32 SubjectToFind = Subject->getMySubjectID();
        Iterator=myGroupSubjectIDs.begin();
        while ( Iterator != myGroupSubjectIDs.end())
        {
            if (*(Iterator) == SubjectToFind)
                return true;
            Iterator++;
        }
    }
    return false;
};

//================================================================================
//
void TpmCryptGroup::storeSubjectList()
{
    debug << "TpmCryptGroup[" << myGroupID << "|storeSubjects()] called!" << endl;
    myGroupStorage.selectSection(myGroupID);
    myGroupStorage.deleteSubSection(TpmCryptGroup_SubjectList);
    myGroupStorage.selectSubSection(TpmCryptGroup_SubjectList);

    vector<UInt32>::const_iterator Iterator;
    Iterator=myGroupSubjectIDs.begin();

    while ( Iterator != myGroupSubjectIDs.end())
    {
        debug << "TpmCryptGroup[" << myGroupID << "|storeSubjects()] Storing SubjectID: " << *(Iterator) << endl;
        myGroupStorage.setEntry(convertUInt32toString(*(Iterator)), convertUInt32toString(*(Iterator)));
        Iterator++;
    }
};

//================================================================================
//
void TpmCryptGroup::loadSubjectList()
{
    debug << "TpmCryptGroup[" << myGroupID << "|loadSubjects()] called!" << endl;
    myGroupStorage.selectSection(myGroupID);
    myGroupStorage.selectSubSection(TpmCryptGroup_SubjectList);
    vector <string> myEntries = myGroupStorage.getAllEntries();

    vector<string>::const_iterator Iterator;
    Iterator=myEntries.begin();
    myGroupSubjectIDs.clear();

    while ( Iterator != myEntries.end())
    {
        debug << "TpmCryptGroup[" << myGroupID << "|loadSubjects()] Loaded SubjectID: " << *(Iterator) << endl;
        myGroupSubjectIDs.push_back(convertStringtoUInt32(*(Iterator)));
        Iterator++;
    }
};

//================================================================================
//
void TpmCryptGroup::storeGroup()
{
    debug << "TpmCryptGroup[" << myGroupID << "|storeGroup()] called!" << endl;
    myGroupStorage.selectSection(myGroupID);
    myGroupStorage.selectSubSection(TpmCryptGroup_GroupProperties);
    myGroupStorage.setEntry(TpmCryptGroup_isGroup, TpmCryptGroup_isGroup_true);
    myGroupStorage.setEntry(TpmCryptGroup_GroupKeyfile, myGroupKeyfile);
	myGroupStorage.setEntry(TpmCryptGroup_GroupKeyUUID, myGroupKeyUUID);
	myGroupStorage.setEntry(TpmCryptGroup_GroupKeyType, myGroupKeyType);

    // assigning group and file permission
    assignFilePermission(myGroupKeyfile, KEYFILEPERMISSION);
    storeSubjectList();
};

//================================================================================
//
void TpmCryptGroup::loadGroup()
{
    debug << "TpmCryptGroup[" << myGroupID << "|loadGroup()] called!" << endl;
    myGroupStorage.selectSection(myGroupID);
    myGroupStorage.selectSubSection(TpmCryptGroup_GroupProperties);
    myGroupKeyUUID = myGroupStorage.getEntry(TpmCryptGroup_GroupKeyUUID);
    myGroupKeyType = myGroupStorage.getEntry(TpmCryptGroup_GroupKeyType);
    myGroupKeyfile = myGroupStorage.getEntry(TpmCryptGroup_GroupKeyfile);
    loadSubjectList();
};

//================================================================================
//
string  TpmCryptGroup::getDecryptedGroupKey(TpmCryptSubject *Subject, string &password)
{
    if (!Subject->getMySubjectID())
        throw InvalidSubjectID("TpmCryptGroup|getDecryptedGroupKey(): Invalid Subject ID supplied!");

    myGroupStorage.selectSection(myGroupID);
    myGroupStorage.selectSubSection(TpmCryptGroup_SubjectKeys);

    debug << "TpmCryptGroup|getDecryptedGroupKey(): receiving BASE64-encoded key" << endl;
    string myBase64EncodedPassword = myGroupStorage.getEntry(Subject->getMySubjectIDString());

    debug << "TpmCryptGroup|getDecryptedGroupKey(): decoding BASE64-encoded key" << endl;
    ByteVector myDecode = DecodeBASE64StringToByteVector(myBase64EncodedPassword);

    debug << "TpmCryptGroup|getDecryptedGroupKey(): decrypting key" << endl;
    ByteVector blub = Subject->decryptBySubject(Subject, myDecode, password);

    if (! myDecode.size() )
        throw DecryptionFailed("TpmCryptGroup|getDecryptedGroupKey(): decryption failed for subject '" + Subject->getMySubjectName() + "'!");

    return convertByteVector2String(blub);
};

//================================================================================
//
string  TpmCryptGroup::getDecryptedGroupKeyByAdmin(string &AdminKeyPassword)
{
    if (myGroupID == TPMCRYPT_ADMIN_GROUP)
        throw InvalidGroup("This operation is not allowed by the admin group!");

    debug << "TpmCryptGroup[" + myGroupID + "|getDecryptedGroupKeyByAdmin()]: Decrypting group password by the admin group" << endl;

    // selecting group keys section
    myGroupStorage.selectSection(myGroupID);
    myGroupStorage.selectSubSection(TpmCryptGroup_GroupKeys);

    // retrieving group key password, that is encrypted for the admin group
    string myBase64EncodedGroupKeyPassword = myGroupStorage.getEntry(TPMCRYPT_ADMIN_GROUP);

    // converting into vector<UInt8>
    ByteVector decodedGroupKeyPassword = DecodeBASE64StringToByteVector(myBase64EncodedGroupKeyPassword);

    // opening admin group
    TpmCryptGroup myAdminGroup(TPMCRYPT_ADMIN_GROUP, myGroupStorage, mySubjectStorage);
    
    // decrypting group key password with admin key
    decodedGroupKeyPassword = myAdminGroup.decryptByGroup(this, decodedGroupKeyPassword, AdminKeyPassword);

    return convertByteVector2String(decodedGroupKeyPassword);
};
//================================================================================
//
ByteVector  TpmCryptGroup::encryptForGroup(TpmCryptGroup *Group, ByteVector &toEncrypt)
{
    return myGroupKey->encrypt(NULL, this, toEncrypt);
};

//================================================================================
//
ByteVector  TpmCryptGroup::decryptByGroup(TpmCryptGroup *Group, ByteVector &toDecrypt, string &myPassword)
{
	return myGroupKey->decrypt(NULL, this, toDecrypt, myPassword);
};

void TpmCryptGroup::addMessageForGroupMembers(string message)
{
    if (! myGroupSubjectIDs.size())
        return;

    if ( ! message.size())
        throw NoMessage("TpmCryptGroup|addMessageForGroupMembers(): Empty message supplied!");

    // convert our subjectIDs into subjects
    vector<UInt32>::const_iterator Iterator;
    Iterator=myGroupSubjectIDs.begin();
    while ( Iterator != myGroupSubjectIDs.end())
    {
        TpmCryptSubject *mySubject = new TpmCryptSubject(*(Iterator), mySubjectStorage);
        mySubject->addMessageForUser(message);
        printToSyslog("TPMCRYPT: Group '" + myGroupID + "': Sending group message => '" + message + "'");
        delete mySubject;
        Iterator++;
    }
};
