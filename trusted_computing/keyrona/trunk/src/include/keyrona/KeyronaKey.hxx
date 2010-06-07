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
 * @file        KeyronaKey.hxx
 * @brief       Implementation of the Keyrona Key class.
 * @version     $Revision: 800 $
 * @date        Last modification at $Date: 2009-08-11 18:44:01 +0200 (Tue, 11 Aug 2009) $ by $Author: selhorst $
 */

#ifndef _KEYRONAKEY_HXX
#define	_KEYRONAKEY_HXX

#include <Keyrona.hxx>
#include <KeyronaDate.hxx>
#include <KeyronaLogin.hxx>
#include <KeyronaSubject.hxx>
#include <KeyronaGroup.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

namespace keyrona
{
    // required for cross-reference between KeyronaKey and KeyronaSubject
    class KeyronaSubject;
    class KeyronaGroup;
    class KeyronaKey
    {
        private:
            bool            valid;

        public:
	    /*!
	     *  @brief constructor, opens an existing (subject or group) key
	     *  @param, &keyfile, string, contains path to the keyfile
	     *  @param &label, string, 
             */
            KeyronaKey(string &keyfile, int &bindkeynum);

            /*!
	     *  @brief constructor, generate new subject key
             *  @param, *Subject, KeyronaSubject, object class of the subject, the key is created for
	     *  @param &myPassword, string, password to encrypt the key
             */
            KeyronaKey(KeyronaSubject *Subject, string &subjectpassword, int &type);

            /*!
	     *  @brief constructor, generate new group key
             *  @param, *Group, KeyronaGroup, object class of the group, the key is created for
	     *  @param &GroupKeyPassword, string, password to encrypt the key
             */
            KeyronaKey(KeyronaGroup *Group, string &grouppassword);

            ~KeyronaKey();

            /*!
	     *  @brief checks if a key is valid
             *  @return, boolean, determines whether or not the key is valid
             */
	    bool            isValid()         { return valid; };

            /*!
	     *  @brief prints the information about a key to the screen
             */
            void            printKeyInformation();

            /*!
	     *  @brief encrypts with the public key
             *	@param &toEncrypt, vector of ByteVector, contains the data to be encrypted
             *	@return, vector of ByteVector, contains the encrypted data
             */
            ByteVector  encrypt(ByteVector &toEncrypt, int &bindkeynum);

            /*!
	     *  @brief decrypts with the private key
             *	@param &toDecrypt, vector of ByteVector, contains the data to be decrypted
	     *	@param &myPassword, string, the password for the private key
             *	@return, vector of ByteVector, contains the decrypted data
             */
            ByteVector  decrypt(ByteVector &toDecrypt, int &bindkeynum, string &password);

            /*!
	     *  @brief changes the password of the private key
             *	@param &oldPassword, string, contains the old password
             *	@param &newPassword, string, contains the new password
             *  @return, boolean, determines whether or not the operation was successful
             */
            bool        changePassword(string &oldPassword, string &newPassword);

        private:
            /*!
             *  @brief  disabled copy constructor
             */
            KeyronaKey ( const KeyronaKey& );

            /*!
             *  @brief  disabled assignment operator
             */
            KeyronaKey& operator=( const KeyronaKey& );
    };
};


#endif	/* _KEYRONAKEY_HXX */
