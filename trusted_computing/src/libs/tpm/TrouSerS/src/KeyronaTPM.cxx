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


#include <KeyronaTPM.hxx>
#include <trousers/tss.h>
#include <trousers/trousers.h>
#include <unistd.h>


using namespace std;
using namespace utils;
using namespace keyrona;

#define TSS_ERROR_CODE(x)       (x & 0xFFF)
#define TSS_ERROR_LAYER(x)      (x & 0x3000)

#define NULL_HOBJECT    0
#define NULL_HKEY       NULL_HOBJECT
#define NULL_HPCRS      NULL_HOBJECT
#define NULL_HHASH      NULL_HOBJECT
#define NULL_HENCDATA   NULL_HOBJECT
#define NULL_HTPM       NULL_HOBJECT
#define NULL_HCONTEXT   NULL_HOBJECT
#define NULL_HDELFAMILY NULL_HOBJECT
#define NULL_HPOLICY    NULL_HOBJECT

const UInt32    TPM_HASH_SIZE       = 20;
const ByteVector trousers("TrouSerS");

string getTSSError(TSS_RESULT r);

KeyronaTPM::KeyronaTPM()
{
    debug << "KeyronaTPM[TrouSerS]|Constructor(): called!" << endl;
    TSS_HCONTEXT hContext;
    BYTE* owner;
    TSS_HTPM hTPM;
    UINT32 len;

    if (Tspi_Context_Create(&hContext) != TSS_SUCCESS)
        throw ContextError("KeyronaTPM[TrouSerS]|Constructor(): Could not create context!");

    if (Tspi_Context_Connect(hContext, NULL) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        throw NoTCSD("KeyronaTPM[TrouSerS]|Constructor(): Could not connect to TPM! The function you requested requires TPM access.\nPlease make sure, that:\n\t- you have a TPM\n\t- your TPM device driver is loaded\n\t- your TPM SRK passphrase is set to the WELL_KNOWN_SECRET\n\t- TrouSerS 'tcsd' is running");
    }

    if (Tspi_Context_GetTpmObject(hContext, &hTPM) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TPMConnectError("KeyronaTPM[TrouSerS]|Constructor(): Could not connect to TPM! The function you requested requires TPM access.\nPlease make sure, that:\n\t- you have a TPM\n\t- your TPM device driver is loaded\n\t- your TPM SRK passphrase is set to the WELL_KNOWN_SECRET\n\t- TrouSerS 'tcsd' is running");
    }

    UINT32 subCap = TSS_TPMCAP_PROP_OWNER;
    if ( Tspi_TPM_GetCapability( hTPM, TSS_TPMCAP_PROPERTY, sizeof(UINT32), (BYTE*) &subCap, &len, &owner ) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|Constructor(): Error reading TPM capabilities.");
    }

    if (*owner == true)
    {
        debug << "TPM is owned!" << endl;
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
    }
    else
    {
        debug << "TPM is unowned!" << endl;
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw InvalidTPM("KeyronaTPM[TrouSerS]|Constructor(): You don't have an owned TPM! Make sure to take ownership of the TPM and set the SRK password to the WELL_KNOWN_SECRET!");
    }
};

KeyronaTPM::~KeyronaTPM()
{
    debug << "KeyronaTPM[TrouSerS]|Destructor(): called!" << endl;
};

void KeyronaTPM::revokeek()
{
	TSS_HCONTEXT hContext;
    TSS_HTPM     hTPM; 
    string credential;
    
    if (Tspi_Context_Create(&hContext) != TSS_SUCCESS)
        throw ContextError("KeyronaTPM[TrouSerS]|revokeek(): Could not create context!");

    if (Tspi_Context_Connect(hContext, NULL) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw NoTCSD("KeyronaTPM[TrouSerS]|revokeek(): Could not connect! Is TrouSerS 'tcsd' running?");
    }

    if (Tspi_Context_GetTpmObject(hContext, &hTPM) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TPMConnectError("KeyronaTPM[TrouSerS]|revokeek(): Could not connect to TPM!");
    }
    
    TSS_RESULT error0 = Tspi_TPM_RevokeEndorsementKey(hTPM, credential.length(),(BYTE*)credential.c_str());
    if ( error0 != TSS_SUCCESS )
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|revokeek(): Error revoking Endorsement Key. (" + getTSSError(error0) + ")");
    }
    
    Tspi_Context_FreeMemory(hContext, NULL);
    Tspi_Context_Close(hContext);
};

vector<ByteVector> KeyronaTPM::seal(ByteVector &dataToSeal, int &local)
{
    TSS_HCONTEXT hContext;
    TSS_HTPM     hTPM;
    TSS_HKEY	 hSRK;
    TSS_HENCDATA hEncData;
    TSS_UUID     SRK_UUID = TSS_UUID_SRK;
    TSS_HPCRS	 hPCR;
    TSS_HPOLICY  hPolicy;
    UINT32 	     pcr_index;
    UINT32 		 encLen;
    UINT32		 pcrvaluelength;
    UINT32		 locality;
    BYTE 	     *pcrvalue;
    BYTE 		 *encData;
    

    if (Tspi_Context_Create(&hContext) != TSS_SUCCESS)
        throw ContextError("KeyronaTPM[TrouSerS]|seal(): Could not create context!");

    if (Tspi_Context_Connect(hContext, NULL) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw NoTCSD("KeyronaTPM[TrouSerS]|seal(): Could not connect! Is TrouSerS 'tcsd' running?");
    }

    if (Tspi_Context_GetTpmObject(hContext, &hTPM) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TPMConnectError("KeyronaTPM[TrouSerS]|seal(): Could not connect to TPM!");
    }

     // create hEncData
    if (Tspi_Context_CreateObject( hContext, TSS_OBJECT_TYPE_ENCDATA, TSS_ENCDATA_SEAL, &hEncData ) != TSS_SUCCESS )
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|seal(): Error creating data object.");
    }

    // Load SRK Key By UUID
    if (Tspi_Context_LoadKeyByUUID( hContext, TSS_PS_TYPE_SYSTEM, SRK_UUID, &hSRK ) != TSS_SUCCESS )
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|seal(): Error loading SRK key object.");
    }
    
	// Create PCR Context and set locality
	if (Tspi_Context_CreateObject ( hContext, TSS_OBJECT_TYPE_PCRS,	TSS_PCRS_STRUCT_INFO_LONG, &hPCR) != TSS_SUCCESS )
	{
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|seal(): Error create PCR Object");
    }

	switch(local) {
							default:
									locality = TPM_LOC_ZERO;
									break;
							case 1:
									locality = TPM_LOC_ONE;
									break;
						    case 2:
									locality = TPM_LOC_TWO;
									break;
							case 3:
									locality = TPM_LOC_THREE;
									break;
							case 4:
									locality = TPM_LOC_FOUR;
									break;
	}
									
    if (Tspi_PcrComposite_SetPcrLocality ( hPCR, locality ) != TSS_SUCCESS )
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|seal(): Error create PCR Object");
    }
    
    //Choose PCR's 
	for( pcr_index = 0; pcr_index < 24; pcr_index++) {
		if (Tspi_TPM_PcrRead (hTPM, pcr_index, &pcrvaluelength, &pcrvalue ) != TSS_SUCCESS )
		{
			Tspi_Context_FreeMemory(hContext, NULL);
			Tspi_Context_Close(hContext);
			throw TSSError("KeyronaTPM[TrouSerS]|seal(): Error can't read PCR value");
		}
		
		if (Tspi_PcrComposite_SetPcrValue (hPCR , pcr_index, pcrvaluelength, pcrvalue ) != TSS_SUCCESS )
		{
			Tspi_Context_FreeMemory(hContext, NULL);
			Tspi_Context_Close(hContext);
			throw TSSError("KeyronaTPM[TrouSerS]|seal(): Error can't set PCR value");
		}
		pcrvaluelength=NULL;
		pcrvalue=NULL;
		cout << "Added PCR" << pcr_index << " digest value to PCR Object for sealing" << endl;
	}

    // Data Seal
    TSS_RESULT error0 = Tspi_Data_Seal( hEncData, hSRK, dataToSeal.size(), (BYTE*) dataToSeal.toCArray(), hPCR );
    if ( error0 != TSS_SUCCESS )
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|seal(): Error sealing data. (" + getTSSError(error0) + ")");
    }

    if ( Tspi_GetAttribData(hEncData, TSS_TSPATTRIB_ENCDATA_BLOB, TSS_TSPATTRIB_ENCDATABLOB_BLOB, &encLen, &encData) != TSS_SUCCESS)
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|seal(): Error retrieving sealed data.");
    }

    debug << "KeyronaTPM[TrouSerS]|seal(): Successfully sealed data." << endl;
    ByteVector sealedData((UInt8*)encData, encLen);

    vector<ByteVector> myData;
    myData.push_back(sealedData);

    Tspi_Context_CloseObject(hContext, hEncData);
    Tspi_Context_FreeMemory(hContext, NULL);
    Tspi_Context_Close(hContext);

    ByteVector plain = unseal(myData);
    if (plain != dataToSeal)
        throw TSSError("KeyronaTPM[TrouSerS]|seal(): Unsealed data doesn't match sealed data.");

    return myData;
    
};

