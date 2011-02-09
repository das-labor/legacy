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
 * @file        TpmCryptStorage.cxx
 * @brief       Implementation of the TpmCrypt storage service.
 * @version     $Revision: 549 $
 * @date        Last modification at $Date: 2009-05-07 10:27:30 +0200 (Thu, 07 May 2009) $ by $Author: selhorst $
 */

#include <TpmCryptStorage.hxx>

using namespace std;
using namespace utils;
using namespace tpmcrypt;

//================================================================================
//
TpmCryptStorage::TpmCryptStorage():
{
	openDB();
};

//================================================================================
//
TpmCryptStorage::~TpmCryptStorage()
{
    if (openDB())
		sqlite3_close(handle);
};

bool TpmCryptStorage::openDB()
{
	if( handle == NULL && sqlite3_open(TpmCryptDefaultConfigfile, &handle) != SQLITE_OK )
	{
		return false;
	}
	
	//Create table if not exists
	sqlite3_exec(handle,users,0,0,0);
	sqlite3_exec(handle,keys,0,0,0); 
	sqlite3_exec(handle,user_keys,0,0,0); 
	sqlite3_exec(handle,sss,0,0,0); 
	sqlite3_exec(handle,sss_entries,0,0,0); 
	sqlite3_exec(handle,volumes,0,0,0); 
	sqlite3_exec(handle,platforms,0,0,0); 
	sqlite3_exec(handle,esd,0,0,0); 
	
	return true;
}

int TpmCryptStorage::protectDB(string command)
{
	vector<string> dangerous;
	vector<string>::const_iterator it;
	
	dangerous.push_back("WHERE");
	dangerous.push_back("SELECT");
	dangerous.push_back("IS");
	dangerous.push_back("INSERT");
	dangerous.push_back("UPDATE");
	dangerous.push_back("SET");
	dangerous.push_back("FROM");
	dangerous.push_back("VALUES");
	dangerous.push_back("LIKE");
	
	it = dangerous.begin();
	while(it != dangerous.end())
	{
		if((*it).compare(command))
			return 1;
		it++;
	}
	return 0;
}

vector<string> TpmCryptStorage::queryDB(string &table, string &select, string &column, string &data)
{
	int cols = 0;
	vector<string> blob;
	string queries = "SELECT " + select + " FROM " + table + " WHERE " + column + " IS " + data;
		
	if(protectDB(data) != 0)
		throw InvalidData("SQL Injection");
		
	sqlite3_prepare_v2(handle,queries,100, &stmt,0 );
	
	sqlite3_step(stmt);
	
	cols = sqlite3_column_count(stmt);
	
	for( int i = 0; i < cols; i++)
	{
		blob.push_back(sqlite3_column_blob(stmt,i));
	}
	
	sqlite3_finalize(stmt);
	
	return blob;
}

void TpmCryptStorage::storeDB(string &table, string &select, string &column, string &new_data, string &old_data)
{
	string queries = "UPDATE " + table + " SET " + select + " IS " + new_data + " WHERE " + column + " IS " + old_data;
	
	if(protectDB(new_data) != 0)
		throw InvalidData("SQL Injection");
	
	if(!sqlite3_exec(handle,queries,0,0,0))
	{
		string val = sqlite3_errmsg(handle);
		throw InvalidDB("TpmCryptStorage: SQLite says " + val);
	}
}

void TpmCryptStorage::deleteDB(string &table, string &column, string &identifier)
{
	string queries = "DELETE FROM " + table + " WHERE " + column + " IS " + identifier;
	
	if(!sqlite3_exec(handle,queries,0,0,0))
	{
		string val = sqlite3_errmsg(handle);
		throw InvalidDB("TpmCryptStorage: SQLite says " + val);
	}
}

void TpmCryptStorage::deleteAllEntryDB(string &column, string &identifier)
{
	string queries = "DELETE FROM * WHERE " + column + " IS " + identifier;
	
	if(!sqlite3_exec(handle,queries,0,0,0))
	{
		string val = sqlite3_errmsg(handle);
		throw InvalidDB("TpmCryptStorage: SQLite says " + val);
	}
}

