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
 * @file        Keyrona.hxx
 * @brief       Some Keyrona definitions...
 * @version     $Revision: 764 $
 * @date        Last modification at $Date: 2009-07-29 15:33:10 +0200 (Wed, 29 Jul 2009) $ by $Author: selhorst $
 */

#ifndef _KEYRONA_HXX
#define _KEYRONA_HXX
#include <map>
#include <string>
#include <iostream>
#include <Debugging.hxx>
#include <KeyronaExceptions.hxx>
#include <Types.hxx>
#include <sstream>
#include <fstream>
#include <binstream.hxx>
#include <sbinstream.hxx>
#include <Streamable.hxx>
#include <iomanip>
#include <stdlib.h>
#include <string_cast.hxx>
#include <VERSION.hxx>
#include <BoolOption.hxx>
#include <CmdLineParser.hxx>
#include <ShellCommand.hxx>
#include <vector>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <termios.h>
#include <BaseOption.hxx>
#include <ByteVector.hxx>
#include <binstream.hxx>
#include <base64.h>
#include <KeyronaCryptlib.hxx>
#include <KeyronaHelper.hxx>


#define MIN_NUMBER_OF_ADMINS    1
#define CLIENT_TIMEOUT          15
#define MIN_YEAR                2000
#define MAX_YEAR                3000
#define BUFFERSIZE              1024
#define SOCKETPERMISSION        S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
#define KEYRONA_MINIMUM_PASSWORD_LENGTH 64
#define KEYRONA_MAX_PASSWORD_TRIES       3
#define KEYRONA_ALWAYS_REQUIRE_LOGIN     0


const UInt32 KEYFILEPERMISSION = 660;
const UInt32 DBFILEPERMISSION = 660;

const std::string KEYRONA_ISC_QUIT_MESSAGE = "QUIT";
const std::string KEYRONA_ISC_PASS_MESSAGE = "PASS";
const std::string KEYRONA_ISC_MESG_MESSAGE = "MESG";
const std::string KEYRONA_ISC_GET_STRING_MESSAGE = "STRG";

const std::string KEYRONA_ADMIN_GROUP       = "Admin";
const std::string KEYRONA_LINUX_SYSTEM_GROUP = "keyrona";
const std::string KEYRONA_PRIVATE_MODE      = "Keyrona Private Mode User";
const std::string KEYRONA_PRIVATE_MODE_C            = "DE";
const std::string KEYRONA_PRIVATE_MODE_USER_MAIL    = "keyrona.user@keyrona.org";
const std::string KEYRONA_PRIVATE_MODE_ADMIN_MAIL   = "keyrona.admin@keyrona.org";
const std::string KEYRONA_TOKEN_STANDARD_PASSWD     = "KeyronaTokenStandardPassword";

extern std::string myKeyDirectory;
extern std::string myScriptDirectory;
extern std::string mySocketPath;
extern std::string myClientSocket;
extern std::string myServerSocket;
extern std::string mySocketPass;
extern std::string UUIDpath;
extern std::string logfile;
extern std::string RNGDevice;
extern bool         EnterpriseMode;
extern bool         printVerbose;

static const std::string KeyronaPathSeparator       = "/";
static const UInt8       KeyronaPathSeparatorChar   = '/';
static const std::string KeyronaFileSeparator       = "_";
static const UInt8       KeyronaFileSeparatorChar   = '_';
static const std::string KeyronaP15FileExtension    = ".p15";
static const std::string KeyronaDefaultConfigfile   = "/etc/keyrona/keyrona.cfg";
static const std::string moveCommandOption          = "-b"; // perform backup

#define bzero(ptr, len)  memset((ptr), NULL, (len))
#endif