ByteVector KeyronaTPM::unseal(vector<ByteVector> &dataToUnseal)
{
    ByteVector   sealedData = dataToUnseal[0];
    TSS_HCONTEXT hContext;
    TSS_HTPM     hTPM;
    TSS_HKEY	 hSRK;
    TSS_HENCDATA hEncData;
    TSS_HPOLICY  hPolicy;
    TSS_UUID     SRK_UUID = TSS_UUID_SRK;
    BYTE         *plainData;
    BYTE         well_known_secret[TPM_SHA1_160_HASH_LEN] = TSS_WELL_KNOWN_SECRET;
    UINT32       plainLen;

    if (Tspi_Context_Create(&hContext) != TSS_SUCCESS)
        throw ContextError("KeyronaTPM[TrouSerS]|unseal(): Could not create context!");

    if (Tspi_Context_Connect(hContext, NULL) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw NoTCSD("KeyronaTPM[TrouSerS]|unseal(): Could not connect! Is TrouSerS 'tcsd' running?");
    }

    if (Tspi_Context_GetTpmObject(hContext, &hTPM) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TPMConnectError("KeyronaTPM[TrouSerS]|unseal(): Could not connect to TPM!");
    }

    // create hEncData
    if (Tspi_Context_CreateObject( hContext, TSS_OBJECT_TYPE_ENCDATA, TSS_ENCDATA_SEAL, &hEncData ) != TSS_SUCCESS )
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unseal(): Error creating data object.");
    }

    if ( Tspi_SetAttribData(hEncData, TSS_TSPATTRIB_ENCDATA_BLOB, TSS_TSPATTRIB_ENCDATABLOB_BLOB, sealedData.size(), (BYTE*) sealedData.toCArray()) != TSS_SUCCESS)
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unseal(): Error creating sealed data object.");
    }

    // Load SRK Key By UUID
    if (Tspi_Context_LoadKeyByUUID( hContext, TSS_PS_TYPE_SYSTEM, SRK_UUID, &hSRK ) != TSS_SUCCESS )
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unseal(): Error loading SRK key object.");
    }

    
    if ( Tspi_GetPolicyObject(hSRK, TSS_POLICY_USAGE, &hPolicy) != TSS_SUCCESS)
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unseal(): Error creating policy object!");
    }

    if (Tspi_Policy_SetSecret(hPolicy, TSS_SECRET_MODE_SHA1, TPM_SHA1_160_HASH_LEN, well_known_secret) != TSS_SUCCESS)
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unseal(): Error setting SRK secret!");
    }
    
    // Data Unseal
    TSS_RESULT error0 = Tspi_Data_Unseal(hEncData, hSRK, &plainLen, &plainData);
    if ( error0 != TSS_SUCCESS )
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unseal(): Error unsealing data. (" + getTSSError(error0) + ")");
    }

    debug << "KeyronaTPM[TrouSerS]|unseal(): Successfully unsealed data." << endl;
    ByteVector unsealedData((UInt8*)plainData, plainLen);

    Tspi_Context_CloseObject(hContext, hEncData);
    Tspi_Context_FreeMemory(hContext, NULL);
    Tspi_Context_Close(hContext);

    return unsealedData;
};

ByteVector KeyronaTPM::bind(ByteVector &dataToBind, UInt32 &bindkeynum)
{
	TSS_HCONTEXT hContext;
	TSS_HTPM	 hTPM;
    TSS_HENCDATA hEncData;
    TSS_HKEY	 hBindKey;
    TSS_HPOLICY  hBindKeyPolicy;
	UINT32 		 encLen;
	TSS_HKEY	 hSRK;
	TSS_UUID	 hBindKey_UUID;
    TSS_UUID     SRK_UUID = TSS_UUID_SRK;
	BYTE 		 *encData;
	
	memset (&hBindKey_UUID, 0, sizeof(hBindKey_UUID));
	hBindKey_UUID.rgbNode[5] = bindkeynum & 0xff;

    if (Tspi_Context_Create(&hContext) != TSS_SUCCESS)
        throw ContextError("KeyronaTPM[TrouSerS]|bind(): Could not create context!");

    if (Tspi_Context_Connect(hContext, NULL) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw NoTCSD("KeyronaTPM[TrouSerS]|bind(): Could not connect! Is TrouSerS 'tcsd' running?");
    }

    if (Tspi_Context_GetTpmObject(hContext, &hTPM) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TPMConnectError("KeyronaTPM[TrouSerS]|bind(): Could not connect to TPM!");
    }
    
    // create hEncData
    if (Tspi_Context_CreateObject( hContext, TSS_OBJECT_TYPE_ENCDATA, TSS_ENCDATA_BIND, &hEncData ) != TSS_SUCCESS )
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|bind(): Error creating data object.");
    }      
    // Load SRK by UUID
    if (Tspi_Context_LoadKeyByUUID( hContext, TSS_PS_TYPE_SYSTEM, SRK_UUID, &hSRK ) != TSS_SUCCESS )
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|bind(): Error loading SRK key object.");
    }
	// Get Key by UUID
	TSS_RESULT error0 = Tspi_Context_GetKeyByUUID( hContext, TSS_PS_TYPE_SYSTEM, hBindKey_UUID, &hBindKey);
	if( error0 != TSS_SUCCESS )
	{
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|bind(): Error get Key by UUID. (" + getTSSError(error0) + ")");
    }
	// Load Key by UUID
    TSS_RESULT error1 = Tspi_Key_LoadKey(hBindKey, hSRK);
	if( error1 != TSS_SUCCESS )
	{
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|bind(): Error load Key by UUID. (" + getTSSError(error1) + ")");
    }
	// Data Bind 
    TSS_RESULT error2 = Tspi_Data_Bind( hEncData, hBindKey, dataToBind.size(), (BYTE*) dataToBind.toCArray());
    if ( error2 != TSS_SUCCESS )
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|bind(): Error binding data. (" + getTSSError(error2) + ")");
    }

    if ( Tspi_GetAttribData(hEncData, TSS_TSPATTRIB_ENCDATA_BLOB, TSS_TSPATTRIB_ENCDATABLOB_BLOB, &encLen, &encData) != TSS_SUCCESS)
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|bind(): Error retrieving binded data.");
    }
    
    if (Tspi_Key_UnloadKey(hBindKey) != TSS_SUCCESS )
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|bind(): Error unload Bind Key.");
    }
    
    debug << "KeyronaTPM[TrouSerS]|bind(): Successfully binded data." << endl;
    ByteVector bindedData((UInt8*)encData, encLen);

    Tspi_Context_CloseObject(hContext, hEncData);
    Tspi_Context_FreeMemory(hContext, NULL);
    Tspi_Context_Close(hContext);
   
    return bindedData;
};