vector<string> TpmCryptStorage::queryAllEntryDB(string &table, string &select)
{
	int cols;
	vector<string> blob;
	string queries = "SELECT " + select + " FROM " + table;
	
	sqlite3_prepare_v2(handle,queries,100, &stmt,0);
	
	sqlite3_step(stmt);
	
	cols = sqlite3_column_count(stmt);
	
	for( int i = 0; i < cols; i++)
	{
		blob.push_back(sqlite3_column_blob(stmt,i));
	}
	
	sqlite3_finalize(stmt);
	
	return blob;
}

void TpmCryptStorage::usersDB(long uuid, string &name, string &email, string &country, string &company, bool isAdmin)
{
	string queries = "INSERT INTO users VALUES(" + uuid + ",'" + name + "','" + email + "','" + country + "','" + company + "'," + isAdmin + ")";
	
	if(protectDB(name) != 0)
		throw InvalidData("SQL Injection");
		
	if(protectDB(email) != 0)
		throw InvalidData("SQL Injection");
		
	if(protectDB(country) != 0)
		throw InvalidData("SQL Injection");
		
	if(protectDB(company) != 0)
		throw InvalidData("SQL Injection");
	
	if(!sqlite3_exec(handle,queries,0,0,0))
	{
		string val = sqlite3_errmsg(handle);
		throw InvalidDB("TpmCryptStorage: SQLite says " + val);
	}
}

void TpmCryptStorage::keysDB(long uuid, string &type, int size, string &valid)
{
	string queries = "INSERT INTO keys VALUES(" + uuid + ",'" + type + "'," + size + ",'" + valid + "')";
	
	if(!sqlite3_exec(handle,queries,0,0,0))
	{
		string val = sqlite3_errmsg(handle);
		throw InvalidDB("TpmCryptStorage: SQLite says " + val);
	}
}

void TpmCryptStorage::user_keysDB(long id, long u_uuid, long k_uuid)
{
	string queries = "INSERT INTO user_keys VALUES(" + id + "," + u_uuid + "," + k_uuid + ")";
	
	if(!sqlite3_exec(handle,queries,0,0,0))
	{
		string val = sqlite3_errmsg(handle);
		throw InvalidDB("TpmCryptStorage: SQLite says " + val);
	}
}

void TpmCryptStorage::volumesDB(long uuid, string &name, string &path, string &blob, string &enc)
{
	string queries = "INSERT INTO volumes VALUES(" + uuid + ",'" + name + "','" + path + "','" + blob + "','" + enc + "')";
	
	if(protectDB(name) != 0)
		throw InvalidData("SQL Injection");
	
	if(!sqlite3_exec(handle,queries,0,0,0))
	{
		string val = sqlite3_errmsg(handle);
		throw InvalidDB("TpmCryptStorage: SQLite says " + val);
	}
}

void TpmCryptStorage::sssDB(long uuid, string &name, int participants, int x)
{
	string queries = "INSERT INTO sss VALUES(" + uuid + ",'" + name + "'," + participants + "," + x + ")";
	
	if(!sqlite3_exec(handle,queries,0,0,0))
	{
		string val = sqlite3_errmsg(handle);
		throw InvalidDB("TpmCryptStorage: SQLite says " + val);
	}
}

void TpmCryptStorage::sssEntriesDB(long uuid, long sss_uuid, long u_uuid, string &x, string &y)
{
	string queries = "INSERT INTO sss_entries VALUES(" + uuid + "," + sss_uuid + "," + u_uuid + ",'" + x + "','" + y + "')";
	
	if(!sqlite3_exec(handle,queries,0,0,0))
	{
		string val = sqlite3_errmsg(handle);
		throw InvalidDB("TpmCryptStorage: SQLite says " + val);
	}
}

void TpmCryptStorage::volumes_usersDB(long v_uuid, long u_uuid)
{
	string queries = "INSERT INTO volumes_users VALUES(" + v_uuid + "," + u_uuid + ")";
	
	if(!sqlite3_exec(handle,queries,0,0,0))
	{
		string val = sqlite3_errmsg(handle);
		throw InvalidDB("TpmCryptStorage: SQLite says " + val);
	}
}

void TpmCryptStorage::volumes_sssDB(long v_uuid, long sss_uuid)
{
	string queries = "INSERT INTO volumes_sss VALUES(" + v_uuid + "," + sss_uuid + ")";
	
	if(!sqlite3_exec(handle,queries,0,0,0))
	{
		string val = sqlite3_errmsg(handle);
		throw InvalidDB("TpmCryptStorage: SQLite says " + val);
	}
}
