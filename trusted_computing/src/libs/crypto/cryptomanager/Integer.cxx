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

#include "Integer.hxx"
#include <stdlib.h>

using namespace std;
using namespace utils;
using namespace integer;

//------------------------------------------------------------------------------
Integer::Integer()
{
    mpz_init( myInteger );
}

//------------------------------------------------------------------------------
Integer::Integer( const Integer &i )
{
    mpz_init_set( myInteger, i.myInteger );
}

//------------------------------------------------------------------------------
Integer::Integer( const UInt32 u32 )
{
    mpz_init_set_ui( myInteger, u32 );
}

//------------------------------------------------------------------------------
Integer::Integer( const Int32 i32 )
{
    mpz_init_set_si( myInteger, i32 );
}

//------------------------------------------------------------------------------
Integer::Integer( const UInt16 u16 )
{
    mpz_init_set_ui( myInteger, u16 );
}

//------------------------------------------------------------------------------
Integer::Integer( const Int16 i16 )
{
    mpz_init_set_si( myInteger, i16 );
}

//------------------------------------------------------------------------------
Integer::Integer( const UInt8 u8 )
{
    mpz_init_set_ui( myInteger, u8 );
}

//------------------------------------------------------------------------------
Integer::Integer( const Int8 i8 )
{
    mpz_init_set_si( myInteger, i8 );
}

//------------------------------------------------------------------------------
Integer::Integer( const std::vector<UInt8> &v8 )
{
    mpz_init( myInteger );

    for ( UInt32 i = 0; i < v8.size(); i++ )
    {
        mpz_mul_2exp( myInteger, myInteger, 8 );
        mpz_add_ui( myInteger, myInteger, ( UInt8 ) v8[ i ] );
    }
}

//------------------------------------------------------------------------------
Integer::Integer( const std::vector<UInt16> &v16 )
{
    mpz_init( myInteger );

    for ( UInt32 i = 0; i < v16.size(); i++ )
    {
        mpz_mul_2exp( myInteger, myInteger, 16 );
        mpz_add_ui( myInteger, myInteger, ( UInt16 ) v16[ i ] );
    }
}

//------------------------------------------------------------------------------
Integer::Integer( const std::vector<UInt32> &v32 )
{
    mpz_init( myInteger );

    for ( UInt32 i = 0; i < v32.size(); i++ )
    {
        mpz_mul_2exp( myInteger, myInteger, 32 );
        mpz_add_ui( myInteger, myInteger, ( UInt32 ) v32[ i ] );
    }
}


//------------------------------------------------------------------------------
Integer::Integer( const string &str, UInt8 base )
{
    mpz_init_set_str( myInteger, str.c_str(), base );
}


//------------------------------------------------------------------------------
Integer::Integer( const ByteVector &bs )
{
    mpz_init( myInteger );

    for ( UInt32 i = 0; i < bs.size(); i++ )
    {
        mpz_mul_2exp( myInteger, myInteger, 8 );
        mpz_add_ui( myInteger, myInteger, ( UInt8 ) bs[ i ] );
    }
}
//------------------------------------------------------------------------------


Integer::Integer(const char *str)
{
	mpz_init_set_str( myInteger, str, 16 );
}



//------------------------------------------------------------------------------
Integer::Integer( ibinstream &is )
{
    // create a string from serialized object in ibinstream
    string s;
    is >> s;

    // init & set myInteger based on s (cf. OBinStream& operator<<( OBinStream &os, const Integer &i ) )
    int ret = mpz_init_set_str( myInteger, s.c_str(), 16 );

    // if previous init failed, clear & re-init with zero
    if ( ret != 0 )
    {
            mpz_clear( myInteger );
            mpz_init( myInteger );
    }
}


//------------------------------------------------------------------------------
Integer::~Integer()
{
    mpz_clear( myInteger );
}

//============================== bit string operators ==========================

