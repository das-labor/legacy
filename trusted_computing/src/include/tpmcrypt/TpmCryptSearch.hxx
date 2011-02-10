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
 * @file        TpmCryptSearch.hxx
 * @brief       Implementation of some search operations.
 * @version     $Revision: 800 $
 * @date        Last modification at $Date: 2009-08-11 18:44:01 +0200 (Tue, 11 Aug 2009) $ by $Author: selhorst $
 */

#include <TpmCrypt.hxx>
#include <TpmCryptConfigfile.hxx>
#include <TpmCryptStorage.hxx>
#include <TpmCryptSubject.hxx>
#include <TpmCryptGroup.hxx>
#include <TpmCryptVolume.hxx>
#include <TpmCryptESD.hxx>

using namespace std;
using namespace utils;
using namespace tpmcrypt;

#ifndef _TPMCRYPTSEARCH_HXX
#define	_TPMCRYPTSEARCH_HXX

bool TpmCryptFindVolume(TpmCryptStorage &myVolumeStorage, const string &VolumeUUID);

bool TpmCryptFindESD(TpmCryptStorage &myESDStorage, const string &esdIdentifier);

bool TpmCryptFindUser(TpmCryptStorage &myUserStorage, const string &userIdentifier);

bool TpmCryptFindAdmin(TpmCryptStorage &myUserStorage, const string &userIdentifier);

bool TpmCryptFindSSS(TpmCryptStorage &mySSSStorage, const string &SSSID);

bool TpmCryptFindPlatform(TpmCryptStorage &myPlatformStorage, const string &platformIdentifier);

bool TpmCryptFindToken(TpmCryptStorage &myTokenStorage, const string &tokenIdentifier);

bool TpmCryptFindUserInVolume(TpmCryptStorage &myVolumeStorage, const string &userIdentifier, const string &volumeIdentifier);

vector<string> TpmCryptFindAllSSSInVolume(TpmCryptStorage &myVolumeStorage, const string &volumeIdentifier);

vector<string> TpmCryptFindAllUsersInVolume(TpmCryptStorage &myVolumeStorage, const string &volumeIdentifier);

vector<string> TpmCryptFindAllAdmins(TpmCryptStorage &myUserStorage);

vector<string> TpmCryptFindAllUsers(TpmCryptStorage &myUserStorage, bool withAdmins);

vector<string> TpmCryptFindAllESDs(TpmCryptStorage &myESDStorage);

vector<string> TpmCryptFindAllVolumes(TpmCryptStorage &myVolumeStorage);

vector<string> TpmCryptFindAllSSS(TpmCryptStorage &mySSSStorage);

vector<string> TpmCryptFindAllPlatforms(TpmCryptStorage &myPlatformStorage);

vector<string> TpmCryptFindAllToken(TpmCryptStorage &myTokenStorage);

#endif	/* _TPMCRYPTSEARCH_HXX */

