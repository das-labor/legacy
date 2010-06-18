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
 * @file        KeyronaVolumeManagement.cxx
 * @brief       Implementation of the command line tool 'keyrona_manager'.
 * @version     $Revision: 789 $
 * @date        Last modification at $Date: 2009-08-04 13:45:17 +0200 (Tue, 04 Aug 2009) $ by $Author: selhorst $
 */

#include <Keyrona.hxx>
#include <KeyronaManager.hxx>
#include <KeyronaSSS.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

enum myMenu
{
    createVolume = 1,
    importVolume,
    deleteVolume,
    attachVolume,
    listVolumes,
    addSubjectToVolume,
    deleteSubjectFromVolume,
    listSubjectsInVolume,
    addGroupToVolume,
    deleteGroupFromVolume,
    listGroupsInVolume,
    addSSSToVolume,
    listSSSInVolume,
    deleteSSSFromVolume
};

// mapping between strings and int values
typedef map<string, int> t_StringMap;

//================================================================================
//
bool KeyronaListVolumes(KeyronaStorage &myVolumeStorage)
{
    vector<StringPair> myVolumes = myVolumeStorage.findAllEntries(KeyronaVolume_isVolume, KeyronaVolume_isVolume_true);

    vector<utils::StringPair>::const_iterator Iterator;
    if (myVolumes.size())
    {
        cout << "We have the following volumes: " << endl;

        Iterator=myVolumes.begin();
        while ( Iterator != myVolumes.end())
        {
            if (!KeyronaFindVolume(myVolumeStorage, Iterator->first))
            {
                Iterator++;
                continue;
            }

            KeyronaVolume myVolume(Iterator->first, myVolumeStorage);
            myVolume.printVolume();
            Iterator++;
        }
        return true;
    }
    else
    {
        cout << "No volumes found." << endl;
        return false;
    }
};

//================================================================================
//
void KeyronaAttachVolume(KeyronaStorage &myVolumeStorage, KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (! KeyronaListVolumes(myVolumeStorage))
        return;

    // get VolumeUUID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string VolumeUUID = selectFromStringVector(myVolumes, "\nNumber:\t\tVolume:", "Please select the according volume: ");

    if (!KeyronaFindVolume(myVolumeStorage, VolumeUUID))
        throw UnknownVolume("Volume '" + VolumeUUID + "' not found.");
    // load Volume
    KeyronaVolume myVolume(VolumeUUID, myVolumeStorage);

    string device;
    UInt8 EncryptionType = myVolume.getMyVolumeType();
    // get the desired volume, if !encfs and !cryptofs
    if (
            (EncryptionType == VOLUMETYPE_DMCRYPT) ||
            (EncryptionType == VOLUMETYPE_TRUECRYPT) ||
            (EncryptionType == VOLUMETYPE_LUKS)
        )
    {
          try
        {
            vector<StringPair> AvailableVolumes = getAvailableVolumes(UUIDpath);
            vector<string> AvailableDevices;
            while (AvailableVolumes.size())
            {
                StringPair current = AvailableVolumes.back();
                AvailableVolumes.pop_back();
                AvailableDevices.push_back(current.first);
            }

            device = selectFromStringVector(AvailableDevices, "Number:\t\tDevice:", "[Note: If the desired device has not been identified automatically, you can still enter the correct device (e.g., /dev/sdb1) below.]\nPlease select the device, where volume '" + VolumeUUID + "' shall be attached to:");
        }
        catch ( NoVolume &e)
        {
            cout << "Error: Could not identify any valid volumes." << endl;
            device = getStringFromUser("Please enter the device, where volume '" + VolumeUUID + "' shall be attached to: ");
        }
        cout << "Selected device '" << device << "'" << endl << endl;
    }
    else
    {
         cout << "Volume '" << VolumeUUID << "' uses an encryption scheme, which does not work on block devices directly, but requires a directory." << endl;
         device = getStringFromUser("Please enter the new target directory: ");
    }

    // verifying supplied device name
    if (!(fileExists(device)))
        throw InvalidFilename("Could not attach volume '" + VolumeUUID + "' to '" + device + "'!");
    myVolume.updateDevicePath(device);
    cout << "Volume '" << VolumeUUID << "' is now attached to: '" << myVolume.getMyVolumePath() << "'." << endl;
};


//================================================================================
//
void KeyronaDeleteVolume(KeyronaStorage &myVolumeStorage, KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage, KeyronaStorage &mySSSStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (! KeyronaListVolumes(myVolumeStorage))
        return;

    // get VolumeUUID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string VolumeUUID = selectFromStringVector(myVolumes, "\nNumber:\t\tVolume:", "Please enter the according volume to be deleted: ");

    if (!KeyronaFindVolume(myVolumeStorage, VolumeUUID))
        throw UnknownVolume("Volume '" + VolumeUUID + "' not found.");

    if (verifySelection())
    {
        // load Volume
        KeyronaVolume * myVolume = new KeyronaVolume(VolumeUUID, myVolumeStorage);

        // remove all existing SSSs
        vector<string> mySSSs = myVolume->getAvailableSSS();

        debug << "KeyronaDeleteVolume(): Removing all existing secret-sharing-schemes from volume '" << VolumeUUID << "'" << endl;
        while (mySSSs.size())
        {
            string currentSSS = mySSSs.back();
            mySSSs.pop_back();

            // load SSS
            KeyronaSSS mySSS(mySSSStorage, currentSSS);
            mySSS.deleteSSS();
            myVolume->deleteSSS(currentSSS);
            cout << "Successfully deleted secret-sharing-scheme with SSSID '" << currentSSS << "'" << endl;
            printToSyslog("KEYRONA: SSS '" + currentSSS + "' was removed from volume '" + VolumeUUID + "'");
        }

        myVolume->deleteVolume();
        delete myVolume;

        if (!KeyronaFindVolume(myVolumeStorage, VolumeUUID))
        {
            cout << "Successfully deleted volume '" << VolumeUUID << "' from the Keyrona system." << endl;
            cout << "Note: the physical volume itself has not been deleted!" << endl;
            printToSyslog("KEYRONA: Volume '" + VolumeUUID + "' deleted from this system");
        }
        else
        {
            cout << "Failed to delete volume '" << VolumeUUID << "'!" << endl;
        }
    }
};

