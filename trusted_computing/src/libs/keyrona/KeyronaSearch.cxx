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
 * @file        KeyronaSearch.cxx
 * @brief       Implementation of some search operations.
 * @version     $Revision: 656 $
 * @date        Last modification at $Date: 2009-06-12 17:33:42 +0200 (Fri, 12 Jun 2009) $ by $Author: selhorst $
 */

#include <KeyronaSearch.hxx>

//================================================================================
//
bool KeyronaFindVolume(KeyronaStorage &myVolumeStorage, const string &VolumeUUID)
{
    vector<StringPair> myVolumes = myVolumeStorage.findAllEntries(KeyronaVolume_isVolume, KeyronaVolume_isVolume_true);
    vector<utils::StringPair>::const_iterator Iterator;
    if (myVolumes.size())
    {
        Iterator=myVolumes.begin();
        while ( Iterator != myVolumes.end())
        {
            StringPair myStringpair = *(Iterator);
            if (myStringpair.first == VolumeUUID)
                return true;
            Iterator++;
        }
    }
    return false;
};

//================================================================================
//
bool KeyronaFindGroup(KeyronaStorage &myGroupStorage, const string &GroupName)
{
    vector<StringPair> myGroups = myGroupStorage.findAllEntries(KeyronaGroup_isGroup, KeyronaGroup_isGroup_true);
    vector<utils::StringPair>::const_iterator Iterator;
    if (myGroups.size())
    {
        Iterator=myGroups.begin();
        while ( Iterator != myGroups.end())
        {
            StringPair myStringpair = *(Iterator);
            if (myStringpair.first == GroupName)
                return true;
            Iterator++;
        }
    }
    return false;
};

//================================================================================
//
bool KeyronaFindSubject(KeyronaStorage &mySubjectStorage, const string &subjectIdentifier)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(KeyronaSubject_isSubject, KeyronaSubject_isSubject_true);
    vector<utils::StringPair>::const_iterator Iterator;
    if (mySubjectIDs.size())
    {
        Iterator=mySubjectIDs.begin();
        while ( Iterator != mySubjectIDs.end())
        {
            StringPair myStringpair = *(Iterator);
            try
            {
                debug << "KeyronaFindSubject: Looking for subject '" << subjectIdentifier << "'" << endl;
                KeyronaSubject *mySubject = new KeyronaSubject(subjectIdentifier, mySubjectStorage);
                debug << "KeyronaFindSubject(): found subject '" << subjectIdentifier << "'" << endl;
                delete mySubject;
                return true;
            }
            catch ( UnknownSubject &e )
            {
                debug << "KeyronaFindSubject(): subject '" << subjectIdentifier << "' not found" << endl;
                exit(-1);
            }
            Iterator++;
        }
    }
    return false;
};

//================================================================================
//
bool KeyronaFindESD(KeyronaStorage &myESDStorage, const string &ESDIdentifier)
{
    vector<StringPair> myESDIDs=myESDStorage.findAllEntries(KeyronaESD_isESD, KeyronaESD_isESD_true);
    vector<utils::StringPair>::const_iterator Iterator;
    if (myESDIDs.size())
    {
        Iterator=myESDIDs.begin();
        while ( Iterator != myESDIDs.end())
        {
            StringPair myStringpair = *(Iterator);
            try
            {
                debug << "KeyronaFindESD: Looking for ESD '" << ESDIdentifier << "'" << endl;
                KeyronaESD *myESD = new KeyronaESD(ESDIdentifier, myESDStorage);
                debug << "KeyronaFindESD(): found ESD '" << ESDIdentifier << "'" << endl;
                delete myESD;
                return true;
            }
            catch ( UnknownESD &e )
            {
                debug << "KeyronaFindESD(): ESD '" << ESDIdentifier << "' not found" << endl;
                exit(-1);
            }
            Iterator++;
        }
    }
    return false;
};

//================================================================================
//
bool KeyronaFindUser(KeyronaStorage &mySubjectStorage, const string &subjectIdentifier)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(KeyronaSubject_SubjectType, KeyronaSubjectType_User);
    vector<utils::StringPair>::const_iterator Iterator;
    Iterator=mySubjectIDs.begin();
    while ( Iterator != mySubjectIDs.end())
    {
        StringPair myStringpair = *(Iterator);
        try
        {
            debug << "KeyronaFindUser: Looking for user '" << subjectIdentifier << "'" << endl;
            KeyronaSubject *mySubject = new KeyronaSubject(subjectIdentifier, mySubjectStorage);
            debug << "KeyronaFindUser(): found user '" << subjectIdentifier << "'" << endl;
            delete mySubject;
            return true;
        }
        catch ( UnknownSubject &e )
        {
            debug << "KeyronaFindUser(): user '" << subjectIdentifier << "' not found" << endl;
        }
        Iterator++;
    }
    return false;
};


