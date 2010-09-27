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
 * @file        TpmCryptVolume.hxx
 * @brief       Implementation of the TpmCrypt volume class.
 * @version     $Revision: 779 $
 * @date        Last modification at $Date: 2009-07-31 15:20:20 +0200 (Fri, 31 Jul 2009) $ by $Author: selhorst $
 */

#ifndef _TpmCryptVolume_HXX
#define	_TpmCryptVolume_HXX

#include <TpmCrypt.hxx>
#include <TpmCryptStorage.hxx>
#include <TpmCryptGroup.hxx>

using namespace std;
using namespace utils;

// Volume Types
enum myTpmCryptVolumeTypes
{
    VOLUMETYPE_BEGIN = 0,
    VOLUMETYPE_DMCRYPT,
    VOLUMETYPE_TRUECRYPT,
    VOLUMETYPE_ECRYPTFS,
    VOLUMETYPE_LUKS,
    VOLUMETYPE_ENCFS,
    VOLUMETYPE_END
};

// Volume Types
enum myTpmCryptFilesystemTypes
{
    FSTYPE_BEGIN = 0,
    FSTYPE_EXT2,
    FSTYPE_EXT3,
    FSTYPE_EXT4,
    FSTYPE_NTFS,
    FSTYPE_REISERFS,
    FSTYPE_VFAT,
    FSTYPE_XFS,
    FSTYPE_END
};

// Volume entries
const std::string TpmCryptVolume_GenericDB       = "Generic";
const std::string TpmCryptVolume_isVolume        = "isVolume";
const std::string TpmCryptVolume_isVolume_true   = "true";
const std::string TpmCryptVolume_isVolume_false  = "false";
const std::string TpmCryptVolume_VolumeUUID      = "UUID";
const std::string TpmCryptVolume_VolumePATH      = "Path";
const std::string TpmCryptVolume_VolumeType      = "Type";

const std::string TpmCryptVolumeType_DMCRYPT     = "DMCRYPT";
const std::string TpmCryptVolumeType_TRUECRYPT   = "TRUECRYPT";
const std::string TpmCryptVolumeType_ECRYPTFS    = "ECRYPTFS";
const std::string TpmCryptVolumeType_LUKS        = "LUKS";
const std::string TpmCryptVolumeType_ENCFS       = "ENCFS";

const std::string TpmCryptVolume_SubjectKeyDB    = "SubjectKeys";
const std::string TpmCryptVolume_GroupKeyDB      = "GroupKeys";
const std::string TpmCryptVolume_SSSDB           = "Secret-Sharing-Schemes";
const std::string TpmCryptVolume_SSSDB_valid     = "true";


namespace tpmcrypt
{
    class TpmCryptVolume
    {
        public:
            TpmCryptVolume(  const string &VolumeUUID, TpmCryptStorage &storageDB );
            TpmCryptVolume(  const string &VolumeUUID,
                            const string &VolumePath,
                            const string &VolumePass,
                            UInt8 VolumeEncryptionSystem,
                            TpmCryptGroup *AdminGroup,
                            TpmCryptStorage &storageDB);

            ~TpmCryptVolume();

            // Inline implementation of public member functions
            UInt8  getMyVolumeType()    { return myVolumeType; };
            string getMyVolumeUUID()    { return myVolumeUUID; };
            string getMyVolumePath()    { return myVolumePath; };

	    /*!
             *  @brief  changes the path of the volume in the database
	     */
            void   updateDevicePath(string &newDevicePath);	    
	    
	    /*!
             *  @brief adds a subject to a volume
	     *  @param Subject, pointer of TpmCryptSubject, that contains the subject to be added
	     *  @param VolumePassword, the password for the currently selected volume
             */
            void   addSubject(TpmCryptSubject *Subject, const string &VolumePassword);	    
	    
	    /*!
             *  @brief removes subject from volume
	     *  @param Subject, pointer of TpmCryptSubject, that contains the subject to be removed
             */
            void   deleteSubject(TpmCryptSubject *Subject);            
	    
