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
 * @file        KeyronaConfigfile.cxx
 * @brief       Implementation of the Keyrona configuration file parser
 * @version     $Revision: 449 $
 * @date        Last modification at $Date: 2009-04-06 00:16:17 +0200 (Mon, 06 Apr 2009) $ by $Author: selhorst $
 */

#include <KeyronaConfigfile.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

namespace keyrona
{

//================================================================================
//
    KeyronaConfigfile::KeyronaConfigfile( string Configfilename ) :
    myConfigfile ( NULL ),
    myConfigfileFilename (Configfilename)
    {
        debug << "KeyronaConfigfile: Opening configuration file '" + myConfigfileFilename + "'." << endl;
        myConfigfile.open(myConfigfileFilename.c_str());

        if (myConfigfile.fail())
            throw NoFilename("KeyronaConfigfile: No valid configuration filename submitted!");

    };

//================================================================================
//
    KeyronaConfigfile::~KeyronaConfigfile()
    {
        debug << "KeyronaConfigfile: Closing configuration file '" + myConfigfileFilename + "'." << endl;
        if (myConfigfile.is_open())
            myConfigfile.close();
    };
    
    
//================================================================================
//
    string KeyronaConfigfile::getConfigfileEntry( string ConfigfileEntryString )
    {
        string line;
        myConfigfile.clear();
        myConfigfile.seekg(0, ios::beg);
        while (!myConfigfile.eof())
        {
            getline(myConfigfile, line);

            // remove empty lines
            if (line.empty())
                continue;

            // remove commentary lines
            if (!line.compare(0,1,KeyronaConfigfile_CommentaryChar))
                continue;

            // get filename
            if (!line.compare(0,ConfigfileEntryString.length(),ConfigfileEntryString))
            {
                return (extractQuotationMarks(line));
            }
        }
        throw EntryNotFound("KeyronaConfigfile: No valid entry found: '" + ConfigfileEntryString + "'");
    };
};
