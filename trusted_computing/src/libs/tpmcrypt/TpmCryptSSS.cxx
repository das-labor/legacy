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
 * @file        TpmCryptSSS.cxx
 * @brief       TpmCrypt Secret Sharing Scheme class
 * @version     $Revision: 684 $
 * @date        Last modification at $Date: 2009-06-29 12:29:43 +0200 (Mon, 29 Jun 2009) $ by $Author: selhorst $
 */

#include <TpmCryptSSS.hxx>

using namespace std;
using namespace tpmcrypt;
using namespace utils;
using namespace algo;
using namespace integer;

const unsigned char myModulus[] = { 0xfb, 0x52, 0x6f, 0x51, 0x8c, 0x7e, 0x5c, 0xfb, 0x63, 0xe4, 0x65, 0x4c, 0xdc, 0xeb, 0xcd, 0x5c, 0x64, 0xb7, 0x3f, 0xda, 0x19, 0x4b, 0xfb, 0x4b, 0x0a, 0x64, 0x41, 0x6e, 0x58, 0x83, 0x20, 0x52, 0x53, 0x2e, 0x1f, 0xf9, 0xa3, 0x9a, 0xd9, 0xc4, 0x82, 0xf9, 0xfc, 0xfb, 0x50, 0xf2, 0xc4, 0x69, 0xdb, 0x60, 0xce, 0x35, 0xcd, 0x38, 0x41, 0xaf, 0x89, 0xe7, 0xa9, 0x4a, 0x01, 0x38, 0x6e, 0xd5 };
ByteVector Modul(myModulus, 64);


//================================================================================
// create new secret sharing scheme
TpmCryptSSS::TpmCryptSSS (TpmCryptStorage &SSSStorage, string SSSID, UInt32 n, vector<TpmCryptUser*> Participants, string SSSKey) :
    mySSSID(SSSID),
    mySubjectsInSSS(),
    mySSSStorage(SSSStorage),
    minimumParticipants(n)
{
    debug << "TpmCryptSSS|Constructor(): Creating new secret-sharing-scheme with ID '" << mySSSID << "'" << endl;

    UInt32 OverallParticipants = Participants.size();
    Integer P(OverallParticipants);
    Integer A(n);

    ByteVector Participant( I2OSP(P, P.size()));
    ByteVector Access( I2OSP(A, A.size()));
    ByteVector Key = convertStringToByteVector(SSSKey); //((const unsigned char*)SSSKey.c_str(), SSSKey.length());

    debug << "TpmCryptSSS|Constructor(): Minimum Participants: " << A << endl;
    debug << "TpmCryptSSS|Constructor(): Overall Participants: " << P << endl;
    debug << "TpmCryptSSS|Constructor(): Modulus: " << Modul << endl;

    debug << "TpmCryptSSS|Constructor(): Initializing SSS" << endl;
    SSS mySSS;
    mySSS.initialisation(Participant,Access,Key,Modul);

    // retrieving keys
    debug << "TpmCryptSSS|Constructor(): Retrieving keys" << endl;
    vector<ByteVector> x=mySSS.getX();
    vector<ByteVector> y=mySSS.getY();
	
    // assigning key for each participant
    // and encrypting it
    for(int i = 0; i<P; i++ )
    {
        string currentParticipant = Participants[i]->getMyUserUUID;
        
        ByteVector currentX = x.back();
        ByteVector currentY = y.back();
		
        ByteVector encryptedX = Participants[i]->encryptForUser(Participants[i], currentX);
        ByteVector encryptedY = Participants[i]->encryptForUser(Participants[i], currentY);

        string Base64encodedX = EncodeByteVectorToBASE64(encryptedX);
        string Base64encodedY = EncodeByteVectorToBASE64(encryptedY);

        storeParticipantKey(Participant[i],Base64encodedX,Base64encodedY);

        mySubjectsInSSS.push_back(currentParticipant);
        x.pop_back();
        y.pop_back();
        string message = "You have been added to secret-sharing-scheme '" + SSSID + "'";
        Participants[i]->addMessageForUser(message);
    }
    storeSSS();
};

//================================================================================
// load existing secret sharing scheme
TpmCryptSSS::TpmCryptSSS (TpmCryptStorage &SSSStorage, string SSSID) :
    mySSSID(SSSID),
    mySubjectsInSSS(),
    mySSSStorage(SSSStorage),
    minimumParticipants(0)
{
    debug << "TpmCryptSSS|Constructor(): Loading existing secret-sharing-scheme with ID '" << mySSSID << "'" << endl;
    if (SSSID.empty())
        throw InvalidSSSID("TpmCryptSSS|Constructor(): Invalid SSSID supplied!");
    loadSSS();
};

//================================================================================
// DESTRUCTOR
TpmCryptSSS::~TpmCryptSSS()
{
    debug << "TpmCryptSSS|Destructor(): Destroying secret-sharing-scheme with ID '" << mySSSID << "'" << endl;
};

//================================================================================
// delete this SSS
void TpmCryptSSS::deleteSSS()
{
    debug << "TpmCryptSSS|delete(): Deleting secret-sharing-scheme with ID '" << mySSSID << "'" << endl;
    mySSSStorage.deleteSection(mySSSID);
    mySSSID.empty();
    mySubjectsInSSS.empty();
    minimumParticipants = 0;
};

