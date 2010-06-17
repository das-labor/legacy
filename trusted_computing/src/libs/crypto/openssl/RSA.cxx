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
#include <RSA.hxx>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/objects.h>
#include <openssl/ssl.h>
#include <string>
#include <Debugging.hxx>
#include <string_cast.hxx>
#include <SSL_Exceptions.hxx>

using namespace utils;
using namespace std;

crypto::RSA::RSA( UInt32 keyLengthInBits, UInt32 exponent, EncryptionAlgorithm encAlgo, SigningAlgorithm sigAlgo ) :
      myRSA( RSA_generate_key( keyLengthInBits, exponent, 0, 0 ) ),
      myEncryptionAlgorithm( encAlgo ),
      mySigningAlgorithm( sigAlgo )
{}

crypto::RSA::RSA( const ByteVector &modulus, const ByteVector &exponent, EncryptionAlgorithm encAlgo, SigningAlgorithm sigAlgo ) :
      myRSA( RSA_new() ),
      myEncryptionAlgorithm( encAlgo ),
      mySigningAlgorithm( sigAlgo )
{
      BIGNUM *mod = BN_new();
      BIGNUM *exp = BN_new();

      BN_bin2bn( modulus.toCArray(), modulus.size(), mod );
      BN_bin2bn( exponent.toCArray(), exponent.size(), exp );

      myRSA->n = mod;
      myRSA->e = exp;
}

crypto::RSA::RSA( const crypto::RSA &r ) :
      myRSA( RSA_new() ),
      myEncryptionAlgorithm( r.myEncryptionAlgorithm ),
      mySigningAlgorithm( r.mySigningAlgorithm )
{
      myRSA->n = BN_dup( r.myRSA->n );
      myRSA->e = BN_dup( r.myRSA->e );
}

crypto::RSA& crypto::RSA::operator=( const crypto::RSA &r ) {
      if (&r != this) {
         BN_copy( myRSA->n, r.myRSA->n );
         BN_copy( myRSA->e, r.myRSA->e );

         myEncryptionAlgorithm = r.myEncryptionAlgorithm;
         mySigningAlgorithm = r.mySigningAlgorithm;
      }

      return *this;
}

crypto::RSA::~RSA() {
      RSA_free( myRSA );
}

ByteVector crypto::RSA::getModulus() const {
      ByteVector  modulus( BN_num_bytes( myRSA->n ) );

      BN_bn2bin( myRSA->n, modulus.toCArray() );

      return modulus;
}

ByteVector crypto::RSA::getExponent() const {
      ByteVector  exponent( BN_num_bytes( myRSA->e ) );

      BN_bn2bin( myRSA->e, exponent.toCArray() );

      return exponent;
}

ByteVector crypto::RSA::getPrivateKey() const {
      ByteVector  privKey( BN_num_bytes( myRSA->p ) );

      BN_bn2bin( myRSA->p, privKey.toCArray() );

      return privKey;
}

ByteVector crypto::RSA::pad( const utils::ByteVector &plaintext ) const {
      ByteVector block( RSA_size( myRSA ) );
      string     padString      = "TCPA";
      int        ret = 0;

      if ( myEncryptionAlgorithm == RSA_Enc_OAEP )
            ret = RSA_padding_add_PKCS1_OAEP( block.toCArray(), block.size(), plaintext.toCArray(), plaintext.size(),
                                     (unsigned char*) padString.c_str(), padString.length() );
      else
            ret = RSA_padding_add_PKCS1_type_2( block.toCArray(), block.size(), plaintext.toCArray(), plaintext.size() );

      if ( ret != 1 ) {
            throw SSL_Exception( "RSA: Error during padding." );
      }

      return block;
}

ByteVector crypto::RSA::encrypt( const ByteVector &plainText ) const {
      ByteVector plainBlock = pad( plainText );
      ByteVector cipherBlock( plainBlock.size() );

      if ( -1 == RSA_public_encrypt( plainBlock.size(), plainBlock.toCArray(), cipherBlock.toCArray(), myRSA, RSA_NO_PADDING ) ) {
            throw SSL_Exception( "RSA: Error during encryption." );
      }

      return cipherBlock;
}

