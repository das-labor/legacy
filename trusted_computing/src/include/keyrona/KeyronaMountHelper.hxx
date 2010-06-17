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
 * @file        KeyronaMountHelper.cxx
 * @brief       MountHelper functions.
 * @version     $Revision: 800 $
 * @date        Last modification at $Date: 2009-08-11 18:44:01 +0200 (Tue, 11 Aug 2009) $ by $Author: selhorst $
 */

#ifndef _MOUNTHELPER_HXX
#define	_MOUNTHELPER_HXX

#include <Keyrona.hxx>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <signal.h>
#include <netdb.h>
#include <KeyronaExceptions.hxx>
#include <dirent.h>
#include <limits.h>
#include<KeyronaLogin.hxx>

using namespace std;
using namespace utils;

extern termios terminalBackup;

namespace keyrona
{
    class MountHelper
    {
    public:
	/*!
	 *  @brief Starts the "Keyrona Mounthelper" client of the "Keyrona Keyprovider"
	 *  @param mount, boolean parameter, to decide, if to mount or unmount
	 *  @param users, string, that contains the users, separated by comma
	 *  @param volume, string, that contains the volume to operate with
	 *  @param path, string, that contains the path a volume is mounted to
	 */    
	MountHelper ( bool mount, string users, string volume, string path );
        
	/*!
	 *  @brief ends the Mounthelper and closes the sockets.
	 *  @param sig, integer with the signal that ends the process
	 */    
	static void quitDaemon(int sig);

    private:
    
	/*!
	 *  @brief Sends a message to the "Keyrona Keyprovider"
	 *  @param tosend, string, to be send to the server
	 */    
        void sendMessage(string tosend);
    };
}

#endif	/* _MOUNTHELPER_HXX */

