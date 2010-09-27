// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

#include "ByteVector.hxx"

#include <iostream>

using namespace std;
using namespace utils;

ByteVector::ByteVector() :
      myData()
{}

ByteVector::ByteVector( const string &str ) :
      myData()
{
      for ( size_t charNo = 0; charNo < str.length(); charNo++ )
            myData.push_back( static_cast<unsigned char>( str[ charNo ] ));
}

ByteVector::ByteVector( const std::vector<unsigned char> &v ) :
      myData( v )
{}

ByteVector::ByteVector( const unsigned char *data, unsigned int length ) :
      myData( data, data + length )
{}

ByteVector::ByteVector( unsigned int length ) :
      myData( length )
{}

obinstream& ByteVector::toBinaryStream( obinstream &ostr ) const {
      vector<unsigned char>::const_iterator it;
      it = myData.begin();

      while( it != myData.end() ) {
            ostr << *it;
            it++;
      }

      return ostr;
}

ibinstream& ByteVector::fromBinaryStream( ibinstream &istr ) {
      ///read as many bytes as ByteVector
      for( size_t idx = 0; idx < myData.size(); ++idx )
            istr >> myData[ idx ];

      return istr;
}

ostream& ByteVector::toStringStream( std::ostream &ostr ) const{
      vector<unsigned char>::const_iterator it = myData.begin();

      ostr << "0x" << hex;

      while( it != myData.end() ) {
            ostr.width( 2 );
            ostr.fill( '0' );
            ostr << (int) (unsigned char) *it;
            ++it;
      }

      ostr << dec;

      return ostr;
}

istream& ByteVector::fromStringStream( std::istream &istr ){
      unsigned int len;
      istr  >> len;

      for( size_t idx = 0; idx < len; ++idx )
            istr >> myData[ idx ];

      return istr;
}

ByteVector& ByteVector::operator+=( const ByteVector &r ) {
      myData.insert( myData.end(), r.myData.begin(), r.myData.end() );
      return *this;
}

ByteVector& ByteVector::operator+=( const unsigned int &r ) {
      myData.push_back( static_cast<unsigned char>( r >> 24 ) );
      myData.push_back( static_cast<unsigned char>( r >> 16 ) );
      myData.push_back( static_cast<unsigned char>( r >> 8 ) );
      myData.push_back( static_cast<unsigned char>( r ) );

      return *this;
}

ByteVector ByteVector::sub( size_t index, size_t numBytes ) const {
      ByteVector sub( numBytes );
      for ( size_t byteNo = 0; byteNo < numBytes; byteNo++ )
            sub[ byteNo ] = myData.at( index + byteNo );   /// throw std::out_of_range exception here

      return sub;
}

ByteVector ByteVector::left( size_t numBytes ) const {
      return sub( 0, numBytes );
}

ByteVector ByteVector::right( size_t numBytes ) const {
      return sub( size() - numBytes, numBytes );
}