ByteVector crypto::RSA::depad( const ByteVector &plainBlock ) const {
      ByteVector plainText( plainBlock.size() );
      string     padString      = "TCPA";
      int        ret = 0;

      if ( myEncryptionAlgorithm == RSA_Enc_OAEP )
            ret = RSA_padding_check_PKCS1_OAEP( plainText.toCArray(), plainText.size(), plainBlock.toCArray()+1, plainBlock.size()-1,
                                            RSA_size( myRSA ), (unsigned char*) padString.c_str(), padString.length() );
      else
            ret = RSA_padding_check_PKCS1_type_2( plainText.toCArray(), plainText.size(), plainBlock.toCArray()+1, plainBlock.size()-1, RSA_size( myRSA ) );

      if ( ret == -1 ) {
            throw SSL_Exception( "RSA: Error during depadding." );
      }
      else {
            plainText.resize( ret );
      }

      return plainText;
}

ByteVector crypto::RSA::decrypt( const ByteVector &cipherText ) const {
      if ( myRSA->d == 0 ) {
            throw CryptoException( "RSA: No private key available." );
            return ByteVector();
      }

      ByteVector  plainText( RSA_size( myRSA ) );

      if ( -1 == RSA_private_decrypt( plainText.size(), cipherText.toCArray(), plainText.toCArray(), myRSA, RSA_NO_PADDING ) ) {
            ERR_load_crypto_strings();
            ERR_free_strings();
      }

      return depad( plainText );
}

ByteVector crypto::RSA::sign( const ByteVector &message ) const {
      unsigned int siglen = RSA_size( myRSA );
      ByteVector   signature( siglen );
      int          ret = 0;

      switch ( mySigningAlgorithm ) {
         case RSA_Sig_SHA1:
               ret = RSA_sign( NID_sha1, message.toCArray(), message.size(), signature.toCArray(), &siglen, myRSA );
               break;
         case RSA_Sig_DER:
               ret = RSA_sign_ASN1_OCTET_STRING( NID_sha1, message.toCArray(), message.size(), signature.toCArray(), &siglen, myRSA );
               break;
         default:
               throw CryptoException( "Unknown or unsupported signing algorithm selected" );
      }

      if ( ret != 1 ) {
            throw SSL_Exception( "RSA: Error while signing message with OpenSSL::RSA." );
      }

    return signature;
}

bool crypto::RSA::verify( const ByteVector &message, const ByteVector &signature ) const {
      int  ret = 0;

      switch ( mySigningAlgorithm ) {
         case RSA_Sig_SHA1:
               ret = RSA_verify( NID_sha1, (unsigned char*) message.toCArray(), message.size(), (unsigned char*) signature.toCArray(), signature.size(), myRSA );

               if ( ret == -1 ) {
                     throw SSL_Exception( "RSA_verify: Error while verifying signature." );
               }

               return (ret == 1);
               break;
         case RSA_Sig_DER:
               ret = RSA_verify_ASN1_OCTET_STRING( NID_sha1, (unsigned char*) message.toCArray(), message.size(), (unsigned char*) signature.toCArray(), signature.size(), myRSA );

               if ( ret == -1 ) {
                     throw SSL_Exception( "RSA_verify_ASN1_OCTET_STRING: Error while verifying signature." );
               }

               return (ret == 1);
               break;
         case RSA_Sig_PKCS: {
               ByteVector block( RSA_size( myRSA ) );
               ret = RSA_public_decrypt( signature.size(), (unsigned char*) signature.toCArray(), block.toCArray(), myRSA, RSA_PKCS1_PADDING );
               if ( ret == -1 ) {
                     throw SSL_Exception( "RSA_public_decrypt: Error while verifying signature." );
               }

               return ( 0 == memcmp( block.toCArray(), message.toCArray(), ret ) );
               break;
         }
         case RSA_Sig_None:
         default:
               throw CryptoException( "RSA: Unknown or unsupported verification algorithm selected: " + string_cast( mySigningAlgorithm ) );
      }

      return false;
}
