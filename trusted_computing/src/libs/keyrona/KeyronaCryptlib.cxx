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
 * @file        KeyronaCryptlib.cxx
 * @brief       Helper methods for cryptlib
 * @version     $Revision: 594 $
 * @date        Last modification at $Date: 2009-06-04 14:53:34 +0200 (Thu, 04 Jun 2009) $ by $Author: selhorst $
 */

#include <KeyronaCryptlib.hxx>

using namespace std;
using namespace utils;

static bool         cryptlibTerminating = false;
static UInt32       maxTries = CRYPTLIB_MAX_TRIES;

//================================================================================
//
void cryptlibCheck(int result, const string &what)
{
    stringstream myResult;

    switch(result)
    {
        case CRYPT_OK: return;
        case CRYPT_ERROR_INITED: return;
        case CRYPT_ENVELOPE_RESOURCE: debug << "Ressource needed, please continue..." << endl; return;
        case CRYPT_ERROR_INCOMPLETE:
                {
                    if (!maxTries)
                        throw Libcrypt("Cryptlib() timeout!");
                    cryptlibTerminating=true;
                    debug << "KeyronaKey(): Terminating cryptlib, but cryptEnd() needs more time: " << convertUInt32toString(maxTries) << endl;
                    sleep(1);
                    maxTries--;
                    cryptlibCheck(cryptEnd(), "KeyronaKey(): Terminating cryptlib");
                }
        default:
        {
            myResult.clear();
            myResult << "KeyronaKey|Cryptlib(): " << what << ": " << cryptlibResultToString(result) << " (" << result << ")";
            if (!cryptlibTerminating)
                throw Libcrypt(myResult.str());
        }
    }
};

//================================================================================
//
string cryptlibResultToString(int result)
{
    if (result == CRYPT_OK)
        return "";

    switch(result)
    {
        case CRYPT_ERROR_PARAM1:    return "Bad argument, parameter 1"; break;
        case CRYPT_ERROR_PARAM2:    return "Bad argument, parameter 2"; break;
        case CRYPT_ERROR_PARAM3:    return "Bad argument, parameter 3"; break;
        case CRYPT_ERROR_PARAM4:    return "Bad argument, parameter 4"; break;
        case CRYPT_ERROR_PARAM5:    return "Bad argument, parameter 5"; break;
        case CRYPT_ERROR_PARAM6:    return "Bad argument, parameter 6"; break;
        case CRYPT_ERROR_PARAM7:    return "Bad argument, parameter 7"; break;
        case CRYPT_ERROR_MEMORY:    return "Out of memory"; break;
        case CRYPT_ERROR_NOTINITED: return "Data has not been initialised"; break;
        case CRYPT_ERROR_INITED:    return "Data has already been init'd"; break;
        case CRYPT_ERROR_NOSECURE:  return "Opn.not avail.at requested sec.level"; break;
        case CRYPT_ERROR_RANDOM:    return "No reliable random data available"; break;
        case CRYPT_ERROR_FAILED:    return "Operation failed"; break;
        case CRYPT_ERROR_INTERNAL:  return "Internal consistency check failed"; break;
        case CRYPT_ERROR_NOTAVAIL:  return "This type of opn.not available"; break;
        case CRYPT_ERROR_PERMISSION:return "No permiss.to perform this operation"; break;
        case CRYPT_ERROR_WRONGKEY:  return "Incorrect key used to decrypt data"; break;
        case CRYPT_ERROR_INCOMPLETE:return "Operation incomplete/still in progress"; break;
        case CRYPT_ERROR_COMPLETE:  return "Operation complete/can't continue"; break;
        case CRYPT_ERROR_TIMEOUT:   return "Operation timed out before completion"; break;
        case CRYPT_ERROR_INVALID:   return "Invalid/inconsistent information"; break;
        case CRYPT_ERROR_SIGNALLED: return "Resource destroyed by extnl.event"; break;
        case CRYPT_ERROR_OVERFLOW:  return "Resources/space exhausted"; break;
        case CRYPT_ERROR_UNDERFLOW: return "Not enough data available"; break;
        case CRYPT_ERROR_BADDATA:   return "Bad/unrecognised data format"; break;
        case CRYPT_ERROR_SIGNATURE: return "Signature/integrity check failed"; break;
        case CRYPT_ERROR_OPEN:      return "Cannot open object"; break;
        case CRYPT_ERROR_READ:      return "Cannot read item from object"; break;
        case CRYPT_ERROR_WRITE:     return "Cannot write item to object"; break;
        case CRYPT_ERROR_NOTFOUND:  return "Requested item not found in object"; break;
        case CRYPT_ERROR_DUPLICATE: return "Item already present in object"; break;
        case CRYPT_ENVELOPE_RESOURCE:return "Need resource to proceed"; break;
        default: return "Unknown return code!";
    }
};

