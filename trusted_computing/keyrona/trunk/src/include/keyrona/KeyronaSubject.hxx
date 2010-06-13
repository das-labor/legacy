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
 * @file        KeyronaSubject.hxx
 * @brief       Implementation of the Keyrona subject class.
 * @version     $Revision: 790 $
 * @date        Last modification at $Date: 2009-08-04 15:07:58 +0200 (Tue, 04 Aug 2009) $ by $Author: goericke $
 */

#ifndef _KEYRONASUBJECT_HXX
#define	_KEYRONASUBJECT_HXX

#include <Keyrona.hxx>
#include <KeyronaStorage.hxx>
#include <KeyronaDate.hxx>
#include <KeyronaKey.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

// Subject Types
enum myKeyronaSubjects
{
    SUBJECTTYPE_BEGIN = 0,
    SUBJECTTYPE_UNDEFINED,
    SUBJECTTYPE_USER,
    SUBJECTTYPE_PLATFORM,
    SUBJECTTYPE_TOKEN,
    SUBJECTTYPE_END
};

// Subject entries
const std::string KeyronaSubject_SubjectProperties= "SubjectProperties";
const std::string KeyronaSubject_isSubject        = "isSubject";
const std::string KeyronaSubject_isSubject_true    = "true";
const std::string KeyronaSubject_isSubject_false   = "false";

const std::string KeyronaSubject_PasswordSection = "Password";
const std::string KeyronaSubject_PasswordMagicEntry = "Magic";

const std::string KeyronaSubject_SubjectName    = "Name";
const std::string KeyronaSubject_SubjectKeyUUID = "KeyUUID";
const std::string KeyronaSubject_SubjectKeyType = "KeyType";
const std::string KeyronaSubject_SubjectEMail   = "EMail";
const std::string KeyronaSubject_SubjectKeyfile = "Keyfile";
const std::string KeyronaSubject_SubjectCountrycode = "Country";
const std::string KeyronaSubject_SubjectOrganisation = "Organisation";
const std::string KeyronaSubject_SubjectOrganisationUnit = "OrganisationUnit";
const std::string KeyronaSubject_SubjectState = "State";
const std::string KeyronaSubject_SubjectLocality = "Locality";


const std::string KeyronaSubject_SubjectType    = "Type";
const std::string KeyronaSubject_SubjectAdmin   = "Admin";

const std::string KeyronaSubjectType_Undefined  = "Undefined";
const std::string KeyronaSubjectType_User       = "User";
const std::string KeyronaSubjectType_Platform   = "Platform";
const std::string KeyronaSubjectType_Token      = "Token";

const std::string KeyronaSubjectLastLogin       = "LastLogin";
const std::string KeyronaSubjectLastLogin_Year  = "LastLoginYear";
const std::string KeyronaSubjectLastLogin_Month = "LastLoginMonth";
const std::string KeyronaSubjectLastLogin_Day   = "LastLoginDay";
const std::string KeyronaSubjectLastLogin_Hour  = "LastLoginHour";
const std::string KeyronaSubjectLastLogin_Minute= "LastLoginMinute";
const std::string KeyronaSubjectLastLogin_Second= "LastLoginSecond";

const std::string KeyronaSubject_TRUE           = "true";
const std::string KeyronaSubject_FALSE          = "false";
const std::string KeyronaSubject_Messages       = "Message";

namespace keyrona
{
    // required for cross-reference between KeryonaKey and KeyronaSubject
    class KeyronaKey;
    class KeyronaGroup;
    class KeyronaSubject
    {
        public:
	    /*!
             *  @brief loads subject by string
	     *  @param subjectIdentifier, string, that contains the identifier of a subject
	     *  @param &storageDB, KeyronaStorage, that contains the subject database
             */
            KeyronaSubject( string subjectIdentifier, KeyronaStorage &storageDB );
	    
	    /*!
             *  @brief loads subject by subjectID
	     *  @param subjectIdentifier, UInt32, that contains the ID of a subject
	     *  @param &storageDB, KeyronaStorage, that contains the subject database
             */
            KeyronaSubject( UInt32 subjectID, KeyronaStorage &storageDB );
 
