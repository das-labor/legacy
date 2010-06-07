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
 * @file        KeyronaKeyproviderMount.cxx
 * @brief       Implementation of the Keyrona Keyprovider Service.
 * @version     $Revision: 498 $
 * @date        Last modification at $Date: 2009-04-24 13:40:38 +0200 (Fri, 24 Apr 2009) $ by $Author: selhorst $
 */

#include <KeyronaKeyproviderMount.hxx>

using namespace keyrona;

KeyronaKeyproviderMount::KeyronaKeyproviderMount(UInt8 cryptoSystem, string device, string destination, string key, KeyronaConfigfile &aKeyronaConfig, string user, string uuid )
{
    KeyronaStorage myKeyproviderStorage( KeyronaKeyproviderDB_Identifier, aKeyronaConfig.getConfigfileEntry(KeyronaConfigfile_KeyproviderDBIdentifier) );

    debug << "Converting probably invalid UUID: " << uuid << endl;
    replaceCharInString(&uuid, KeyronaPathSeparatorChar, KeyronaFileSeparatorChar);
    debug << "UUID after conversion: " << uuid << endl;

    switch(cryptoSystem)
    {
        case VOLUMETYPE_LUKS:
            mountLUKS(device, destination, key, myKeyproviderStorage, user, uuid);
            break;
        case VOLUMETYPE_TRUECRYPT:
            mountTRUECRYPT(device, destination, key, myKeyproviderStorage, user, uuid);
            break;
        case VOLUMETYPE_DMCRYPT:
            mountDMCRYPT(device, destination, key, myKeyproviderStorage, user, uuid);
            break;
        case VOLUMETYPE_ECRYPTFS:
            mountECRYPTFS(device, destination, key, myKeyproviderStorage, user, uuid);
            break;
        case VOLUMETYPE_ENCFS:
            mountENCFS(device, destination, key, myKeyproviderStorage, user, uuid);
            break;
        default:
            cout << "The desired cryptosystem has not been implemented!" << endl;
    }
};

KeyronaKeyproviderMount::KeyronaKeyproviderMount(string destination, KeyronaConfigfile &aKeyronaConfig, string user )
{
    KeyronaStorage myKeyproviderStorage( KeyronaKeyproviderDB_Identifier, aKeyronaConfig.getConfigfileEntry(KeyronaConfigfile_KeyproviderDBIdentifier) );
    myKeyproviderStorage.selectSection(user);
    if ( myKeyproviderStorage.findSubSection(destination)){// there is a device mounted under destination
        myKeyproviderStorage.selectSubSection(destination);
        UInt8 cryptoSystem = convertStringtoUInt32(myKeyproviderStorage.getEntry(KeyronaKeyproviderCrypt_Entry));
        switch(cryptoSystem)
        {
            case VOLUMETYPE_LUKS:
                unmountLUKS(destination, myKeyproviderStorage, user);
                break;
            case VOLUMETYPE_TRUECRYPT:
                unmountTRUECRYPT(destination, myKeyproviderStorage, user);
                break;
            case VOLUMETYPE_DMCRYPT:
                unmountDMCRYPT(destination, myKeyproviderStorage, user);
                break;
            case VOLUMETYPE_ECRYPTFS:
                unmountECRYPTFS(destination, myKeyproviderStorage, user);
                break;
            case VOLUMETYPE_ENCFS:
                unmountENCFS(destination, myKeyproviderStorage, user);
                break;
            default:
            cout << "The desired cryptosystem has not been implemented!" << endl;
        }
    }
    else
    {
        throw NotMounted("According to database, user has not mounted '" + destination + "'");
    }
};

void KeyronaKeyproviderMount::mountLUKS(string device, string destination, string key, KeyronaStorage &myKeyproviderStorage, string user, string uuid)
{
    
    myKeyproviderStorage.selectSection(user);
    if ( !myKeyproviderStorage.findSubSection(destination))
    {// there is no other device mounted under destination
        debug << "device: " << device << "\t destination: " << destination << endl;
        string myCommand = (myScriptDirectory + KeyronaPathSeparator + "mount_cryptsetup_luks.sh " + device + " \"keyrona-" + uuid + "\"" + " " + key + " " + destination + " " + logfile);
        int ret = system(myCommand.c_str());
        if (ret)
            throw MountError("An error occured while trying to mount device '" + device + "'. Please look into Keyrona's logfile: '" + logfile + "'");

        myKeyproviderStorage.selectSubSection(destination);
        myKeyproviderStorage.setEntry(KeyronaKeyproviderDevice,uuid);
        myKeyproviderStorage.setEntry(KeyronaKeyproviderCrypt_Entry,convertUInt32toString(VOLUMETYPE_LUKS));
    }
    else
    {
        string myMessage = "Device '" + device + "' already mounted on destination '" + destination + "'";
        debug << myMessage << endl;
        throw AlreadyMounted(myMessage);
    }
}

