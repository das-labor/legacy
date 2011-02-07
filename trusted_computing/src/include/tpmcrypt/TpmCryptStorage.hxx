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
 * @file        TpmCryptStorage.hxx
 * @brief       Implementation of the TpmCrypt storage service.
 * @version     $Revision: 790 $
 * @date        Last modification at $Date: 2009-08-04 15:07:58 +0200 (Tue, 04 Aug 2009) $ by $Author: goericke $
 */

#ifndef _TPMCRYPTSTORAGE_HXX
#define _TPMCRYPTSTORAGE_HXX

#include <TpmCrypt.hxx>
#include <sqlite3.h>

sqlite3 *handle = NULL;
sqlite3_stmt *stmt;

// Database entries
const std::string keys = "CREATE TABLE IF NOT EXISTS keys(uuid INTEGER PRIMARY KEY ASC, key_type TEXT,key_size INTEGER, valid_until DATETIME)";
const std::string users = "CREATE TABLE IF NOT EXISTS users(uuid INTEGER PRIMARY KEY ASC, name TEXT UNIQUE NOT NULL, email TEXT, country TEXT, organization TEXT, is_admin BOOL)";
const std::string user_keys = "CREATE TABLE IF NOT EXISTS user_keys(id INTEGER PRIMARY KEY ASC, user_uuid INTEGER NOT NULL, key_uuid INTEGER NOT NULL)";
const std::string volumes = "CREATE TABLE IF NOT EXISTS volumes(uuid INTEGER PRIMARY KEY ASC, name TEXT UNIQUE, path TEXT, key_blob BLOB, enc_tool TEXT)";
const std::string sss = "CREATE TABLE IF NOT EXISTS sss(uuid INTEGER PRIMARY KEY ASC, name TEXT UNIQUE, min_participants INTEGER NOT NULL, last_x_val INTEGER NOT NULL)";
const std::string sss_entries = "CREATE TABLE IF NOT EXISTS sss_entries(uuid INTEGER PRIMARY KEY, sss_uuid INTEGER NOT NULL, user_uuid INTEGER NOT NULL, x_val TEXT NOT NULL, y_val TEXT NOT NULL)";
const std::string volumes_users = "CREATE TABLE IF NOT EXISTS volumes_users(uuid INTEGER PRIMARY KEY, u_uuid INTEGER NOT NULL)";
const std::string volumes_sss = "CREATE TABLE IF NOT EXISTS volumes_sss(uuid INTEGER PRIMARY KEY, sss_uuid INTEGER NOT NULL)";
const std::string esd = "CREATE TABLE IF NOT EXISTS esd(uuid INTEGER PRIMARY KEY, sss_uuid INTEGER NOT NULL)";
const std::string platform = "CREATE TABLE IF NOT EXISTS esd(uuid INTEGER PRIMARY KEY, sss_uuid INTEGER NOT NULL)";
const std::string token = "CREATE TABLE IF NOT EXISTS esd(uuid INTEGER PRIMARY KEY, sss_uuid INTEGER NOT NULL)";

using namespace std;
using namespace utils;

namespace tpmcrypt
{
    class TpmCryptStorage
    {
        public:
	    /*!
             *  @brief constructor for a TpmCryptStorage object
	     *  @param &StorageName, const string, contains the name of the storage
	     * 	@param &StorageFile, const string, path to the database
	     *	@param clear, bool, determines whether or not the database should be reset
	     */
            TpmCryptStorage();

            ~TpmCryptStorage();


        bool openDB();
        
        vector<string> queryDB(string table, string select, string column, string data);
        
        void storeDB(string table, string select, string column, string new_data, string old_data);
        
        vector<string> queryAllEntryDB(string table, string select);
	    
	    void usersDB(long uuid, string name, string email, string country, string company, bool isAdmin);
	    
	    void keysDB(long uuid, string type, int size, string valid);
	    
	    void user_keysDB(long id, long u_uuid, long k_uuid);
	    
	    void volumesDB(long uuid, string name, string path, string blob, string enc);
	    
	    void sssDB(long uuid, string name, int participants, int x);
	    
	    void sssEntriesDB(long uuid, long sss_uuid, long u_uuid, string x, string y);
	    
	    void volumes_usersDB(long v_uuid, long u_uuid);
	    
	    void volumes_sssDB(long v_uuid, long sss_uuid);
	    
	    private:
	    
	    int protectDB(string command);

	    
	    TpmCryptStorage &myDB;

        /*!
         *  @brief  disabled copy constructor
         */
        TpmCryptStorage( const TpmCryptStorage& );

        /*!
         *  @brief  disabled assignment operator
         */
        TpmCryptStorage& operator=( const TpmCryptStorage& );

        };
};

#endif