//================================================================================
//
bool convertPEM2p15(string &myPublicKey, string &myPrivateKey, string &myPassword, string &myLabel, string &myP15File)
{
    if (! verifyFilename(myPublicKey, false))
        throw InvalidFilename("convertPEM2p15(): invalid public key file given!");
    if (! verifyFilename(myPrivateKey, false))
        throw InvalidFilename("convertPEM2p15(): invalid private key file given!");
    if (verifyFilename(myP15File, false))
        throw InvalidFilename("convertPEM2p15(): target file already exists!");
    if (myLabel.empty())
        throw InvalidUsername("convertPEM2p15(): invalid label supplied!");
    if (myPassword.empty())
        throw InvalidPassword("convertPEM2p15(): empty password supplied!");

    // the code below was derived from 'pemtrans.c':
    // <http://www.oryx.com/ams/pemtrans.html>
    // original copyright was: 2004 Abhijit Menon-Sen <ams@oryx.com>

    FILE *f = NULL;
    int usage;
    fstream myCert;
    int myCertSize;
    char *certData = NULL;
    unsigned char *buf[8];

    RSA *key = NULL;
    EVP_PKEY *evp = NULL;
    CRYPT_KEYSET keyset;
    CRYPT_CONTEXT pKey;
    CRYPT_PKCINFO_RSA rsa;
    CRYPT_CERTIFICATE cert;

    cout << "Loading private key from keyfile '" << myPrivateKey << "': ";
    if ( ( f = fopen( myPrivateKey.c_str(), "r" ) ) == NULL ||
         ( evp = PEM_read_PrivateKey( f, NULL, NULL, NULL ) ) == NULL ||
         ( key = EVP_PKEY_get1_RSA( evp ) ) == NULL )
    {
        cout << "failed!" << endl;
        if ( f )
            fclose( f );
        if ( evp )
            EVP_PKEY_free( evp );
        return false;
    }
    cout << "success!" << endl;

    cout << "Loading public key from keyfile '" << myPublicKey << "'" << endl;
    myCert.open(myPublicKey.c_str(), ios::in);
    // get my filesize
    myCert.clear();
    myCert.seekg(0, ios::end);
    myCertSize = myCert.tellg();

    certData = new char[myCertSize];
    memset (certData, 0, myCertSize);

    // read the file
    myCert.clear();
    myCert.seekg(0, ios::beg);
    myCert.read(reinterpret_cast <char*>(certData), myCertSize);
    myCert.close();

    cryptlibCheck(cryptInit(), "convertPEM2p15(): initialising cryptlib");

    cout << "Initialising PKCINFO_RSA data components: " ;
    cryptInitComponents( &rsa, CRYPT_KEYTYPE_PRIVATE );
    if ( ( buf[0] = (unsigned char*)malloc( BN_num_bytes( key->n ) ) ) != NULL &&
         ( buf[1] = (unsigned char*)malloc( BN_num_bytes( key->e ) ) ) != NULL &&
         ( buf[2] = (unsigned char*)malloc( BN_num_bytes( key->d ) ) ) != NULL &&
         ( buf[3] = (unsigned char*)malloc( BN_num_bytes( key->p ) ) ) != NULL &&
         ( buf[4] = (unsigned char*)malloc( BN_num_bytes( key->q ) ) ) != NULL &&
         ( buf[5] = (unsigned char*)malloc( BN_num_bytes( key->iqmp ) ) ) != NULL &&
         ( buf[6] = (unsigned char*)malloc( BN_num_bytes( key->dmp1 ) ) ) != NULL &&
         ( buf[7] = (unsigned char*)malloc( BN_num_bytes( key->dmq1 ) ) ) != NULL
       )
    {
        int i;

        BN_bn2bin( key->n, buf[0] );
        BN_bn2bin( key->e, buf[1] );
        BN_bn2bin( key->d, buf[2] );
        BN_bn2bin( key->p, buf[3] );
        BN_bn2bin( key->q, buf[4] );
        BN_bn2bin( key->iqmp, buf[5] );
        BN_bn2bin( key->dmp1, buf[6] );
        BN_bn2bin( key->dmq1, buf[7] );

        cryptSetComponent( (&rsa)->n, buf[0], BN_num_bits( key->n ) );
        cryptSetComponent( (&rsa)->e, buf[1], BN_num_bits( key->e ) );
        cryptSetComponent( (&rsa)->d, buf[2], BN_num_bits( key->d ) );
        cryptSetComponent( (&rsa)->p, buf[3], BN_num_bits( key->p ) );
        cryptSetComponent( (&rsa)->q, buf[4], BN_num_bits( key->q ) );
        cryptSetComponent( (&rsa)->u, buf[5], BN_num_bits( key->iqmp ) );
        cryptSetComponent( (&rsa)->e1, buf[6], BN_num_bits( key->dmp1 ) );
        cryptSetComponent( (&rsa)->e2, buf[7], BN_num_bits( key->dmq1 ) );

        i = 0;
        while ( i < 8 )
            free( buf[i++] );
    }
    else
    {
        cout << "failed!" << endl;
        return false;
    }
    cout << "success!" << endl;

    cryptlibCheck(cryptCreateContext( &pKey, CRYPT_UNUSED, CRYPT_ALGO_RSA ), "convertPEM2p15(): creating context");
    cryptlibCheck(cryptSetAttributeString( pKey, CRYPT_CTXINFO_LABEL,
                                 myLabel.c_str(), myLabel.length()), "convertPEM2p15(): setting CTXINFO label");
    cryptlibCheck(cryptSetAttributeString( pKey, CRYPT_CTXINFO_KEY_COMPONENTS,
                                 &rsa, sizeof( CRYPT_PKCINFO_RSA )), "convertPEM2p15(): setting PKCINFO RSA data");
    cout << "Importing certificate" << endl;
    cryptlibCheck(cryptImportCert( certData, myCertSize, CRYPT_UNUSED, &cert), "convertPEM2p15(): importing certificate");

    char myString[MAX_CRYPT_BUFFER_SIZE];
    int length;

    cout << "Retrieved the following information from certificate: " << endl;
    if (cryptGetAttributeString(cert, CRYPT_CERTINFO_COMMONNAME, myString, &length) == CRYPT_OK)
        cout << "\tUser's common name: " << string(myString, length) << endl;

    if (cryptGetAttributeString(cert, CRYPT_CERTINFO_COUNTRYNAME, myString, &length) == CRYPT_OK)
        cout << "\tUser's country code: " << string(myString, length) << endl;

    if (cryptGetAttributeString(cert, CRYPT_CERTINFO_ORGANISATIONNAME, myString, &length) == CRYPT_OK)
        cout << "\tUser's organisation: " << string(myString, length) << endl;

    if (cryptGetAttributeString(cert, CRYPT_CERTINFO_ORGANISATIONALUNITNAME, myString, &length) == CRYPT_OK)
        cout << "\tUser's organisation unit: " << string(myString, length) << endl;

    if (cryptGetAttributeString(cert, CRYPT_CERTINFO_STATEORPROVINCENAME, myString, &length) == CRYPT_OK)
        cout << "\tUser's state: " << string(myString, length) << endl;

    if (cryptGetAttributeString(cert, CRYPT_CERTINFO_LOCALITYNAME, myString, &length) == CRYPT_OK)
        cout << "\tUser's locality: " << string(myString, length) << endl;
    
    cout << "Verifying KEYUSAGE: ";
    if ( cryptGetAttribute( cert, CRYPT_CERTINFO_KEYUSAGE, &usage ) != CRYPT_OK )
    {
        cout << "Warning: The certificate specifies no KEYUSAGE." << endl;
        cout << "Cryptlib may not permit its use. See <http://www.oryx.com/ams/pemtrans.html>." << endl;
    }
    else
        cout << "OK" << endl;

    cryptlibCheck(cryptKeysetOpen( &keyset, CRYPT_UNUSED, CRYPT_KEYSET_FILE,
                         myP15File.c_str(), CRYPT_KEYOPT_CREATE), "convertPEM2p15(): opening keyset");
    cout << "Storing private key" << endl;
    cryptlibCheck(cryptAddPrivateKey( keyset, pKey, myPassword.c_str()), "convertPEM2p15(): adding private key to keyset");
    cout << "Storing public key" << endl;
    cryptlibCheck(cryptAddPublicKey( keyset, cert ), "convertPEM2p15(): adding public key to keyset");

    cryptlibCheck(cryptKeysetClose( keyset ), "convertPEM2p15(): closing keyset");
    cryptDestroyComponents( &rsa );
    cryptDestroyContext( pKey );
    cryptDestroyCert( cert );
    cout << "Finished, successfully converted OpenSSL / PEM key to .p15-keyfile!" << endl;
    return true;
};