//================================================================================
//
bool KeyronaFindAdmin(KeyronaStorage &mySubjectStorage, const string &subjectIdentifier)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(KeyronaSubject_SubjectAdmin, KeyronaSubject_TRUE);
    vector<utils::StringPair>::const_iterator Iterator;
    if (mySubjectIDs.size())
    {
        Iterator=mySubjectIDs.begin();
        while ( Iterator != mySubjectIDs.end())
        {
            StringPair myStringpair = *(Iterator);
            try
            {
                debug << "KeyronaFindSubject: Looking for subject '" << subjectIdentifier << "'" << endl;
                KeyronaSubject *mySubject = new KeyronaSubject(subjectIdentifier, mySubjectStorage);
                debug << "KeyronaFindSubject(): found subject '" << subjectIdentifier << "'" << endl;
                delete mySubject;
                return true;
            }
            catch ( UnknownSubject &e )
            {
                debug << "KeyronaFindSubject(): subject '" << subjectIdentifier << "' not found" << endl;
            }
            Iterator++;
        }
    }
    return false;
};

//================================================================================
//
bool KeyronaFindSSS(KeyronaStorage &mySSSStorage, const string &SSSID)
{
    vector<string> myAvailableSSSs = mySSSStorage.getAvailableSections();
    vector<string>::const_iterator Iterator;
    if (myAvailableSSSs.size())
    {
        Iterator=myAvailableSSSs.begin();
        while ( Iterator != myAvailableSSSs.end())
        {
            string SSS = *(Iterator);
            if (SSS == SSSID)
                return true;
            Iterator++;
        }
    }
    return false;
};

//================================================================================
//
bool KeyronaFindPlatform(KeyronaStorage &mySubjectStorage, const string &subjectIdentifier)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(KeyronaSubject_SubjectType, KeyronaSubjectType_Platform);

    vector<utils::StringPair>::const_iterator Iterator;
    if (mySubjectIDs.size())
    {
        Iterator=mySubjectIDs.begin();
        while ( Iterator != mySubjectIDs.end())
        {
            StringPair myStringpair = *(Iterator);
            try
            {
                debug << "KeyronaFindSubject: Looking for subject '" << subjectIdentifier << "'" << endl;
                KeyronaSubject *mySubject = new KeyronaSubject(subjectIdentifier, mySubjectStorage);
                debug << "KeyronaFindSubject(): found subject '" << subjectIdentifier << "'" << endl;
                delete mySubject;
                return true;
            }
            catch ( UnknownSubject &e )
            {
                debug << "KeyronaFindSubject(): subject '" << subjectIdentifier << "' not found" << endl;
                exit(-1);
            }
            Iterator++;
        }
    }
    return false;
};

//================================================================================
//
bool KeyronaFindToken(KeyronaStorage &mySubjectStorage, const string &subjectIdentifier)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(KeyronaSubject_SubjectType, KeyronaSubjectType_Token);

    vector<utils::StringPair>::const_iterator Iterator;
    if (mySubjectIDs.size())
    {
        Iterator=mySubjectIDs.begin();
        while ( Iterator != mySubjectIDs.end())
        {
            StringPair myStringpair = *(Iterator);
            try
            {
                debug << "KeyronaFindSubject: Looking for subject '" << subjectIdentifier << "'" << endl;
                KeyronaSubject *mySubject = new KeyronaSubject(subjectIdentifier, mySubjectStorage);
                debug << "KeyronaFindSubject(): found subject '" << subjectIdentifier << "'" << endl;
                delete mySubject;
                return true;
            }
            catch ( UnknownSubject &e )
            {
                debug << "KeyronaFindSubject(): subject '" << subjectIdentifier << "' not found" << endl;
                exit(-1);
            }
            Iterator++;
        }
    }
    return false;
};

//================================================================================
//
vector<string> KeyronaFindAllAdmins(KeyronaStorage &mySubjectStorage)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(KeyronaSubject_SubjectType, KeyronaSubjectType_User);
    vector<string> myAdmins;
    vector<utils::StringPair>::const_iterator Iterator;
    Iterator=mySubjectIDs.begin();
    while ( Iterator != mySubjectIDs.end())
    {
        StringPair myStringpair = *(Iterator);
        KeyronaSubject *mySubject = new KeyronaSubject(Iterator->first, mySubjectStorage);
        if (mySubject->isAdmin())
            myAdmins.push_back(mySubject->getMySubjectName());
        delete mySubject;
        Iterator++;
    }
    return myAdmins;
};


