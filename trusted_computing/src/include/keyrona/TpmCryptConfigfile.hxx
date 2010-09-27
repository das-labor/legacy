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
 * @file        TpmCryptConfigfile.hxx
 * @brief       Implementation of the TpmCrypt configuration file parser
 * @version     $Revision: 800 $
 * @date        Last modification at $Date: 2009-08-11 18:44:01 +0200 (Tue, 11 Aug 2009) $ by $Author: selhorst $
 */

#ifndef _TPMCRYPTCONFIGFILE_HXX
#define _TPMCRYPTCONFIGFILE_HXX

#include <TpmCrypt.hxx>

// Configfile entries
static const std::string TpmCryptConfigfile_CommentaryChar               = "#";
static const std::string TpmCryptConfigfile_VolumeDBIdentifier           = "VOLUME_DB";
static const std::string TpmCryptConfigfile_SubjectDBIdentifier          = "SUBJECT_DB";
static const std::string TpmCryptConfigfile_GroupDBIdentifier            = "GROUP_DB";
static const std::string TpmCryptConfigfile_SSSDBIdentifier              = "SSS_DB";
static const std::string TpmCryptConfigfile_ESDDBIdentifier              = "ESD_DB";
static const std::string TpmCryptConfigfile_KeyproviderDBIdentifier      = "KEYPROVIDER_DB";
static const std::string TpmCryptConfigfile_KeyPathIdentifier            = "TPMCRYPT_KEY_PATH";
static const std::string TpmCryptConfigfile_TpmCryptScriptPathIdentifier  = "TPMCRYPT_SCRIPT_PATH";
static const std::string TpmCryptConfigfile_TpmCryptSocketPathIdentifier  = "TPMCRYPT_SOCKET_PATH";
static const std::string TpmCryptConfigfile_TpmCryptServerSocketIdentifier= "TPMCRYPT_SERVER_SOCKET";
static const std::string TpmCryptConfigfile_TpmCryptClientSocketIdentifier= "TPMCRYPT_CLIENT_SOCKET";
static const std::string TpmCryptConfigfile_TpmCryptSocketPass            = "TPMCRYPT_SOCKET_PASS";
static const std::string TpmCryptConfigfile_TpmCryptUUIDPath              = "TPMCRYPT_UUID_PATH";
static const std::string TpmCryptConfigfile_TpmCryptEnterpriseMode        = "TPMCRYPT_MODE";
static const std::string TpmCryptConfigfile_TpmCryptEnterpriseMode_TRUE   = "ENTERPRISE";
static const std::string TpmCryptConfigfile_TpmCryptLogfile               = "TPMCRYPT_LOGFILE";
static const std::string TpmCryptConfigfile_TpmCryptRNGDevice             = "TPMCRYPT_RNG_PATH";

using namespace std;
using namespace utils;

namespace keyrona
{

    class TpmCryptConfigfile
    {
        public:
            /*!
             *  @brief  constructor for TpmCryptConfigfile object
             */
            TpmCryptConfigfile( string Configfilename );

            /*!
             *  @brief  destructor for TpmCryptConfigfile object
             */
            ~TpmCryptConfigfile();

            /*!
             *  @brief  returns an entry of the configuration file
             */
            string getConfigfileEntry( string ConfigfileEntryString );

            /*!
             *  @brief  disabled copy constructor
             */
            TpmCryptConfigfile( const TpmCryptConfigfile& );

            /*!
             *  @brief  disabled assignment operator
             */
            TpmCryptConfigfile& operator=( const TpmCryptConfigfile& );

        private:
            ifstream myConfigfile;
            string   myConfigfileFilename;
    };
}

#endif
