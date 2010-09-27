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
    vector<StringPair> myVolumes = myVolumeStorage.findAllEntries(TpmCryptVolume_isVolume, TpmCryptVolume_isVolume_true);
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
bool TpmCryptFindGroup(TpmCryptStorage &myGroupStorage, const string &GroupName)
{
    vector<StringPair> myGroups = myGroupStorage.findAllEntries(TpmCryptGroup_isGroup, TpmCryptGroup_isGroup_true);
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
bool TpmCryptFindSubject(TpmCryptStorage &mySubjectStorage, const string &subjectIdentifier)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(TpmCryptSubject_isSubject, TpmCryptSubject_isSubject_true);
    vector<utils::StringPair>::const_iterator Iterator;
    if (mySubjectIDs.size())
    {
        Iterator=mySubjectIDs.begin();
        while ( Iterator != mySubjectIDs.end())
        {
            StringPair myStringpair = *(Iterator);
            try
            {
                debug << "TpmCryptFindSubject: Looking for subject '" << subjectIdentifier << "'" << endl;
                TpmCryptSubject *mySubject = new TpmCryptSubject(subjectIdentifier, mySubjectStorage);
                debug << "TpmCryptFindSubject(): found subject '" << subjectIdentifier << "'" << endl;
                delete mySubject;
                return true;
            }
            catch ( UnknownSubject &e )
            {
                debug << "TpmCryptFindSubject(): subject '" << subjectIdentifier << "' not found" << endl;
                exit(-1);
            }
            Iterator++;
        }
    }
    return false;
};

//================================================================================
//
bool TpmCryptFindESD(TpmCryptStorage &myESDStorage, const string &ESDIdentifier)
{
    vector<StringPair> myESDIDs=myESDStorage.findAllEntries(TpmCryptESD_isESD, TpmCryptESD_isESD_true);
    vector<utils::StringPair>::const_iterator Iterator;
    if (myESDIDs.size())
    {
        Iterator=myESDIDs.begin();
        while ( Iterator != myESDIDs.end())
        {
            StringPair myStringpair = *(Iterator);
            try
            {
                debug << "TpmCryptFindESD: Looking for ESD '" << ESDIdentifier << "'" << endl;
                TpmCryptESD *myESD = new TpmCryptESD(ESDIdentifier, myESDStorage);
                debug << "TpmCryptFindESD(): found ESD '" << ESDIdentifier << "'" << endl;
                delete myESD;
                return true;
            }
            catch ( UnknownESD &e )
            {
                debug << "TpmCryptFindESD(): ESD '" << ESDIdentifier << "' not found" << endl;
                exit(-1);
            }
            Iterator++;
        }
    }
    return false;
};

//================================================================================
//
bool TpmCryptFindUser(TpmCryptStorage &mySubjectStorage, const string &subjectIdentifier)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(TpmCryptSubject_SubjectType, TpmCryptSubjectType_User);
    vector<utils::StringPair>::const_iterator Iterator;
    Iterator=mySubjectIDs.begin();
    while ( Iterator != mySubjectIDs.end())
    {
        StringPair myStringpair = *(Iterator);
        try
        {
            debug << "TpmCryptFindUser: Looking for user '" << subjectIdentifier << "'" << endl;
            TpmCryptSubject *mySubject = new TpmCryptSubject(subjectIdentifier, mySubjectStorage);
            debug << "TpmCryptFindUser(): found user '" << subjectIdentifier << "'" << endl;
            delete mySubject;
            return true;
        }
        catch ( UnknownSubject &e )
        {
            debug << "TpmCryptFindUser(): user '" << subjectIdentifier << "' not found" << endl;
        }
        Iterator++;
    }
    return false;
};


//================================================================================
//
bool TpmCryptFindAdmin(TpmCryptStorage &mySubjectStorage, const string &subjectIdentifier)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(TpmCryptSubject_SubjectAdmin, TpmCryptSubject_TRUE);
    vector<utils::StringPair>::const_iterator Iterator;
    if (mySubjectIDs.size())
    {
        Iterator=mySubjectIDs.begin();
        while ( Iterator != mySubjectIDs.end())
        {
            StringPair myStringpair = *(Iterator);
            try
            {
                debug << "TpmCryptFindSubject: Looking for subject '" << subjectIdentifier << "'" << endl;
                TpmCryptSubject *mySubject = new TpmCryptSubject(subjectIdentifier, mySubjectStorage);
                debug << "TpmCryptFindSubject(): found subject '" << subjectIdentifier << "'" << endl;
                delete mySubject;
                return true;
            }
            catch ( UnknownSubject &e )
            {
                debug << "TpmCryptFindSubject(): subject '" << subjectIdentifier << "' not found" << endl;
            }
            Iterator++;
        }
    }
    return false;
};

//================================================================================
//
bool TpmCryptFindSSS(TpmCryptStorage &mySSSStorage, const string &SSSID)
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
bool TpmCryptFindPlatform(TpmCryptStorage &mySubjectStorage, const string &subjectIdentifier)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(TpmCryptSubject_SubjectType, TpmCryptSubjectType_Platform);

    vector<utils::StringPair>::const_iterator Iterator;
    if (mySubjectIDs.size())
    {
        Iterator=mySubjectIDs.begin();
        while ( Iterator != mySubjectIDs.end())
        {
            StringPair myStringpair = *(Iterator);
            try
            {
                debug << "TpmCryptFindSubject: Looking for subject '" << subjectIdentifier << "'" << endl;
                TpmCryptSubject *mySubject = new TpmCryptSubject(subjectIdentifier, mySubjectStorage);
                debug << "TpmCryptFindSubject(): found subject '" << subjectIdentifier << "'" << endl;
                delete mySubject;
                return true;
            }
            catch ( UnknownSubject &e )
            {
                debug << "TpmCryptFindSubject(): subject '" << subjectIdentifier << "' not found" << endl;
                exit(-1);
            }
            Iterator++;
        }
    }
    return false;
};

