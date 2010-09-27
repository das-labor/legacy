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
 * @file        TpmCryptGroup.hxx
 * @brief       Implementation of the TpmCrypt group class.
 * @version     $Revision: 800 $
 * @date        Last modification at $Date: 2009-08-11 18:44:01 +0200 (Tue, 11 Aug 2009) $ by $Author: selhorst $
 */

#ifndef _TPMCRYPTGROUP_HXX
#define	_TPMCRYPTGROUP_HXX

#include <TpmCrypt.hxx>
#include <TpmCryptSubject.hxx>
#include <TpmCryptStorage.hxx>
#include <TpmCryptKey.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

// Subject entries
const std::string TpmCryptGroup_GroupProperties= "GroupProperties";
const std::string TpmCryptGroup_isGroup        = "isGroup";
const std::string TpmCryptGroup_isGroup_true    = "true";
const std::string TpmCryptGroup_isGroup_false   = "false";
const std::string TpmCryptGroup_GroupKeyfile = "GroupKey";
const std::string TpmCryptGroup_GroupKeyType = "GroupKeyType";
const std::string TpmCryptGroup_GroupKeyUUID = "GroupKeyUUID";
const std::string TpmCryptGroup_TestVector = "TestVector";
const std::string TpmCryptGroup_GroupKeyIdentifier = "TpmCryptGroup";
const std::string TpmCryptGroup_SubjectList    = "Subjects";
const std::string TpmCryptGroup_SubjectKeys    = "SubjectKeys";
const std::string TpmCryptGroup_GroupKeys      = "GroupKeys";

typedef std::pair<std::string, std::string> SubjectKeyCombination;

namespace keyrona
{
    class TpmCryptKey;
    class TpmCryptSubject;
    class TpmCryptGroup
    {
        public: 
            /*!
             *  @brief constructor of a TpmCrypt group object
             *  @param GroupID, const string, contains the id of the group
             *  @param &GroupStorage, TpmCryptStorage, specifies the current TpmCrypt group storage
             *  @param &UserStorage, TpmCryptStorage, specifies the current TpmCrypt user storage
             */
            TpmCryptGroup( const string GroupID, TpmCryptStorage &GroupStorage, TpmCryptStorage &UserStorage);

            /*!
             *  @brief destructor of a TpmCrypt group object
             */
            ~TpmCryptGroup();

            // Inline implementation of public member functions

            /*!
             *  @brief returns all ids of the subject that belong to the current group
             *  @returns vector of UInt32, contains the subject ids
             */
            vector<UInt32> getMySubjects()  { return myGroupSubjectIDs; };

            /*!
             *  @brief returns the id of the current group
             *  @returns string, contains the group id
             */
            string getMyGroupID()           { return myGroupID; };

            /*!
             *  @brief returns the key file of the current group
             *  @returns string, contains the groups key file
             */
            string getMyGroupKeyfile()      { return myGroupKeyfile; };
                        
            string getMyGroupKeyUUID()      { return myGroupKeyUUID; };

            string getMyGroupKeyType()      { return myGroupKeyType; };

            /*!
             *  @brief prints the information about the current group to the screen
             *  @param verbose, boolean, determines whether or not all information are printed
             */
            void    printGroup(bool verbose);

            /*!
             *  @brief prints the information about the subjects of current group to the screen
             *  @param verbose, boolean, determines whether or not all information are printed
             */
            void    printSubjectsInGroup(bool verbose);

            /*!
             *  @brief deletes the current group
             */
            void    deleteGroup();

            /*!
             *  @brief adds a subject to a group
             *  @param *Subject, TpmCryptSubject, points to the subject to be added
             *  @param &groupKeyPassword, string, contains the password for the group key
             */
            void    addSubjectToGroup(TpmCryptSubject *Subject, string &groupKeyPassword);

            /*!
             *  @brief deletes a subject from a group
             *  @param *Subject, TpmCryptSubject, points to the subject to be deleted
             */
            void    deleteSubjectFromGroup(TpmCryptSubject *Subject);

            /*!
             *  @brief checks if a subject is a member of a group
             *  @param *Subject, TpmCryptSubject, points to the subject whose membership is checked
             *  @return boolean, indicates whether or not the subject is a member of the group
             */
            bool    isSubjectInGroup(TpmCryptSubject *Subject);

            /*!
             *  @brief adds a message for all members of a group
             *  @param message, string, contains the message
             */
            void    addMessageForGroupMembers(string message);

            /*!
             *  @brief retrieves the decrypted group key
             *  @param *Subject, TpmCryptSubject, points to the subject, that wants to decrypt the group key
             *  @param &password, string, contains the password of the subject
             *  @return string, contains the decrypted password
             */
            string  getDecryptedGroupKey(TpmCryptSubject *Subject, string &password);

            /*!
             *  @brief retrieves the decrypted group key for an admin
             *  @param &AdminKeyPassword, string, contains the password of the admin
             *  @return string, contains the decrypted password
             */
            string  getDecryptedGroupKeyByAdmin(string &AdminKeyPassword);

            /*!
             *  @brief encrypts a byte vector with the public group key
             *  @param &toEncrypt, ByteVector, contains the data to be encrypted
             *  @return ByteVector, contains the encrypted data
             */
            ByteVector      encryptForGroup(TpmCryptGroup *Group, ByteVector &toEncrypt);

            /*!
             *  @brief decrypts a byte vector with the private group key
             *  @param &toEncrypt, ByteVector, contains the data to be decrypted
             *  @param &myPassword, string, contains the group password to retrieve the private key
             *  @return ByteVector, contains the decrypted data
             */
            ByteVector      decryptByGroup(TpmCryptGroup *Group, ByteVector &toDecrypt, string &myPassword);

        private:
            /*!
             *  @brief  loads a group from the database
             */
            void loadGroup();

            /*!
             *  @brief  loads a subject from the member list of a group
             */
            void loadSubjectList();

            /*!
             *  @brief  stores a group to the database
             */
            void storeGroup();

            /*!
             *  @brief  stores a subject to the member list of a group
             */
            void storeSubjectList();

            /*!
             *  @brief  deletes a group
             */
            void deleteGroupByAdmin();

            /*!
             *  @brief  disabled copy constructor
             */
            TpmCryptGroup ( const TpmCryptGroup& );

            /*!
             *  @brief  disabled assignment operator
             */
            TpmCryptGroup& operator=( const TpmCryptGroup& );

            vector<UInt32>      myGroupSubjectIDs;
            string              myGroupID;
            string              myGroupKeyfile;
            string				myGroupKeyUUID;
            string				myGroupKeyType;
            TpmCryptKey          *myGroupKey;
            TpmCryptStorage      &myGroupStorage;
            TpmCryptStorage      &mySubjectStorage;
    };

};
#endif	/* _TPMCRYPTGROUP_HXX */