	    /*!
             *  @brief creates new subject and generates a key for the new subject
	     *  @param &subjectName, string, that contains the full name of a subject
	     *  @param &subjectEMail, string, that contains the email address of a subject
	     *  @param &subjectCountryCode, string, that contains the country code of a subject
	     *  @param &subjectOrganisation, string, that contains the organisation a subject is related to
	     *  @param &subjectOrganisationUnit, string, that contains organisation unit a subject is related to
	     *  @param &subjectLocality, string, that contains place a subject is located
	     *  @param admin, boolean, determines whether or not a subject is an admin
	     *  @param &keyDirectory, string, that contains the directory containing the key of a subject
	     *  @param &storageDB, KeyronaStorage, that contains the subject database
             */
            KeyronaSubject( UInt8 subjectType,
                            string &subjectName,
                            string    &subjectKeyUUID,
                            string &subjectKeyType,
                            string &subjectEMail,
                            string &subjectCountryCode,
                            string &subjectOrganisation,
                            string &subjectOrganisationUnit,
                            string &subjectState,
                            string &subjectLocality,
                            bool   admin,
                            string &keyDirectory,
                            KeyronaStorage &storageDB);

	    /*!
             *  @brief imports subject without key generation
	     *  @param &subjectName, string, that contains the full name of a subject
	     *  @param &subjectEMail, string, that contains the email address of a subject
	     *  @param &subjectCountryCode, string, that contains the country code of a subject
	     *  @param &subjectOrganisation, string, that contains the organisation a subject is related to
	     *  @param &subjectOrganisationUnit, string, that contains organisation unit a subject is related to
	     *  @param &subjectLocality, string, that contains place a subject is located
	     *  @param admin, boolean, determines whether or not a subject is an admin
	     *  @param &keyDirectory, string, that contains the directory containing the key of a subject
	     *  @param &subjectKeyfile, string, path to the file that contains the key of a subject
	     *  @param copyKeyfile, boolean, determines whether or not the keyfile should be copied
	     *  @param &storageDB, KeyronaStorage, that contains the subject database
             */
            KeyronaSubject( UInt8 subjectType,
                            string &subjectName,
                            string    &subjectKeyUUID,
                            string &subjectKeyType,
                            string &subjectEMail,
                            string &subjectCountryCode,
                            string &subjectOrganisation,
                            string &subjectOrganisationUnit,
                            string &subjectState,
                            string &subjectLocality,
                            bool   admin,
                            string &keyDirectory,
                            string &subjectKeyfile,
                            bool copyKeyfile,
                            KeyronaStorage &storageDB);


            ~KeyronaSubject();

            // Inline implementation of public member functions
	    /*!
             *  @brief get the type of a subject
	     *  @return UInt8, subject type
             */
            UInt8  getMySubjectType()   { return mySubjectType; };

	    /*!
             *  @brief get the ID of a subject as integer
	     *  @return UInt32, ID of the subject
             */
	    UInt32 getMySubjectID()     { return mySubjectID; };

            /*!
             *  @brief get the ID of a subject as string
	     *  @return string, ID of the subject
             */
	    string getMySubjectIDString()   { return mySubjectIDString; };

            /*!
             *  @brief get the name of a subject
	     *  @return string, name of the subject
             */
	    string getMySubjectName()   { return mySubjectName; };
	    
	        /*!
             *  @brief get the keyuuid of a subject
	     *  @return int, keyuuid of the subject
             */
	    string getMySubjectKeyUUID()   { return mySubjectKeyUUID; };
	   
	        /*!
             *  @brief get the keytype of a subject
	     *  @return string, keytype of the subject
             */
	    string getMySubjectKeyType()  { return mySubjectKeyType; };
	    
            /*!
             *  @brief get the email of a subject
	     *  @return string, email of the subject
             */
	    string getMySubjectEMail()  { return mySubjectEMail; };

            /*!
             *  @brief get the country code of a subject
	     *  @return string, country code of the subject
             */
	    string getMySubjectCountrycode()  { return mySubjectCountrycode; };

            /*!
             *  @brief get the organisation of a subject
	     *  @return string, organisation of the subject
             */
	    string getMySubjectOrganisation()  { return mySubjectOrganisation; };

            /*!
             *  @brief get the organisation unit of a subject
	     *  @return string, organisation unit of the subject
             */
	    string getMySubjectOrganisationUnit()  { return mySubjectOrganisationUnit; };

            /*!
             *  @brief get the state of a subject
	     *  @return string, state of the subject
             */
	    string getMySubjectState()  { return mySubjectState; };