//================================================================================
//
vector<string> KeyronaFindAllUsers(KeyronaStorage &mySubjectStorage, bool withAdmins)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(KeyronaSubject_SubjectType, KeyronaSubjectType_User);
    vector<string> myUsers;
    vector<utils::StringPair>::const_iterator Iterator;
    Iterator=mySubjectIDs.begin();
    while ( Iterator != mySubjectIDs.end())
    {
        StringPair myStringpair = *(Iterator);
        KeyronaSubject *mySubject = new KeyronaSubject(Iterator->first, mySubjectStorage);
        if (mySubject->isAdmin())
        {
            if (withAdmins)
                myUsers.push_back(mySubject->getMySubjectName());
        }
        else
        {
            myUsers.push_back(mySubject->getMySubjectName());
        }
        delete mySubject;
        Iterator++;
    }
    return myUsers;
};

//================================================================================
//
vector<string> KeyronaFindAllESDs(KeyronaStorage &myESDStorage)
{
    vector<StringPair> myESDIDs=myESDStorage.findAllEntries(KeyronaESD_isESD, KeyronaESD_isESD_true);
    vector<string> myESDs;
    vector<utils::StringPair>::const_iterator Iterator;
    Iterator=myESDIDs.begin();
    while ( Iterator != myESDIDs.end())
    {
        StringPair myStringpair = *(Iterator);
        KeyronaESD *myESD = new KeyronaESD(Iterator->first, myESDStorage);
        myESDs.push_back(myESD->getMyESDName());
        
        delete myESD;
        Iterator++;
    }
    return myESDs;
};

//================================================================================
//
vector<string> KeyronaFindAllGroups(KeyronaStorage &myGroupStorage, KeyronaStorage &mySubjectStorage, bool withAdminGroup)
{
    vector<StringPair> myGroups = myGroupStorage.findAllEntries(KeyronaGroup_isGroup, KeyronaGroup_isGroup_true);
    vector<string> myGroupVector;

    vector<utils::StringPair>::const_iterator Iterator;
    if (myGroups.size())
    {
        Iterator=myGroups.begin();
        while ( Iterator != myGroups.end())
        {
            KeyronaGroup *myGroup = new KeyronaGroup (Iterator->first, myGroupStorage, mySubjectStorage);
            if (myGroup->getMyGroupID() == KEYRONA_ADMIN_GROUP)
            {
                if (withAdminGroup)
                    myGroupVector.push_back(Iterator->first);
            }
            else
            {
                myGroupVector.push_back(Iterator->first);
            }
            delete myGroup;
            Iterator++;
        }
    }
    return myGroupVector;
};

//================================================================================
//
vector<string> KeyronaFindAllSSS(KeyronaStorage &mySSSStorage)
{
    return mySSSStorage.getAvailableSections();
};

//================================================================================
//
vector<string> KeyronaFindAllPlatforms(KeyronaStorage &mySubjectStorage)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(KeyronaSubject_SubjectType, KeyronaSubjectType_Platform);
    vector<string> myPlatforms;
    vector<utils::StringPair>::const_iterator Iterator;
    Iterator=mySubjectIDs.begin();
    while ( Iterator != mySubjectIDs.end())
    {
        StringPair myStringpair = *(Iterator);
        KeyronaSubject *mySubject = new KeyronaSubject(Iterator->first, mySubjectStorage);
        myPlatforms.push_back(mySubject->getMySubjectName());
        delete mySubject;
        Iterator++;
    }
    return myPlatforms;
};

//================================================================================
//
vector<string> KeyronaFindAllToken(KeyronaStorage &mySubjectStorage)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(KeyronaSubject_SubjectType, KeyronaSubjectType_Token);
    vector<string> myToken;
    vector<utils::StringPair>::const_iterator Iterator;
    Iterator=mySubjectIDs.begin();
    while ( Iterator != mySubjectIDs.end())
    {
        StringPair myStringpair = *(Iterator);
        KeyronaSubject *mySubject = new KeyronaSubject(Iterator->first, mySubjectStorage);
        myToken.push_back(mySubject->getMySubjectName());
        delete mySubject;
        Iterator++;
    }
    return myToken;
};