//================================================================================
// retrieve the key by n subjects
string TpmCryptSSS::retrieveKey(vector<TpmCryptUser*> AvailableParticipants, vector<string> subjectPasswords)
{
    debug << "TpmCryptSSS|retrieveKey(): Retrieving key from secret-sharing-scheme with ID '" << mySSSID << "'" << endl;
    if (AvailableParticipants.size() < minimumParticipants)
     throw NotEnoughParticipants("TpmCryptSSS|retrieveKey(): Not enough participants to retrieve key for SSSID '" + mySSSID + "'");

    if (AvailableParticipants.size() != subjectPasswords.size())
        throw InvalidPassword("TpmCryptSSS|retrieveKey(): Amount of participants and passwords do not match!");

    SSS mySSS;

    vector<ByteVector> x;
    vector<ByteVector> y;

    vector<TpmCryptUser*>::const_iterator UserIterator;
    vector<string>::const_iterator PasswordIterator;

    UserIterator=AvailableParticipants.begin();
    PasswordIterator=userPasswords.begin();

    while ( UserIterator != AvailableParticipants.end())
    {
        TpmCryptUser *myUser = *(UserIterator);
        string currentParticipantsUUID = myUser->getMyUserUUID();
        string currentParticipantsPassword = *(PasswordIterator);
        StringPair myXY = loadParticipantKey(currentParticipantsUUID);

        string myX = myXY.first;
        string myY = myXY.second;

        ByteVector decodedX = DecodeBASE64StringToByteVector(myX);
        ByteVector decodedY = DecodeBASE64StringToByteVector(myY);

        decodedX = myUser->decryptByUser(myUser, decodedX, currentParticipantsPassword);
        decodedY = myUser->decryptByUser(myUser, decodedY, currentParticipantsPassword);

        UserIterator++;
        PasswordIterator++;
        x.push_back(decodedX);
        y.push_back(decodedY);
    }
    // reconstruction
    debug << "TpmCryptSSS|retrieveKey(): Reconstruction..." << endl;
    ByteVector myReconstructedKey = mySSS.reconstruction(x, y, Modul);
    return convertByteVector2String(myReconstructedKey);
};

//================================================================================
void TpmCryptSSS::storeSSS()
{
	mySSSStorage.storeDB("sss","min_participants",convertUInt32toString(minimumParticipants),"uuid",mySSSID);
}

//================================================================================
void TpmCryptSSS::storeParticipantKey(string &Participant, string &X, string &Y)
{
    debug << "TpmCryptSSS|storeSSS(): Storing secret-sharing-scheme with ID '" << mySSSID << "'" << endl;
    bool found = true;
    string uuid;
    
    srand(time(NULL));
    
    while(found)
    {
		uuid = convertUInt32toString((rand() % 100000 + 1));
		found = TpmCryptFindSSS(mySSSStorage,uuid);
	}
	
	mySSSStorage.sssEntriesDB(uuid,SSSID,*(Iterator),X,Y);
};

//================================================================================
void TpmCryptSSS::loadSSS()
{
    debug << "TpmCryptSSS|storeSSS(): Loading secret-sharing-scheme with ID '" << mySSSID << "'" << endl;
    if (! TpmCryptFindSSS(mySSSStorage,SSSID))
        throw InvalidSSSID("TpmCryptSSS|loadSSS(): Error loading SSS, invalid SSSID supplied!");
    minimumParticipants = convertStringtoUInt32(mySSSStorage.queryDB("sss","min_participants","uuid",SSSID));
    if (!minimumParticipants)
        throw NotEnoughParticipants("TpmCryptSSS|loadSSS(): Error loading SSS, n is zero!");

    mySubjectsInSSS = mySSSStorage.queryDB("sss_entries","u_uuid","sss_uuid",SSSID);
    if (!(mySubjectsInSSS.size()) || (mySubjectsInSSS.size() < minimumParticipants))
        throw NotEnoughParticipants("TpmCryptSSS|loadSSS(): Error loading SSS, n is bigger than participant list!");
};

//================================================================================
StringPair TpmCryptSSS::loadParticipantKey(string &Participant)
{
    debug << "TpmCryptSSS|loadParticipantKey(): Storing participant key in secret-sharing-scheme with ID '" << mySSSID << "'" << endl;
    string X = mySSSStorage.queryDB("sss_entries","x","u_uuid",Participant);
    string Y = mySSSStorage.queryDB("sss_entries","y","u_uuid",Participant);
    StringPair xy(X,Y);
    return xy;
};

//================================================================================
void TpmCryptSSS::printSSS()
{
     debug << "TpmCryptSSS|printSSS(): Details for secret-sharing-scheme with ID '" << mySSSID << "'" << endl;
     cout << "Secret-Sharing-Scheme ID: " << mySSSID << endl;
     cout << "\tMinimal required participants: " << minimumParticipants << " (n)" << endl;
     cout << "\tTotal Participants: " << mySubjectsInSSS.size() << " (m)" <<  endl;
     cout << "\tParticipant list: " << endl;
     cout << "\t---------------------" << endl;
     printStringVector(&mySubjectsInSSS);
};