ByteVector KeyronaTPM::unbind(ByteVector &dataToUnbind, UInt32 &bindkeynum, string &password)
{
    
    TSS_HCONTEXT hContext;
    TSS_HTPM     hTPM;
    TSS_HKEY	 hBindKey;
    TSS_HKEY	 hSRK;
    TSS_HENCDATA hEncData;
    TSS_HPOLICY  hBindKeyPolicy;
    TSS_HPOLICY  hPolicy;
    TSS_UUID     hBindKey_UUID;
    TSS_UUID     SRK_UUID = TSS_UUID_SRK;
    BYTE         well_known_secret[TPM_SHA1_160_HASH_LEN] = TSS_WELL_KNOWN_SECRET;
    BYTE         *plainData;
    UINT32       plainLen;
	
	memset (&hBindKey_UUID, 0, sizeof(hBindKey_UUID));
	hBindKey_UUID.rgbNode[5] = bindkeynum & 0xff;
	
    if (Tspi_Context_Create(&hContext) != TSS_SUCCESS)
        throw ContextError("KeyronaTPM[TrouSerS]|unbind(): Could not create context!");

    if (Tspi_Context_Connect(hContext, NULL) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw NoTCSD("KeyronaTPM[TrouSerS]|unbind(): Could not connect! Is TrouSerS 'tcsd' running?");
    }

    if (Tspi_Context_GetTpmObject(hContext, &hTPM) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TPMConnectError("KeyronaTPM[TrouSerS]|unbind(): Could not connect to TPM!");
    }

    // create hEncData
    if (Tspi_Context_CreateObject( hContext, TSS_OBJECT_TYPE_ENCDATA, TSS_ENCDATA_BIND, &hEncData ) != TSS_SUCCESS )
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unbind(): Error creating data object.");
    }
    // Load SRK
    if (Tspi_Context_LoadKeyByUUID( hContext, TSS_PS_TYPE_SYSTEM, SRK_UUID, &hSRK ) != TSS_SUCCESS )
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unbind(): Error loading SRK key object.");
    }

    if ( Tspi_GetPolicyObject(hSRK, TSS_POLICY_USAGE, &hPolicy) != TSS_SUCCESS)
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unbind(): Error creating policy object!");
    }
	// Set SRK secret
    if (Tspi_Policy_SetSecret(hPolicy, TSS_SECRET_MODE_SHA1, TPM_SHA1_160_HASH_LEN, well_known_secret) != TSS_SUCCESS)
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unbind(): Error setting SRK secret!");
    }

    if ( Tspi_SetAttribData(hEncData, TSS_TSPATTRIB_ENCDATA_BLOB, TSS_TSPATTRIB_ENCDATABLOB_BLOB, dataToUnbind.size(), (BYTE*) dataToUnbind.toCArray()) != TSS_SUCCESS)
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unbind(): Error creating binded data object.");
    }
	// Get Key by UUID
    TSS_RESULT error0 = Tspi_Context_GetKeyByUUID( hContext, TSS_PS_TYPE_SYSTEM, hBindKey_UUID, &hBindKey);
	if( error0 != TSS_SUCCESS )
	{
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unbind(): Error get Key by UUID. (" + getTSSError(error0) + ")");
    }
    
    if (Tspi_GetPolicyObject(hBindKey, TSS_POLICY_USAGE, &hBindKeyPolicy) != TSS_SUCCESS )
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unbind(): Error loading policy object.");
    }

    // SetPolicySecret
    if (Tspi_Policy_SetSecret( hBindKeyPolicy, TSS_SECRET_MODE_PLAIN, password.length(), (BYTE*)password.c_str()  ) != TSS_SUCCESS )
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unbind(): Error set policy secret.");
    }
	// Load Key
    TSS_RESULT error1 = Tspi_Key_LoadKey(hBindKey, hSRK);
	if( error1 != TSS_SUCCESS )
	{
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unbind(): Error loading Key. (" + getTSSError(error1) + ")");
    }
	// Data Unbind
	TSS_RESULT error2 = Tspi_Data_Unbind(hEncData, hBindKey, &plainLen, &plainData);
    if ( error2 != TSS_SUCCESS )
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unbind(): Error unbind data. (" + getTSSError(error2) + ")");
    }
    
    if (Tspi_Key_UnloadKey(hBindKey) != TSS_SUCCESS )
    {
        Tspi_Context_CloseObject(hContext, hEncData);
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unbind(): Error unload Key.");
    }

    debug << "KeyronaTPM[TrouSerS]|unbind(): Successfully unbinded data." << endl;
    ByteVector unbindedData((UInt8*)plainData, plainLen);

    Tspi_Context_CloseObject(hContext, hEncData);
    Tspi_Context_FreeMemory(hContext, NULL);
    Tspi_Context_Close(hContext);

    return unbindedData;
};

