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
 * @file        TpmCryptSearch.cxx
 * @brief       Implementation of some search operations.
 * @version     $Revision: 656 $
 * @date        Last modification at $Date: 2009-06-12 17:33:42 +0200 (Fri, 12 Jun 2009) $ by $Author: selhorst $
 */

#include <TpmCryptSearch.hxx>

//================================================================================
//
bool TpmCryptFindVolume(TpmCryptStorage &myVolumeStorage, const string &VolumeUUID)
{
	vector<string> myVolumes = myVolumeStorage.queryAllEntryDB("volumes","v_uuid");
    vector<string>::const_iterator it;
    if (myVolumes.size())
    {
        it=myVolumes.begin();
        while ( it != myVolumes.end())
        {
            
            if (VolumeUUID == *(it))
                return true;
            it++;
        }
    }
    return false;
};

//================================================================================
//
bool TpmCryptFindESD(TpmCryptStorage &myESDStorage, const string &esdIdentifier)
{
    vector<string> myESD = myESDStorage.queryAllEntryDB("esd","name");
    vector<string>::const_iterator it;
    if (myESD.size())
    {
        it = myESD.begin();
        while ( it != myESD.end())
        {
			if( esdIdentifier == *(it) )
				return true;
            it++;
        }
    }
    return false;
};

//================================================================================
//
bool TpmCryptFindUser(TpmCryptStorage &myUserStorage, const string &userIdentifier)
{
	vector<string> myUser = myUserStorage.queryAllEntryDB("users","name");
    vector<string>::const_iterator it;
    if (myUser.size())
    {
		it = myUser.begin();
		while ( it != myUser.end())
		{
			if( userIdentifier == *(it) )
				return true;
			it++;
		}
	}
	return false;
};


//================================================================================
//
bool TpmCryptFindAdmin(TpmCryptStorage &myUserStorage, const string &userIdentifier)
{
	vector<string> myUser = myUserStorage.queryDB("users","isAdmin","name",userIdentifier);
    vector<string>::const_iterator it;
    if (myUser.size())
    {
		it = myUser.begin();
		while ( it != myUser.end())
		{
			if( *(it) = "1" )
				return true;
			it++;
		}
	}
		
	return false;
};

//================================================================================
//
bool TpmCryptFindSSS(TpmCryptStorage &mySSSStorage, const string &SSSID)
{
	vector<string> mySSS = mySSSStorage.queryAllEntryDB("sss","sss_uuid");
    vector<string>::const_iterator it;
    if (mySSS.size())
    {
		it = mySSS.begin();
		while ( it != mySSS.end())
		{
			if( SSSID == *(it) )
				return true;
			it++;
		}
	}
	return false;
};

//================================================================================
//
bool TpmCryptFindPlatform(TpmCryptStorage &myPlatformStorage, const string &platformIdentifier)
{
	vector<string> myPlatform = myPlatformStorage.queryAllEntryDB("platforms","name");
    vector<string>::const_iterator it;
    if (myPlatform.size())
    {
		it = myPlatform.begin();
		while ( it != myPlatform.end())
		{
			if( platformIdentifier == *(it) )
				return true;
			it++;
		}
	}
	return false;
};

//================================================================================
//
bool TpmCryptFindToken(TpmCryptStorage &myTokenStorage, const string &tokenIdentifier)
{
	vector<string> myToken = myTokenStorage.queryAllEntryDB("token","name");
    vector<string>::const_iterator it;
    if (myToken.size())
    {
		it = myToken.begin();
		while ( it != myToken.end())
		{
			if( tokenIdentifier == *(it) )
				return true;
			it++;
		}
	}
	return false;
};

//================================================================================
//
bool TpmCryptFindUserInVolume(TpmCryptStorage &myVolumeStorage, const string &userIdentifier, const string &volumeIdentifier)
{
	vector<string> myVolume = myVolumeStorage.queryDB("volume_users","u_uuid","v_uuid",volumeIdentifier);
    vector<string>::const_iterator it;
    if (myVolume.size())
    {
		it = myVolume.begin();
		while ( it != myVolume.end())
		{
			if( userIdentifier == *(it) )
				return true;
			it++;
		}
	}
	return false;
};

//================================================================================
//

vector<string> TpmCryptFindAllUsersInVolume(TpmCryptStorage &myVolumeStorage, const string &volumeIdentifier)
{
	vector<string> myUserNames;
	vector<string> myUsers = myVolumeStorage.queryDB("volume_users","u_uuid","v_uuid",volumeIdentifier);
	vector<string>::const_iterator it;
    if (myUsers.size())
    {
		it = myUsers.begin();
		while ( it != myUsers.end())
		{
			myUserNames.push_back(myVolumeStorage.queryDB("users","name","u_uuid",*(it)));
			it++;
		}
	}
	return myUserNames;
};

//================================================================================
//

vector<string> TpmCryptFindAllSSSInVolume(TpmCryptStorage &myVolumeStorage, const string &volumeIdentifier)
{
	vector<string> myUserNames;
	vector<string> myUsers = myVolumeStorage.queryDB("volume_sss","sss_uuid","v_uuid",volumeIdentifier);
	vector<string>::const_iterator it;
    if (myUsers.size())
    {
		it = myUsers.begin();
		while ( it != myUsers.end())
		{
			myUserNames.push_back(myVolumeStorage.queryDB("sss","name","u_uuid",*(it)));
			it++;
		}
	}
	return myUserNames;
};

//================================================================================
//
vector<string> TpmCryptFindAllAdmins(TpmCryptStorage &myUserStorage)
{
	vector<string> myAdmins = myUserStorage.queryDB("users","name","isAdmin","1");
	
	return myAdmins;
};


//================================================================================
//
vector<string> TpmCryptFindAllUsers(TpmCryptStorage &myUserStorage, bool withAdmins)
{
	if(withAdmins)
	{
		vector<string> myUsers = myUserStorage.queryAllEntryDB("users","name");
		return myUsers;
	}
	else
	{
		vector<string> myUsers = myUserStorage.queryDB("users","name","isAdmin","0");
		return myUsers;
	}
};

//================================================================================
//
vector<string> TpmCryptFindAllESDs(TpmCryptStorage &myESDStorage)
{
	vector<string> myESDs = myESDStorage.queryAllEntryDB("esd","name");
	
	return myESDs;
};

//================================================================================
//
vector<string> TpmCryptFindAllVolumes(TpmCryptStorage &myVolumeStorage)
{
	vector<string> myVolumes = myVolumeStorage.queryAllEntryDB("volumes","name");
	
	return myVolumes;
};

//================================================================================
//
vector<string> TpmCryptFindAllSSS(TpmCryptStorage &mySSSStorage)
{
    vector<string> mySSSs = mySSSStorage.queryAllEntryDB("sss","name");
    
    return mySSSs;
};

//================================================================================
//
vector<string> TpmCryptFindAllPlatforms(TpmCryptStorage &myPlatformStorage)
{
	vector<string> myPlatforms = myPlatformStorage.queryAllEntryDB("platforms","name");
    
    return myPlatforms;
};

//================================================================================
//
vector<string> TpmCryptFindAllToken(TpmCryptStorage &myTokenStorage)
{
	vector<string> myToken = myTokenStorage.queryAllEntryDB("token","name");
	
    return myToken;
};
