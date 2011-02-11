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


#include <TpmCrypt.hxx>
#include <TpmCryptManager.hxx>
#include <TpmCryptSSS.hxx>

using namespace std;
using namespace utils;
using namespace tpmcrypt;

enum myMenu
{
    createVolume = 1,
    importVolume,
    deleteVolume,
    attachVolume,
    listVolumes,
    addUserToVolume,
    deleteUserFromVolume,
    listUsersInVolume,
    addSSSToVolume,
    listSSSInVolume,
    deleteSSSFromVolume
};

// mapping between strings and int values
typedef map<string, int> t_StringMap;

//================================================================================
//
bool TpmCryptListVolumes(TpmCryptStorage &myVolumeStorage)
{
    vector<string> myVolumes = myVolumeStorage.queryAllEntryDB("volumes","name");

    vector<string>::const_iterator Iterator;
    if (myVolumes.size())
    {
        cout << "We have the following volumes: " << endl;

        Iterator=myVolumes.begin();
        while ( Iterator != myVolumes.end())
        {
            TpmCryptVolume myVolume(Iterator->first, myVolumeStorage);
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
void TpmCryptAttachVolume(TpmCryptStorage &myVolumeStorage, TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (! TpmCryptListVolumes(myVolumeStorage))
        return;

    // get VolumeUUID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string VolumeUUID = selectFromStringVector(myVolumes, "\nNumber:\t\tVolume:", "Please select the according volume: ");

    if (!TpmCryptFindVolume(myVolumeStorage, VolumeUUID))
        throw UnknownVolume("Volume '" + VolumeUUID + "' not found.");
    // load Volume
    TpmCryptVolume myVolume(VolumeUUID, myVolumeStorage);

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
void TpmCryptDeleteVolume(TpmCryptStorage &myVolumeStorage, TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage, TpmCryptStorage &mySSSStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (! TpmCryptListVolumes(myVolumeStorage))
        return;

    // get VolumeUUID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string VolumeUUID = selectFromStringVector(myVolumes, "\nNumber:\t\tVolume:", "Please enter the according volume to be deleted: ");

    if (!TpmCryptFindVolume(myVolumeStorage, VolumeUUID))
        throw UnknownVolume("Volume '" + VolumeUUID + "' not found.");

    if (verifySelection())
    {
        // load Volume
        TpmCryptVolume * myVolume = new TpmCryptVolume(VolumeUUID, myVolumeStorage);

        // remove all existing SSSs
        vector<string> mySSSs = myVolume->getAvailableSSS();

        debug << "TpmCryptDeleteVolume(): Removing all existing secret-sharing-schemes from volume '" << VolumeUUID << "'" << endl;
        while (mySSSs.size())
        {
            string currentSSS = mySSSs.back();
            mySSSs.pop_back();

            // load SSS
            TpmCryptSSS mySSS(mySSSStorage, currentSSS);
            mySSS.deleteSSS();
            myVolume->deleteSSS(currentSSS);
            cout << "Successfully deleted secret-sharing-scheme with SSSID '" << currentSSS << "'" << endl;
            printToSyslog("TPMCRYPT: SSS '" + currentSSS + "' was removed from volume '" + VolumeUUID + "'");
        }

        myVolume->deleteVolume();
        delete myVolume;

        if (!TpmCryptFindVolume(myVolumeStorage, VolumeUUID))
        {
            cout << "Successfully deleted volume '" << VolumeUUID << "' from the TpmCrypt system." << endl;
            cout << "Note: the physical volume itself has not been deleted!" << endl;
            printToSyslog("TPMCRYPT: Volume '" + VolumeUUID + "' deleted from this system");
        }
        else
        {
            cout << "Failed to delete volume '" << VolumeUUID << "'!" << endl;
        }
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
void TpmCryptCreateVolume(TpmCryptStorage &myVolumeStorage, TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage)
{
    UInt8 EncryptionType = 0;
    UInt8 FileSystem = 0;
    string FSString;
    string device;
    string UUID;

    vector<string> myAdmins = TpmCryptFindAllAdmins(mySubjectStorage);
    if (! myAdmins.size())
    {
        throw NoAdminAvailable("No admins found, please add an administrator first!");
    }
    
    if (loginSuperUser())
        cout << "Superuser successfully logged in!" << endl;

    t_StringMap TpmCryptVolumeTypeMap;
    TpmCryptVolumeTypeMap["DMCRYPT"] = VOLUMETYPE_DMCRYPT;
    TpmCryptVolumeTypeMap["TRUECRYPT"] = VOLUMETYPE_TRUECRYPT;
    TpmCryptVolumeTypeMap["ECRYPTFS"] = VOLUMETYPE_ECRYPTFS;
    TpmCryptVolumeTypeMap["LUKS"] = VOLUMETYPE_LUKS;
    TpmCryptVolumeTypeMap["ENCFS"] = VOLUMETYPE_ENCFS;

    vector<string> AvailableEncryption;
    findAvailableCryptosystems("DMCRYPT", "cryptsetup", &AvailableEncryption);
    findAvailableCryptosystems("TRUECRYPT", "truecrypt", &AvailableEncryption);
    findAvailableCryptosystems("ECRYPTFS", "ecryptfs-manager", &AvailableEncryption);
    findAvailableCryptosystems("LUKS", "cryptsetup", &AvailableEncryption);
    findAvailableCryptosystems("ENCFS", "encfs", &AvailableEncryption);
    if (! AvailableEncryption.size())
        throw InvalidEncSystem("No encryption systems available. Make sure to at least install one of the supported crypto systems.");

    t_StringMap TpmCryptVolumeFilesystemMap;
    TpmCryptVolumeTypeMap["ext2"] = FSTYPE_EXT2;
    TpmCryptVolumeTypeMap["ext3"] = FSTYPE_EXT3;
    TpmCryptVolumeTypeMap["ext4"] = FSTYPE_EXT4;
    TpmCryptVolumeTypeMap["ntfs"] = FSTYPE_NTFS;
    TpmCryptVolumeTypeMap["reiserfs"] = FSTYPE_REISERFS;
    TpmCryptVolumeTypeMap["vfat"] = FSTYPE_VFAT;
    TpmCryptVolumeTypeMap["xfs"] = FSTYPE_XFS;

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

    switch(TpmCryptVolumeTypeMap[EncTypeString])
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
         cout << "TpmCrypt will now create a new key for target '" << device << "' to be used with '" << EncTypeString << "'" << endl;
         if (! dirExists(device))
             throw DirNotExist("The directory '" + device + "' does not exist!");
    }

    UUID = getStringFromUser("Please enter a unique identifier for device '" + device + "': ");
    if (UUID.empty())
        throw NotCreated("Please enter a valid identifier!");

    if (TpmCryptFindVolume(myVolumeStorage, UUID))
        throw VolumeExists("TpmCryptCreateVolume: The desired volume already exists in our database!\n");

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

        switch(TpmCryptVolumeTypeMap[FSString])
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

    string VolumePassword = generateRandomString(TPMCRYPT_MINIMUM_PASSWORD_LENGTH);
    cout << VolumePassword << endl;
    if (verifySelection())
    {
        string myCommand;
        // creating volume!
        switch(EncryptionType)
        {
            case VOLUMETYPE_DMCRYPT: myCommand = (myScriptDirectory + TpmCryptPathSeparator + "create_dmcrypt.sh " + device + " " + FSString + " " + VolumePassword ); break;
            case VOLUMETYPE_TRUECRYPT: myCommand = (myScriptDirectory + TpmCryptPathSeparator + "create_truecrypt.sh " + device + " " + FSString + " " + VolumePassword ); break;
            case VOLUMETYPE_LUKS: myCommand = (myScriptDirectory + TpmCryptPathSeparator + "create_cryptsetup_luks.sh " + device + " " + FSString + " " + VolumePassword ); break;
            case VOLUMETYPE_ENCFS: myCommand = (myScriptDirectory + TpmCryptPathSeparator + "create_encfs.sh " + device + " " + VolumePassword ); break;
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
        printToSyslog("TPMCRYPT: Volume '" + device + "' created");

        cout << "Importing volume '" << device << "' into TpmCrypt." << endl;

        // load admin group
        TpmCryptGroup *myAdminGroup = new TpmCryptGroup(TPMCRYPT_ADMIN_GROUP, myGroupStorage, mySubjectStorage);

        // import volume
        TpmCryptVolume newVolume(UUID, device, VolumePassword, EncryptionType, myAdminGroup, myVolumeStorage);

        // cleanup
        delete myAdminGroup;
        if (!TpmCryptFindVolume(myVolumeStorage, UUID))
            throw NotCreated("Volume could not be imported!");

        cout << "Successfully imported volume '" << newVolume.getMyVolumeUUID() << "'." << endl;
        printToSyslog("TPMCRYPT: Volume '" + UUID + "' imported into this system");
        newVolume.printVolume();
    }
};

//================================================================================
//
void TpmCryptImportVolume(TpmCryptStorage &myVolumeStorage, TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    UInt8 EncryptionType = 0;
    string device;
    string UUID;

    t_StringMap TpmCryptVolumeTypeMap;
    TpmCryptVolumeTypeMap["DMCRYPT"] = VOLUMETYPE_DMCRYPT;
    TpmCryptVolumeTypeMap["TRUECRYPT"] = VOLUMETYPE_TRUECRYPT;
    TpmCryptVolumeTypeMap["ECRYPTFS"] = VOLUMETYPE_ECRYPTFS;
    TpmCryptVolumeTypeMap["LUKS"] = VOLUMETYPE_LUKS;
    TpmCryptVolumeTypeMap["ENCFS"] = VOLUMETYPE_ENCFS;
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

    switch(TpmCryptVolumeTypeMap[EncTypeString])
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

    if (TpmCryptFindVolume(myVolumeStorage, UUID))
        throw VolumeExists("TpmCryptCreateVolume: The desired volume already exists in our database!\n");

    string VolumePassword = getPasswordVerified("Please enter the according password for '" + device + "': ");

    // load admin group
    TpmCryptGroup *myAdminGroup = new TpmCryptGroup(TPMCRYPT_ADMIN_GROUP, myGroupStorage, mySubjectStorage);

    // import volume
    TpmCryptVolume newVolume(UUID, device, VolumePassword, EncryptionType, myAdminGroup, myVolumeStorage);

    // cleanup
    delete myAdminGroup;

    if (!TpmCryptFindVolume(myVolumeStorage, UUID))
        throw NotCreated("Volume could not be imported!");

    cout << "Successfully imported volume '" << newVolume.getMyVolumeUUID() << "'." << endl;
    printToSyslog("TPMCRYPT: Volume '" + UUID + "' imported into this system");
    newVolume.printVolume();
};

//================================================================================
//
void TpmCryptAddSubjectToVolume(TpmCryptStorage &myVolumeStorage, TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (!TpmCryptListVolumes(myVolumeStorage))
        return;

    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string volumeID = selectFromStringVector(myVolumes, "\nNumber:\t\tUUID:", "Please select the desired volume:");

    if (!TpmCryptFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    TpmCryptVolume myVolume(volumeID, myVolumeStorage);

    // get all Users
    vector<string> myUsers = TpmCryptFindAllUsers(mySubjectStorage, false);
    // get all Token
    vector<string> myToken = TpmCryptFindAllToken(mySubjectStorage);
	// get all Platforms
	vector<string> myPlatform = TpmCryptFindAllPlatforms(mySubjectStorage);

    // add Token to Userlist
    myUsers.insert(myUsers.end(), myToken.begin(), myToken.end());
	myUsers.insert(myUsers.end(), myPlatform.begin(), myPlatform.end());
    if (!myUsers.size())
    {
        cout << "No users / platforms / token available!" << endl;
        return;
    }

    // load all subjects already in volume
    vector<string> mySubjectsInVolume = myVolume.getSubjects();
    
    // remove existing users from volume user list
    vector<string>::const_iterator Iterator;
    Iterator=mySubjectsInVolume.begin();
    while ( Iterator != mySubjectsInVolume.end())
    {
        TpmCryptSubject *mySubject = new TpmCryptSubject(*(Iterator), mySubjectStorage);
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

    if (!TpmCryptFindSubject(mySubjectStorage, subjectID))
        throw UnknownSubject("Subject '" + subjectID + "' not found.");

    // load subject
    TpmCryptSubject mySubject(subjectID, mySubjectStorage);

    // is entered subject already in volume?
    if (myVolume.isSubjectInVolume(&mySubject))
        throw InvalidUID("Subject '" + mySubject.getMySubjectName() + "' is already in volume '" + volumeID + "'!");

    // load admin
    TpmCryptSubject *myExistingAdmin = getCurrentAdmin(mySubjectStorage);
    
    string myPassword = getPassword("Please enter the according password for admin '" + myExistingAdmin->getMySubjectName() + "' (" + myExistingAdmin->getMySubjectIDString() + "): ");

    // get admin key
    TpmCryptGroup myAdminGroup(TPMCRYPT_ADMIN_GROUP, myGroupStorage, mySubjectStorage);
    string myAdminKeyPassword = myAdminGroup.getDecryptedGroupKey(myExistingAdmin, myPassword);

    // get volume key
    string myVolumePassword = myVolume.getDecryptedVolumeKey(&myAdminGroup, myAdminKeyPassword);
    // finally, add subject
    myVolume.addSubject(&mySubject, myVolumePassword);
    cout << "Successfully added subject '" << subjectID << "' to volume '" << volumeID << "'." << endl;
};

//================================================================================
//
void TpmCryptDeleteSubjectFromVolume(TpmCryptStorage &myVolumeStorage, TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (!TpmCryptListVolumes(myVolumeStorage))
        return;

    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string volumeID = selectFromStringVector(myVolumes, "Number:\t\tUUID:", "Please select the desired volume:");

    if (!TpmCryptFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    TpmCryptVolume myVolume(volumeID, myVolumeStorage);

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
        TpmCryptSubject *mySubject = new TpmCryptSubject(*(Iterator), mySubjectStorage);
        mySubjectNamesInVolume.push_back(mySubject->getMySubjectName());
        delete mySubject;
        Iterator++;
    }
    // select the subject to be deleted
    string subjectID = selectFromStringVector(mySubjectNamesInVolume, "Number:\t\tUser:", "Please select the subject to be deleted:");

    if (!TpmCryptFindSubject(mySubjectStorage, subjectID))
        throw UnknownSubject("Subject '" + subjectID + "' not found.");

    // load subject
    TpmCryptSubject mySubject(subjectID, mySubjectStorage);

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
void TpmCryptListSubjectsInVolume(TpmCryptStorage &myVolumeStorage, TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage)
{

    if (!TpmCryptListVolumes(myVolumeStorage))
        return;

    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string volumeID = selectFromStringVector(myVolumes, "\nNumber:\t\tUUID:", "Please select the desired volume:");

    if (!TpmCryptFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    TpmCryptVolume myVolume(volumeID, myVolumeStorage);

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
        TpmCryptSubject *mySubject = new TpmCryptSubject(*(Iterator), mySubjectStorage);
        mySubject->printSubject(printVerbose);
        delete mySubject;
        Iterator++;
    }
};


//================================================================================
//
void TpmCryptAddGroupToVolume(TpmCryptStorage &myVolumeStorage, TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (!TpmCryptListVolumes(myVolumeStorage))
        return;

    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string volumeID = selectFromStringVector(myVolumes, "\nNumber:\t\tUUID:", "Please select the desired volume:");

    if (!TpmCryptFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    TpmCryptVolume myVolume(volumeID, myVolumeStorage);

    // get groupID
    vector<string> myGroups = TpmCryptFindAllGroups(myGroupStorage, mySubjectStorage, false);
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
    if (groupID == TPMCRYPT_ADMIN_GROUP)
        throw InvalidGroup("Group '" + groupID + "' is not allowed to be added manually!");

    if (!TpmCryptFindGroup(myGroupStorage, groupID))
        throw UnknownGroup("Group '" + groupID + "' not found.");

    // load user group
    TpmCryptGroup myGroup(groupID, myGroupStorage, mySubjectStorage);

    // is entered group already in volume?
    if (myVolume.isGroupInVolume(&myGroup))
        throw InvalidGroup("Group '" + myGroup.getMyGroupID() + "' is already in volume '" + volumeID + "'!");

    // load admin
    TpmCryptSubject *myExistingAdmin = getCurrentAdmin(mySubjectStorage);

    string myPassword = getPassword("Please enter the according password for admin '" + myExistingAdmin->getMySubjectName() + "' (" + myExistingAdmin->getMySubjectIDString() + "): ");

    // load admin group
    TpmCryptGroup myAdminGroup(TPMCRYPT_ADMIN_GROUP, myGroupStorage, mySubjectStorage);
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
void TpmCryptDeleteGroupFromVolume(TpmCryptStorage &myVolumeStorage, TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    if (!TpmCryptListVolumes(myVolumeStorage))
        return;

    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string volumeID = selectFromStringVector(myVolumes, "\nNumber:\t\tUUID:", "Please select the desired volume:");

    if (!TpmCryptFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    TpmCryptVolume myVolume(volumeID, myVolumeStorage);

    // get groupID
    vector<string> groupsInVolume = myVolume.getGroups();
    // remove the admin group from selection list
    removeStringFromStringVector(&groupsInVolume, TPMCRYPT_ADMIN_GROUP);
    if (!groupsInVolume.size())
    {
        cout << "No groups exist in volume '" << volumeID << "'!" << endl;
        return;
    }

    string groupID = selectFromStringVector(groupsInVolume, "Number:\t\tGroup:", "Please select the group to be deleted:");

    if (!TpmCryptFindGroup(myGroupStorage, groupID))
        throw UnknownGroup("Group '" + groupID + "' not found.");

    if (groupID == TPMCRYPT_ADMIN_GROUP)
        throw InvalidGroup("Group '" + groupID + "' is not allowed to be deleted manually!");

    // load user group
    TpmCryptGroup myGroup(groupID, myGroupStorage, mySubjectStorage);

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
void TpmCryptListGroupsInVolume(TpmCryptStorage &myVolumeStorage, TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage)
{
    if (!TpmCryptListVolumes(myVolumeStorage))
        return;

    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string volumeID = selectFromStringVector(myVolumes, "\nNumber:\t\tUUID:", "Please select the desired volume:");

    if (!TpmCryptFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    TpmCryptVolume myVolume(volumeID, myVolumeStorage);

    // get all existing groups in volume
    vector<string> groupsInVolume = myVolume.getGroups();
    if (!groupsInVolume.size())
    {
        cout << "No groups exist in volume '" << volumeID << "'!" << endl;
        return;
    }

    if ((groupsInVolume.size() == 1) && (groupsInVolume[0] == TPMCRYPT_ADMIN_GROUP))
    {
        cout << "Only the admin group exists in volume '" << volumeID << "'!" << endl;
        return;
    }

    cout << "The following groups have access to volume '" << volumeID << "':" << endl;
    vector<string>::const_iterator Iterator;
    Iterator=groupsInVolume.begin();
    while ( Iterator != groupsInVolume.end())
    {
        TpmCryptGroup *myGroup = new TpmCryptGroup(*(Iterator), myGroupStorage, mySubjectStorage);
        if (myGroup->getMyGroupID() != TPMCRYPT_ADMIN_GROUP)
            myGroup->printGroup(printVerbose);
        delete myGroup;
        Iterator++;
    }
};

//================================================================================
//
void TpmCryptAddSSSToVolume(TpmCryptStorage &myVolumeStorage, TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage, TpmCryptStorage &mySSSStorage)
{
    // load admin -> we do not require a call to loginAdmin(), since the admin is needed anyway. Therefore no dual login is required
    TpmCryptSubject *myExistingAdmin = getCurrentAdmin(mySubjectStorage);

    string myPassword = getPassword("Please enter the according password for admin '" + myExistingAdmin->getMySubjectName() + "' (" + myExistingAdmin->getMySubjectIDString() + "): ");

    if (!TpmCryptListVolumes(myVolumeStorage))
        return;

    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return;
    }
    string volumeID = selectFromStringVector(myVolumes, "\nNumber:\t\tUUID:", "Please select the desired volume:");

    if (!TpmCryptFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    TpmCryptVolume myVolume(volumeID, myVolumeStorage);


    // load admin group
    TpmCryptGroup myAdminGroup(TPMCRYPT_ADMIN_GROUP, myGroupStorage, mySubjectStorage);
    string myAdminGroupKeyPassword = myAdminGroup.getDecryptedGroupKey(myExistingAdmin, myPassword);

    // load the volume key by admin
    string myVolumeKey = myVolume.getDecryptedVolumeKey(&myAdminGroup, myAdminGroupKeyPassword);

    string SSSID = getStringFromUser("Please enter an identifier for the Secret-Sharing-Scheme: ");
    if (SSSID.empty())
        throw InvalidSSSID("Invalid SSSID entered!");
    if (TpmCryptFindSSS(mySSSStorage, SSSID))
        throw InvalidSSSID("A secret-sharing-scheme already exists with SSSID '" + SSSID + "'");

    // get all Users
    vector<string> myUsers = TpmCryptFindAllUsers(mySubjectStorage, false);
    // get all Token
    vector<string> myToken = TpmCryptFindAllToken(mySubjectStorage);
    // get all Platforms
    vector<string> myPlatforms = TpmCryptFindAllPlatforms(mySubjectStorage);

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
    TpmCryptGroup *myGroup = NULL;
    vector<string> myGroups = TpmCryptFindAllGroups(myGroupStorage, mySubjectStorage, false);

    if (myGroups.size())
    {
        // get groupID
        string groupID = selectFromStringVector(myGroups, "Number:\t\tGroup:", "If you want to add a group to this SSS, please select the group to be added (-1 for no group):");

        if (groupID != "-1")
        {
            if (groupID == TPMCRYPT_ADMIN_GROUP)
                throw InvalidGroup("Group '" + groupID + "' is not allowed to be added manually!");

            if (!TpmCryptFindGroup(myGroupStorage, groupID))
                throw UnknownGroup("Group '" + groupID + "' not found.");

            // load user group
            myGroup = new TpmCryptGroup(groupID, myGroupStorage, mySubjectStorage);
            m++;
            cout << "Please add the group to the userlist while mounting, e.g., 'tpmcrypt_mount --mount --user myUser," << groupID << " --volume /dev/sdxy --path /some/mointpoint" << endl;
        }
    }

    string toStop = "-1";
    string subjectID;
    vector<TpmCryptSubject*> Participants;
    while ((subjectID.compare(0, toStop.length(), toStop)) && (myUsers.size()))
    {
        subjectID = selectFromStringVector(myUsers, "Number:\t\tSubject:", "Please select further participants for this SSS (-1 to stop): ");
        if (subjectID == toStop)
            break;
        if (!TpmCryptFindSubject(mySubjectStorage, subjectID))
            throw UnknownSubject("Subject '" + subjectID + "' not found.");

        Participants.push_back ( new TpmCryptSubject(subjectID, mySubjectStorage));
        m++;
        cout << "Successfully added subject '" << (Participants.back())->getMySubjectName() << "' to SSS '" << SSSID << "'." << endl;
        cout << "New amount of participants: " << m << endl;
        removeStringFromStringVector(&myUsers, subjectID);
    }

    cout << "You selected " << m << " participants!" << endl;

    switch(m)
    {
        case 0: throw NotEnoughParticipants("TpmCryptAddSSS: Invalid amount of overall participants (m)!");
        case 1: throw NotEnoughParticipants("TpmCryptAddSSS: You selected, that one participant alone can gain access. Please add user or group directly!");
        case 2: { n=2; break; }
        default: 
        {
            n = convertStringtoUInt32(getStringFromUser("How many of the above " + convertUInt32toString(m) + " participants have to be available to retrieve the key: "));
            break;
        }
    }

    switch(n)
    {
        case 0: throw NotEnoughParticipants("TpmCryptAddSSS: Invalid amount of participants (n)!");
        case 1: throw NotEnoughParticipants("TpmCryptAddSSS: You selected, that one participant alone can gain access. Please add user or group directly!");
    }

    if (n>m)
        throw NotEnoughParticipants("TpmCryptAddSSS: Invalid Selection, n > m!");

    cout << "You selected a " << n << " (n) out of " << m << " (m) scheme." << endl;

    TpmCryptSSS *mySSS = NULL;
    try
    {
        // do we have a group in the SSS?
        if (myGroup)
        {
            mySSS = new TpmCryptSSS(mySSSStorage, SSSID, n, Participants, myGroup, myVolumeKey);
        }
        // otherwise, add only subjects
        else
        {
			cout << myVolumeKey << endl;
            mySSS = new TpmCryptSSS(mySSSStorage, SSSID, n, Participants, myVolumeKey);
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
            printToSyslog("TPMCRYPT: SSS '" + SSSID + "' was created for volume '" + volumeID + "'");
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
string TpmCryptListSSSInVolume(TpmCryptStorage &myVolumeStorage, TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage, TpmCryptStorage &mySSSStorage)
{
    // get VolumeID
    vector<string> myVolumes = myVolumeStorage.getAvailableSections();
    if (!myVolumes.size())
    {
        cout << "No volumes available!" << endl;
        return "";
    }
    string volumeID = selectFromStringVector(myVolumes, "Number:\t\tUUID:", "Please select the desired volume:");

    if (!TpmCryptFindVolume(myVolumeStorage, volumeID))
        throw UnknownVolume("Volume '" + volumeID + "' not found.");

    // load volume
    TpmCryptVolume myVolume(volumeID, myVolumeStorage);

    vector<string> myAvailableSSSs = myVolume.getAvailableSSS();
    vector<string>::const_iterator Iterator;
    if (myAvailableSSSs.size())
    {
        cout << endl;
        cout << "We have the following secret-sharing-schemes: " << endl;

        Iterator=myAvailableSSSs.begin();
        while ( Iterator != myAvailableSSSs.end())
        {
            TpmCryptSSS mySSS(mySSSStorage, *(Iterator));
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
void TpmCryptDeleteSSSFromVolume(TpmCryptStorage &myVolumeStorage, TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage, TpmCryptStorage &mySSSStorage)
{
    if (loginAdmin(mySubjectStorage))
        cout << "Admin successfully logged in!" << endl;

    string volumeID = TpmCryptListSSSInVolume(myVolumeStorage, mySubjectStorage, myGroupStorage, mySSSStorage);
    if (volumeID.empty())
        return;

     // load volume
    TpmCryptVolume myVolume(volumeID, myVolumeStorage);

    vector<string> mySSSs = myVolume.getAvailableSSS();

    string selectedSSS = selectFromStringVector(mySSSs, "Number:\t\tSSS:", "Please enter the according SSS to be deleted: ");

    if (!TpmCryptFindSSS(mySSSStorage, selectedSSS))
        throw InvalidSSSID("SSS '" + selectedSSS + "' not found!");

    if (verifySelection())
    {
        // load SSS
        TpmCryptSSS mySSS(mySSSStorage, selectedSSS);
        mySSS.deleteSSS();
        myVolume.deleteSSS(selectedSSS);
        cout << "Successfully deleted secret-sharing-scheme with SSSID '" << selectedSSS << "'" << endl;
        printToSyslog("TPMCRYPT: SSS '" + selectedSSS + "' was removed from volume '" + volumeID + "'");
    }
};

//================================================================================
//
void TpmCryptVolumeManagement     ( string volumeParam, TpmCryptConfigfile &myConfigfile )
{
    t_StringMap TpmCryptVolumeMap;
    TpmCryptVolumeMap["createVolume"]            = createVolume;
    TpmCryptVolumeMap["cv"]                      = createVolume;
    TpmCryptVolumeMap["importVolume"]            = importVolume;
    TpmCryptVolumeMap["iv"]                      = importVolume;
    TpmCryptVolumeMap["deleteVolume"]            = deleteVolume;
    TpmCryptVolumeMap["dv"]                      = deleteVolume;
    TpmCryptVolumeMap["attachVolume"]            = attachVolume;
    TpmCryptVolumeMap["av"]                      = attachVolume;
    TpmCryptVolumeMap["listVolumes"]             = listVolumes;
    TpmCryptVolumeMap["lv"]                      = listVolumes;
    TpmCryptVolumeMap["addSubjectToVolume"]      = addSubjectToVolume;
    TpmCryptVolumeMap["astv"]                    = addSubjectToVolume;
    TpmCryptVolumeMap["deleteSubjectFromVolume"] = deleteSubjectFromVolume;
    TpmCryptVolumeMap["dsfv"]                    = deleteSubjectFromVolume;
    TpmCryptVolumeMap["listSubjectsInVolume"]    = listSubjectsInVolume;
    TpmCryptVolumeMap["lsiv"]                    = listSubjectsInVolume;
    TpmCryptVolumeMap["addGroupToVolume"]        = addGroupToVolume;
    TpmCryptVolumeMap["agtv"]                    = addGroupToVolume;
    TpmCryptVolumeMap["deleteGroupFromVolume"]   = deleteGroupFromVolume;
    TpmCryptVolumeMap["dgfv"]                    = deleteGroupFromVolume;
    TpmCryptVolumeMap["listGroupsInVolume"]      = listGroupsInVolume;
    TpmCryptVolumeMap["lgiv"]                    = listGroupsInVolume;
    TpmCryptVolumeMap["addSSSToVolume"]          = addSSSToVolume;
    TpmCryptVolumeMap["assstv"]                  = addSSSToVolume;
    TpmCryptVolumeMap["listSSSInVolume"]         = listSSSInVolume;
    TpmCryptVolumeMap["lsssiv"]                  = listSSSInVolume;
    TpmCryptVolumeMap["deleteSSSFromVolume"]     = deleteSSSFromVolume;
    TpmCryptVolumeMap["dsssfv"]                  = deleteSSSFromVolume;

    // Create Volume database storage object
    TpmCryptStorage myVolumeStorage( "VolumeDB", myConfigfile.getConfigfileEntry(TpmCryptConfigfile_VolumeDBIdentifier) );
    // Create User database storage object
    TpmCryptStorage mySubjectStorage( "SubjectDB", myConfigfile.getConfigfileEntry(TpmCryptConfigfile_SubjectDBIdentifier) );
    // Create Group database storage object
    TpmCryptStorage myGroupStorage( "GroupDB", myConfigfile.getConfigfileEntry(TpmCryptConfigfile_GroupDBIdentifier) );
    // Create SSS database storage object
    TpmCryptStorage mySSSStorage( "SSSDB", myConfigfile.getConfigfileEntry(TpmCryptConfigfile_SSSDBIdentifier) );


    switch(TpmCryptVolumeMap[volumeParam])
    {
        case createVolume:
            TpmCryptCreateVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case importVolume:
            TpmCryptImportVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case deleteVolume:
            TpmCryptDeleteVolume(myVolumeStorage, mySubjectStorage, myGroupStorage, mySSSStorage);
            break;
        case attachVolume:
            TpmCryptAttachVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case listVolumes:
            TpmCryptListVolumes(myVolumeStorage);
            break;
        case addSubjectToVolume:
            TpmCryptAddSubjectToVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case deleteSubjectFromVolume:
            TpmCryptDeleteSubjectFromVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case listSubjectsInVolume:
            TpmCryptListSubjectsInVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case addGroupToVolume:
            TpmCryptAddGroupToVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case deleteGroupFromVolume:
            TpmCryptDeleteGroupFromVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case listGroupsInVolume:
            TpmCryptListGroupsInVolume(myVolumeStorage, mySubjectStorage, myGroupStorage);
            break;
        case addSSSToVolume:
            TpmCryptAddSSSToVolume(myVolumeStorage, mySubjectStorage, myGroupStorage, mySSSStorage);
            break;
        case listSSSInVolume:
            TpmCryptListSSSInVolume(myVolumeStorage, mySubjectStorage, myGroupStorage, mySSSStorage);
            break;
        case deleteSSSFromVolume:
            TpmCryptDeleteSSSFromVolume(myVolumeStorage, mySubjectStorage, myGroupStorage, mySSSStorage);
            break;
        default:
            throw ParseError("invalid parameter");
    }
};