//================================================================================
//
void findAvailableFilesystems(string fs, vector<string>* AvailableFilesystems)
{
    string mkfs;

    mkfs = "/sbin/mkfs." + fs;
    if (fileExists(mkfs))
    {
        debug << "Found '" << fs << "' at '" << mkfs << "'." << endl;
        AvailableFilesystems->push_back(fs);
    }

    mkfs.clear();
    mkfs = "/usr/sbin/mkfs." + fs;
    if (fileExists(mkfs))
    {
        debug << "Found '" << fs << "' at '" << mkfs << "'." << endl;
        AvailableFilesystems->push_back(fs);
    }
};

//================================================================================
//
void findAvailableCryptosystems(string cs, string binary, vector<string>* AvailableCryptoSystems)
{
    string crypto;
    crypto = "/bin/" + binary;
    if (fileExists(crypto))
    {
        debug << "Found '" << binary << "' at '" << crypto << "'." << endl;
        AvailableCryptoSystems->push_back(cs);
    }

    crypto.clear();
    crypto = "/usr/bin/" + binary;
    if (fileExists(crypto))
    {
        debug << "Found '" << binary << "' at '" << crypto << "'." << endl;
        AvailableCryptoSystems->push_back(cs);
    }

    crypto.clear();
    crypto = "/sbin/" + binary;
    if (fileExists(crypto))
    {
        debug << "Found '" << binary << "' at '" << crypto << "'." << endl;
        AvailableCryptoSystems->push_back(cs);
    }

    crypto.clear();
    crypto = "/usr/sbin/" + binary;
    if (fileExists(crypto))
    {
        debug << "Found '" << binary << "' at '" << crypto << "'." << endl;
        AvailableCryptoSystems->push_back(cs);
    }
};

