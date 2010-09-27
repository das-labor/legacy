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
 * @file        KeyronaConfigfile.hxx
 * @brief       Implementation of the Keyrona configuration file parser
 * @version     $Revision: 800 $
 * @date        Last modification at $Date: 2009-08-11 18:44:01 +0200 (Tue, 11 Aug 2009) $ by $Author: selhorst $
 */

#ifndef _KEYRONACONFIGFILE_HXX
#define _KEYRONACONFIGFILE_HXX

#include <Keyrona.hxx>

// Configfile entries
static const std::string KeyronaConfigfile_CommentaryChar               = "#";
static const std::string KeyronaConfigfile_VolumeDBIdentifier           = "VOLUME_DB";
static const std::string KeyronaConfigfile_SubjectDBIdentifier          = "SUBJECT_DB";
static const std::string KeyronaConfigfile_GroupDBIdentifier            = "GROUP_DB";
static const std::string KeyronaConfigfile_SSSDBIdentifier              = "SSS_DB";
static const std::string KeyronaConfigfile_ESDDBIdentifier              = "ESD_DB";
static const std::string KeyronaConfigfile_KeyproviderDBIdentifier      = "KEYPROVIDER_DB";
static const std::string KeyronaConfigfile_KeyPathIdentifier            = "KEYRONA_KEY_PATH";
static const std::string KeyronaConfigfile_KeyronaScriptPathIdentifier  = "KEYRONA_SCRIPT_PATH";
static const std::string KeyronaConfigfile_KeyronaSocketPathIdentifier  = "KEYRONA_SOCKET_PATH";
static const std::string KeyronaConfigfile_KeyronaServerSocketIdentifier= "KEYRONA_SERVER_SOCKET";
static const std::string KeyronaConfigfile_KeyronaClientSocketIdentifier= "KEYRONA_CLIENT_SOCKET";
static const std::string KeyronaConfigfile_KeyronaSocketPass            = "KEYRONA_SOCKET_PASS";
static const std::string KeyronaConfigfile_KeyronaUUIDPath              = "KEYRONA_UUID_PATH";
static const std::string KeyronaConfigfile_KeyronaEnterpriseMode        = "KEYRONA_MODE";
static const std::string KeyronaConfigfile_KeyronaEnterpriseMode_TRUE   = "ENTERPRISE";
static const std::string KeyronaConfigfile_KeyronaLogfile               = "KEYRONA_LOGFILE";
static const std::string KeyronaConfigfile_KeyronaRNGDevice             = "KEYRONA_RNG_PATH";

using namespace std;
using namespace utils;

namespace keyrona
{

    class KeyronaConfigfile
    {
        public:
            /*!
             *  @brief  constructor for KeyronaConfigfile object
             */
            KeyronaConfigfile( string Configfilename );

            /*!
             *  @brief  destructor for KeyronaConfigfile object
             */
            ~KeyronaConfigfile();

            /*!
             *  @brief  returns an entry of the configuration file
             */
            string getConfigfileEntry( string ConfigfileEntryString );

            /*!
             *  @brief  disabled copy constructor
             */
            KeyronaConfigfile( const KeyronaConfigfile& );

            /*!
             *  @brief  disabled assignment operator
             */
            KeyronaConfigfile& operator=( const KeyronaConfigfile& );

        private:
            ifstream myConfigfile;
            string   myConfigfileFilename;
    };
}

#endif