void KeyronaKeyproviderMount::unmountLUKS(string destination, KeyronaStorage &myKeyproviderStorage, string user)
{
    string uuid = myKeyproviderStorage.getEntry(KeyronaKeyproviderDevice);
    string myCommand = (myScriptDirectory + KeyronaPathSeparator + "unmount_cryptsetup_luks.sh " + destination + " \"keyrona-" + uuid + "\"" + " " + logfile);
    int ret = system(myCommand.c_str());
    if (ret)
        throw UnmountError("An error occured while trying to unmount  '" + destination + "'. Please look into Keyrona's logfile: '" + logfile + "'");
    myKeyproviderStorage.deleteSubSection(destination);
}

void KeyronaKeyproviderMount::mountDMCRYPT(string device, string destination, string key, KeyronaStorage &myKeyproviderStorage, string user, string uuid)
{
    
    myKeyproviderStorage.selectSection(user);
    if ( !myKeyproviderStorage.findSubSection(destination))
    {// there is no other device mounted under destination
        debug << "device: " << device << "\t destination: " << destination << endl;
        string myCommand = (myScriptDirectory + KeyronaPathSeparator + "mount_dmcrypt.sh " + device + " \"keyrona-" + uuid + "\"" + " " + key + " " + destination + " " + logfile );
        int ret = system(myCommand.c_str());
        if (ret)
            throw MountError("An error occured while trying to mount device '" + device + "'. Please look into Keyrona's logfile: '" + logfile + "'");

        myKeyproviderStorage.selectSubSection(destination);
        myKeyproviderStorage.setEntry(KeyronaKeyproviderDevice,uuid);
        myKeyproviderStorage.setEntry(KeyronaKeyproviderCrypt_Entry,convertUInt32toString(VOLUMETYPE_LUKS));
    }
    else
    {
        string myMessage = "Device '" + device + "' already mounted on destination '" + destination + "'";
        debug << myMessage << endl;
        throw AlreadyMounted(myMessage);
    }
}

void KeyronaKeyproviderMount::unmountDMCRYPT(string destination, KeyronaStorage &myKeyproviderStorage, string user)
{
    string uuid = myKeyproviderStorage.getEntry(KeyronaKeyproviderDevice);
    string myCommand = (myScriptDirectory + KeyronaPathSeparator + "unmount_dmcrypt.sh " + destination + " \"keyrona-" + uuid + "\"" + " " + logfile);
    int ret = system(myCommand.c_str());
    if (ret)
        throw UnmountError("An error occured while trying to unmount '" + destination + "'. Please look into Keyrona's logfile: '" + logfile + "'");
    myKeyproviderStorage.deleteSubSection(destination);
}

void KeyronaKeyproviderMount::mountTRUECRYPT(string device, string destination, string key, KeyronaStorage &myKeyproviderStorage, string user, string uuid)
{
    
    myKeyproviderStorage.selectSection(user);
    if ( !myKeyproviderStorage.findSubSection(destination))
    {// there is no other device mounted under destination
        debug << "device: " << device << "\t destination: " << destination << endl;
        string myCommand = (myScriptDirectory + KeyronaPathSeparator + "mount_truecrypt.sh " + device + " " + destination + " " + key + " " + logfile );
        int ret = system(myCommand.c_str());
        if (ret)
            throw MountError("An error occured while trying to mount device '" + device + "'. Please look into Keyrona's logfile: '" + logfile + "'");

        myKeyproviderStorage.selectSubSection(destination);
        myKeyproviderStorage.setEntry(KeyronaKeyproviderDevice,uuid);
        myKeyproviderStorage.setEntry(KeyronaKeyproviderCrypt_Entry,convertUInt32toString(VOLUMETYPE_TRUECRYPT));
    }
    else
    {
        string myMessage = "Device '" + device + "' already mounted on destination '" + destination + "'";
        debug << myMessage << endl;
        throw AlreadyMounted(myMessage);
    }
}