ByteVector KeyronaTPM::create_key(string &password, UInt32 &keynum, string &type)
{
    TSS_HCONTEXT hContext;
	TSS_HTPM	 hTPM;
    TSS_HKEY	 hKey;
    TSS_HPOLICY  hKeyPolicy;
    TSS_HPOLICY  hPolicy;
	TSS_HKEY	 hSRK;
	TSS_UUID	 hKey_UUID;
    TSS_UUID     SRK_UUID = TSS_UUID_SRK;
    BYTE		 *blob;
    BYTE         well_known_secret[TPM_SHA1_160_HASH_LEN] = TSS_WELL_KNOWN_SECRET;
    UINT32 		 blobLen;
    UINT32		 flags = NULL;

    //Choose between Bind, Storage or Legacy Key. 
    if(type == "subject") 
    {
		flags = TSS_KEY_TYPE_BIND | TSS_KEY_STRUCT_KEY12 | TSS_KEY_SIZE_2048 | TSS_KEY_NON_VOLATILE | TSS_KEY_NOT_MIGRATABLE | TSS_KEY_AUTHORIZATION;
	}
	if( type == "group" ) 
	{				
		flags = TSS_KEY_TYPE_STORAGE | TSS_KEY_STRUCT_KEY12 | TSS_KEY_SIZE_2048 | TSS_KEY_NON_VOLATILE | TSS_KEY_NOT_MIGRATABLE;
	}

	// Allocate Key UUID number.
	memset (&hKey_UUID, 0, sizeof(hKey_UUID));
	hKey_UUID.rgbNode[5] = keynum & 0xff;

    if (Tspi_Context_Create(&hContext) != TSS_SUCCESS)
        throw ContextError("KeyronaTPM[TrouSerS]|create_key(): Could not create context!");

    if (Tspi_Context_Connect(hContext, NULL) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw NoTCSD("KeyronaTPM[TrouSerS]|create_key(): Could not connect! Is TrouSerS 'tcsd' running?");
    }

    if (Tspi_Context_GetTpmObject(hContext, &hTPM) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TPMConnectError("KeyronaTPM[TrouSerS]|create_key(): Could not connect to TPM!");
    }
    
	TSS_RESULT error0 = Tspi_Context_LoadKeyByUUID( hContext, TSS_PS_TYPE_SYSTEM, SRK_UUID, &hSRK );
	if( error0 != TSS_SUCCESS )
	{
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|create_key(): Error load Key by UUID. (" + getTSSError(error0) + ")");
    }

    if ( Tspi_GetPolicyObject(hSRK, TSS_POLICY_USAGE, &hPolicy) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|create_key(): Error creating policy object!");
    }

    TSS_RESULT error1 = Tspi_Policy_SetSecret(hPolicy, TSS_SECRET_MODE_SHA1, TPM_SHA1_160_HASH_LEN, well_known_secret);
	if( error1 != TSS_SUCCESS )
	{
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|create_key(): Error set policy secret. (" + getTSSError(error1) + ")");
    }
    
    if (Tspi_Context_CreateObject( hContext, TSS_OBJECT_TYPE_RSAKEY, flags, &hKey ) != TSS_SUCCESS )
	{
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
       throw TSSError("KeyronaTPM[TrouSerS]|create_key(): Error create Key object.");
    }
	
	if (Tspi_SetAttribUint32(hKey, TSS_TSPATTRIB_KEY_INFO, TSS_TSPATTRIB_KEYINFO_ENCSCHEME, TSS_ES_RSAESOAEP_SHA1_MGF1) != TSS_SUCCESS )
	{
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|create_key(): Error set attributes.");
    }
	
    if (Tspi_Context_CreateObject( hContext, TSS_OBJECT_TYPE_POLICY, TSS_POLICY_USAGE, &hKeyPolicy ) != TSS_SUCCESS )
	{
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|create_key(): Error create policy object.");
    }
	
	if (Tspi_Policy_AssignToObject( hKeyPolicy, hKey ) != TSS_SUCCESS )
	{
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|create_key(): Error assign policy Object.");
    }

    // SetPolicySecret
    if (Tspi_Policy_SetSecret( hKeyPolicy, TSS_SECRET_MODE_PLAIN, password.length(), (BYTE*)password.c_str()  ) != TSS_SUCCESS )
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|create_key(): Error set policy secret.");
    }
	
    TSS_RESULT error2 = Tspi_Key_CreateKey(hKey, hSRK, NULL);
	if( error2 != TSS_SUCCESS )
	{
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|create_key(): Error create key. (" + getTSSError(error2) + ")");
    }
    
    TSS_RESULT error4 = Tspi_Context_RegisterKey(hContext, hKey, TSS_PS_TYPE_SYSTEM, hKey_UUID, TSS_PS_TYPE_SYSTEM, SRK_UUID);
    if( error4 != TSS_SUCCESS )
	{
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|create_key(): Error register key. (" + getTSSError(error4) + ")");
    }

/*
	Tspi_Key_LoadKey(hKey, hSRK);
	
	Tspi_GetAttribData (hKey, TSS_TSPATTRIB_KEY_INFO, NULL, &blobLen, &blob);
		
	Tspi_Key_UnloadKey(hKey);
*/	
	
	ByteVector keyData;
	
    Tspi_Context_FreeMemory(hContext, NULL);
    Tspi_Context_Close(hContext);

	return keyData;
};

ByteVector KeyronaTPM::get_keys_by_uuid()
{
	TSS_HCONTEXT hContext;
	TSS_HTPM	 hTPM;
	UINT32 		 keyblobsize;
	TSS_KM_KEYINFO2 *keyblob;
	
	
	if (Tspi_Context_Create(&hContext) != TSS_SUCCESS)
		throw ContextError("KeyronaTPM[TrouSerS]|get_keys_by_uuid(): Could not create context!");

    if (Tspi_Context_Connect(hContext, NULL) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw NoTCSD("KeyronaTPM[TrouSerS]|get_keys_by_uuid(): Could not connect! Is TrouSerS 'tcsd' running?");
    }

    if (Tspi_Context_GetTpmObject(hContext, &hTPM) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TPMConnectError("KeyronaTPM[TrouSerS]|get_keys_by_uuid(): Could not connect to TPM!");
    }
    
    TSS_RESULT error0 = Tspi_Context_GetRegisteredKeysByUUID2(hContext, TSS_PS_TYPE_SYSTEM, NULL, &keyblobsize, &keyblob);
	if ( error0 != TSS_SUCCESS )
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|get_keys_by_uuid(): Error get registered keys by UUID. (" + getTSSError(error0) + ")");
    }
    
    	for( UInt32 i = 0; i<keyblobsize; i++) {
printf("Version      : %hhu.%hhu.%hhu.%hhu\n", keyblob[i].versionInfo.bMajor, keyblob[i].versionInfo.bMinor,
	       keyblob[i].versionInfo.bRevMajor, keyblob[i].versionInfo.bRevMinor);
	cout << (keyblob[i].keyUUID.rgbNode[5] & 0xff) << endl;
	printf("parent UUID  : %08x-%04hx-%04hx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx\n",
	       keyblob[i].parentKeyUUID.ulTimeLow, keyblob[i].parentKeyUUID.usTimeMid, keyblob[i].parentKeyUUID.usTimeHigh,
	       keyblob[i].parentKeyUUID.bClockSeqHigh, keyblob[i].parentKeyUUID.bClockSeqLow,
	       keyblob[i].parentKeyUUID.rgbNode[0] & 0xff, keyblob[i].parentKeyUUID.rgbNode[1] & 0xff,
	       keyblob[i].parentKeyUUID.rgbNode[2] & 0xff, keyblob[i].parentKeyUUID.rgbNode[3] & 0xff,
	       keyblob[i].parentKeyUUID.rgbNode[4] & 0xff, keyblob[i].parentKeyUUID.rgbNode[5] & 0xff);
	printf("Auth?        : %s\n", keyblob[i].bAuthDataUsage ? "YES" : "NO");

	if	(keyblob[i].persistentStorageType == TSS_PS_TYPE_SYSTEM)	printf("Store        : System\n");
	else if (keyblob[i].persistentStorageType == TSS_PS_TYPE_USER)		printf("Store        : User\n");
	else 								printf("Store        : Error\n");

	if	(keyblob[i].persistentStorageTypeParent == TSS_PS_TYPE_SYSTEM)	printf("Parent Store : System\n");
	else if	(keyblob[i].persistentStorageTypeParent == TSS_PS_TYPE_USER)	printf("Parent Store : User\n");
	else								printf("Parent Store : Error\n");

	if	(keyblob[i].ulVendorDataLength)					printf("Vendor Data  : \"%s\" (%u bytes)\n", keyblob[i].rgbVendorData, keyblob[i].ulVendorDataLength);
	else								printf("Vendor Data  : (0 bytes)\n");

	/* custom additions below */
	printf("Loaded?      : %s\n", keyblob[i].fIsLoaded ? "YES" : "NO");
	printf("\n");	       	
	}
    
    ByteVector keydata((UInt8*)keyblob, keyblobsize);
    
	Tspi_Context_FreeMemory (hContext, (BYTE*)keyblob);
    Tspi_Context_FreeMemory(hContext, NULL);
    Tspi_Context_Close(hContext);
    
    return keydata;
};

