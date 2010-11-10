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
 * @file        TpmCryptSubject.hxx
 * @brief       Implementation of the TpmCrypt subject class.
 * @version     $Revision: 790 $
 * @date        Last modification at $Date: 2009-08-04 15:07:58 +0200 (Tue, 04 Aug 2009) $ by $Author: goericke $
 */

#ifndef _TPMCRYPTSUBJECT_HXX
#define	_TPMCRYPTSUBJECT_HXX

#include <TpmCrypt.hxx>
#include <TpmCryptStorage.hxx>
#include <TpmCryptDate.hxx>
#include <TpmCryptKey.hxx>

using namespace std;
using namespace utils;
using namespace tpmcrypt;

// Subject Types
enum myTpmCryptSubjects
{
    SUBJECTTYPE_BEGIN = 0,
    SUBJECTTYPE_UNDEFINED,
    SUBJECTTYPE_USER,
    SUBJECTTYPE_PLATFORM,
    SUBJECTTYPE_TOKEN,
    SUBJECTTYPE_END
};

// Subject entries
const std::string TpmCryptSubject_SubjectProperties= "SubjectProperties";
const std::string TpmCryptSubject_isSubject        = "isSubject";
const std::string TpmCryptSubject_isSubject_true    = "true";
const std::string TpmCryptSubject_isSubject_false   = "false";

const std::string TpmCryptSubject_PasswordSection = "Password";
const std::string TpmCryptSubject_PasswordMagicEntry = "Magic";

const std::string TpmCryptSubject_SubjectName    = "Name";
const std::string TpmCryptSubject_SubjectKeyType = "KeyType";
const std::string TpmCryptSubject_SubjectAuth    = "Auth";
const std::string TpmCryptSubject_SubjectKeyUUID = "KeyUUID";
const std::string TpmCryptSubject_SubjectEMail   = "EMail";
const std::string TpmCryptSubject_SubjectKeyfile = "Keyfile";
const std::string TpmCryptSubject_SubjectCountrycode = "Country";
const std::string TpmCryptSubject_SubjectOrganisation = "Organisation";
const std::string TpmCryptSubject_SubjectOrganisationUnit = "OrganisationUnit";
const std::string TpmCryptSubject_SubjectState = "State";
const std::string TpmCryptSubject_SubjectLocality = "Locality";


const std::string TpmCryptSubject_SubjectType    = "Type";
const std::string TpmCryptSubject_SubjectAdmin   = "Admin";

const std::string TpmCryptSubjectType_Undefined  = "Undefined";
const std::string TpmCryptSubjectType_User       = "User";
const std::string TpmCryptSubjectType_Platform   = "Platform";
const std::string TpmCryptSubjectType_Token      = "Token";

const std::string TpmCryptSubjectLastLogin       = "LastLogin";
const std::string TpmCryptSubjectLastLogin_Year  = "LastLoginYear";
const std::string TpmCryptSubjectLastLogin_Month = "LastLoginMonth";
const std::string TpmCryptSubjectLastLogin_Day   = "LastLoginDay";
const std::string TpmCryptSubjectLastLogin_Hour  = "LastLoginHour";
const std::string TpmCryptSubjectLastLogin_Minute= "LastLoginMinute";
const std::string TpmCryptSubjectLastLogin_Second= "LastLoginSecond";

const std::string TpmCryptSubject_TRUE           = "true";
const std::string TpmCryptSubject_FALSE          = "false";
const std::string TpmCryptSubject_Messages       = "Message";
const std::string TpmCryptSubject_SubjectPlatformSelection = "PCR";

namespace tpmcrypt
{
    // required for cross-reference between KeryonaKey and TpmCryptSubject
    class TpmCryptKey;
    class TpmCryptGroup;
    class TpmCryptSubject
    {
        public:
	    /*!
             *  @brief loads subject by string
	     *  @param subjectIdentifier, string, that contains the identifier of a subject
	     *  @param &storageDB, TpmCryptStorage, that contains the subject database
             */
            TpmCryptSubject( string subjectIdentifier, TpmCryptStorage &storageDB );
	    
	    /*!
             *  @brief loads subject by subjectID
	     *  @param subjectIdentifier, UInt32, that contains the ID of a subject
	     *  @param &storageDB, TpmCryptStorage, that contains the subject database
             */
            TpmCryptSubject( UInt32 subjectID, TpmCryptStorage &storageDB );
 
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
	     *  @param &storageDB, TpmCryptStorage, that contains the subject database
             */
            TpmCryptSubject( UInt8 subjectType,
                            string &subjectName,
                            string &pcr_string,
                            string &subjectEMail,
                            string &subjectCountryCode,
                            string &subjectOrganisation,
                            string &subjectOrganisationUnit,
                            string &subjectState,
                            string &subjectLocality,
                            bool   admin,
                            string &keyDirectory,
                            TpmCryptStorage &storageDB);

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
	     *  @param &storageDB, TpmCryptStorage, that contains the subject database
             */
            TpmCryptSubject( UInt8 subjectType,
                            string &subjectName,
                            string &pcr_string,
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
                            TpmCryptStorage &storageDB);


            ~TpmCryptSubject();

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
	    string getMySubjectAuth() { return mySubjectAuth; };
	    
            /*!
             *  @brief get the email of a subject
	     *  @return string, email of the subject
             */
        string getMySubjectPlatformSelection() { return mySubjectPlatformSelection; };     
         
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
	     *  @return TpmCryptDate, contains the time stamp of the last login.
	     */
	    TpmCryptDate getMyLastLogin();
	    
            /*!
             *  @brief sets the date of the last login
	     *  @param &LoginDate, TpmCryptDate, contains the time stamp of the last login.
	     */	    
            void setLastLogin(TpmCryptDate &LoginDate);

            /*!
             *  @brief encrypts data with the public key of the current subject
	     *  @param &toEncrypt, ByteVector, contains the data to be encrypted
	     * 	@return ByteVector, contains the encrypted data
	     */
	    ByteVector      encryptForSubject(TpmCryptSubject *Subject, ByteVector &toEncrypt);

            /*!
             *  @brief decrypts data with the private key of the current subject
	     *  @param &toDecrypt, ByteVector, contains the data to be decrypted
	     * 	@param &myPassword, contains the password for the private key of the user
	     * 	@return ByteVector, contains the decrypted data
	     */
	    ByteVector      decryptBySubject(TpmCryptSubject *Subject, ByteVector &toDecrypt, string &myPassword);

            /*!
             *  @brief verifies the entered password of a subject
	     * 	@param &myPassword, string, contains the password for the private key of the user
	     * 	@return boolean, determines whether or not the given password was valid
	     */
	    bool verifyPassword(string &myPassword);
	    
	    string verifyAuth(TpmCryptSubject *Subject);

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
            string  mySubjectAuth;
            string  mySubjectPlatformSelection;
            string  mySubjectEMail;
            string  mySubjectKeyfile;
            string  mySubjectCountrycode;
            string  mySubjectOrganisation;
            string  mySubjectOrganisationUnit;
            string  mySubjectState;
            string  mySubjectLocality;

            bool    mySubjectisAdmin;
            TpmCryptKey     *mySubjectKey;
            TpmCryptStorage &mySubjectStorage;

            /*!
             *  @brief  disabled copy constructor
             */
            TpmCryptSubject( const TpmCryptSubject& );

            /*!
             *  @brief  disabled assignment operator
             */
            TpmCryptSubject& operator=( const TpmCryptSubject& );
    };
};

#endif	/* _TPMCRYPTSUBJECT_HXX */

