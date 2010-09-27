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


#include <TpmCryptKeyproviderMount.hxx>

using namespace keyrona;

TpmCryptKeyproviderMount::TpmCryptKeyproviderMount(UInt8 cryptoSystem, string device, string destination, string key, TpmCryptConfigfile &aTpmCryptConfig, string user, string uuid )
{
    TpmCryptStorage myKeyproviderStorage( TpmCryptKeyproviderDB_Identifier, aTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_KeyproviderDBIdentifier) );

    debug << "Converting probably invalid UUID: " << uuid << endl;
    replaceCharInString(&uuid, TpmCryptPathSeparatorChar, TpmCryptFileSeparatorChar);
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

TpmCryptKeyproviderMount::TpmCryptKeyproviderMount(string destination, TpmCryptConfigfile &aTpmCryptConfig, string user )
{
    TpmCryptStorage myKeyproviderStorage( TpmCryptKeyproviderDB_Identifier, aTpmCryptConfig.getConfigfileEntry(TpmCryptConfigfile_KeyproviderDBIdentifier) );
    myKeyproviderStorage.selectSection(user);
    if ( myKeyproviderStorage.findSubSection(destination)){// there is a device mounted under destination
        myKeyproviderStorage.selectSubSection(destination);
        UInt8 cryptoSystem = convertStringtoUInt32(myKeyproviderStorage.getEntry(TpmCryptKeyproviderCrypt_Entry));
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

void TpmCryptKeyproviderMount::mountLUKS(string device, string destination, string key, TpmCryptStorage &myKeyproviderStorage, string user, string uuid)
{
    
    myKeyproviderStorage.selectSection(user);
    if ( !myKeyproviderStorage.findSubSection(destination))
    {// there is no other device mounted under destination
        debug << "device: " << device << "\t destination: " << destination << endl;
        string myCommand = (myScriptDirectory + TpmCryptPathSeparator + "mount_cryptsetup_luks.sh " + device + " \"keyrona-" + uuid + "\"" + " " + key + " " + destination + " " + logfile);
        cout << myCommand << endl;
        int ret = system(myCommand.c_str());
        if (ret)
            throw MountError("An error occured while trying to mount device '" + device + "'. Please look into TpmCrypt's logfile: '" + logfile + "'");

        myKeyproviderStorage.selectSubSection(destination);
        myKeyproviderStorage.setEntry(TpmCryptKeyproviderDevice,uuid);
        myKeyproviderStorage.setEntry(TpmCryptKeyproviderCrypt_Entry,convertUInt32toString(VOLUMETYPE_LUKS));
    }
    else
    {
        string myMessage = "Device '" + device + "' already mounted on destination '" + destination + "'";
        debug << myMessage << endl;
        throw AlreadyMounted(myMessage);
    }
}

void TpmCryptKeyproviderMount::unmountLUKS(string destination, TpmCryptStorage &myKeyproviderStorage, string user)
{
    string uuid = myKeyproviderStorage.getEntry(TpmCryptKeyproviderDevice);
    string myCommand = (myScriptDirectory + TpmCryptPathSeparator + "unmount_cryptsetup_luks.sh " + destination + " \"keyrona-" + uuid + "\"" + " " + logfile);
    int ret = system(myCommand.c_str());
    if (ret)
        throw UnmountError("An error occured while trying to unmount  '" + destination + "'. Please look into TpmCrypt's logfile: '" + logfile + "'");
    myKeyproviderStorage.deleteSubSection(destination);
}

void TpmCryptKeyproviderMount::mountDMCRYPT(string device, string destination, string key, TpmCryptStorage &myKeyproviderStorage, string user, string uuid)
{
    
    myKeyproviderStorage.selectSection(user);
    if ( !myKeyproviderStorage.findSubSection(destination))
    {// there is no other device mounted under destination
        debug << "device: " << device << "\t destination: " << destination << endl;
        string myCommand = (myScriptDirectory + TpmCryptPathSeparator + "mount_dmcrypt.sh " + device + " \"keyrona-" + uuid + "\"" + " " + key + " " + destination + " " + logfile );
        int ret = system(myCommand.c_str());
        if (ret)
            throw MountError("An error occured while trying to mount device '" + device + "'. Please look into TpmCrypt's logfile: '" + logfile + "'");

        myKeyproviderStorage.selectSubSection(destination);
        myKeyproviderStorage.setEntry(TpmCryptKeyproviderDevice,uuid);
        myKeyproviderStorage.setEntry(TpmCryptKeyproviderCrypt_Entry,convertUInt32toString(VOLUMETYPE_LUKS));
    }
    else
    {
        string myMessage = "Device '" + device + "' already mounted on destination '" + destination + "'";
        debug << myMessage << endl;
        throw AlreadyMounted(myMessage);
    }
}

void TpmCryptKeyproviderMount::unmountDMCRYPT(string destination, TpmCryptStorage &myKeyproviderStorage, string user)
{
    string uuid = myKeyproviderStorage.getEntry(TpmCryptKeyproviderDevice);
    string myCommand = (myScriptDirectory + TpmCryptPathSeparator + "unmount_dmcrypt.sh " + destination + " \"keyrona-" + uuid + "\"" + " " + logfile);
    int ret = system(myCommand.c_str());
    if (ret)
        throw UnmountError("An error occured while trying to unmount '" + destination + "'. Please look into TpmCrypt's logfile: '" + logfile + "'");
    myKeyproviderStorage.deleteSubSection(destination);
}

void TpmCryptKeyproviderMount::mountTRUECRYPT(string device, string destination, string key, TpmCryptStorage &myKeyproviderStorage, string user, string uuid)
{
    
    myKeyproviderStorage.selectSection(user);
    if ( !myKeyproviderStorage.findSubSection(destination))
    {// there is no other device mounted under destination
        debug << "device: " << device << "\t destination: " << destination << endl;
        string myCommand = (myScriptDirectory + TpmCryptPathSeparator + "mount_truecrypt.sh " + device + " " + destination + " " + key + " " + logfile );
        int ret = system(myCommand.c_str());
        if (ret)
            throw MountError("An error occured while trying to mount device '" + device + "'. Please look into TpmCrypt's logfile: '" + logfile + "'");

        myKeyproviderStorage.selectSubSection(destination);
        myKeyproviderStorage.setEntry(TpmCryptKeyproviderDevice,uuid);
        myKeyproviderStorage.setEntry(TpmCryptKeyproviderCrypt_Entry,convertUInt32toString(VOLUMETYPE_TRUECRYPT));
    }
    else
    {
        string myMessage = "Device '" + device + "' already mounted on destination '" + destination + "'";
        debug << myMessage << endl;
        throw AlreadyMounted(myMessage);
    }
}

void TpmCryptKeyproviderMount::unmountTRUECRYPT(string destination, TpmCryptStorage &myKeyproviderStorage, string user)
{
    string uuid = myKeyproviderStorage.getEntry(TpmCryptKeyproviderDevice);
    string myCommand = (myScriptDirectory + TpmCryptPathSeparator + "unmount_truecrypt.sh " + destination + " " + logfile );
    int ret = system(myCommand.c_str());
    if (ret)
        throw UnmountError("An error occured while trying to unmount '" + destination + "'. Please look into TpmCrypt's logfile: '" + logfile + "'");
    myKeyproviderStorage.deleteSubSection(destination);
}

void TpmCryptKeyproviderMount::mountECRYPTFS(string device, string destination, string key, TpmCryptStorage &myKeyproviderStorage, string user, string uuid)
{
    
    myKeyproviderStorage.selectSection(user);
    if ( !myKeyproviderStorage.findSubSection(destination))
    {// there is no other device mounted under destination
        debug << "device: " << device << "\t destination: " << destination << endl;
        string myCommand = (myScriptDirectory + TpmCryptPathSeparator + "mount_ecryptfs.sh " + device + " " + destination + " " + key + " " + logfile );
        int ret = system(myCommand.c_str());
        if (ret)
            throw MountError("An error occured while trying to mount device '" + device + "'. Please look into TpmCrypt's logfile: '" + logfile + "'");

        myKeyproviderStorage.selectSubSection(destination);
        myKeyproviderStorage.setEntry(TpmCryptKeyproviderDevice,uuid);
        myKeyproviderStorage.setEntry(TpmCryptKeyproviderCrypt_Entry,convertUInt32toString(VOLUMETYPE_ECRYPTFS));
    }
    else
    {
        string myMessage = "Device '" + device + "' already mounted on destination '" + destination + "'";
        debug << myMessage << endl;
        throw AlreadyMounted(myMessage);
    }
}

void TpmCryptKeyproviderMount::unmountECRYPTFS(string destination, TpmCryptStorage &myKeyproviderStorage, string user)
{
    string uuid = myKeyproviderStorage.getEntry(TpmCryptKeyproviderDevice);
    string myCommand = (myScriptDirectory + TpmCryptPathSeparator + "unmount_ecryptfs.sh " + destination + " " + logfile );
    int ret = system(myCommand.c_str());
    if (ret)
        throw UnmountError("An error occured while trying to unmount device '" + destination + "'. Please look into TpmCrypt's logfile: '" + logfile + "'");
    myKeyproviderStorage.deleteSubSection(destination);
}

void TpmCryptKeyproviderMount::mountENCFS(string device, string destination, string key, TpmCryptStorage &myKeyproviderStorage, string user, string uuid)
{
    myKeyproviderStorage.selectSection(user);
    if ( !myKeyproviderStorage.findSubSection(destination))
    {// there is no other device mounted under destination
        debug << "device: " << device << "\t destination: " << destination << endl;
        string myCommand = (myScriptDirectory + TpmCryptPathSeparator + "mount_encfs.sh " + device + " " + destination + " " + key + " " + logfile );
        int ret = system(myCommand.c_str());
        if (ret)
            throw MountError("An error occured while trying to mount device '" + device + "'. Please look into TpmCrypt's logfile: '" + logfile + "'");

        myKeyproviderStorage.selectSubSection(destination);
        myKeyproviderStorage.setEntry(TpmCryptKeyproviderDevice,uuid);
        myKeyproviderStorage.setEntry(TpmCryptKeyproviderCrypt_Entry,convertUInt32toString(VOLUMETYPE_ENCFS));
    }
    else
    {
        string myMessage = "Device '" + device + "' already mounted on destination '" + destination + "'";
        debug << myMessage << endl;
        throw AlreadyMounted(myMessage);
    }
}

void TpmCryptKeyproviderMount::unmountENCFS(string destination, TpmCryptStorage &myKeyproviderStorage, string user)
{
    string uuid = myKeyproviderStorage.getEntry(TpmCryptKeyproviderDevice);
    string myCommand = (myScriptDirectory + TpmCryptPathSeparator + "unmount_encfs.sh " + destination + " " + logfile );
    int ret = system(myCommand.c_str());
    if (ret)
        throw UnmountError("An error occured while trying to unmount '" + destination + "'. Please look into TpmCrypt's logfile: '" + logfile + "'");
    myKeyproviderStorage.deleteSubSection(destination);
}