//================================================================================
//
void KeyronaCreateVolume(KeyronaStorage &myVolumeStorage, KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage)
{
    UInt8 EncryptionType = 0;
    UInt8 FileSystem = 0;
    string FSString;
    string device;
    string UUID;

    vector<string> myAdmins = KeyronaFindAllAdmins(mySubjectStorage);
    if (! myAdmins.size())
    {
        throw NoAdminAvailable("No admins found, please add an administrator first!");
    }
    
    if (loginSuperUser())
        cout << "Superuser successfully logged in!" << endl;

    t_StringMap KeyronaVolumeTypeMap;
    KeyronaVolumeTypeMap["DMCRYPT"] = VOLUMETYPE_DMCRYPT;
    KeyronaVolumeTypeMap["TRUECRYPT"] = VOLUMETYPE_TRUECRYPT;
    KeyronaVolumeTypeMap["ECRYPTFS"] = VOLUMETYPE_ECRYPTFS;
    KeyronaVolumeTypeMap["LUKS"] = VOLUMETYPE_LUKS;
    KeyronaVolumeTypeMap["ENCFS"] = VOLUMETYPE_ENCFS;

    vector<string> AvailableEncryption;
    findAvailableCryptosystems("DMCRYPT", "cryptsetup", &AvailableEncryption);
    findAvailableCryptosystems("TRUECRYPT", "truecrypt", &AvailableEncryption);
    findAvailableCryptosystems("ECRYPTFS", "ecryptfs-manager", &AvailableEncryption);
    findAvailableCryptosystems("LUKS", "cryptsetup", &AvailableEncryption);
    findAvailableCryptosystems("ENCFS", "encfs", &AvailableEncryption);
    if (! AvailableEncryption.size())
        throw InvalidEncSystem("No encryption systems available. Make sure to at least install one of the supported crypto systems.");

    t_StringMap KeyronaVolumeFilesystemMap;
    KeyronaVolumeTypeMap["ext2"] = FSTYPE_EXT2;
    KeyronaVolumeTypeMap["ext3"] = FSTYPE_EXT3;
    KeyronaVolumeTypeMap["ext4"] = FSTYPE_EXT4;
    KeyronaVolumeTypeMap["ntfs"] = FSTYPE_NTFS;
    KeyronaVolumeTypeMap["reiserfs"] = FSTYPE_REISERFS;
    KeyronaVolumeTypeMap["vfat"] = FSTYPE_VFAT;
    KeyronaVolumeTypeMap["xfs"] = FSTYPE_XFS;

    vector<string> AvailableFilesystems;
    findAvailableFilesystems("ext2", &AvailableFilesystems);
    findAvailableFilesystems("ext3", &AvailableFilesystems);
    findAvailableFilesystems("ext4", &AvailableFilesystems);
    findAvailableFilesystems("ntfs", &AvailableFilesystems);
    findAvailableFilesystems("reiserfs", &AvailableFilesystems);
    findAvailableFilesystems("vfat", &AvailableFilesystems);
    findAvailableFilesystems("xfs", &AvailableFilesystems);

    // first, get the encryption scheme
    string EncTypeString = selectFromStringVector(AvailableEncryption, "Number:\t\tEncryption Scheme:", "Please enter the desired encryption scheme: ");
    cout << "Selected encryption scheme '" << EncTypeString << "'" << endl << endl;

    switch(KeyronaVolumeTypeMap[EncTypeString])
    {
        case VOLUMETYPE_DMCRYPT: EncryptionType = VOLUMETYPE_DMCRYPT; break;
        case VOLUMETYPE_TRUECRYPT: EncryptionType = VOLUMETYPE_TRUECRYPT; break;
        case VOLUMETYPE_ECRYPTFS: EncryptionType = VOLUMETYPE_ECRYPTFS; break;
        case VOLUMETYPE_LUKS: EncryptionType = VOLUMETYPE_LUKS; break;
        case VOLUMETYPE_ENCFS: EncryptionType = VOLUMETYPE_ENCFS; break;
    }
    if (!EncryptionType)
        throw NotCreated("Please enter valid encryption type!");

    // get the desired volume, if !encfs and !cryptofs
    if (
            (EncryptionType == VOLUMETYPE_DMCRYPT) ||
            (EncryptionType == VOLUMETYPE_TRUECRYPT) ||
            (EncryptionType == VOLUMETYPE_LUKS)
        )
    {
        try
        {
            vector<StringPair> AvailableVolumes = getAvailableVolumes(UUIDpath);
            vector<string> AvailableDevices;
            while (AvailableVolumes.size())
            {
                StringPair current = AvailableVolumes.back();
                AvailableVolumes.pop_back();
                if (! isDeviceMounted(current.first))
                    AvailableDevices.push_back(current.first);
            }

            device = selectFromStringVector(AvailableDevices, "Number:\t\tDevice:", "[Note: If the desired device has not been identified automatically, you can still enter the correct device (e.g., /dev/sdb1) below.]\nPlease select the device to be created:");
        }
        catch ( NoVolume &e)
        {
            cout << "Error: Could not identify any valid volumes." << endl;
            device = getStringFromUser("Please enter the device to be created: ");
        }
        if (isDeviceMounted(device))
            throw InvalidFilename("The device you selected is currently mounted. Please unmount it first, before re-creating the volume.");
        cout << "Selected device '" << device << "'" << endl << endl;
    }
    else
    {
         cout << "You selected an encryption scheme, which does not work on block devices directly, but requires a directory." << endl;
         cout << "In order to use it on a block device, please create a filesystem first and mount it to the target directory." << endl;
         device = getStringFromUser("Please enter the target directory: ");
         cout << "Keyrona will now create a new key for target '" << device << "' to be used with '" << EncTypeString << "'" << endl;
         if (! dirExists(device))
             throw DirNotExist("The directory '" + device + "' does not exist!");
    }

    UUID = getStringFromUser("Please enter a unique identifier for device '" + device + "': ");
    if (UUID.empty())
        throw NotCreated("Please enter a valid identifier!");

    if (KeyronaFindVolume(myVolumeStorage, UUID))
        throw VolumeExists("KeyronaCreateVolume: The desired volume already exists in our database!\n");

    if (
            (EncryptionType == VOLUMETYPE_DMCRYPT) ||
            (EncryptionType == VOLUMETYPE_TRUECRYPT) ||
            (EncryptionType == VOLUMETYPE_LUKS)
        )
    {
        cout << "WARNING: All data will be permanently lost on device '" << device << "'" << endl;
        // get the desired filesystem
        FSString = selectFromStringVector(AvailableFilesystems, "Number:\t\tFilesystem:", "Please enter the desired filesystem for volume '" + UUID + "': ");
        cout << "Selected filesystem scheme '" << FSString << "'" << endl << endl;

        switch(KeyronaVolumeTypeMap[FSString])
        {
            case FSTYPE_EXT2: FileSystem = FSTYPE_EXT2; break;
            case FSTYPE_EXT3: FileSystem = FSTYPE_EXT3; break;
            case FSTYPE_EXT4: FileSystem = FSTYPE_EXT4; break;
            case FSTYPE_NTFS: FileSystem = FSTYPE_NTFS; break;
            case FSTYPE_REISERFS: FileSystem = FSTYPE_REISERFS; break;
            case FSTYPE_VFAT: FileSystem = FSTYPE_VFAT; break;
            case FSTYPE_XFS: FileSystem = FSTYPE_XFS; break;
        }
        if (!FileSystem)
            throw NotCreated("Please enter valid filesystem!");
    }

    // generate new device passphrase
    debug << "Generating new volume password" << endl;

    string VolumePassword = generateRandomString(KEYRONA_MINIMUM_PASSWORD_LENGTH);
    cout << VolumePassword << endl;
    if (verifySelection())
    {
        string myCommand;
        // creating volume!
        switch(EncryptionType)
        {
            case VOLUMETYPE_DMCRYPT: myCommand = (myScriptDirectory + KeyronaPathSeparator + "create_dmcrypt.sh " + device + " " + FSString + " " + VolumePassword ); break;
            case VOLUMETYPE_TRUECRYPT: myCommand = (myScriptDirectory + KeyronaPathSeparator + "create_truecrypt.sh " + device + " " + FSString + " " + VolumePassword ); break;
            case VOLUMETYPE_LUKS: myCommand = (myScriptDirectory + KeyronaPathSeparator + "create_cryptsetup_luks.sh " + device + " " + FSString + " " + VolumePassword ); break;
            case VOLUMETYPE_ENCFS: myCommand = (myScriptDirectory + KeyronaPathSeparator + "create_encfs.sh " + device + " " + VolumePassword ); break;
        }

        // note: we don't need a setup script for ecryptfs
        if (myCommand.size())
        {
            debug << "Now executing: " << myCommand << endl;
            int ret = system(myCommand.c_str());
            if (ret)
                throw NotCreated("Volume could not be created!");
        }

        cout << "Successfully created volume '" << device << "'." << endl;
        printToSyslog("KEYRONA: Volume '" + device + "' created");

        cout << "Importing volume '" << device << "' into Keyrona." << endl;

        // load admin group
        KeyronaGroup *myAdminGroup = new KeyronaGroup(KEYRONA_ADMIN_GROUP, myGroupStorage, mySubjectStorage);

        // import volume
        KeyronaVolume newVolume(UUID, device, VolumePassword, EncryptionType, myAdminGroup, myVolumeStorage);

        // cleanup
        delete myAdminGroup;
        if (!KeyronaFindVolume(myVolumeStorage, UUID))
            throw NotCreated("Volume could not be imported!");

        cout << "Successfully imported volume '" << newVolume.getMyVolumeUUID() << "'." << endl;
        printToSyslog("KEYRONA: Volume '" + UUID + "' imported into this system");
        newVolume.printVolume();
    }
};

