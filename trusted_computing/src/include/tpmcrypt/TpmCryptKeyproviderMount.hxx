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
 * @file        TpmCryptKeyproviderMount.hxx
 * @brief       Implementation of the TpmCrypt Keyprovider Service.
 * @version     $Revision: 792 $
 * @date        Last modification at $Date: 2009-08-05 15:03:56 +0200 (Wed, 05 Aug 2009) $ by $Author: goericke $
 */

#ifndef _TPMCRYPTKEYPROVIDERMOUNT_HXX
#define	_TPMCRYPTKEYPROVIDERMOUNT_HXX

#include <TpmCrypt.hxx>
#include <TpmCryptHelper.hxx>
#include <TpmCryptStorage.hxx>
#include <TpmCryptConfigfile.hxx>
#include <TpmCryptVolume.hxx>
#include <TpmCryptSubject.hxx>

#include <dirent.h>
#include <limits.h>


using namespace std;

const std::string TpmCryptKeyproviderDB_Identifier = "KeyproviderDB";
const std::string TpmCryptKeyproviderCrypt_Entry = "CryptoSystem";
const std::string TpmCryptKeyproviderDevice = "Device";
        
namespace tpmcrypt
{
    class TpmCryptKeyproviderMount
    {
    public:
	/*!
	 *  @brief constructor for mount operations
	 *  @param cryptoSystem, UInt8, indicates the cryptosystem to be used
	 *  @param device, string, the path to the device to be mounted
	 *  @param destination, string, the path to the mount destination
	 *  @param key, string, the key to perform the decryption
	 *  @param &aTpmCryptConfig, TpmCryptConfigfile, points to the current TpmCrypt configuration
	 *  @param user, string, the user or users, that wish to perform the mount operation
	 *  @param uuid, string, the uuid of the device
	 */
        TpmCryptKeyproviderMount(UInt8 cryptoSystem, string device, string destination, string key, TpmCryptConfigfile &aTpmCryptConfig, string user, string uuid ); // mount
        
	/*!
	 *  @brief constructor for unmount operations
	 *  @param destination, string, the path to the mounted device
	 *  @param &aTpmCryptConfig, TpmCryptConfigfile, points to the current TpmCrypt configuration
	 *  @param user, string, the user or users, that wish to perform the unmount operation
	 */
	TpmCryptKeyproviderMount(string destination, TpmCryptConfigfile &aTpmCryptConfig, string user ); // unmount
        
    private:        
	/*!
	 *  @brief mounts a device encrypted with cryptsetup LUKS
	 *  @param device, string, the path to the device to be mounted
	 *  @param destination, string, the path to the mount destination
	 *  @param key, string, the key to perform the decryption
	 *  @param &myKeyproviderStorage, TpmCryptStorage, points to the current volume database
	 *  @param user, string, the user or users, that wish to perform the mount operation
	 *  @param uuid, string, the uuid of the device
	 */        
	void mountLUKS(string device, string destination, string key, TpmCryptStorage &myKeyproviderStorage, string user, string uuid);
        
	/*!
	 *  @brief unmounts a device encrypted with cryptsetup LUKS
	 *  @param destination, string, the path to the mounted device
	 *  @param &myKeyproviderStorage, TpmCryptStorage, points to the current volume database
	 *  @param user, string, the user or users, that wish to perform the unmount operation
	 */        
	void unmountLUKS(string destination, TpmCryptStorage &myKeyproviderStorage, string user);
        
	/*!
	 *  @brief mounts a device encrypted with DMCRYPT
	 *  @param device, string, the path to the device to be mounted
	 *  @param destination, string, the path to the mount destination
	 *  @param key, string, the key to perform the decryption
	 *  @param &myKeyproviderStorage, TpmCryptStorage, points to the current volume database
	 *  @param user, string, the user or users, that wish to perform the mount operation
	 *  @param uuid, string, the uuid of the device
	 */
	void mountDMCRYPT(string device, string destination, string key, TpmCryptStorage &myKeyproviderStorage, string user, string uuid);

	/*!
	 *  @brief unmounts a device encrypted with DMCRYPT
	 *  @param destination, string, the path to the mounted device
	 *  @param &myKeyproviderStorage, TpmCryptStorage, points to the current volume database
	 *  @param user, string, the user or users, that wish to perform the unmount operation
	 */        
        void unmountDMCRYPT(string destination, TpmCryptStorage &myKeyproviderStorage, string user);

	/*!
	 *  @brief mounts a device encrypted with TRUECRYPT

	 *  @param device, string, the path to the device to be mounted
	 *  @param destination, string, the path to the mount destination
	 *  @param key, string, the key to perform the decryption
	 *  @param &myKeyproviderStorage, TpmCryptStorage, points to the current volume database
	 *  @param user, string, the user or users, that wish to perform the mount operation
	 *  @param uuid, string, the uuid of the device
	 */
        void mountTRUECRYPT(string device, string destination, string key, TpmCryptStorage &myKeyproviderStorage, string user, string uuid);

	/*!
	 *  @brief unmounts a device encrypted with TRUECRYPT
	 *  @param destination, string, the path to the mounted device
	 *  @param &myKeyproviderStorage, TpmCryptStorage, points to the current volume database
	 *  @param user, string, the user or users, that wish to perform the unmount operation
	 */        
        void unmountTRUECRYPT(string destination, TpmCryptStorage &myKeyproviderStorage, string user);

	/*!
	 *  @brief mounts a device encrypted with ECRYPTFS
	 *  @param device, string, the path to the device to be mounted
	 *  @param destination, string, the path to the mount destination
	 *  @param key, string, the key to perform the decryption
	 *  @param &myKeyproviderStorage, TpmCryptStorage, points to the current volume database
	 *  @param user, string, the user or users, that wish to perform the mount operation
	 *  @param uuid, string, the uuid of the device
	 */
        void mountECRYPTFS(string device, string destination, string key, TpmCryptStorage &myKeyproviderStorage, string user, string uuid);

	/*!
	 *  @brief unmounts a device encrypted with ECRYPTFS
	 *  @param destination, string, the path to the mounted device
	 *  @param &myKeyproviderStorage, TpmCryptStorage, points to the current volume database
	 *  @param user, string, the user or users, that wish to perform the unmount operation
	 */        
        void unmountECRYPTFS(string destination, TpmCryptStorage &myKeyproviderStorage, string user);

	/*!
	 *  @brief mounts a device encrypted with ENCFS
	 *  @param device, string, the path to the device to be mounted
	 *  @param destination, string, the path to the mount destination
	 *  @param key, string, the key to perform the decryption
	 *  @param &myKeyproviderStorage, TpmCryptStorage, points to the current volume database
	 *  @param user, string, the user or users, that wish to perform the mount operation
	 *  @param uuid, string, the uuid of the device
	 */
        void mountENCFS(string device, string destination, string key, TpmCryptStorage &myKeyproviderStorage, string user, string uuid);

	/*!
	 *  @brief unmounts a device encrypted with ENCFS
	 *  @param destination, string, the path to the mounted device
	 *  @param &myKeyproviderStorage, TpmCryptStorage, points to the current volume database
	 *  @param user, string, the user or users, that wish to perform the unmount operation
	 */        
        void unmountENCFS(string destination, TpmCryptStorage &myKeyproviderStorage, string user);
    };
}        


#endif	/* _TPMCRYPTKEYPROVIDERMOUNT_HXX */