void KeyronaKeyproviderMount::unmountTRUECRYPT(string destination, KeyronaStorage &myKeyproviderStorage, string user)
{
    string uuid = myKeyproviderStorage.getEntry(KeyronaKeyproviderDevice);
    string myCommand = (myScriptDirectory + KeyronaPathSeparator + "unmount_truecrypt.sh " + destination + " " + logfile );
    int ret = system(myCommand.c_str());
    if (ret)
        throw UnmountError("An error occured while trying to unmount '" + destination + "'. Please look into Keyrona's logfile: '" + logfile + "'");
    myKeyproviderStorage.deleteSubSection(destination);
}

void KeyronaKeyproviderMount::mountECRYPTFS(string device, string destination, string key, KeyronaStorage &myKeyproviderStorage, string user, string uuid)
{
    
    myKeyproviderStorage.selectSection(user);
    if ( !myKeyproviderStorage.findSubSection(destination))
    {// there is no other device mounted under destination
        debug << "device: " << device << "\t destination: " << destination << endl;
        string myCommand = (myScriptDirectory + KeyronaPathSeparator + "mount_ecryptfs.sh " + device + " " + destination + " " + key + " " + logfile );
        int ret = system(myCommand.c_str());
        if (ret)
            throw MountError("An error occured while trying to mount device '" + device + "'. Please look into Keyrona's logfile: '" + logfile + "'");

        myKeyproviderStorage.selectSubSection(destination);
        myKeyproviderStorage.setEntry(KeyronaKeyproviderDevice,uuid);
        myKeyproviderStorage.setEntry(KeyronaKeyproviderCrypt_Entry,convertUInt32toString(VOLUMETYPE_ECRYPTFS));
    }
    else
    {
        string myMessage = "Device '" + device + "' already mounted on destination '" + destination + "'";
        debug << myMessage << endl;
        throw AlreadyMounted(myMessage);
    }
}

void KeyronaKeyproviderMount::unmountECRYPTFS(string destination, KeyronaStorage &myKeyproviderStorage, string user)
{
    string uuid = myKeyproviderStorage.getEntry(KeyronaKeyproviderDevice);
    string myCommand = (myScriptDirectory + KeyronaPathSeparator + "unmount_ecryptfs.sh " + destination + " " + logfile );
    int ret = system(myCommand.c_str());
    if (ret)
        throw UnmountError("An error occured while trying to unmount device '" + destination + "'. Please look into Keyrona's logfile: '" + logfile + "'");
    myKeyproviderStorage.deleteSubSection(destination);
}

void KeyronaKeyproviderMount::mountENCFS(string device, string destination, string key, KeyronaStorage &myKeyproviderStorage, string user, string uuid)
{
    myKeyproviderStorage.selectSection(user);
    if ( !myKeyproviderStorage.findSubSection(destination))
    {// there is no other device mounted under destination
        debug << "device: " << device << "\t destination: " << destination << endl;
        string myCommand = (myScriptDirectory + KeyronaPathSeparator + "mount_encfs.sh " + device + " " + destination + " " + key + " " + logfile );
        int ret = system(myCommand.c_str());
        if (ret)
            throw MountError("An error occured while trying to mount device '" + device + "'. Please look into Keyrona's logfile: '" + logfile + "'");

        myKeyproviderStorage.selectSubSection(destination);
        myKeyproviderStorage.setEntry(KeyronaKeyproviderDevice,uuid);
        myKeyproviderStorage.setEntry(KeyronaKeyproviderCrypt_Entry,convertUInt32toString(VOLUMETYPE_ENCFS));
    }
    else
    {
        string myMessage = "Device '" + device + "' already mounted on destination '" + destination + "'";
        debug << myMessage << endl;
        throw AlreadyMounted(myMessage);
    }
}

void KeyronaKeyproviderMount::unmountENCFS(string destination, KeyronaStorage &myKeyproviderStorage, string user)
{
    string uuid = myKeyproviderStorage.getEntry(KeyronaKeyproviderDevice);
    string myCommand = (myScriptDirectory + KeyronaPathSeparator + "unmount_encfs.sh " + destination + " " + logfile );
    int ret = system(myCommand.c_str());
    if (ret)
        throw UnmountError("An error occured while trying to unmount '" + destination + "'. Please look into Keyrona's logfile: '" + logfile + "'");
    myKeyproviderStorage.deleteSubSection(destination);
}