	    /*!
             *  @brief prints all subjects to the screen, that have access to the currently selected volume
             */
            void   listSubjects();	    
	    
	    /*!
             *  @brief returns the subjects that have access to the currently selected volume
	     *  @return vector of string, that contains the names of subjects in volume
             */
            vector<string> getSubjects();
	    
	    /*!
             *  @brief checks if a subject has access to a volume
	     *  @param Subject, pointer of TpmCryptSubject, that contains the subject to be checked
	     *  @return boolean, that indicates, if the subject has access to a volume
             */
            bool isSubjectInVolume(TpmCryptSubject *Subject);

	    /*!
             *  @brief adds a group to a volume
	     *  @param Group, pointer of TpmCryptGroup, that contains the group to be added
	     *  @param VolumePassword, the password for the currently selected volume
             */
            void   addGroup(TpmCryptGroup *Group, const string &VolumePassword);
	    
	    /*!
             *  @brief removes group from volume
	     *  @param Group, pointer of TpmCryptGroup, that contains the group to be removed
             */
            void   deleteGroup(TpmCryptGroup *Group);
            
	    /*!
             *  @brief prints all groups to the screen, that have access to the currently selected volume
             */
	     void   listGroups();
	    
	    /*!
             *  @brief returns the groups that have access to the currently selected volume
	     *  @return vector of string, that contains the names of groups in volume
             */
	    vector<string> getGroups();

	    /*!
             *  @brief checks if a group has access to a volume
	     *  @param Group, pointer of TpmCryptGroup, that contains the group, that is checked
	     *  @return boolean, that indicates, if the group has access to a volume
             */
            bool isGroupInVolume(TpmCryptGroup *Group);
	    

            /*!
             *  @brief returns the decrypted volume key of a volume
	     *  @param Subject, pointer of TpmCryptSubject, that wants to access the volume
	     *  @param password, address of a string, that contains the subject's password
	     *  @return string, that contains the decrypted volume key
             */
	     string getDecryptedVolumeKey(TpmCryptSubject *Subject, string &password);
	    
            /*!
             *  @brief returns the decrypted volume key of a volume
	     *  @param Group, pointer of TpmCryptGroup, that contains the group, that wants to access the volume
	     *  @param password, address of a string, that contains the group's password
	     *  @return string, that contains the decrypted volume key
             */
            string getDecryptedVolumeKey(TpmCryptGroup *Group, string &password);

            /*!
             *  @brief prints the information about a volume to the screen
	     */
	    void printVolume();

            /*!
             *  @brief deletes a volume from the database
             */
            void deleteVolume();

            // Secret-Sharing-Scheme
            /*!
             *  @brief adds a new secret sharing scheme to the volume
	     *  @param SSSID string, that contains the ID of the secret sharing scheme
             */
            void addSSS(string &SSSID);

            /*!
             *  @brief returns the available secret sharing schemes in the current volume
	     *  @return vector of strings with the available secret sharing schemes
             */
            vector<string> getAvailableSSS();

            /*!
             *  @brief deletes a secret sharing scheme from the volume
	     *  @param SSSID string, that contains the ID of the secret sharing scheme
             */
	     void deleteSSS(string &SSSID);

        private:
            /*!
             *  @brief stores a volume in the volume database
             */
            void storeVolume();

            /*!
             *  @brief loads a volume from the volume database
             */
	    void loadVolume();

            string  myVolumeUUID;
            string  myVolumePath;
            UInt8   myVolumeType;
            TpmCryptStorage &myVolumeStorage;

            /*!
             *  @brief  disabled copy constructor
             */
            TpmCryptVolume( const TpmCryptVolume& );

            /*!
             *  @brief  disabled assignment operator
             */
            TpmCryptVolume& operator=( const TpmCryptVolume& );
    };
};

#endif	/* _TpmCryptVolume_HXX */