//================================================================================
//
void KeyronaImportVolume(KeyronaStorage &myVolumeStorage, KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    UInt8 EncryptionType = 0;
    string device;
    string UUID;

    t_StringMap KeyronaVolumeTypeMap;
    KeyronaVolumeTypeMap["DMCRYPT"] = VOLUMETYPE_DMCRYPT;
    KeyronaVolumeTypeMap["TRUECRYPT"] = VOLUMETYPE_TRUECRYPT;
    KeyronaVolumeTypeMap["ECRYPTFS"] = VOLUMETYPE_ECRYPTFS;
    KeyronaVolumeTypeMap["LUKS"] = VOLUMETYPE_LUKS;
    KeyronaVolumeTypeMap["ENCFS"] = VOLUMETYPE_ENCFS;
    vector<string> AvailableEncryption;
    string enc = "DMCRYPT";
    AvailableEncryption.push_back(enc);
    enc = "TRUECRYPT";
    AvailableEncryption.push_back(enc);
    enc = "ECRYPTFS";
    AvailableEncryption.push_back(enc);
    enc = "LUKS";
    AvailableEncryption.push_back(enc);
    enc = "ENCFS";
    AvailableEncryption.push_back(enc);

    // first, get the encryption scheme
    string EncTypeString = selectFromStringVector(AvailableEncryption, "Number:\t\tEncryption Scheme:", "Please enter the desired encryption scheme: ");
    cout << "Selected encryption scheme '" << EncTypeString << "'" << endl << endl;

    switch(KeyronaVolumeTypeMap[EncTypeString])
    {
        case VOLUMETYPE_DMCRYPT: EncryptionType = VOLUMETYPE_DMCRYPT; break;
        case VOLUMETYPE_TRUECRYPT: EncryptionType = VOLUMETYPE_TRUECRYPT; break;
        case VOLUMETYPE_ECRYPTFS: EncryptionType = VOLUMETYPE_ECRYPTFS; break;
        case VOLUMETYPE_LUKS: EncryptionType = VOLUMETYPE_LUKS; break;
        case VOLUMETYPE_ENCFS: EncryptionType = VOLUMETYPE_ENCFS; break;
    }
    if (!EncryptionType)
        throw NotCreated("Please enter valid encryption type!");

    // get the desired volume, if !encfs and !cryptofs
    if (
            (EncryptionType == VOLUMETYPE_DMCRYPT) ||
            (EncryptionType == VOLUMETYPE_TRUECRYPT) ||
            (EncryptionType == VOLUMETYPE_LUKS)
        )
    {
        try
        {
            vector<StringPair> AvailableVolumes = getAvailableVolumes(UUIDpath);
            vector<string> AvailableDevices;
            while (AvailableVolumes.size())
            {
                StringPair current = AvailableVolumes.back();
                AvailableVolumes.pop_back();
                AvailableDevices.push_back(current.first);
            }

            device = selectFromStringVector(AvailableDevices, "Number:\t\tDevice:", "[Note: If the desired device has not been identified automatically, you can still enter the correct device (e.g., /dev/sdb1) below.]\nPlease select the device to be created:");
        }
        catch ( NoVolume &e)
        {
            cout << "Error: Could not identify any valid volumes." << endl;
            device = getStringFromUser("Please enter the device to be created: ");
        }
        cout << "Selected device '" << device << "'" << endl << endl;
    }
    else
    {
         cout << "You selected an encryption scheme, which does not work on block devices directly, but requires a directory." << endl;
         device = getStringFromUser("Please enter the target directory of the existing '" + EncTypeString + "' installation: ");
         cout << "Selected device '" << device << "'" << endl << endl;
    }

    UUID = getStringFromUser("Please enter a unique identifier for device '" + device + "': ");

    if (KeyronaFindVolume(myVolumeStorage, UUID))
        throw VolumeExists("KeyronaCreateVolume: The desired volume already exists in our database!\n");

    string VolumePassword = getPasswordVerified("Please enter the according password for '" + device + "': ");

    // load admin group
    KeyronaGroup *myAdminGroup = new KeyronaGroup(KEYRONA_ADMIN_GROUP, myGroupStorage, mySubjectStorage);

    // import volume
    KeyronaVolume newVolume(UUID, device, VolumePassword, EncryptionType, myAdminGroup, myVolumeStorage);

    // cleanup
    delete myAdminGroup;

    if (!KeyronaFindVolume(myVolumeStorage, UUID))
        throw NotCreated("Volume could not be imported!");

    cout << "Successfully imported volume '" << newVolume.getMyVolumeUUID() << "'." << endl;
    printToSyslog("KEYRONA: Volume '" + UUID + "' imported into this system");
    newVolume.printVolume();
};

