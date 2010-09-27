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
 * @file        TpmCryptKeyprovider.hxx
 * @brief       Implementation of the TpmCrypt Keyprovider Service.
 * @version     $Revision: 800 $
 * @date        Last modification at $Date: 2009-08-11 18:44:01 +0200 (Tue, 11 Aug 2009) $ by $Author: selhorst $
 */

#ifndef _TPMCRYPTKEYPROVIDER_HXX
#define	_TPMCRYPTKEYPROVIDER_HXX

#include <TpmCrypt.hxx>
#include <TpmCryptHelper.hxx>
#include <TpmCryptStorage.hxx>
#include <TpmCryptConfigfile.hxx>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <signal.h>
#include <netdb.h>
#include <TpmCryptKeyproviderMount.hxx>
#include <TpmCryptVolume.hxx>
#include <TpmCryptSubject.hxx>

using namespace std;
using namespace keyrona;
using namespace utils;

namespace keyrona
{
    class TpmCryptKeyprovider
    {
    public:
	/*!
	 *  @brief constructor for the TpmCrypt keyprovider
	 *  @param foreground, bool, determines whether or not the daemon should fork into the background after being started
	 *  @param &myTpmCryptConfig, TpmCryptConfigfile, points to the current TpmCrypt configuration
	 */
        TpmCryptKeyprovider(bool foreground, TpmCryptConfigfile &myTpmCryptConfig);
        
    private:
        
	/*!
	 *  @brief starts the TpmCrypt keyprovider
	 *  @param &myTpmCryptConfig, TpmCryptConfigfile, points to the current TpmCrypt configuration
	 */
        void startKeyprovider(TpmCryptConfigfile &myTpmCryptConfig);

	/*!
	 *  @brief separates incoming messages from clients into tokens
	 *  @param str, const string&, the string to be separated
	 *  @param tokens, vector of string&, contains the tokens
	 */
        void Tokenize(const string& str, vector<string>& tokens);

	/*!
	 *  @brief mounts an encrypted device
	 *  @param parameter, vector of string, contains the required parameter for the mount operation
	 *  @param &myTpmCryptConfig, TpmCryptConfigfile, points to the current TpmCrypt configuration
	 */
        void mount(vector<string> parameter, TpmCryptConfigfile &myTpmCryptConfig);

	/*!
	 *  @brief unmounts an encrypted device
	 *  @param parameter, vector of string, contains the required parameter for the unmount operation
	 *  @param &myTpmCryptConfig, TpmCryptConfigfile, points to the current TpmCrypt configuration
	 */        
	void unmount(vector<string> parameter, TpmCryptConfigfile &myTpmCryptConfig);

	/*!
	 *  @brief changes the permission of a file
	 */        
        void changepermission(void);

	/*!
	 *  @brief sends a message to the client and if necessary requests further operations
	 *  @param event, string, contains the requested operation (display string, request string, request password )
	 *  @param message, string, contains a message send to the client
	 *  @return string, contains any feedback from the client
	 */        
        string clientCommunication(string event, string message);

	/*!
	 *  @brief creates a name for the device from the parameters, send by the client
	 *  @param inputVector, vector of string, contains all parameters send by the client
	 *  @return string, contains the mount name
	 */        
        string createMountName(vector<string> inputVector);

	/*!
	 *  @brief quits the deamon and terminates existing socket connections
	 */        
        static void quitDaemon(int sig);

	/*!
	 *  @brief mounts a volume with a secret sharing scheme user identification
	 *  @param &myConfigfile, TpmCryptConfigfile, points to the current TpmCrypt configuration
	 *  @param &myVolume, TpmCryptVolume, points to the volume to be mounted
	 *  @param &mySSSStorage, TpmCryptStorage, points to the current secret sharing scheme database
	 *  @param &mySubjectStorage, TpmCryptStorage, points to the current subject database
	 *  @param &myGroupStorage, TpmCryptStorage, points to the current group database
	 *  @param &parameter, vector of string, contains all parameters send by the client
	 *  @param &path, string, contains the path for the mount operation
	 *  @param &volume, string, contains current location path of the volume
	 *  @param &mountName, string, contains mount name
	 */        
        void mountBySSS(TpmCryptConfigfile &myConfigfile, TpmCryptVolume &myVolume, TpmCryptStorage &mySSSStorage, TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage, vector<string> &parameter, string &path, string &volume, string &mountName );

	/*!
	 *  @brief mounts a volume with a user identification
	 *  @param &myConfigfile, TpmCryptConfigfile, points to the current TpmCrypt configuration
	 *  @param &myVolume, TpmCryptVolume, points to the volume to be mounted
	 *  @param &mySubjectStorage, TpmCryptStorage, points to the current subject database
	 *  @param &parameter, vector of string, contains all parameters send by the client
	 *  @param &path, string, contains the path for the mount operation
	 *  @param &volume, string, contains current location path of the volume
	 *  @param &mountName, string, contains mount name
	 */        
        void mountBySubject(TpmCryptConfigfile &myConfigfile, TpmCryptVolume &myVolume, TpmCryptStorage &mySubjectStorage, vector<string> &parameter, string &path, string &volume, string &mountName );

	/*!
	 *  @brief mounts a volume with a group identification
	 *  @param &myConfigfile, TpmCryptConfigfile, points to the current TpmCrypt configuration
	 *  @param &myVolume, TpmCryptVolume, points to the volume to be mounted
	 *  @param &mySubjectStorage, TpmCryptStorage, points to the current subject database
	 *  @param &myGroupStorage, TpmCryptStorage, points to the current group database
	 *  @param &parameter, vector of string, contains all parameters send by the client
	 *  @param &path, string, contains the path for the mount operation
	 *  @param &volume, string, contains current location path of the volume
	 *  @param &mountName, string, contains mount name
	 */        
        void mountByGroup(TpmCryptConfigfile &myConfigfile, TpmCryptVolume &myVolume, TpmCryptStorage &mySubjectStorage, TpmCryptStorage &myGroupStorage, vector<string> &parameter, string &path, string &volume, string &mountName );

	/*!
	 *  @brief sends system message to a user
	 *  @param, *mySubject, TpmCryptSubject, the subject class of the user, the message is send to
	 */   
        void sendMessagesForUser(TpmCryptSubject *mySubject);

	/*!
	 *  @brief gets the password from a subject
	 *  @param, *mySubject, TpmCryptSubject, the subject class of the user, the password is retrieved from
	 *  @return string, contains the password of the subject
	 */   
        string getSubjectPassword(TpmCryptSubject *mySubject);

	/*!
	 *  @brief gets the password from a group
	 *  @param, *myGroup, TpmCryptGroup, the subject class of the group, the password is retrieved from
	 *  @param &mySubjectStorage, TpmCryptStorage, points to the current subject database
	 *  @return string, contains the password of the group
	 */
        string getGroupPassword(TpmCryptGroup *myGroup, TpmCryptStorage &mySubjectStorage);
    };
}

#endif	/* _TPMCRYPTKEYPROVIDER_HXX */
