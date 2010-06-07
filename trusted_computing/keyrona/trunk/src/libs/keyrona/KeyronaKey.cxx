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
 * @file        KeyronaKey.cxx
 * @brief       Implementation of the Keyrona Key class.
 * @version     $Revision: 449 $
 * @date        Last modification at $Date: 2009-04-06 00:16:17 +0200 (Mon, 06 Apr 2009) $ by $Author: selhorst $
 */

#include <KeyronaKey.hxx>

using namespace std;
using namespace utils;
using namespace keyrona;

static UInt32 KeyCounter = 0;

// load existing key
//================================================================================
//
KeyronaKey::KeyronaKey(string &keyfile, int &bindkeynum) :
valid (false)
{
	
	
    valid = true;
    KeyCounter++;
};

// create a new key
//================================================================================
//
KeyronaKey::KeyronaKey(KeyronaSubject *Subject, string &password, int &type) :
valid (false)
{
	KeyronaTPM;
	
	myTPM.create_key(toEncrypt, bindkeynum);

    valid = true;
    KeyCounter++;
};

//================================================================================
//
KeyronaKey::KeyronaKey(KeyronaGroup *Group, string &groupKeyPassword) :
valid (false)
{

    valid = true;
    KeyCounter++;
};


//================================================================================
//
KeyronaKey::~KeyronaKey()
{
    if (KeyCounter == 1)
    {
        // garbage collection and destroying of cryptlib objects
        debug << "KeyronaKey|Destructor(): Cleaning up cryptlib..." << endl;
//        cryptlibCheck(cryptEnd(), "Cryptlib cleanup process failed!");
    }
    else
        debug << "KeyronaKey|Destructor(): Not finishing cryptlib, since we still have " << KeyCounter << " open objects!" << endl;

    KeyCounter--;
};

//================================================================================
//
ByteVector  KeyronaKey::encrypt(ByteVector &toEncrypt, int &bindkeynum)
{
   	KeyronaTPM;

	vector<ByteVector> mySealedDataWithKey = myTPM.bind(toEncrypt, bindkeynum);
    ByteVector result = mySealedDataWithKey.back();
    mySealedDataWithKey.pop_back();
    
    return result;
};

//================================================================================
//
ByteVector  KeyronaKey::decrypt(ByteVector &toDecrypt,int &bindkeynum, string &myPassword)
{
	KeyronaTPM;

	vector<ByteVector> mySealedDataWithKey = myTPM.unbind(toDecrypt, bindkeynum, password);
    ByteVector result = mySealedDataWithKey.back();
    mySealedDataWithKey.pop_back();

    return result;
};

//================================================================================
//
void KeyronaKey::printKeyInformation()
{

};

//================================================================================
//
bool KeyronaKey::changePassword(string &oldPassword, string &newPassword)
{
   
   
   
    return true;
  
};
