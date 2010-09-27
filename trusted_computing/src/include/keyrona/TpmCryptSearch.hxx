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
using namespace keyrona;

#ifndef _TPMCRYPTSEARCH_HXX
#define	_TPMCRYPTSEARCH_HXX

/*!
 *  @brief determines whether or not the given subject is an admin in the database
 *  @param &mySubjectStorage, TpmCryptStorage, indicates the storage where to search
 *  @param &SubjectID, const string, the ID of the subject to search
 *  @return bool, determines whether or not the given subject is an admin
 */
bool TpmCryptFindAdmin(TpmCryptStorage &mySubjectStorage, const string &SubjectID);

/*!
 *  @brief determines whether or not the given group exists in the database
 *  @param &myGroupStorage, TpmCryptStorage, indicates the storage where to search
 *  @param &GroupName, const string, the name of the group to search
 *  @return bool, determines whether or not the given group exists
 */
bool TpmCryptFindGroup(TpmCryptStorage &myGroupStorage, const string &GroupName);

/*!
 *  @brief determines whether or not the given platform exists in the database
 *  @param &mySubjectStorage, TpmCryptStorage, indicates the storage where to search
 *  @param &SubjectID, const string, the id of the platform to search
 *  @return bool, determines whether or not the given platform exists
 */
bool TpmCryptFindPlatform(TpmCryptStorage &mySubjectStorage, const string &SubjectID);

/*!
 *  @brief determines whether or not the given subject exists in the database
 *  @param &mySubjectStorage, TpmCryptStorage, indicates the storage where to search
 *  @param &SubjectID, const string, the id of the subject to search
 *  @return bool, determines whether or not the given subject exists
 */
bool TpmCryptFindSubject(TpmCryptStorage &mySubjectStorage, const string &SubjectID);


bool TpmCryptFindESD(TpmCryptStorage &myESDStorage, const string &ESDID);

/*!
 *  @brief determines whether or not the given token exists in the database
 *  @param &mySubjectStorage, TpmCryptStorage, indicates the storage where to search
 *  @param &SubjectID, const string, the id of the token to search
 *  @return bool, determines whether or not the given token exists
 */
bool TpmCryptFindToken(TpmCryptStorage &mySubjectStorage, const string &SubjectID);

/*!
 *  @brief determines whether or not the given user exists in the database
 *  @param &mySubjectStorage, TpmCryptStorage, indicates the storage where to search
 *  @param &SubjectID, const string, the id of the user to search
 *  @return bool, determines whether or not the given user exists
 */
bool TpmCryptFindUser(TpmCryptStorage &mySubjectStorage, const string &SubjectID);

/*!
 *  @brief determines whether or not the given secret sharing scheme exists in the database
 *  @param &mySSSStorage, TpmCryptStorage, indicates the storage where to search
 *  @param &SSSID, const string, the id of the sss to search
 *  @return bool, determines whether or not the given sss exists
 */
bool TpmCryptFindSSS(TpmCryptStorage &mySSSStorage, const string &SSSID);

/*!
 *  @brief determines whether or not the given volume exists in the database
 *  @param &myVolumeStorage, TpmCryptStorage, indicates the storage where to search
 *  @param &VolumeUUID, const string, the id of the volume to search
 *  @return bool, determines whether or not the given volume exists
 */
bool TpmCryptFindVolume(TpmCryptStorage &myVolumeStorage, const string &VolumeUUID);

/*!
 *  @brief finds all administrators in a given database
 *  @param &mySubjectStorage, TpmCryptStorage, indicates the storage where to search
 *  @return vector of string, contains the administrators that have been found
 */
vector<string> TpmCryptFindAllAdmins(TpmCryptStorage &mySubjectStorage);

/*!
 *  @brief finds all users in a given database
 *  @param &mySubjectStorage, TpmCryptStorage, indicates the storage where to search
 *  @param withAdmins, bool, determines whether or not the admins are included in the search
 *  @return vector of string, contains the users that have been found
 */
vector<string> TpmCryptFindAllUsers(TpmCryptStorage &mySubjectStorage, bool withAdmins);

/*!
 *  @brief finds all members of all groups in a given database
 *  @param &myGroupStorage, TpmCryptStorage, indicates the storage where to search
 *  @param &mySubjectStorage, TpmCryptStorage, indicates the storage where to search
 *  @param withAdminGroup, bool, determines whether or not the admingroup are included in the search
 *  @return vector of string, contains the groups that have been found
 */
vector<string> TpmCryptFindAllGroups(TpmCryptStorage &myGroupStorage, TpmCryptStorage &mySubjectStorage, bool withAdminGroup);

/*!
 *  @brief finds all platforms in a given database
 *  @param &mySubjectStorage, TpmCryptStorage, indicates the storage where to search
 *  @return vector of string, contains the platforms that have been found
 */
vector<string> TpmCryptFindAllPlatforms(TpmCryptStorage &mySubjectStorage);

/*!
 *  @brief finds all secret sharings schemes in a given database
 *  @param &mySSSStorage, TpmCryptStorage, indicates the storage where to search
 *  @return vector of string, contains the secret sharing shemes that have been found
 */
vector<string> TpmCryptFindAllSSS(TpmCryptStorage &mySSSStorage);

/*!
 *  @brief finds all tokens in a given database
 *  @param &mySubjectStorage, TpmCryptStorage, indicates the storage where to search
 *  @return vector of string, contains the tokens that have been found
 */
vector<string> TpmCryptFindAllToken(TpmCryptStorage &mySubjectStorage);

vector<string> TpmCryptFindAllESDs(TpmCryptStorage &myESDStorage);


#endif	/* _TPMCRYPTSEARCH_HXX */

