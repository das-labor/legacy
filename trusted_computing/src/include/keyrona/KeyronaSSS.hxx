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
 * @file        KeyronaSSS.hxx
 * @brief       Keyrona Secret Sharing Scheme class
 * @version     $Revision: 800 $
 * @date        Last modification at $Date: 2009-08-11 18:44:01 +0200 (Tue, 11 Aug 2009) $ by $Author: selhorst $
 */

#ifndef _KEYRONASSS_HXX
#define	_KEYRONASSS_HXX

#include <Keyrona.hxx>
#include <KeyronaSubject.hxx>
#include <KeyronaStorage.hxx>
#include <KeyronaGroup.hxx>
#include <Integer.hxx>
#include <SSS.hxx>
#include <ConversionPrimitives.hxx>


const std::string KeyronaSSS_Properties = "SSSProperties";

const std::string KeyronaSSS_SSS_MinimumParticipants = "RequiredParticipants";

const std::string KeyronaSSS_Participants = "SSSParticipants";
const std::string KeyronaSSS_ParticipantValid = "true";

const std::string KeyronaSSS_ParticipantsXKey = "SSSParticipantXKey";
const std::string KeyronaSSS_ParticipantsYKey = "SSSParticipantYKey";


namespace keyrona
{
    class KeyronaSSS
    {
        public:
	    /*!
             *  @brief create new secret sharing scheme
	     *  @param &SSSStorage, KeyronaStorage, contains address of the pointer to the current SSSStorage
	     *	@param SSSID, string, contains the ID of the secret sharing scheme
	     *	@param n, UInt32, contains the number of participants
	     *  @param Participants, vector of KeyronaSubject, contains the participants of the secret sharing scheme
	     *	@param VolumeKey, string, contains the decrypted volume key, that is the shared secret of the sss
	     */	   
            KeyronaSSS (KeyronaStorage &SSSStorage, string SSSID, UInt32 n, vector<KeyronaSubject*> Participants, string VolumeKey);

            /*!
             *  @brief create new secret sharing scheme including one group
	     *  @param &SSSStorage, KeyronaStorage, contains address of the pointer to the current SSSStorage
	     *	@param SSSID, string, contains the ID of the secret sharing scheme
	     *	@param n, UInt32, contains the number of participants
	     *  @param Participants, vector of *KeyronaSubject, contains the participants of the secret sharing scheme
	     *  @param *myGroup, KeyronaGroup, contains the group that becomes part of the sss
	     *	@param VolumeKey, string, contains the decrypted volume key, that is the shared secret of the sss
	     */
	    KeyronaSSS (KeyronaStorage &SSSStorage, string SSSID, UInt32 n, vector<KeyronaSubject*> Participants, KeyronaGroup* myGroup, string VolumeKey);

            /*!
             *  @brief load existing secret sharing scheme
	     *  @param &SSSStorage, KeyronaStorage, contains address of the pointer to the current SSSStorage
	     *	@param SSSID, string, contains the ID of the secret sharing scheme to be loaded
	     */
	    KeyronaSSS (KeyronaStorage &SSSStorage, string SSSID);

            /*!
             *  @brief destructor
	     */
	    ~KeyronaSSS();

            ////////////////////////////////////
	    /*!
             *  @brief delete this secret sharing scheme
	     */
            void deleteSSS();

	    /*!
             *  @brief prints the information about the current secret sharing scheme to screen
	     */
            void printSSS();

	    /*!
             *  @brief returns the subjects, that share a secret
	     *  @return vector of string, containing the subjects
	     */
            vector<string> getSubjectsInSSS() { return mySubjectsInSSS; };

            /*!
             *  @brief returns the minimum number of participants for the current secret sharing scheme
	     *  @return UInt32, contains the number of minimum participants
	     */
	    UInt32 getMinimumParticipants() { return minimumParticipants; };

	    /*!
             *  @brief retrieves the key of a secret sharing scheme by n subjects
	     *	@param AvailablePArticipants, vector of *KeyronaSubject, contains the list of subjects
	     *	@param subjectPasswords, vector of string, contains the passwords of the subjects
	     *  @return vector of string, containing the subjects
	     */
            string retrieveKey(vector<KeyronaSubject*> AvailableParticipants, vector<string> subjectPasswords);

            /*!
             *  @brief retrieve the key by n subjects and a group
	     *	@param AvailablePArticipants, vector of *KeyronaSubject, contains the list of subjects
	     *	@param subjectPasswords, vector of string, contains the passwords of the subjects
	     *	@param *myGroup, KeyronaGroup, contains the group
	     *	@param groupPassword, string, contains the passwords of the group
	     *  @return vector of string, containing the subjects
	     */
	    string retrieveKey(vector<KeyronaSubject*> AvailableParticipants, vector<string> subjectPasswords, KeyronaGroup *myGroup, string groupPassword);

    private:
            /*!
             *  @brief saves a sss to the database
	     */
	    void storeSSS();

	    /*!
             *  @brief stores the key of a participant to the database
	     *	@param &Participant, string, contains the name of the participant
	     * 	@param &X, string, contains part one of the participants key 
	     * 	@param &Y, string, contains part two of the participants key
	     */
            void storeParticipantKey(string &Participant, string &X, string &Y);

            /*!
             *  @brief loads the key of a participant from the database
	     *	@param &Participant, string, contains the name of the participant
	     * 	@return StringPair, contains the key of the participant
	     */
	    StringPair loadParticipantKey(string &Participant);

            /*!
             *  @brief loads a secret sharing scheme from the database
	     */
	    void loadSSS();

            /*!
             *  @brief  disabled copy constructor
             */
            KeyronaSSS( const KeyronaSSS& );

            /*!
             *  @brief  disabled assignment operator
             */
            KeyronaSSS& operator=( const KeyronaSSS& );

            string mySSSID;
            vector<string> mySubjectsInSSS;
            KeyronaStorage &mySSSStorage;
            UInt32 minimumParticipants;

    };
};

#endif	/* _KEYRONASSS_HXX */