void KeyronaTPM::change_key_auth(string &password, string &password_old, UInt32 &keynum)
{
	TSS_HCONTEXT hContext;
	TSS_HTPM	 hTPM;
    TSS_HKEY	 hKey;
    TSS_HKEY	 hSRK;
    TSS_HPOLICY  hPolicy;
    TSS_HPOLICY	 hKeyPolicy;
    TSS_HPOLICY  hKeyPolicyOld;
	TSS_UUID	 hKey_UUID;
	TSS_UUID     SRK_UUID = TSS_UUID_SRK;
	BYTE well_known_secret[TPM_SHA1_160_HASH_LEN] = TSS_WELL_KNOWN_SECRET;

	memset (&hKey_UUID, 0, sizeof(hKey_UUID));
	hKey_UUID.rgbNode[5] = keynum & 0xff;
	
	if (Tspi_Context_Create(&hContext) != TSS_SUCCESS)
    throw ContextError("KeyronaTPM[TrouSerS]|unseal(): Could not create context!");

    if (Tspi_Context_Connect(hContext, NULL) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw NoTCSD("KeyronaTPM[TrouSerS]|unseal(): Could not connect! Is TrouSerS 'tcsd' running?");
    }

    if (Tspi_Context_GetTpmObject(hContext, &hTPM) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TPMConnectError("KeyronaTPM[TrouSerS]|unseal(): Could not connect to TPM!");
    }
    
    if (Tspi_Context_CreateObject( hContext, TSS_OBJECT_TYPE_POLICY, TSS_POLICY_USAGE, &hKeyPolicy ) != TSS_SUCCESS )
	{
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|seal(): Error create AIK policy object.");
    }
    
	// Load SRK by UUID
    if (Tspi_Context_LoadKeyByUUID( hContext, TSS_PS_TYPE_SYSTEM, SRK_UUID, &hSRK ) != TSS_SUCCESS )
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unseal(): Error loading SRK key object.");
    }
    
    if ( Tspi_GetPolicyObject(hSRK, TSS_POLICY_USAGE, &hPolicy) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unseal(): Error creating policy object!");
    }
	// Set SRK secret
    if (Tspi_Policy_SetSecret(hPolicy, TSS_SECRET_MODE_SHA1, TPM_SHA1_160_HASH_LEN, well_known_secret) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|unseal(): Error setting SRK secret!");
    }
    
	// Get Key by UUID
	TSS_RESULT boo = Tspi_Context_GetKeyByUUID( hContext, TSS_PS_TYPE_SYSTEM, hKey_UUID, &hKey);
	if( boo != TSS_SUCCESS )
	{
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|seal(): Error binding data. (" + getTSSError(boo) + ")");
    }

    if (Tspi_GetPolicyObject(hKey, TSS_POLICY_USAGE, &hKeyPolicyOld) != TSS_SUCCESS )
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|seal(): Error loading TPM policy object.");
    }

    // SetPolicySecret
    if (Tspi_Policy_SetSecret( hKeyPolicyOld, TSS_SECRET_MODE_PLAIN, password_old.length(), (BYTE*)password_old.c_str()  ) != TSS_SUCCESS )
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|seal(): Error loading TPM policy object.");
    }
    
    // SetPolicySecret
    if (Tspi_Policy_SetSecret( hKeyPolicy, TSS_SECRET_MODE_PLAIN, password.length(), (BYTE*)password.c_str()  ) != TSS_SUCCESS )
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|seal(): Error loading TPM policy object.");
    }
    
    Tspi_Key_LoadKey(hKey, hSRK);
        
    TSS_RESULT result = Tspi_ChangeAuth( hKey, hSRK, hKeyPolicy );
    if ( result != TSS_SUCCESS )
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TSSError("KeyronaTPM[TrouSerS]|seal(): Error sealing data. (" + getTSSError(result) + ")");
    }
    
    Tspi_Context_FreeMemory(hContext, NULL);
    Tspi_Context_Close(hContext);
};

void KeyronaTPM::delete_key(UInt32 &keynum)
{
	
	TSS_HCONTEXT hContext;
	TSS_HTPM	 hTPM;
    TSS_HKEY	 hKey;
	TSS_UUID	 hKey_UUID;

	memset (&hKey_UUID, 0, sizeof(hKey_UUID));
	hKey_UUID.rgbNode[5] = keynum & 0xff;
	
	if (Tspi_Context_Create(&hContext) != TSS_SUCCESS)
    throw ContextError("KeyronaTPM[TrouSerS]|unseal(): Could not create context!");

    if (Tspi_Context_Connect(hContext, NULL) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw NoTCSD("KeyronaTPM[TrouSerS]|unseal(): Could not connect! Is TrouSerS 'tcsd' running?");
    }

    if (Tspi_Context_GetTpmObject(hContext, &hTPM) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TPMConnectError("KeyronaTPM[TrouSerS]|unseal(): Could not connect to TPM!");
    }
	
	Tspi_Context_UnregisterKey(hContext, TSS_PS_TYPE_SYSTEM, hKey_UUID, &hKey);
    
    Tspi_Context_FreeMemory(hContext, NULL);
    Tspi_Context_Close(hContext);	
};

ByteVector KeyronaTPM::getRandom(UInt8 amountOfRandomBytes)
{
    if (! amountOfRandomBytes)
        throw TSSError("KeyronaTPM[TrouSerS]|getRandom(): Invalid amount of bytes requested!");

    BYTE		*random;
    TSS_HCONTEXT	hContext;
    TSS_HTPM		hTPM;

    if (Tspi_Context_Create(&hContext) != TSS_SUCCESS)
        throw ContextError("KeyronaTPM[TrouSerS]|getRandom(): Could not create context!");

    if (Tspi_Context_Connect(hContext, NULL) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw NoTCSD("KeyronaTPM[TrouSerS]|getRandom(): Could not connect! Is TrouSerS 'tcsd' running?");
    }

    if (Tspi_Context_GetTpmObject(hContext, &hTPM) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TPMConnectError("KeyronaTPM[TrouSerS]|getRandom(): Could not connect to TPM!");
    }

    // Get random numbers
    if (Tspi_TPM_GetRandom( hTPM, amountOfRandomBytes, &random ) != TSS_SUCCESS)
    {
        Tspi_Context_FreeMemory(hContext, NULL);
        Tspi_Context_Close(hContext);
        throw TPMConnectError("KeyronaTPM[TrouSerS]|getRandom(): Could not retrieve random numbers from TPM!");
    }

    debug << "KeyronaTPM[TrouSerS]|getRandom(): Successfully received random numbers." << endl;
    ByteVector randomNumbers((UInt8*)random, amountOfRandomBytes);

    Tspi_Context_FreeMemory(hContext, NULL);
    Tspi_Context_Close(hContext);

    return randomNumbers;
};

