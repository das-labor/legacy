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

using namespace std;
using namespace utils;
using namespace tpmcrypt;


enum myMenu
{
    createESD,
    deleteESD,
    showESD
};

// mapping between strings and int values
typedef map<string, int> t_StringMap;

//================================================================================
//
void TpmCryptCreateESD(TpmCryptStorage &myESDStorage)
{
	t_StringMap TpmCryptVolumeFilesystemMap;
    TpmCryptVolumeFilesystemMap["ext2"] = FSTYPE_EXT2;
    TpmCryptVolumeFilesystemMap["vfat"] = FSTYPE_VFAT;
    TpmCryptVolumeFilesystemMap["xfs"] = FSTYPE_XFS;

    vector<string> AvailableFilesystems;
    findAvailableFilesystems("ext2", &AvailableFilesystems);
    findAvailableFilesystems("vfat", &AvailableFilesystems);
    findAvailableFilesystems("xfs", &AvailableFilesystems);
	
	if (loginSuperUser())
        cout << "Superuser successfully logged in!" << endl;

    string myESD = getStringFromUser("Please enter name for new ESD: ");

    if (TpmCryptFindESD(myESDStorage, myESD))
        throw InvalidUsername("TpmCryptCreateESD(): ESD already exists!");

    string myESDDevice;    
    
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

        myESDDevice = selectFromStringVector(AvailableDevices, "Number:\t\tDevice:", "Please select the device to create an ESD:");
        
    }
    catch ( NoVolume &e)
    {
		cout << "Error: Could not identify any valid volumes." << endl;
		myESDDevice = getStringFromUser("Please enter the device to create an ESD: ");
    }
    
    if (isDeviceMounted(myESDDevice))
		throw InvalidFilename("The device you selected is currently mounted. Please unmount it first, before re-creating the volume.");
        
    cout << "Selected device '" << myESDDevice << "'" << endl << endl;
    
    string myESDUUID = getStringFromUser("Please enter name for ESD Device: ");
    
    if (myESDUUID.empty())
        throw NotCreated("Please enter a valid identifier!");

    if (TpmCryptFindESD(myESDStorage, myESDUUID))
        throw VolumeExists("TpmCryptCreateVolume: The desired volume already exists in our database!\n");
    
    bool FileSystem;
    
    cout << "WARNING: All data will be permanently lost on device '" << myESDDevice << "'" << endl;
    string FSString = selectFromStringVector(AvailableFilesystems, "Number:\t\tFilesystem:", "Please enter the desired filesystem for volume '" + myESDUUID + "': ");
    cout << "Selected filesystem scheme '" << FSString << "'" << endl << endl;
    switch(TpmCryptVolumeFilesystemMap[FSString])
    {
		case FSTYPE_EXT2: FileSystem = FSTYPE_EXT2; break;
        case FSTYPE_VFAT: FileSystem = FSTYPE_VFAT; break;
        case FSTYPE_XFS: FileSystem = FSTYPE_XFS; break;
    }
    
    if (!FileSystem)
        throw NotCreated("Please enter valid filesystem!");
    
    string mountpath = getStringFromUser("Please enter a mount path for ESD Device: ");
    
    string myCommand = ("mkfs." + FSString + " " + myESDDevice + " && " + "mount " + myESDDevice + " " + mountpath);
        
    if (myCommand.size())
    {
		debug << "Now executing: " << myCommand << endl;
        int ret = system(myCommand.c_str());
        if (ret)
			throw NotCreated("Volume could not be created!");
    }
    
    TpmCryptESD newESD(myESD, myESDDevice, myESDUUID, myESDStorage);
        
    if (! newESD.getMyESDID())
		throw NotCreated("ESD could not be created!");
    cout << "Successfully created new ESD'" << newESD.getMyESDID() << "'." << endl;
    printToSyslog("TPMCRYPT: ESD '" + myESD + "' was added to this system");

	string myCommand2 = ("umount " + mountpath);

    if (myCommand2.size())
    {
		debug << "Now executing: " << myCommand2 << endl;
		int ret = system(myCommand2.c_str());
		if (ret)
			throw NotCreated("Volume could not be created!");
    }
};

void TpmCryptDeleteESD(TpmCryptStorage &myESDStorage)
{
    vector<string> myESDs = TpmCryptFindAllESDs(myESDStorage);
    if (! myESDs.size())
    {
        cout << "No ESDs found." << endl;
        return;
    }
    string esdToDelete = selectFromStringVector(myESDs, "\nNumber:\t\tESD:", "Please select the ESD to be deleted:");

    if (!TpmCryptFindESD(myESDStorage, esdToDelete))
        throw UnknownESD("ESD '" + esdToDelete + "' not found.");
        
    string myESDDevice;    
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

            myESDDevice = selectFromStringVector(AvailableDevices, "Number:\t\tDevice:", "[Note: If the desired device has not been identified automatically, you can still enter the correct device (e.g., /dev/sdb1) below.]\nPlease select the device to be created:");
        }
    catch ( NoVolume &e)
        {
            cout << "Error: Could not identify any valid volumes." << endl;
            myESDDevice = getStringFromUser("Please enter the device to be created: ");
        }
        if (isDeviceMounted(myESDDevice))
            throw InvalidFilename("The device you selected is currently mounted. Please unmount it first, before re-creating the volume.");
        cout << "Selected device '" << myESDDevice << "'" << endl << endl;
        string mountpath = getStringFromUser("Please enter a mount path for ESD Device: ");

    string myCommand = ("mount " + myESDDevice + " " + mountpath + " && " + "wipe -l2 -p8 -r -x8 -v " + mountpath + "/*" );
        
        // note: we don't need a setup script for ecryptfs
        if (myCommand.size())
        {
            debug << "Now executing: " << myCommand << endl;
            int ret = system(myCommand.c_str());
            if (ret)
                throw NotCreated("Volume could not be created!");
        }

    TpmCryptESD *myESD = new TpmCryptESD(esdToDelete, myESDStorage);
    myESD->deleteESD();
    
    	        string myCommand2 = ("umount " + mountpath);
        
        // note: we don't need a setup script for ecryptfs
        if (myCommand2.size())
        {
            debug << "Now executing: " << myCommand2 << endl;
            int ret = system(myCommand2.c_str());
            if (ret)
                throw NotCreated("Volume could not be created!");
        }
};

void TpmCryptShowESD(TpmCryptStorage &myESDStorage)
{
	int test;
};

void TpmCryptESDManagement( string esdParam, TpmCryptConfigfile &myConfigfile )
{
    t_StringMap TpmCryptESDMap;
    TpmCryptESDMap["createESD"]   = createESD;
    TpmCryptESDMap["ce"]          = createESD;
    TpmCryptESDMap["deleteESD"]   = deleteESD;
    TpmCryptESDMap["de"]          = deleteESD;
    TpmCryptESDMap["showESD"]     = showESD;
    TpmCryptESDMap["se"]          = showESD;

    // Create User database storage object
    TpmCryptStorage myESDStorage( "ESDDB", myConfigfile.getConfigfileEntry(TpmCryptConfigfile_ESDDBIdentifier) );
    
    switch(TpmCryptESDMap[esdParam])
    {
        case createESD:
            TpmCryptCreateESD(myESDStorage);
            break;
        case deleteESD:
			TpmCryptDeleteESD(myESDStorage);
			break;
        case showESD:
            TpmCryptShowESD(myESDStorage);
            break;
        default:
            throw ParseError("invalid parameter");
    }

};