//================================================================================
//
void KeyronaAddSubjectToVolume(KeyronaStorage &myVolumeStorage, KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (!KeyronaListVolumes(myVolumeStorage))
        return;

    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string volumeID = selectFromStringVector(myVolumes, "\nNumber:\t\tUUID:", "Please select the desired volume:");

    if (!KeyronaFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    KeyronaVolume myVolume(volumeID, myVolumeStorage);

    // get all Users
    vector<string> myUsers = KeyronaFindAllUsers(mySubjectStorage, false);
    // get all Token
    vector<string> myToken = KeyronaFindAllToken(mySubjectStorage);

    // add Token to Userlist
    myUsers.insert(myUsers.end(), myToken.begin(), myToken.end());

    if (!myUsers.size())
    {
        cout << "No users / token available!" << endl;
        return;
    }

    // load all subjects already in volume
    vector<string> mySubjectsInVolume = myVolume.getSubjects();
    
    // remove existing users from volume user list
    vector<string>::const_iterator Iterator;
    Iterator=mySubjectsInVolume.begin();
    while ( Iterator != mySubjectsInVolume.end())
    {
        KeyronaSubject *mySubject = new KeyronaSubject(*(Iterator), mySubjectStorage);
        removeStringFromStringVector(&myUsers, mySubject->getMySubjectName());
        delete mySubject;
        Iterator++;
    }

    if (!myUsers.size())
    {
        cout << "No more users / token available to be added to volume '" << volumeID << "'!" << endl;
        return;
    }

    string subjectID = selectFromStringVector(myUsers, "Number:\t\tSubject:", "Please select the subject to be added:");

    if (!KeyronaFindSubject(mySubjectStorage, subjectID))
        throw UnknownSubject("Subject '" + subjectID + "' not found.");

    // load subject
    KeyronaSubject mySubject(subjectID, mySubjectStorage);

    // is entered subject already in volume?
    if (myVolume.isSubjectInVolume(&mySubject))
        throw InvalidUID("Subject '" + mySubject.getMySubjectName() + "' is already in volume '" + volumeID + "'!");

    // load admin
    KeyronaSubject *myExistingAdmin = getCurrentAdmin(mySubjectStorage);
    
    string myPassword = getPassword("Please enter the according password for admin '" + myExistingAdmin->getMySubjectName() + "' (" + myExistingAdmin->getMySubjectIDString() + "): ");

    // get admin key
    KeyronaGroup myAdminGroup(KEYRONA_ADMIN_GROUP, myGroupStorage, mySubjectStorage);
    string myAdminKeyPassword = myAdminGroup.getDecryptedGroupKey(myExistingAdmin, myPassword);

    // get volume key
    string myVolumePassword = myVolume.getDecryptedVolumeKey(&myAdminGroup, myAdminKeyPassword);
    // finally, add subject
    myVolume.addSubject(&mySubject, myVolumePassword);
    cout << "Successfully added subject '" << subjectID << "' to volume '" << volumeID << "'." << endl;
};

//================================================================================
//
void KeyronaDeleteSubjectFromVolume(KeyronaStorage &myVolumeStorage, KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (!KeyronaListVolumes(myVolumeStorage))
        return;

    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string volumeID = selectFromStringVector(myVolumes, "Number:\t\tUUID:", "Please select the desired volume:");

    if (!KeyronaFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    KeyronaVolume myVolume(volumeID, myVolumeStorage);

    // get all subjects in volume
    vector<string> mySubjectsInVolume = myVolume.getSubjects();
    if (!mySubjectsInVolume.size())
    {
        cout << "No subjects exist in volume!" << endl;
        return;
    }

    // convert our subjectIDs into names
    vector<string> mySubjectNamesInVolume;
    vector<string>::const_iterator Iterator;
    Iterator=mySubjectsInVolume.begin();
    while ( Iterator != mySubjectsInVolume.end())
    {
        KeyronaSubject *mySubject = new KeyronaSubject(*(Iterator), mySubjectStorage);
        mySubjectNamesInVolume.push_back(mySubject->getMySubjectName());
        delete mySubject;
        Iterator++;
    }
    // select the subject to be deleted
    string subjectID = selectFromStringVector(mySubjectNamesInVolume, "Number:\t\tUser:", "Please select the subject to be deleted:");

    if (!KeyronaFindSubject(mySubjectStorage, subjectID))
        throw UnknownSubject("Subject '" + subjectID + "' not found.");

    // load subject
    KeyronaSubject mySubject(subjectID, mySubjectStorage);

    // is entered subject in volume?
    if (! myVolume.isSubjectInVolume(&mySubject))
        throw InvalidUID("Subject '" + mySubject.getMySubjectName() + "' is not in volume '" + volumeID + "'!");

    if (verifySelection())
    {
        myVolume.deleteSubject(&mySubject);
        cout << "Successfully deleted subject '" << subjectID << "' from volume '" << volumeID << "'." << endl;
    }
};

//================================================================================
//
void KeyronaListSubjectsInVolume(KeyronaStorage &myVolumeStorage, KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage)
{

    if (!KeyronaListVolumes(myVolumeStorage))
        return;

    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string volumeID = selectFromStringVector(myVolumes, "\nNumber:\t\tUUID:", "Please select the desired volume:");

    if (!KeyronaFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    KeyronaVolume myVolume(volumeID, myVolumeStorage);

    vector<string> mySubjectsInVolume = myVolume.getSubjects();
    if (!mySubjectsInVolume.size())
    {
        cout << "No subjects exist in volume!" << endl;
        return;
    }

    cout << "The following subjects have access to volume '" << volumeID << "':" << endl;
    vector<string>::const_iterator Iterator;
    Iterator=mySubjectsInVolume.begin();
    while ( Iterator != mySubjectsInVolume.end())
    {
        KeyronaSubject *mySubject = new KeyronaSubject(*(Iterator), mySubjectStorage);
        mySubject->printSubject(printVerbose);
        delete mySubject;
        Iterator++;
    }
};


//================================================================================
//
void KeyronaAddGroupToVolume(KeyronaStorage &myVolumeStorage, KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (!KeyronaListVolumes(myVolumeStorage))
        return;

    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string volumeID = selectFromStringVector(myVolumes, "\nNumber:\t\tUUID:", "Please select the desired volume:");

    if (!KeyronaFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    KeyronaVolume myVolume(volumeID, myVolumeStorage);

    // get groupID
    vector<string> myGroups = KeyronaFindAllGroups(myGroupStorage, mySubjectStorage, false);
    if (!myGroups.size())
    {
        cout << "No groups available!" << endl;
        return;
    }

    // load all groups already in volume
    vector<string> myGroupsInVolume = myVolume.getGroups();

    // remove existing groups from volume group list
    vector<string>::const_iterator Iterator;
    Iterator=myGroupsInVolume.begin();
    while ( Iterator != myGroupsInVolume.end())
    {
        removeStringFromStringVector(&myGroups, *(Iterator));
        Iterator++;
    }
    if (!myGroups.size())
    {
        cout << "No more groups available to be added to volume '" << volumeID << "'!" << endl;
        return;
    }

    string groupID = selectFromStringVector(myGroups, "Number:\t\tGroup:", "Please select the group to be added:");
    if (groupID == KEYRONA_ADMIN_GROUP)
        throw InvalidGroup("Group '" + groupID + "' is not allowed to be added manually!");

    if (!KeyronaFindGroup(myGroupStorage, groupID))
        throw UnknownGroup("Group '" + groupID + "' not found.");

    // load user group
    KeyronaGroup myGroup(groupID, myGroupStorage, mySubjectStorage);

    // is entered group already in volume?
    if (myVolume.isGroupInVolume(&myGroup))
        throw InvalidGroup("Group '" + myGroup.getMyGroupID() + "' is already in volume '" + volumeID + "'!");

    // load admin
    KeyronaSubject *myExistingAdmin = getCurrentAdmin(mySubjectStorage);

    string myPassword = getPassword("Please enter the according password for admin '" + myExistingAdmin->getMySubjectName() + "' (" + myExistingAdmin->getMySubjectIDString() + "): ");

    // load admin group
    KeyronaGroup myAdminGroup(KEYRONA_ADMIN_GROUP, myGroupStorage, mySubjectStorage);
    string myAdminGroupKeyPassword = myAdminGroup.getDecryptedGroupKey(myExistingAdmin, myPassword);

    // load the volume key
    string myVolumeKey = myVolume.getDecryptedVolumeKey(&myAdminGroup, myAdminGroupKeyPassword);
	cout << myVolumeKey << endl;
    // finally, add the group
    myVolume.addGroup(&myGroup, myVolumeKey);
    cout << "Successfully added group '" << groupID << "' to volume '" << volumeID << "'." << endl;
};

//================================================================================
//
void KeyronaDeleteGroupFromVolume(KeyronaStorage &myVolumeStorage, KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (!KeyronaListVolumes(myVolumeStorage))
        return;

    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string volumeID = selectFromStringVector(myVolumes, "\nNumber:\t\tUUID:", "Please select the desired volume:");

    if (!KeyronaFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    KeyronaVolume myVolume(volumeID, myVolumeStorage);

    // get groupID
    vector<string> groupsInVolume = myVolume.getGroups();
    // remove the admin group from selection list
    removeStringFromStringVector(&groupsInVolume, KEYRONA_ADMIN_GROUP);
    if (!groupsInVolume.size())
    {
        cout << "No groups exist in volume '" << volumeID << "'!" << endl;
        return;
    }

    string groupID = selectFromStringVector(groupsInVolume, "Number:\t\tGroup:", "Please select the group to be deleted:");

    if (!KeyronaFindGroup(myGroupStorage, groupID))
        throw UnknownGroup("Group '" + groupID + "' not found.");

    if (groupID == KEYRONA_ADMIN_GROUP)
        throw InvalidGroup("Group '" + groupID + "' is not allowed to be deleted manually!");

    // load user group
    KeyronaGroup myGroup(groupID, myGroupStorage, mySubjectStorage);

    // is entered group in volume?
    if (! myVolume.isGroupInVolume(&myGroup))
        throw InvalidGroup("Group '" + myGroup.getMyGroupID() + "' is not in volume '" + volumeID + "'!");

    if (verifySelection())
    {
        // delete volume
        myVolume.deleteGroup(&myGroup);
        cout << "Successfully deleted group '" << groupID << "' from volume '" << volumeID << "'." << endl;
    }
};

//================================================================================
//
void KeyronaListGroupsInVolume(KeyronaStorage &myVolumeStorage, KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage)
{
    if (!KeyronaListVolumes(myVolumeStorage))
        return;

    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string volumeID = selectFromStringVector(myVolumes, "\nNumber:\t\tUUID:", "Please select the desired volume:");

    if (!KeyronaFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    KeyronaVolume myVolume(volumeID, myVolumeStorage);

    // get all existing groups in volume
    vector<string> groupsInVolume = myVolume.getGroups();
    if (!groupsInVolume.size())
    {
        cout << "No groups exist in volume '" << volumeID << "'!" << endl;
        return;
    }

    if ((groupsInVolume.size() == 1) && (groupsInVolume[0] == KEYRONA_ADMIN_GROUP))
    {
        cout << "Only the admin group exists in volume '" << volumeID << "'!" << endl;
        return;
    }

    cout << "The following groups have access to volume '" << volumeID << "':" << endl;
    vector<string>::const_iterator Iterator;
    Iterator=groupsInVolume.begin();
    while ( Iterator != groupsInVolume.end())
    {
        KeyronaGroup *myGroup = new KeyronaGroup(*(Iterator), myGroupStorage, mySubjectStorage);
        if (myGroup->getMyGroupID() != KEYRONA_ADMIN_GROUP)
            myGroup->printGroup(printVerbose);
        delete myGroup;
        Iterator++;
    }
};

//================================================================================
//
void KeyronaAddSSSToVolume(KeyronaStorage &myVolumeStorage, KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage, KeyronaStorage &mySSSStorage)
{
    // load admin -> we do not require a call to loginAdmin(), since the admin is needed anyway. Therefore no dual login is required
    KeyronaSubject *myExistingAdmin = getCurrentAdmin(mySubjectStorage);

    string myPassword = getPassword("Please enter the according password for admin '" + myExistingAdmin->getMySubjectName() + "' (" + myExistingAdmin->getMySubjectIDString() + "): ");

    if (!KeyronaListVolumes(myVolumeStorage))
        return;

    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string volumeID = selectFromStringVector(myVolumes, "\nNumber:\t\tUUID:", "Please select the desired volume:");

    if (!KeyronaFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    KeyronaVolume myVolume(volumeID, myVolumeStorage);


    // load admin group
    KeyronaGroup myAdminGroup(KEYRONA_ADMIN_GROUP, myGroupStorage, mySubjectStorage);
    string myAdminGroupKeyPassword = myAdminGroup.getDecryptedGroupKey(myExistingAdmin, myPassword);

    // load the volume key by admin
    string myVolumeKey = myVolume.getDecryptedVolumeKey(&myAdminGroup, myAdminGroupKeyPassword);

    string SSSID = getStringFromUser("Please enter an identifier for the Secret-Sharing-Scheme: ");
    if (SSSID.empty())
        throw InvalidSSSID("Invalid SSSID entered!");
    if (KeyronaFindSSS(mySSSStorage, SSSID))
        throw InvalidSSSID("A secret-sharing-scheme already exists with SSSID '" + SSSID + "'");

    // get all Users
    vector<string> myUsers = KeyronaFindAllUsers(mySubjectStorage, false);
    // get all Token
    vector<string> myToken = KeyronaFindAllToken(mySubjectStorage);
    // get all Platforms
    vector<string> myPlatforms = KeyronaFindAllPlatforms(mySubjectStorage);

    // add Token to Userlist
    myUsers.insert(myUsers.end(), myToken.begin(), myToken.end());
    // add Platforms to Userlist
    myUsers.insert(myUsers.end(), myPlatforms.begin(), myPlatforms.end());

    if (!myUsers.size())
    {
        cout << "No users / token / platforms available!" << endl;
        return;
    }

    UInt32 n = 0;
    UInt32 m = 0;

        // -------------
    // ADD GROUP?
    KeyronaGroup *myGroup = NULL;
    vector<string> myGroups = KeyronaFindAllGroups(myGroupStorage, mySubjectStorage, false);

    if (myGroups.size())
    {
        // get groupID
        string groupID = selectFromStringVector(myGroups, "Number:\t\tGroup:", "If you want to add a group to this SSS, please select the group to be added (-1 for no group):");

        if (groupID != "-1")
        {
            if (groupID == KEYRONA_ADMIN_GROUP)
                throw InvalidGroup("Group '" + groupID + "' is not allowed to be added manually!");

            if (!KeyronaFindGroup(myGroupStorage, groupID))
                throw UnknownGroup("Group '" + groupID + "' not found.");

            // load user group
            myGroup = new KeyronaGroup(groupID, myGroupStorage, mySubjectStorage);
            m++;
            cout << "Please add the group to the userlist while mounting, e.g., 'keyrona_mount --mount --user myUser," << groupID << " --volume /dev/sdxy --path /some/mointpoint" << endl;
        }
    }

    string toStop = "-1";
    string subjectID;
    vector<KeyronaSubject*> Participants;
    while ((subjectID.compare(0, toStop.length(), toStop)) && (myUsers.size()))
    {
        subjectID = selectFromStringVector(myUsers, "Number:\t\tSubject:", "Please select further participants for this SSS (-1 to stop): ");
        if (subjectID == toStop)
            break;
        if (!KeyronaFindSubject(mySubjectStorage, subjectID))
            throw UnknownSubject("Subject '" + subjectID + "' not found.");

        Participants.push_back ( new KeyronaSubject(subjectID, mySubjectStorage));
        m++;
        cout << "Successfully added subject '" << (Participants.back())->getMySubjectName() << "' to SSS '" << SSSID << "'." << endl;
        cout << "New amount of participants: " << m << endl;
        removeStringFromStringVector(&myUsers, subjectID);
    }

    cout << "You selected " << m << " participants!" << endl;

    switch(m)
    {
        case 0: throw NotEnoughParticipants("KeyronaAddSSS: Invalid amount of overall participants (m)!");
        case 1: throw NotEnoughParticipants("KeyronaAddSSS: You selected, that one participant alone can gain access. Please add user or group directly!");
        case 2: { n=2; break; }
        default: 
        {
            n = convertStringtoUInt32(getStringFromUser("How many of the above " + convertUInt32toString(m) + " participants have to be available to retrieve the key: "));
            break;
        }
    }

    switch(n)
    {
        case 0: throw NotEnoughParticipants("KeyronaAddSSS: Invalid amount of participants (n)!");
        case 1: throw NotEnoughParticipants("KeyronaAddSSS: You selected, that one participant alone can gain access. Please add user or group directly!");
    }

    if (n>m)
        throw NotEnoughParticipants("KeyronaAddSSS: Invalid Selection, n > m!");

    cout << "You selected a " << n << " (n) out of " << m << " (m) scheme." << endl;

    KeyronaSSS *mySSS = NULL;
    try
    {
        // do we have a group in the SSS?
        if (myGroup)
        {
            mySSS = new KeyronaSSS(mySSSStorage, SSSID, n, Participants, myGroup, myVolumeKey);
        }
        // otherwise, add only subjects
        else
        {
            mySSS = new KeyronaSSS(mySSSStorage, SSSID, n, Participants, myVolumeKey);
        }

        // now add the SSS to the volume
        myVolume.addSSS(SSSID);

        // clean up objects
        for (int i=Participants.size(); i>0; i--)
        {
            delete Participants[i-1];
        }
        if (myGroup)
        {
            delete myGroup;
        }

        // done
        if (mySSS)
        {
            cout << "Successfully created secret-sharing-scheme with SSSID '" << SSSID << "'" << endl;
            printToSyslog("KEYRONA: SSS '" + SSSID + "' was created for volume '" + volumeID + "'");
            mySSS->printSSS();
            delete mySSS;
        }
    }
    catch ( std::exception &e )
    {
        cout << "Exception caught: " << e.what() << endl;
        myVolume.deleteSSS(SSSID);
    }
};


//================================================================================
//
string KeyronaListSSSInVolume(KeyronaStorage &myVolumeStorage, KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage, KeyronaStorage &mySSSStorage)
{
    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return "";
    }
    string volumeID = selectFromStringVector(myVolumes, "Number:\t\tUUID:", "Please select the desired volume:");

    if (!KeyronaFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    KeyronaVolume myVolume(volumeID, myVolumeStorage);

    vector<string> myAvailableSSSs = myVolume.getAvailableSSS();
    vector<string>::const_iterator Iterator;
    if (myAvailableSSSs.size())
    {
        cout << endl;
        cout << "We have the following secret-sharing-schemes: " << endl;

        Iterator=myAvailableSSSs.begin();
        while ( Iterator != myAvailableSSSs.end())
        {
            KeyronaSSS mySSS(mySSSStorage, *(Iterator));
            mySSS.printSSS();
            cout << endl;
            Iterator++;
        }
        return volumeID;
    }
    else
    {
        cout << "No secret-sharing-schemes found." << endl;
        return "";
    }
};


//================================================================================
//
void KeyronaDeleteSSSFromVolume(KeyronaStorage &myVolumeStorage, KeyronaStorage &mySubjectStorage, KeyronaStorage &myGroupStorage, KeyronaStorage &mySSSStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    string volumeID = KeyronaListSSSInVolume(myVolumeStorage, mySubjectStorage, myGroupStorage, mySSSStorage);
    if (volumeID.empty())
        return;

     // load volume
    KeyronaVolume myVolume(volumeID, myVolumeStorage);

    vector<string> mySSSs = myVolume.getAvailableSSS();

    string selectedSSS = selectFromStringVector(mySSSs, "Number:\t\tSSS:", "Please enter the according SSS to be deleted: ");

    if (!KeyronaFindSSS(mySSSStorage, selectedSSS))
        throw InvalidSSSID("SSS '" + selectedSSS + "' not found!");

    if (verifySelection())
    {
        // load SSS
        KeyronaSSS mySSS(mySSSStorage, selectedSSS);
        mySSS.deleteSSS();
        myVolume.deleteSSS(selectedSSS);
        cout << "Successfully deleted secret-sharing-scheme with SSSID '" << selectedSSS << "'" << endl;
        printToSyslog("KEYRONA: SSS '" + selectedSSS + "' was removed from volume '" + volumeID + "'");
    }
};

//================================================================================
//
void KeyronaVolumeManagement     ( string volumeParam, KeyronaConfigfile &myConfigfile )
{
    t_StringMap KeyronaVolumeMap;
    KeyronaVolumeMap["createVolume"]            = createVolume;
    KeyronaVolumeMap["cv"]                      = createVolume;
    KeyronaVolumeMap["importVolume"]            = importVolume;
    KeyronaVolumeMap["iv"]                      = importVolume;
    KeyronaVolumeMap["deleteVolume"]            = deleteVolume;
    KeyronaVolumeMap["dv"]                      = deleteVolume;
    KeyronaVolumeMap["attachVolume"]            = attachVolume;
    KeyronaVolumeMap["av"]                      = attachVolume;
    KeyronaVolumeMap["listVolumes"]             = listVolumes;
    KeyronaVolumeMap["lv"]                      = listVolumes;
    KeyronaVolumeMap["addSubjectToVolume"]      = addSubjectToVolume;
    KeyronaVolumeMap["astv"]                    = addSubjectToVolume;
    KeyronaVolumeMap["deleteSubjectFromVolume"] = deleteSubjectFromVolume;
    KeyronaVolumeMap["dsfv"]                    = deleteSubjectFromVolume;
    KeyronaVolumeMap["listSubjectsInVolume"]    = listSubjectsInVolume;
    KeyronaVolumeMap["lsiv"]                    = listSubjectsInVolume;
    KeyronaVolumeMap["addGroupToVolume"]        = addGroupToVolume;
    KeyronaVolumeMap["agtv"]                    = addGroupToVolume;
    KeyronaVolumeMap["deleteGroupFromVolume"]   = deleteGroupFromVolume;
    KeyronaVolumeMap["dgfv"]                    = deleteGroupFromVolume;
    KeyronaVolumeMap["listGroupsInVolume"]      = listGroupsInVolume;
    KeyronaVolumeMap["lgiv"]                    = listGroupsInVolume;
    KeyronaVolumeMap["addSSSToVolume"]          = addSSSToVolume;
    KeyronaVolumeMap["assstv"]                  = addSSSToVolume;
    KeyronaVolumeMap["listSSSInVolume"]         = listSSSInVolume;
    KeyronaVolumeMap["lsssiv"]                  = listSSSInVolume;
    KeyronaVolumeMap["deleteSSSFromVolume"]     = deleteSSSFromVolume;
    KeyronaVolumeMap["dsssfv"]                  = deleteSSSFromVolume;

    // Create Volume database storage object
    KeyronaStorage myVolumeStorage( "VolumeDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_VolumeDBIdentifier) );
    // Create User database storage object
    KeyronaStorage mySubjectStorage( "SubjectDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_SubjectDBIdentifier) );
    // Create Group database storage object
    KeyronaStorage myGroupStorage( "GroupDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_GroupDBIdentifier) );
    // Create SSS database storage object
    KeyronaStorage mySSSStorage( "SSSDB", myConfigfile.getConfigfileEntry(KeyronaConfigfile_SSSDBIdentifier) );


    switch(KeyronaVolumeMap[volumeParam])
    {
        case createVolume:
            KeyronaCreateVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case importVolume:
            KeyronaImportVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case deleteVolume:
            KeyronaDeleteVolume(myVolumeStorage, mySubjectStorage, myGroupStorage, mySSSStorage);
            break;
        case attachVolume:
            KeyronaAttachVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case listVolumes:
            KeyronaListVolumes(myVolumeStorage);
            break;
        case addSubjectToVolume:
            KeyronaAddSubjectToVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case deleteSubjectFromVolume:
            KeyronaDeleteSubjectFromVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case listSubjectsInVolume:
            KeyronaListSubjectsInVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case addGroupToVolume:
            KeyronaAddGroupToVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case deleteGroupFromVolume:
            KeyronaDeleteGroupFromVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case listGroupsInVolume:
            KeyronaListGroupsInVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case addSSSToVolume:
            KeyronaAddSSSToVolume(myVolumeStorage, mySubjectStorage, myGroupStorage, mySSSStorage);
            break;
        case listSSSInVolume:
            KeyronaListSSSInVolume(myVolumeStorage, mySubjectStorage, myGroupStorage, mySSSStorage);
            break;
        case deleteSSSFromVolume:
            KeyronaDeleteSSSFromVolume(myVolumeStorage, mySubjectStorage, myGroupStorage, mySSSStorage);
            break;
        default:
            throw ParseError("invalid parameter");
    }
};