//------------------------------------------------------------------------------
Size Integer::bitlen() const
{
    return mpz_sizeinbase( myInteger, 2 );
}

//------------------------------------------------------------------------------
Integer& Integer::complement()
{
    mpz_com( myInteger, myInteger );
    return *this;
}

//------------------------------------------------------------------------------
Integer& Integer::neg()
{
    mpz_neg( myInteger, myInteger );
    return *this;
}

//------------------------------------------------------------------------------
// Because GMP seems not to have a function to invert all bits
// of a number (and even no xor function) we have to do this here.
// The function has to consider that topmost null bits must not be
// inverted.
Integer& Integer::invert()
{
    int mask = 0xffffffff >> ( ( 32 - bitlen() ) % 32 );
    int i;

    for ( i = 0; i < myInteger->_mp_size; i++ )
        myInteger->_mp_d[ i ] = ~myInteger->_mp_d[ i ];

    myInteger->_mp_d[ i - 1 ] &= mask;

    return *this;
}

//------------------------------------------------------------------------------
Integer& Integer::operator<<=( const Size &lshift )
{
    mpz_mul_2exp( myInteger, myInteger, lshift );

    return *this;
}

//------------------------------------------------------------------------------
Integer& Integer::operator>>=( const Size &rshift )
{
    mpz_tdiv_q_2exp( myInteger, myInteger, rshift );

    return *this;
}

//------------------------------------------------------------------------------
Integer& Integer::operator|=( const Integer &r )
{
    mpz_ior( myInteger, myInteger, r.myInteger );

    return *this;
}

//------------------------------------------------------------------------------
Integer& Integer::operator&=( const Integer &r )
{
    mpz_and( myInteger, myInteger, r.myInteger );

    return *this;
}

//------------------------------------------------------------------------------
Integer& Integer::operator^=( const Integer &r )
{
    mpz_xor( myInteger, myInteger, r.myInteger );

    return *this;
}

//------------------------------------------------------------------------------
Integer& Integer::setBit( const Size &index, bool value )
{
    if ( value )
        mpz_setbit( myInteger, index );
    else
        mpz_clrbit( myInteger, index );

    return *this;
}

//------------------------------------------------------------------------------
bool Integer::getBit( const Size &index ) const
{
    mpz_t tmp;

    mpz_init( tmp );
    mpz_tdiv_q_2exp( tmp, myInteger, index );


    return ( mpz_get_ui( tmp ) & 1 );
}

//============================== elementary math operations ====================

//------------------------------------------------------------------------------
Integer& Integer::operator+= ( const Integer &r )
{
    mpz_add( myInteger, myInteger, r.myInteger );

    return *this;
}

//------------------------------------------------------------------------------
Integer& Integer::operator-= ( const Integer &r )
{
    mpz_sub( myInteger, myInteger, r.myInteger );

    return *this;
}

//------------------------------------------------------------------------------
Integer& Integer::operator*= ( const Integer &r )
{
    mpz_mul( myInteger, myInteger, r.myInteger );

    return *this;
}

//------------------------------------------------------------------------------
Integer& Integer::operator/= ( const Integer &r )
{
    mpz_tdiv_q( myInteger, myInteger, r.myInteger );

    return *this;
}

//------------------------------------------------------------------------------
Integer& Integer::pow( const UInt32 e )
{
    mpz_pow_ui( myInteger, myInteger, e );

    return *this;
}

//------------------------------------------------------------------------------
Integer& Integer::root( const UInt32 n )
{
    mpz_root( myInteger, myInteger, n );

    return *this;
}

//============================== math operations modulo m ======================

//------------------------------------------------------------------------------
Integer& Integer::operator%= ( const Integer &r )
{
    mpz_mod( myInteger, myInteger, r.myInteger );

    return *this;
}

//------------------------------------------------------------------------------
Integer& Integer::powm( const Integer &e, const Integer &m )
{
    mpz_powm( myInteger, myInteger, e.myInteger, m.myInteger );

    return *this;
}

