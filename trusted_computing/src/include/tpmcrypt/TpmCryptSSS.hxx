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
 * @file        TpmCryptSSS.hxx
 * @brief       TpmCrypt Secret Sharing Scheme class
 * @version     $Revision: 800 $
 * @date        Last modification at $Date: 2009-08-11 18:44:01 +0200 (Tue, 11 Aug 2009) $ by $Author: selhorst $
 */

#ifndef _TPMCRYPTSSS_HXX
#define	_TPMCRYPTSSS_HXX

#include <TpmCrypt.hxx>
#include <TpmCryptSubject.hxx>
#include <TpmCryptStorage.hxx>
#include <TpmCryptGroup.hxx>
#include <Integer.hxx>
#include <SSS.hxx>
#include <ConversionPrimitives.hxx>


const std::string TpmCryptSSS_Properties = "SSSProperties";

const std::string TpmCryptSSS_SSS_MinimumParticipants = "RequiredParticipants";

const std::string TpmCryptSSS_Participants = "SSSParticipants";
const std::string TpmCryptSSS_ParticipantValid = "true";

const std::string TpmCryptSSS_ParticipantsXKey = "SSSParticipantXKey";
const std::string TpmCryptSSS_ParticipantsYKey = "SSSParticipantYKey";


namespace tpmcrypt
{
    class TpmCryptSSS
    {
        public:
	    /*!
             *  @brief create new secret sharing scheme
	     *  @param &SSSStorage, TpmCryptStorage, contains address of the pointer to the current SSSStorage
	     *	@param SSSID, string, contains the ID of the secret sharing scheme
	     *	@param n, UInt32, contains the number of participants
	     *  @param Participants, vector of TpmCryptSubject, contains the participants of the secret sharing scheme
	     *	@param VolumeKey, string, contains the decrypted volume key, that is the shared secret of the sss
	     */	   
            TpmCryptSSS (TpmCryptStorage &SSSStorage, string SSSID, UInt32 n, vector<TpmCryptUser*> Participants, string VolumeKey);

            /*!
             *  @brief load existing secret sharing scheme
	     *  @param &SSSStorage, TpmCryptStorage, contains address of the pointer to the current SSSStorage
	     *	@param SSSID, string, contains the ID of the secret sharing scheme to be loaded
	     */
	    TpmCryptSSS (TpmCryptStorage &SSSStorage, string SSSID);

            /*!
             *  @brief destructor
	     */
	    ~TpmCryptSSS();

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
	     *	@param AvailablePArticipants, vector of *TpmCryptSubject, contains the list of subjects
	     *	@param subjectPasswords, vector of string, contains the passwords of the subjects
	     *  @return vector of string, containing the subjects
	     */
            string retrieveKey(vector<TpmCryptUser*> AvailableParticipants, vector<string> userPasswords);

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
            TpmCryptSSS( const TpmCryptSSS& );

            /*!
             *  @brief  disabled assignment operator
             */
            TpmCryptSSS& operator=( const TpmCryptSSS& );

            string mySSSID;
            vector<string> mySubjectsInSSS;
            TpmCryptStorage &mySSSStorage;
            UInt32 minimumParticipants;

    };
};

#endif	/* _TPMCRYPTSSS_HXX */