            /*!
             *  @brief get the localisation of a subject
	     *  @return string, localisation of the subject
             */
	    string getMySubjectLocality()  { return mySubjectLocality; };

            /*!
             *  @brief get the keyfile of a subject
	     *  @return string, keyfile of the subject
             */
	    string getMySubjectKeyfile(){ return mySubjectKeyfile; };

            /*!
             *  @brief checks if the subject is an admin
	     *  @return boolean, determines whether or not the subject is an admin
             */
	    bool   isAdmin()            { return mySubjectisAdmin; };

            /*!
             *  @brief prints the information of a subject to the screen
	     *  @param verbose, boolean, determines whether or not all information are printed, or only the name of the subject
             */
            void   printSubject(bool verbose);

	    /*!
             *  @brief removes subject from database
	     */
            void   deleteSubject();

            /*!
             *  @brief returns the date of the last login
	     *  @return KeyronaDate, contains the time stamp of the last login.
	     */
	    KeyronaDate getMyLastLogin();
	    
	    string setKeyOption();

            /*!
             *  @brief sets the date of the last login
	     *  @param &LoginDate, KeyronaDate, contains the time stamp of the last login.
	     */	    
            void setLastLogin(KeyronaDate &LoginDate);

            /*!
             *  @brief encrypts data with the public key of the current subject
	     *  @param &toEncrypt, ByteVector, contains the data to be encrypted
	     * 	@return ByteVector, contains the encrypted data
	     */
	    ByteVector      encryptForSubject(ByteVector &toEncrypt);

            /*!
             *  @brief decrypts data with the private key of the current subject
	     *  @param &toDecrypt, ByteVector, contains the data to be decrypted
	     * 	@param &myPassword, contains the password for the private key of the user
	     * 	@return ByteVector, contains the decrypted data
	     */
	    ByteVector      decryptBySubject(ByteVector &toDecrypt, string &myPassword);

            /*!
             *  @brief verifies the entered password of a subject
	     * 	@param &myPassword, string, contains the password for the private key of the user
	     * 	@return boolean, determines whether or not the given password was valid
	     */
	    bool verifyPassword(string &myPassword);

            /*!
             *  @brief changes the current password of a subject
	     * 	@param &oldPassword, string, contains the old password for the private key of the user
	     * 	@param &newPassword, string, contains the new password for the private key of the user
	     * 	@return boolean, determines whether or not password change succeeded
	     */
	    bool changePassword( string &oldPassword, string &newPassword);

            /*!
             *  @brief adds a new message for a user
	     * 	@param &message, string, contains the new message for a user
	     */
	    void addMessageForUser(string &message);

            /*!
             *  @brief prints all messages of a user to the screen
	     */
	    void printCurrentMessagesForUser();

            /*!
             *  @brief returns all messages for the current user
	     * 	@return vector of StringPair, contains the messages of a user
	     */
	    vector<StringPair> getCurrentMessagesForUser();

            /*!
             *  @brief deletes all messages of a user
	     */
	    void deleteUserMessages();

        private:
	    /*!
             *  @brief stores subject in the database
	     */
            bool storeSubject();
	    
	    /*!
             *  @brief loads subject from the database
	     */
            bool loadSubject(string &subjectIdentifier);
	    
	    /*!
             *  @brief  checks if entered username is valid and not empty
	     */
            void checkUsername();

            UInt8   mySubjectType;
            UInt32  mySubjectID;
            string  mySubjectIDString;
            string  mySubjectName;
            string	mySubjectKeyUUID;
            string  mySubjectKeyType;
            string  mySubjectEMail;
            string  mySubjectKeyfile;
            string  mySubjectCountrycode;
            string  mySubjectOrganisation;
            string  mySubjectOrganisationUnit;
            string  mySubjectState;
            string  mySubjectLocality;

            bool    mySubjectisAdmin;
            KeyronaKey     *mySubjectKey;
            KeyronaStorage &mySubjectStorage;

            /*!
             *  @brief  disabled copy constructor
             */
            KeyronaSubject( const KeyronaSubject& );

            /*!
             *  @brief  disabled assignment operator
             */
            KeyronaSubject& operator=( const KeyronaSubject& );
    };
};

#endif	/* _KEYRONASUBJECT_HXX */