//================================================================================
//
void printCryptlibAlgo(int cryptAlgo)
{
    switch (cryptAlgo)
    {
        case CRYPT_ALGO_DES: cout << "DES"; break;
	case CRYPT_ALGO_3DES: cout << "Triple DES"; break;
	case CRYPT_ALGO_IDEA: cout << "IDEA"; break;
	case CRYPT_ALGO_CAST: cout << "CAST-128"; break;
	case CRYPT_ALGO_RC2: cout << "RC2"; break;
	case CRYPT_ALGO_RC4: cout << "RC4"; break;
	case CRYPT_ALGO_RC5: cout << "RC5"; break;
	case CRYPT_ALGO_AES: cout << "AES"; break;
	case CRYPT_ALGO_BLOWFISH: cout << "Blowfish"; break;
	case CRYPT_ALGO_SKIPJACK: cout << "Skipjack"; break;
        case CRYPT_ALGO_DH: cout << "Diffie-Hellman"; break;
	case CRYPT_ALGO_RSA: cout << "RSA"; break;
	case CRYPT_ALGO_DSA: cout << "DSA"; break;
	case CRYPT_ALGO_ELGAMAL: cout << "ElGamal"; break;
	case CRYPT_ALGO_KEA: cout << "KEA"; break;
	case CRYPT_ALGO_ECDSA: cout << "ECDSA"; break;
	case CRYPT_ALGO_MD2: cout << "MD2"; break;
	case CRYPT_ALGO_MD4: cout << "MD4"; break;
	case CRYPT_ALGO_MD5: cout << "MD5"; break;
	case CRYPT_ALGO_SHA1: cout << "SHA/SHA1"; break;
	case CRYPT_ALGO_RIPEMD160: cout << "RIPE-MD 160"; break;
	case CRYPT_ALGO_SHA2: cout << "SHA2 (SHA-256/384/512)"; break;
	case CRYPT_ALGO_HMAC_MD5: cout << "HMAC-MD5"; break;
	case CRYPT_ALGO_HMAC_SHA1: cout << "HMAC-SHA"; break;
	case CRYPT_ALGO_HMAC_RIPEMD160: cout << "HMAC-RIPEMD-160"; break;
	case CRYPT_ALGO_HMAC_SHA2: cout << "HMAC-SHA2"; break;
        default: cout << "Undefined"; break;
    }
    cout << endl;

};
