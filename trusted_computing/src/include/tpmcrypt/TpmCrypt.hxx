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
 * @file        TpmCrypt.hxx
 * @brief       Some TpmCrypt definitions...
 * @version     $Revision: 764 $
 * @date        Last modification at $Date: 2009-07-29 15:33:10 +0200 (Wed, 29 Jul 2009) $ by $Author: selhorst $
 */

#ifndef _TPMCRYPT_HXX
#define _TPMCRYPT_HXX
#include <map>
#include <string>
#include <iostream>
#include <Debugging.hxx>
#include <TpmCryptExceptions.hxx>
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
#include <TpmCryptHelper.hxx>


#define MIN_NUMBER_OF_ADMINS    1
#define CLIENT_TIMEOUT          15
#define MIN_YEAR                2000
#define MAX_YEAR                3000
#define BUFFERSIZE              1024
#define SOCKETPERMISSION        S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
#define TPMCRYPT_MINIMUM_PASSWORD_LENGTH 64
#define TPMCRYPT_MAX_PASSWORD_TRIES       3
#define TPMCRYPT_ALWAYS_REQUIRE_LOGIN     0

const int ETIMEDOUT = 30;

const UInt32 KEYFILEPERMISSION = 660;
const UInt32 DBFILEPERMISSION = 660;

const std::string TPMCRYPT_ISC_QUIT_MESSAGE = "QUIT";
const std::string TPMCRYPT_ISC_PASS_MESSAGE = "PASS";
const std::string TPMCRYPT_ISC_MESG_MESSAGE = "MESG";
const std::string TPMCRYPT_ISC_GET_STRING_MESSAGE = "STRG";

const std::string TPMCRYPT_ADMIN_GROUP       = "Admin";
const std::string TPMCRYPT_LINUX_SYSTEM_GROUP = "tpmcrypt";
const std::string TPMCRYPT_PRIVATE_MODE      = "TpmCrypt Private Mode User";
const std::string TPMCRYPT_PRIVATE_MODE_C            = "DE";
const std::string TPMCRYPT_PRIVATE_MODE_USER_MAIL    = "tpmcrypt.user@tpmcrypt.org";
const std::string TPMCRYPT_PRIVATE_MODE_ADMIN_MAIL   = "tpmcrypt.admin@tpmcrypt.org";
const std::string TPMCRYPT_TOKEN_STANDARD_PASSWD     = "TpmCryptTokenStandardPassword";

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

static const std::string TpmCryptPathSeparator       = "/";
static const UInt8       TpmCryptPathSeparatorChar   = '/';
static const std::string TpmCryptFileSeparator       = "_";
static const UInt8       TpmCryptFileSeparatorChar   = '_';
static const std::string TpmCryptP15FileExtension    = ".p15";
static const std::string TpmCryptDefaultConfigfile   = "/etc/tpmcrypt/tpmcrypt.db";
static const std::string moveCommandOption          = "-b"; // perform backup

#define bzero(ptr, len)  memset((ptr), NULL, (len))
#endif
