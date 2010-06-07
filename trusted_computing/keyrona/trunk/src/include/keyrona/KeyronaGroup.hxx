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
 * @file        KeyronaGroup.hxx
 * @brief       Implementation of the Keyrona group class.
 * @version     $Revision: 800 $
 * @date        Last modification at $Date: 2009-08-11 18:44:01 +0200 (Tue, 11 Aug 2009) $ by $Author: selhorst $
 */

#ifndef _KEYRONAGROUP_HXX
#define	_KEYRONAGROUP_HXX

#include <Keyrona.hxx>
#include <KeyronaSubject.hxx>
#include <KeyronaStorage.hxx>
#include <KeyronaKey.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

// Subject entries
const std::string KeyronaGroup_GroupProperties= "GroupProperties";
const std::string KeyronaGroup_isGroup        = "isGroup";
const std::string KeyronaGroup_isGroup_true    = "true";
const std::string KeyronaGroup_isGroup_false   = "false";
const std::string KeyronaGroup_GroupKeyfile = "GroupKey";
const std::string KeyronaGroup_TestVector = "TestVector";

const std::string KeyronaGroup_GroupKeyIdentifier = "KeyronaGroup";
const std::string KeyronaGroup_SubjectList    = "Subjects";
const std::string KeyronaGroup_SubjectKeys    = "SubjectKeys";
const std::string KeyronaGroup_GroupKeys      = "GroupKeys";

typedef std::pair<std::string, std::string> SubjectKeyCombination;

namespace keyrona
{
    class KeyronaKey;
    class KeyronaSubject;
    class KeyronaGroup
    {
        public: 
            /*!
             *  @brief constructor of a Keyrona group object
             *  @param GroupID, const string, contains the id of the group
             *  @param &GroupStorage, KeyronaStorage, specifies the current Keyrona group storage
             *  @param &UserStorage, KeyronaStorage, specifies the current Keyrona user storage
             */
            KeyronaGroup( const string GroupID, KeyronaStorage &GroupStorage, KeyronaStorage &UserStorage);

            /*!
             *  @brief destructor of a Keyrona group object
             */
            ~KeyronaGroup();

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
             *  @param *Subject, KeyronaSubject, points to the subject to be added
             *  @param &groupKeyPassword, string, contains the password for the group key
             */
            void    addSubjectToGroup(KeyronaSubject *Subject, string &groupKeyPassword);

            /*!
             *  @brief deletes a subject from a group
             *  @param *Subject, KeyronaSubject, points to the subject to be deleted
             */
            void    deleteSubjectFromGroup(KeyronaSubject *Subject);

            /*!
             *  @brief checks if a subject is a member of a group
             *  @param *Subject, KeyronaSubject, points to the subject whose membership is checked
             *  @return boolean, indicates whether or not the subject is a member of the group
             */
            bool    isSubjectInGroup(KeyronaSubject *Subject);

            /*!
             *  @brief adds a message for all members of a group
             *  @param message, string, contains the message
             */
            void    addMessageForGroupMembers(string message);

            /*!
             *  @brief retrieves the decrypted group key
             *  @param *Subject, KeyronaSubject, points to the subject, that wants to decrypt the group key
             *  @param &password, string, contains the password of the subject
             *  @return string, contains the decrypted password
             */
            string  getDecryptedGroupKey(KeyronaSubject *Subject, string &password);

            /*!
             *  @brief retrieves the decrypted group key for an admin
             *  @param &AdminKeyPassword, string, contains the password of the admin
             *  @return string, contains the decrypted password
             */
            string  getDecryptedGroupKeyByAdmin(string &AdminKeyPassword);

            /*!
             *  @brief encrypts an integer vector with the public group key
             *  @param &toEncrypt, vector of UInt8, contains the data to be encrypted
             *  @return vector of UInt8, contains the encrypted data
             */
            vector<UInt8>   encryptForGroup(vector<UInt8> &toEncrypt);

            /*!
             *  @brief decrypts an integer vector with the private group key
             *  @param &toDecrypt, vector of UInt8, contains the data to be decrypted
             *  @param &myPassword, string, contains the group password to retrieve the private key
             *  @return vector of UInt8, contains the decrypted data
             */
            vector<UInt8>   decryptByGroup(vector<UInt8> &toDecrypt, string &myPassword);

            /*!
             *  @brief encrypts a byte vector with the public group key
             *  @param &toEncrypt, ByteVector, contains the data to be encrypted
             *  @return ByteVector, contains the encrypted data
             */
            ByteVector      encryptForGroup(ByteVector &toEncrypt);

            /*!
             *  @brief decrypts a byte vector with the private group key
             *  @param &toEncrypt, ByteVector, contains the data to be decrypted
             *  @param &myPassword, string, contains the group password to retrieve the private key
             *  @return ByteVector, contains the decrypted data
             */
            ByteVector      decryptByGroup(ByteVector &toDecrypt, string &myPassword);

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
            KeyronaGroup ( const KeyronaGroup& );

            /*!
             *  @brief  disabled assignment operator
             */
            KeyronaGroup& operator=( const KeyronaGroup& );

            vector<UInt32>      myGroupSubjectIDs;
            string              myGroupID;
            string              myGroupKeyfile;
            KeyronaKey          *myGroupKey;
            KeyronaStorage      &myGroupStorage;
            KeyronaStorage      &mySubjectStorage;
    };

};
#endif	/* _KEYRONAGROUP_HXX */