//------------------------------------------------------------------------------
Integer& Integer::powm( const UInt32 e, const Integer &m )
{
    mpz_powm_ui( myInteger, myInteger, e, m.myInteger );

    return *this;
}

//------------------------------------------------------------------------------
Integer& Integer::invertm( const Integer &m )
{
    int ret = mpz_invert( myInteger, myInteger, m.myInteger );

    // check if inverse exists
    if ( ret == 0 ) {
        cerr << hex << "Integer::invertm( 0x" << *this << ") mod 0x"  << m << "Inverse doesn't exist!" << endl;
    }

    return *this;
}

//============================== import / export ===============================

//------------------------------------------------------------------------------
Integer& Integer::operator= ( const Integer &r )
{
    if ( &r != this )
        mpz_set( myInteger, r.myInteger );

    return *this;
}

//------------------------------------------------------------------------------
UInt32 Integer::lowU32() const
{
    return (UInt32) mpz_get_ui( myInteger );
}

//------------------------------------------------------------------------------
void Integer::extract( vector<UInt8> &v8 ) const
{
    mpz_t tmp;

    mpz_init_set( tmp, myInteger );

    while ( mpz_sgn( tmp ) != 0 )
    {
        v8.insert( v8.begin(), (UInt8) ( mpz_get_ui( tmp ) & 0xff) );
        mpz_tdiv_q_2exp( tmp, tmp, 8 );
    }
}

//------------------------------------------------------------------------------
void Integer::extract( vector<UInt16> &v16 ) const
{
    mpz_t tmp;

    mpz_init_set( tmp, myInteger );

    while ( mpz_sgn( tmp ) != 0 )
    {
        v16.insert( v16.begin(), (UInt16) ( mpz_get_ui( tmp ) & 0xffff) );
        mpz_tdiv_q_2exp( tmp, tmp, 16 );
    }
}

//------------------------------------------------------------------------------
void Integer::extract( vector<UInt32> &v32 ) const
{
    mpz_t tmp;

    mpz_init_set( tmp, myInteger );

    while ( mpz_sgn( tmp ) != 0 ) {
        v32.insert( v32.begin(), (UInt32) mpz_get_ui( tmp ) );
        mpz_tdiv_q_2exp( tmp, tmp, 32 );
    }
}

ibinstream& Integer::fromBinaryStream( ibinstream &istr ) {
	return istr;
}

obinstream& Integer::toBinaryStream( obinstream &ostr ) const {
	return ostr;
}
//============================== others ========================================

//------------------------------------------------------------------------------
bool Integer::isPrime( UInt32 reps ) const
{
    bool a = ( mpz_probab_prime_p( myInteger, reps ) > 0 );

    return a;

}

//------------------------------------------------------------------------------
Integer& Integer::nextPrime() {
    mpz_nextprime( myInteger, myInteger );

    return *this;
}

//------------------------------------------------------------------------------
ostream& integer::operator<<( ostream &os, const Integer &i ) {
	UInt32 base=10;
	string base_str="";

	if (os.flags() & ios::hex) {
		base = 16;
		base_str = "0x";
	}
	else if (os.flags() & ios::oct) {
		base = 8;
		base_str = "0";
	}

	return os << base_str << string( mpz_get_str( 0, 16, i.myInteger ) );
}

//------------------------------------------------------------------------------
const Integer integer::CRA( const Integer &r1, const Integer &r2, const Integer &m1, const Integer &m2, const Integer &invm1modm2 )
{
    Integer result = r1 + m1 * ( ( ( r2 - r1 ) * invm1modm2 ) % m2 );

    if ( result < Integer( (UInt32) 0 ) )
        result = result + m1 * m2;

    return result;
}

//------------------------------------------------------------------------------
UInt32 Integer::size() const
{
	return mpz_sizeinbase (myInteger, 16);
}