string getTSSError(TSS_RESULT r)
{
	/* Check the return code to see if it is common to all layers.
	 * If so, return it.
	 */
	switch (TSS_ERROR_CODE(r)) {
		case TSS_SUCCESS:			return "TSS_SUCCESS";
		default:
			break;
	}

	/* The return code is either unknown, or specific to a layer */
	if (TSS_ERROR_LAYER(r) == TSS_LAYER_TPM) {
		switch (TSS_ERROR_CODE(r)) {
			case TPM_E_AUTHFAIL:			return "TPM_E_AUTHFAIL";
			case TPM_E_BAD_PARAMETER:		return "TPM_E_BAD_PARAMETER";
			case TPM_E_BADINDEX:			return "TPM_E_BADINDEX";
			case TPM_E_AUDITFAILURE:		return "TPM_E_AUDITFAILURE";
			case TPM_E_CLEAR_DISABLED:		return "TPM_E_CLEAR_DISABLED";
			case TPM_E_DEACTIVATED:			return "TPM_E_DEACTIVATED";
			case TPM_E_DISABLED:			return "TPM_E_DISABLED";
			case TPM_E_FAIL:			return "TPM_E_FAIL";
			case TPM_E_BAD_ORDINAL:			return "TPM_E_BAD_ORDINAL";
			case TPM_E_INSTALL_DISABLED:		return "TPM_E_INSTALL_DISABLED";
			case TPM_E_INVALID_KEYHANDLE:		return "TPM_E_INVALID_KEYHANDLE";
			case TPM_E_KEYNOTFOUND:			return "TPM_E_KEYNOTFOUND";
			case TPM_E_INAPPROPRIATE_ENC:		return "TPM_E_INAPPROPRIATE_ENC";
			case TPM_E_MIGRATEFAIL:			return "TPM_E_MIGRATEFAIL";
			case TPM_E_INVALID_PCR_INFO:		return "TPM_E_INVALID_PCR_INFO";
			case TPM_E_NOSPACE:			return "TPM_E_NOSPACE";
			case TPM_E_NOSRK:			return "TPM_E_NOSRK";
			case TPM_E_NOTSEALED_BLOB:		return "TPM_E_NOTSEALED_BLOB";
			case TPM_E_OWNER_SET:			return "TPM_E_OWNER_SET";
			case TPM_E_RESOURCES:			return "TPM_E_RESOURCES";
			case TPM_E_SHORTRANDOM:			return "TPM_E_SHORTRANDOM";
			case TPM_E_SIZE:			return "TPM_E_SIZE";
			case TPM_E_WRONGPCRVAL:			return "TPM_E_WRONGPCRVAL";
			case TPM_E_BAD_PARAM_SIZE:		return "TPM_E_BAD_PARAM_SIZE";
			case TPM_E_SHA_THREAD:			return "TPM_E_SHA_THREAD";
			case TPM_E_SHA_ERROR:			return "TPM_E_SHA_ERROR";
			case TPM_E_FAILEDSELFTEST:		return "TPM_E_FAILEDSELFTEST";
			case TPM_E_AUTH2FAIL:			return "TPM_E_AUTH2FAIL";
			case TPM_E_BADTAG:			return "TPM_E_BADTAG";
			case TPM_E_IOERROR:			return "TPM_E_IOERROR";
			case TPM_E_ENCRYPT_ERROR:		return "TPM_E_ENCRYPT_ERROR";
			case TPM_E_DECRYPT_ERROR:		return "TPM_E_DECRYPT_ERROR";
			case TPM_E_INVALID_AUTHHANDLE:		return "TPM_E_INVALID_AUTHHANDLE";
			case TPM_E_NO_ENDORSEMENT:		return "TPM_E_NO_ENDORSEMENT";
			case TPM_E_INVALID_KEYUSAGE:		return "TPM_E_INVALID_KEYUSAGE";
			case TPM_E_WRONG_ENTITYTYPE:		return "TPM_E_WRONG_ENTITYTYPE";
			case TPM_E_INVALID_POSTINIT:		return "TPM_E_INVALID_POSTINIT";
			case TPM_E_INAPPROPRIATE_SIG:		return "TPM_E_INAPPROPRIATE_SIG";
			case TPM_E_BAD_KEY_PROPERTY:		return "TPM_E_BAD_KEY_PROPERTY";
			case TPM_E_BAD_MIGRATION:		return "TPM_E_BAD_MIGRATION";
			case TPM_E_BAD_SCHEME:			return "TPM_E_BAD_SCHEME";
			case TPM_E_BAD_DATASIZE:		return "TPM_E_BAD_DATASIZE";
			case TPM_E_BAD_MODE:			return "TPM_E_BAD_MODE";
			case TPM_E_BAD_PRESENCE:		return "TPM_E_BAD_PRESENCE";
			case TPM_E_BAD_VERSION:			return "TPM_E_BAD_VERSION";
			case TPM_E_NO_WRAP_TRANSPORT:		return "TPM_E_NO_WRAP_TRANSPORT";
			case TPM_E_AUDITFAIL_UNSUCCESSFUL:	return "TPM_E_AUDITFAIL_UNSUCCESSFUL";
			case TPM_E_AUDITFAIL_SUCCESSFUL:	return "TPM_E_AUDITFAIL_SUCCESSFUL";
			case TPM_E_NOTRESETABLE:		return "TPM_E_NOTRESETABLE";
			case TPM_E_NOTLOCAL:			return "TPM_E_NOTLOCAL";
			case TPM_E_BAD_TYPE:			return "TPM_E_BAD_TYPE";
			case TPM_E_INVALID_RESOURCE:		return "TPM_E_INVALID_RESOURCE";
			case TPM_E_NOTFIPS:			return "TPM_E_NOTFIPS";
			case TPM_E_INVALID_FAMILY:		return "TPM_E_INVALID_FAMILY";
			case TPM_E_NO_NV_PERMISSION:		return "TPM_E_NO_NV_PERMISSION";
			case TPM_E_REQUIRES_SIGN:		return "TPM_E_REQUIRES_SIGN";
			case TPM_E_KEY_NOTSUPPORTED:		return "TPM_E_KEY_NOTSUPPORTED";
			case TPM_E_AUTH_CONFLICT:		return "TPM_E_AUTH_CONFLICT";
			case TPM_E_AREA_LOCKED:			return "TPM_E_AREA_LOCKED";
			case TPM_E_BAD_LOCALITY:		return "TPM_E_BAD_LOCALITY";
			case TPM_E_READ_ONLY:			return "TPM_E_READ_ONLY";
			case TPM_E_PER_NOWRITE:			return "TPM_E_PER_NOWRITE";
			case TPM_E_FAMILYCOUNT:			return "TPM_E_FAMILYCOUNT";
			case TPM_E_WRITE_LOCKED:		return "TPM_E_WRITE_LOCKED";
			case TPM_E_BAD_ATTRIBUTES:		return "TPM_E_BAD_ATTRIBUTES";
			case TPM_E_INVALID_STRUCTURE:		return "TPM_E_INVALID_STRUCTURE";
			case TPM_E_KEY_OWNER_CONTROL:		return "TPM_E_KEY_OWNER_CONTROL";
			case TPM_E_BAD_COUNTER:			return "TPM_E_BAD_COUNTER";
			case TPM_E_NOT_FULLWRITE:		return "TPM_E_NOT_FULLWRITE";
			case TPM_E_CONTEXT_GAP:			return "TPM_E_CONTEXT_GAP";
			case TPM_E_MAXNVWRITES:			return "TPM_E_MAXNVWRITES";
			case TPM_E_NOOPERATOR:			return "TPM_E_NOOPERATOR";
			case TPM_E_RESOURCEMISSING:		return "TPM_E_RESOURCEMISSING";
			case TPM_E_DELEGATE_LOCK:		return "TPM_E_DELEGATE_LOCK";
			case TPM_E_DELEGATE_FAMILY:		return "TPM_E_DELEGATE_FAMILY";
			case TPM_E_DELEGATE_ADMIN:		return "TPM_E_DELEGATE_ADMIN";
			case TPM_E_TRANSPORT_NOTEXCLUSIVE:	return "TPM_E_TRANSPORT_NOTEXCLUSIVE";
			case TPM_E_OWNER_CONTROL:		return "TPM_E_OWNER_CONTROL";
			case TPM_E_DAA_RESOURCES:		return "TPM_E_DAA_RESOURCES";
			case TPM_E_DAA_INPUT_DATA0:		return "TPM_E_DAA_INPUT_DATA0";
			case TPM_E_DAA_INPUT_DATA1:		return "TPM_E_DAA_INPUT_DATA1";
			case TPM_E_DAA_ISSUER_SETTINGS:		return "TPM_E_DAA_ISSUER_SETTINGS";
			case TPM_E_DAA_TPM_SETTINGS:		return "TPM_E_DAA_TPM_SETTINGS";
			case TPM_E_DAA_STAGE:			return "TPM_E_DAA_STAGE";
			case TPM_E_DAA_ISSUER_VALIDITY:		return "TPM_E_DAA_ISSUER_VALIDITY";
			case TPM_E_DAA_WRONG_W:			return "TPM_E_DAA_WRONG_W";
			case TPM_E_BAD_HANDLE:			return "TPM_E_BAD_HANDLE";
			case TPM_E_BAD_DELEGATE:		return "TPM_E_BAD_DELEGATE";
			case TPM_E_BADCONTEXT:			return "TPM_E_BADCONTEXT";
			case TPM_E_TOOMANYCONTEXTS:		return "TPM_E_TOOMANYCONTEXTS";
			case TPM_E_MA_TICKET_SIGNATURE:		return "TPM_E_MA_TICKET_SIGNATURE";
			case TPM_E_MA_DESTINATION:		return "TPM_E_MA_DESTINATION";
			case TPM_E_MA_SOURCE:			return "TPM_E_MA_SOURCE";
			case TPM_E_MA_AUTHORITY:		return "TPM_E_MA_AUTHORITY";
			case TPM_E_PERMANENTEK:			return "TPM_E_PERMANENTEK";
			case TPM_E_BAD_SIGNATURE:		return "TPM_E_BAD_SIGNATURE";
			case TPM_E_NOCONTEXTSPACE:		return "TPM_E_NOCONTEXTSPACE";
			case TPM_E_RETRY:			return "TPM_E_RETRY";
			case TPM_E_NEEDS_SELFTEST:		return "TPM_E_NEEDS_SELFTEST";
			case TPM_E_DOING_SELFTEST:		return "TPM_E_DOING_SELFTEST";
			case TPM_E_DEFEND_LOCK_RUNNING:		return "TPM_E_DEFEND_LOCK_RUNNING";
			case TPM_E_DISABLED_CMD:		return "TPM_E_DISABLED_CMD";
			default:				return "UNKNOWN TPM ERROR";
		}
	} else if (TSS_ERROR_LAYER(r) == TSS_LAYER_TDDL) {
		switch (TSS_ERROR_CODE(r)) {
			case TSS_E_FAIL:			return "TSS_E_FAIL";
			case TSS_E_BAD_PARAMETER:		return "TSS_E_BAD_PARAMETER";
			case TSS_E_INTERNAL_ERROR:		return "TSS_E_INTERNAL_ERROR";
			case TSS_E_NOTIMPL:			return "TSS_E_NOTIMPL";
			case TSS_E_PS_KEY_NOTFOUND:		return "TSS_E_PS_KEY_NOTFOUND";
			case TSS_E_KEY_ALREADY_REGISTERED:	return "TSS_E_KEY_ALREADY_REGISTERED";
			case TSS_E_CANCELED:			return "TSS_E_CANCELED";
			case TSS_E_TIMEOUT:			return "TSS_E_TIMEOUT";
			case TSS_E_OUTOFMEMORY:			return "TSS_E_OUTOFMEMORY";
			case TSS_E_TPM_UNEXPECTED:		return "TSS_E_TPM_UNEXPECTED";
			case TSS_E_COMM_FAILURE:		return "TSS_E_COMM_FAILURE";
			case TSS_E_TPM_UNSUPPORTED_FEATURE:	return "TSS_E_TPM_UNSUPPORTED_FEATURE";
			case TDDL_E_COMPONENT_NOT_FOUND:	return "TDDL_E_COMPONENT_NOT_FOUND";
			case TDDL_E_ALREADY_OPENED:		return "TDDL_E_ALREADY_OPENED";
			case TDDL_E_BADTAG:			return "TDDL_E_BADTAG";
			case TDDL_E_INSUFFICIENT_BUFFER:	return "TDDL_E_INSUFFICIENT_BUFFER";
			case TDDL_E_COMMAND_COMPLETED:		return "TDDL_E_COMMAND_COMPLETED";
			case TDDL_E_COMMAND_ABORTED:		return "TDDL_E_COMMAND_ABORTED";
			case TDDL_E_ALREADY_CLOSED:		return "TDDL_E_ALREADY_CLOSED";
			case TDDL_E_IOERROR:			return "TDDL_E_IOERROR";
			default:				return "UNKNOWN TDDL ERROR";
		}
	} else if (TSS_ERROR_LAYER(r) == TSS_LAYER_TCS) {
		switch (TSS_ERROR_CODE(r)) {
			case TSS_E_FAIL:			return "TSS_E_FAIL";
			case TSS_E_BAD_PARAMETER:		return "TCS_E_BAD_PARAMETER";
			case TSS_E_INTERNAL_ERROR:		return "TCS_E_INTERNAL_ERROR";
			case TSS_E_NOTIMPL:			return "TCS_E_NOTIMPL";
			case TSS_E_PS_KEY_NOTFOUND:		return "TSS_E_PS_KEY_NOTFOUND";
			case TSS_E_KEY_ALREADY_REGISTERED:	return "TCS_E_KEY_ALREADY_REGISTERED";
			case TSS_E_CANCELED:			return "TSS_E_CANCELED";
			case TSS_E_TIMEOUT:			return "TSS_E_TIMEOUT";
			case TSS_E_OUTOFMEMORY:			return "TCS_E_OUTOFMEMORY";
			case TSS_E_TPM_UNEXPECTED:		return "TSS_E_TPM_UNEXPECTED";
			case TSS_E_COMM_FAILURE:		return "TSS_E_COMM_FAILURE";
			case TSS_E_TPM_UNSUPPORTED_FEATURE:	return "TSS_E_TPM_UNSUPPORTED_FEATURE";
			case TCS_E_KEY_MISMATCH:		return "TCS_E_KEY_MISMATCH";
			case TCS_E_KM_LOADFAILED:		return "TCS_E_KM_LOADFAILED";
			case TCS_E_KEY_CONTEXT_RELOAD:		return "TCS_E_KEY_CONTEXT_RELOAD";
			case TCS_E_BAD_INDEX:			return "TCS_E_BAD_INDEX";
			case TCS_E_INVALID_CONTEXTHANDLE:	return "TCS_E_INVALID_CONTEXTHANDLE";
			case TCS_E_INVALID_KEYHANDLE:		return "TCS_E_INVALID_KEYHANDLE";
			case TCS_E_INVALID_AUTHHANDLE:		return "TCS_E_INVALID_AUTHHANDLE";
			case TCS_E_INVALID_AUTHSESSION:		return "TCS_E_INVALID_AUTHSESSION";
			case TCS_E_INVALID_KEY:			return "TCS_E_INVALID_KEY";
			default:				return "UNKNOWN TCS ERROR";
		}
	} else {
		switch (TSS_ERROR_CODE(r)) {
			case TSS_E_FAIL:			return "TSS_E_FAIL";
			case TSS_E_BAD_PARAMETER:		return "TSS_E_BAD_PARAMETER";
			case TSS_E_INTERNAL_ERROR:		return "TSS_E_INTERNAL_ERROR";
			case TSS_E_NOTIMPL:			return "TSS_E_NOTIMPL";
			case TSS_E_PS_KEY_NOTFOUND:		return "TSS_E_PS_KEY_NOTFOUND";
			case TSS_E_KEY_ALREADY_REGISTERED:	return "TSS_E_KEY_ALREADY_REGISTERED";
			case TSS_E_CANCELED:			return "TSS_E_CANCELED";
			case TSS_E_TIMEOUT:			return "TSS_E_TIMEOUT";
			case TSS_E_OUTOFMEMORY:			return "TSS_E_OUTOFMEMORY";
			case TSS_E_TPM_UNEXPECTED:		return "TSS_E_TPM_UNEXPECTED";
			case TSS_E_COMM_FAILURE:		return "TSS_E_COMM_FAILURE";
			case TSS_E_TPM_UNSUPPORTED_FEATURE:	return "TSS_E_TPM_UNSUPPORTED_FEATURE";
			case TSS_E_INVALID_OBJECT_TYPE:		return "TSS_E_INVALID_OBJECT_TYPE";
			case TSS_E_INVALID_OBJECT_INITFLAG:	return "TSS_E_INVALID_OBJECT_INITFLAG";
			case TSS_E_INVALID_HANDLE:		return "TSS_E_INVALID_HANDLE";
			case TSS_E_NO_CONNECTION:		return "TSS_E_NO_CONNECTION";
			case TSS_E_CONNECTION_FAILED:		return "TSS_E_CONNECTION_FAILED";
			case TSS_E_CONNECTION_BROKEN:		return "TSS_E_CONNECTION_BROKEN";
			case TSS_E_HASH_INVALID_ALG:		return "TSS_E_HASH_INVALID_ALG";
			case TSS_E_HASH_INVALID_LENGTH:		return "TSS_E_HASH_INVALID_LENGTH";
			case TSS_E_HASH_NO_DATA:		return "TSS_E_HASH_NO_DATA";
			case TSS_E_SILENT_CONTEXT:		return "TSS_E_SILENT_CONTEXT";
			case TSS_E_INVALID_ATTRIB_FLAG:		return "TSS_E_INVALID_ATTRIB_FLAG";
			case TSS_E_INVALID_ATTRIB_SUBFLAG:	return "TSS_E_INVALID_ATTRIB_SUBFLAG";
			case TSS_E_INVALID_ATTRIB_DATA:		return "TSS_E_INVALID_ATTRIB_DATA";
			case TSS_E_NO_PCRS_SET:			return "TSS_E_NO_PCRS_SET";
			case TSS_E_KEY_NOT_LOADED:		return "TSS_E_KEY_NOT_LOADED";
			case TSS_E_KEY_NOT_SET:			return "TSS_E_KEY_NOT_SET";
			case TSS_E_VALIDATION_FAILED:		return "TSS_E_VALIDATION_FAILED";
			case TSS_E_TSP_AUTHREQUIRED:		return "TSS_E_TSP_AUTHREQUIRED";
			case TSS_E_TSP_AUTH2REQUIRED:		return "TSS_E_TSP_AUTH2REQUIRED";
			case TSS_E_TSP_AUTHFAIL:		return "TSS_E_TSP_AUTHFAIL";
			case TSS_E_TSP_AUTH2FAIL:		return "TSS_E_TSP_AUTH2FAIL";
			case TSS_E_KEY_NO_MIGRATION_POLICY:	return "TSS_E_KEY_NO_MIGRATION_POLICY";
			case TSS_E_POLICY_NO_SECRET:		return "TSS_E_POLICY_NO_SECRET";
			case TSS_E_INVALID_OBJ_ACCESS:		return "TSS_E_INVALID_OBJ_ACCESS";
			case TSS_E_INVALID_ENCSCHEME:		return "TSS_E_INVALID_ENCSCHEME";
			case TSS_E_INVALID_SIGSCHEME:		return "TSS_E_INVALID_SIGSCHEME";
			case TSS_E_ENC_INVALID_LENGTH:		return "TSS_E_ENC_INVALID_LENGTH";
			case TSS_E_ENC_NO_DATA:			return "TSS_E_ENC_NO_DATA";
			case TSS_E_ENC_INVALID_TYPE:		return "TSS_E_ENC_INVALID_TYPE";
			case TSS_E_INVALID_KEYUSAGE:		return "TSS_E_INVALID_KEYUSAGE";
			case TSS_E_VERIFICATION_FAILED:		return "TSS_E_VERIFICATION_FAILED";
			case TSS_E_HASH_NO_IDENTIFIER:		return "TSS_E_HASH_NO_IDENTIFIER";
			case TSS_E_PS_KEY_EXISTS:		return "TSS_E_PS_KEY_EXISTS";
			case TSS_E_PS_BAD_KEY_STATE:		return "TSS_E_PS_BAD_KEY_STATE";
			case TSS_E_EK_CHECKSUM:			return "TSS_E_EK_CHECKSUM";
			case TSS_E_DELEGATION_NOTSET:		return "TSS_E_DELEGATION_NOTSET";
			case TSS_E_DELFAMILY_NOTFOUND:		return "TSS_E_DELFAMILY_NOTFOUND";
			case TSS_E_DELFAMILY_ROWEXISTS:		return "TSS_E_DELFAMILY_ROWEXISTS";
			case TSS_E_VERSION_MISMATCH:		return "TSS_E_VERSION_MISMATCH";
			case TSS_E_DAA_AR_DECRYPTION_ERROR:	return "TSS_E_DAA_AR_DECRYPTION_ERROR";
			case TSS_E_DAA_AUTHENTICATION_ERROR:	return "TSS_E_DAA_AUTHENTICATION_ERROR";
			case TSS_E_DAA_CHALLENGE_RESPONSE_ERROR:return "TSS_E_DAA_CHALLENGE_RESPONSE_ERROR";
			case TSS_E_DAA_CREDENTIAL_PROOF_ERROR:	return "TSS_E_DAA_CREDENTIAL_PROOF_ERROR";
			case TSS_E_DAA_CREDENTIAL_REQUEST_PROOF_ERROR:return "TSS_E_DAA_CREDENTIAL_REQUEST_PROOF_ERROR";
			case TSS_E_DAA_ISSUER_KEY_ERROR:	return "TSS_E_DAA_ISSUER_KEY_ERROR";
			case TSS_E_DAA_PSEUDONYM_ERROR:		return "TSS_E_DAA_PSEUDONYM_ERROR";
			case TSS_E_INVALID_RESOURCE:		return "TSS_E_INVALID_RESOURCE";
			case TSS_E_NV_AREA_EXIST:		return "TSS_E_NV_AREA_EXIST";
			case TSS_E_NV_AREA_NOT_EXIST:		return "TSS_E_NV_AREA_NOT_EXIST";
			case TSS_E_TSP_TRANS_AUTHFAIL:		return "TSS_E_TSP_TRANS_AUTHFAIL";
			case TSS_E_TSP_TRANS_AUTHREQUIRED:	return "TSS_E_TSP_TRANS_AUTHREQUIRED";
			case TSS_E_TSP_TRANS_NOTEXCLUSIVE:	return "TSS_E_TSP_TRANS_NOTEXCLUSIVE";
			case TSS_E_NO_ACTIVE_COUNTER:		return "TSS_E_NO_ACTIVE_COUNTER";
			case TSS_E_TSP_TRANS_NO_PUBKEY:		return "TSS_E_TSP_TRANS_NO_PUBKEY";
			case TSS_E_TSP_TRANS_FAIL:		return "TSS_E_TSP_TRANS_FAIL";
			default:				return "UNKNOWN TSS ERROR";
		}
	}
}
