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

#ifndef _INTEGER_HH_
#define _INTEGER_HH_

#include <string>
#include <vector>
#include <iostream>
#include "Types.hxx"
#include <binstream.hxx>
#include <ByteVector.hxx>
#include <Streamable.hxx>
#include <babinstream.hxx>
#include <KeyronaTPM.hxx>

// include libgmp C library
#if defined __cplusplus
#undef __cplusplus
#endif

extern "C" {
  #include <gmp.h>
};

#ifndef __cplusplus
#define __cplusplus
#endif

    namespace integer {
	/**
	*  @example    Integer_UnitTests.cpp
	*/

	/**
	*  @brief      Class for Multi-Precision Integer Arithmetics
	*  @author     Christian Stueble (stueble@sirrix.com)
	*/
	class Integer {
	    friend bool operator> ( const Integer &, const Integer & );  ///< compares two Integer objects, true if left > right
	    friend bool operator>=( const Integer &, const Integer & );  ///< compares two Integer objects, true if left >= right
	    friend bool operator< ( const Integer &, const Integer & );  ///< compares two Integer objects, true if left < right
	    friend bool operator<=( const Integer &, const Integer & );  ///< compares two Integer objects, true if left <= right
	    friend bool operator==( const Integer &, const Integer & );  ///< compares two Integer objects, true if left == right

	    friend utils::ibinstream& operator>>( utils::ibinstream &is, Integer & );          ///< Reads an Integer from given IBinStream
	    friend utils::obinstream& operator<<( utils::obinstream &os, const Integer & );  ///< Stream given Integer into a OBinStream

	    friend std::ostream& operator<<( std::ostream &os, const Integer & ); ///< stream given Integer into a OTextStream
	    friend const Integer GCD( const Integer &, const Integer & );
	    friend const Integer Inverse( const Integer &in, const Integer &m);

	public:

	    Integer();                                         ///< Creates an Integer of value zero (0)
	    Integer( const UInt32 );                           ///< Creates an Integer using an UInt32
	    Integer( const Int32 );                            ///< Creates an Integer using an Int32
	    Integer( const UInt16 );                           ///< Creates an Integer using an UInt16
	    Integer( const Int16 );                            ///< Creates an Integer using an Int16
	    Integer( const UInt8 );                            ///< Creates an Integer using an UInt8
	    Integer( const Int8 );                             ///< Creates an Integer using an Int8
	    Integer( const utils::ByteVector & );              ///< Creates an Integer using a ByteString
	    Integer( const std::vector<UInt8> & );             ///< Creates an Integer using a Vector<UInt8>
	    Integer( const std::vector<UInt16> & );            ///< Creates an Integer using a Vector<UInt15>
	    Integer( const std::vector<UInt32> & );            ///< Creates an Integer using a Vector<UInt32>
	    Integer( const std::string &, UInt8 base );        ///< Creates an Integer using a String
	    Integer( const char *);			   				   ///< Creates an Integer using a *char
	    Integer( utils::ibinstream &is );           ///< Creates an Integer using a binary input stream
	    Integer( const Integer & );                        ///< Copy constructor


	    /// Destructor.
	    ~Integer();

	    // Bit string operators
	    Size bitlen() const;                        ///< returns the length of this number in bits.
	    Integer& complement();                      ///< creates the one's complement (-12346 from 12345).
	    Integer& invert();                          ///< inverts all bits. Consider a.invert().invert() != a.
	    Integer& neg();                             ///< inverts the sign.
	    Integer& operator<<=( const Size & );       ///< bitshift left.
	    Integer& operator>>=( const Size & );       ///< bitshift right.
	    Integer& operator|=( const Integer & );     ///< binary OR.
	    Integer& operator&=( const Integer & );     ///< binary AND.
	    Integer& operator^=( const Integer & );     ///< binary XOR.
	    Integer& setBit( const Size &, bool );      ///< Sets bit at given index.
	    bool getBit( const Size & ) const;          ///< @return Bit value at given index.

	    // Elementary math operations
	    Integer& operator+= ( const Integer & );             ///< addition.
	    Integer& operator-= ( const Integer & );             ///< minus.
	    Integer& operator*= ( const Integer & );             ///< multiply.
	    Integer& operator/= ( const Integer & );             ///< division (truncate result to int).
	    Integer& pow( const UInt32 e );                      ///< raise to the power of e.
	    Integer& root( const UInt32 n );                     ///< n-th root (truncate result to int).

	    // math operations modulo m
	    Integer& operator%= ( const Integer & );             ///< modulus m
	    Integer& powm( const Integer &e, const Integer &m ); ///< raise to the power of e modulus m
	    Integer& powm( const UInt32 e, const Integer &m );   ///< raise to the power of e modulus m
	    Integer& invertm( const Integer &m );                ///< invert modulus m

	    // import / export
	    Integer& operator= ( const Integer & );                   ///< assign operator
	    void extract( std::vector<UInt8> &v8 ) const;      ///< converts Integer to vector of uint8
	    void extract( std::vector<UInt16> &v16 ) const;    ///< converts Integer to vector of uint16
	    void extract( std::vector<UInt32> &v32 ) const;    ///< converts Integer to vector of uint32
	    utils::ibinstream& fromBinaryStream( utils::ibinstream &istr );
	    utils::obinstream& toBinaryStream( utils::obinstream &ostr ) const;

	    UInt32 lowU32() const;                                    ///< returns last U32

	    /**
	    * @brief Tests whether number is prime.
	    * @return FALSE if the number is definitely not prime. If it returns TRUE, then it
	    * is probably prime. The probability of a false positive is (1/4)^reps. The default
	    * value is 25.
	    */
	    bool isPrime( UInt32 reps = 25 ) const;

	    Integer& nextPrime(); ///< return the next prime greater than Integer

	     //return size in hex
	    UInt32 size() const;

	private:
	    mpz_t myInteger;     ///< Internal libGMP variable.
	};


	//------------------------------------------------------------------------------

	// bit string operators
	const Integer operator~ ( const Integer & );                   ///< Inverts all bits. Consider ~~a != a !!!.
	const Integer operator| ( const Integer &, const Integer & );  ///< bitwise OR
	const Integer operator& ( const Integer &, const Integer & );  ///< bitwise AND
	UInt32 operator& ( const Integer &, UInt32 );                         ///< bitwise AND
	UInt8  operator& ( const Integer &, UInt8  );                         ///< bitwise AND
	const Integer operator^ ( const Integer &, const Integer & );  ///< bitwise XOR
	const Integer operator>>( const Integer &, const Size & );     ///< bitshift right
	const Integer operator<<( const Integer &, const Size & );     ///< bitshift left

	// elementary math operators
	const Integer operator- ( const Integer & );                   ///< unary minus
	const Integer operator+ ( const Integer &, const Integer & );  ///< add
	const Integer operator- ( const Integer &, const Integer & );  ///< subtract
	const Integer operator* ( const Integer &, const Integer & );  ///< multiply
	const Integer operator/ ( const Integer &, const Integer & );  ///< divide
	const Integer operator% ( const Integer &, const Integer & );  ///< modulo

	// comparison operators
	bool operator!=( const Integer &, const Integer & );
    bool operator> ( const Integer &l, const Integer &r );
    bool operator>=( const Integer &l, const Integer &r );
    bool operator< ( const Integer &l, const Integer &r );
    bool operator<=( const Integer &l, const Integer &r );
    bool operator==( const Integer &l, const Integer &r );

	// others
	const Integer PowerMod( const Integer &b, const Integer &e, const Integer &m ); ///< ret = b^e mod m
	const Integer PowerMod( const Integer &b, const UInt32 &e, const Integer &m );  ///< ret = b^e mod m
	const Integer Power( const Integer &b, const UInt32 &e );                       ///< ret = b^e
	const Integer ModMultInv( const Integer &in, const Integer &m );                ///< ret = in^(-1) mod m
	const Integer GCD( const Integer &, const Integer & );                          ///< returns greatest common divisor
	const Integer Inverse(const Integer &in, const Integer &m);			///< Compute the inverse in modulo m; only if the inverse exists 
	const Integer CRA( const Integer &, const Integer &, const Integer &, const Integer &, const Integer & ); ///< solve a linear system using Cramer's rule

	utils::ibinstream& operator>>( utils::ibinstream &is, Integer & );            ///< reads an Integer from given IBinStream
	utils::obinstream& operator<<( utils::obinstream &os, const Integer & );    ///< stream given Integer into a OBinStream

	std::ostream& operator<<( std::ostream &ots, const Integer & );  ///< stream given Integer into a OTextStream
	std::istream& operator<<( std::istream &bs, const Integer & );     ///< stream given Integer into a ByteString


	//------------------ inline implementations ------------------------------------

	inline const Integer operator~ ( const Integer &r ) {
	    Integer tmp = r;
	    return tmp.invert();
	};

	inline const Integer operator| ( const Integer &l, const Integer &r ) {
	    Integer tmp = l;
	    return tmp |= r;
	};

	inline const Integer operator& ( const Integer &l, const Integer &r ) {
	    Integer tmp = l;
	    return tmp &= r;
	};

	inline const Integer operator^ ( const Integer &l, const Integer &r ) {
	    Integer tmp = l;
	    return tmp ^= r;
	};

	inline const Integer operator>>( const Integer &bs, const Size &bits ) {
	    Integer tmp = bs;
	    return tmp >>= bits;
	};

	inline const Integer operator<<( const Integer &bs, const Size &bits ) {
	    Integer tmp = bs;
	    return tmp <<= bits;
	};

	inline const Integer operator- ( const Integer &r ) {
	    Integer tmp = r;
	    return tmp.neg();
	};

	inline const Integer operator+ ( const Integer &l, const Integer &r ) {
	    Integer tmp = l;
	    return tmp += r;
	};

	inline const Integer operator- ( const Integer &l, const Integer &r ) {
	    Integer tmp = l;
	    return tmp -= r;
	};

	inline const Integer operator* ( const Integer &l, const Integer &r ) {
	    Integer tmp = l;
	    return tmp *= r;
	};

	inline const Integer operator/ ( const Integer &l, const Integer &r ) {
	    Integer tmp = l;
	    return tmp /= r;
	};

	inline const Integer operator% ( const Integer &l, const Integer &r ) {
	    Integer tmp = l;
	    return tmp %= r;
	};

	inline utils::ibinstream& operator>>( utils::ibinstream &is, Integer &i ) {
  		return i.fromBinaryStream( is );
	}

	inline utils::obinstream& operator<<( utils::obinstream &os, const Integer &i ) {
    	return i.toBinaryStream( os );
	}

	inline bool operator!= ( const Integer &l, const Integer &r ) {
	    return !( l == r );
	};

    inline bool operator> ( const Integer &l, const Integer &r ) {
    	return mpz_cmp( l.myInteger, r.myInteger ) > 0;
	}

	inline bool operator>=( const Integer &l, const Integer &r ) {
    	return mpz_cmp( l.myInteger, r.myInteger ) >= 0;
	}

	inline bool operator< ( const Integer &l, const Integer &r ) {
    	return mpz_cmp( l.myInteger, r.myInteger ) < 0;
	}

	inline bool operator<=( const Integer &l, const Integer &r ) {
    	return mpz_cmp( l.myInteger, r.myInteger ) <= 0;
	}

	inline bool operator==( const Integer &l, const Integer &r ) {
    	return ( mpz_cmp( l.myInteger, r.myInteger ) == 0 );
	}

	inline const Integer PowerMod( const Integer &b, const Integer &e, const Integer &m ) {
	    Integer tmp = b;
	    return tmp.powm( e, m );
	};

	inline const Integer PowerMod( const Integer &b, const UInt32 &e, const Integer &m ) {
	    Integer tmp = b;
	    return tmp.powm( e, m );
	};

	inline const Integer ModMultInv( const Integer &in, const Integer &m ) {
	    Integer tmp = in;
	    return tmp.invertm( m );
	};

	inline const Integer GCD( const Integer &a, const Integer &b ) {
	    Integer tmp;

	    mpz_gcd( tmp.myInteger, a.myInteger, b.myInteger );

	    return tmp;
	};

	inline const Integer Inverse( const Integer &in, const Integer &m) {
		Integer tmp;
		UInt32 e;
		
		e = mpz_invert ( tmp.myInteger, in.myInteger, m.myInteger );
		
		if(e==0) {
			std::cerr << " Inverse does not exist";
			return -1;
		}
		else {
			return tmp;
		}	
	}; 
    } // namespace integer

#endif // _INTEGER_HH_
