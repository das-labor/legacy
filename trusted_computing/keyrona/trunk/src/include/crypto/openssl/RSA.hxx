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
#ifndef RSA_Hxx_included
#define RSA_Hxx_included

#include <Hash.hxx>  // access ByteVector
#include <Types.hxx>
#include <vector>

struct rsa_st;

namespace crypto {

      enum EncryptionAlgorithm {
            RSA_Enc_None,
            RSA_Enc_PKCSv15,
            RSA_Enc_OAEP
      };

      enum SigningAlgorithm {
            RSA_Sig_None,           ///< No RSA signatures allowed.
            RSA_Sig_SHA1,           ///< Only accepts a 20 byte string to be signed.
            RSA_Sig_DER,            ///< Creates an ASN.1 encoded octet string to be signed.
            RSA_Sig_PKCS,           ///< Signs a plain message an pads it using PKCS #1
      };

      /**
       * @brief RSA implementation based on the OpenSSL library
       *
       * @author Christian Stueble (stueble@sirrix.com)
       */
      class RSA {
         public:
            /// Creates a new RSA object
            RSA( UInt32 keyLengthInBits = 2048, UInt32 exponent = 17, EncryptionAlgorithm encAlgo = RSA_Enc_OAEP, SigningAlgorithm sigAlgo = RSA_Sig_SHA1 );

            /// Reads an existing RSA public key
            RSA( const utils::ByteVector &modulus, const utils::ByteVector &exponent, EncryptionAlgorithm encAlgo = RSA_Enc_OAEP, SigningAlgorithm sigAlgo = RSA_Sig_SHA1 );

            /// Copy constructor.
            RSA( const RSA & );

            /// Assignment operator.
            RSA& operator=( const RSA & );

            /// Destructor
            virtual ~RSA();

            /// Encryption
            virtual utils::ByteVector encrypt( const utils::ByteVector &plainText ) const;

            /// Decryption
            virtual utils::ByteVector decrypt( const utils::ByteVector &cipherText ) const;

            /// Signing
            virtual utils::ByteVector sign( const utils::ByteVector &message ) const;

            /// Verification
            virtual bool verify( const utils::ByteVector &message, const utils::ByteVector &signature ) const;

            /// Returns the RSA modulus
            utils::ByteVector getModulus() const;

            /// Returns the RSA exponent
            utils::ByteVector getExponent() const;

            /// Returns the private key
            utils::ByteVector getPrivateKey() const;

            /// Returns the used encryption algorithm.
            inline EncryptionAlgorithm getEncryptionAlgorithm() const;

            /// Returns the used signing algorithm.
            inline SigningAlgorithm getSigningAlgorithm() const;

         protected:
            /// Pads the plaintext block and returns the padded block.
            utils::ByteVector pad( const utils::ByteVector &plaintext ) const;

            /// Depads the decrypted plain text block and returns the plain text block.
            utils::ByteVector depad( const utils::ByteVector &plainBlock ) const;

            mutable rsa_st          *myRSA;
            EncryptionAlgorithm      myEncryptionAlgorithm;
            SigningAlgorithm         mySigningAlgorithm;
      };

      // inline implmeentations
      EncryptionAlgorithm RSA::getEncryptionAlgorithm() const {
            return myEncryptionAlgorithm;
      }

      SigningAlgorithm RSA::getSigningAlgorithm() const {
            return mySigningAlgorithm;
      }

}   // namespace crypto

#endif   // RSA_Hxx_included