//================================================================================
//
bool TpmCryptFindToken(TpmCryptStorage &mySubjectStorage, const string &subjectIdentifier)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(TpmCryptSubject_SubjectType, TpmCryptSubjectType_Token);

    vector<utils::StringPair>::const_iterator Iterator;
    if (mySubjectIDs.size())
    {
        Iterator=mySubjectIDs.begin();
        while ( Iterator != mySubjectIDs.end())
        {
            StringPair myStringpair = *(Iterator);
            try
            {
                debug << "TpmCryptFindSubject: Looking for subject '" << subjectIdentifier << "'" << endl;
                TpmCryptSubject *mySubject = new TpmCryptSubject(subjectIdentifier, mySubjectStorage);
                debug << "TpmCryptFindSubject(): found subject '" << subjectIdentifier << "'" << endl;
                delete mySubject;
                return true;
            }
            catch ( UnknownSubject &e )
            {
                debug << "TpmCryptFindSubject(): subject '" << subjectIdentifier << "' not found" << endl;
                exit(-1);
            }
            Iterator++;
        }
    }
    return false;
};

//================================================================================
//
vector<string> TpmCryptFindAllAdmins(TpmCryptStorage &mySubjectStorage)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(TpmCryptSubject_SubjectType, TpmCryptSubjectType_User);
    vector<string> myAdmins;
    vector<utils::StringPair>::const_iterator Iterator;
    Iterator=mySubjectIDs.begin();
    while ( Iterator != mySubjectIDs.end())
    {
        StringPair myStringpair = *(Iterator);
        TpmCryptSubject *mySubject = new TpmCryptSubject(Iterator->first, mySubjectStorage);
        if (mySubject->isAdmin())
            myAdmins.push_back(mySubject->getMySubjectName());
        delete mySubject;
        Iterator++;
    }
    return myAdmins;
};


//================================================================================
//
vector<string> TpmCryptFindAllUsers(TpmCryptStorage &mySubjectStorage, bool withAdmins)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(TpmCryptSubject_SubjectType, TpmCryptSubjectType_User);
    vector<string> myUsers;
    vector<utils::StringPair>::const_iterator Iterator;
    Iterator=mySubjectIDs.begin();
    while ( Iterator != mySubjectIDs.end())
    {
        StringPair myStringpair = *(Iterator);
        TpmCryptSubject *mySubject = new TpmCryptSubject(Iterator->first, mySubjectStorage);
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
vector<string> TpmCryptFindAllESDs(TpmCryptStorage &myESDStorage)
{
    vector<StringPair> myESDIDs=myESDStorage.findAllEntries(TpmCryptESD_isESD, TpmCryptESD_isESD_true);
    vector<string> myESDs;
    vector<utils::StringPair>::const_iterator Iterator;
    Iterator=myESDIDs.begin();
    while ( Iterator != myESDIDs.end())
    {
        StringPair myStringpair = *(Iterator);
        TpmCryptESD *myESD = new TpmCryptESD(Iterator->first, myESDStorage);
        myESDs.push_back(myESD->getMyESDName());
        
        delete myESD;
        Iterator++;
    }
    return myESDs;
};

//================================================================================
//
vector<string> TpmCryptFindAllGroups(TpmCryptStorage &myGroupStorage, TpmCryptStorage &mySubjectStorage, bool withAdminGroup)
{
    vector<StringPair> myGroups = myGroupStorage.findAllEntries(TpmCryptGroup_isGroup, TpmCryptGroup_isGroup_true);
    vector<string> myGroupVector;

    vector<utils::StringPair>::const_iterator Iterator;
    if (myGroups.size())
    {
        Iterator=myGroups.begin();
        while ( Iterator != myGroups.end())
        {
            TpmCryptGroup *myGroup = new TpmCryptGroup (Iterator->first, myGroupStorage, mySubjectStorage);
            if (myGroup->getMyGroupID() == TPMCRYPT_ADMIN_GROUP)
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
vector<string> TpmCryptFindAllSSS(TpmCryptStorage &mySSSStorage)
{
    return mySSSStorage.getAvailableSections();
};

//================================================================================
//
vector<string> TpmCryptFindAllPlatforms(TpmCryptStorage &mySubjectStorage)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(TpmCryptSubject_SubjectType, TpmCryptSubjectType_Platform);
    vector<string> myPlatforms;
    vector<utils::StringPair>::const_iterator Iterator;
    Iterator=mySubjectIDs.begin();
    while ( Iterator != mySubjectIDs.end())
    {
        StringPair myStringpair = *(Iterator);
        TpmCryptSubject *mySubject = new TpmCryptSubject(Iterator->first, mySubjectStorage);
        myPlatforms.push_back(mySubject->getMySubjectName());
        delete mySubject;
        Iterator++;
    }
    return myPlatforms;
};

//================================================================================
//
vector<string> TpmCryptFindAllToken(TpmCryptStorage &mySubjectStorage)
{
    vector<StringPair> mySubjectIDs=mySubjectStorage.findAllEntries(TpmCryptSubject_SubjectType, TpmCryptSubjectType_Token);
    vector<string> myToken;
    vector<utils::StringPair>::const_iterator Iterator;
    Iterator=mySubjectIDs.begin();
    while ( Iterator != mySubjectIDs.end())
    {
        StringPair myStringpair = *(Iterator);
        TpmCryptSubject *mySubject = new TpmCryptSubject(Iterator->first, mySubjectStorage);
        myToken.push_back(mySubject->getMySubjectName());
        delete mySubject;
        Iterator++;
    }
    return myToken;
};
