// Copyright (C) 2007, 2008 by Sirrix AG security technologies
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

#ifndef ByteVector_hxx_included
#define ByteVector_hxx_included

#include <vector>
#include "Streamable.hxx"
#include <sbinstream.hxx>

namespace utils {

      /**
       * @brief Encapsulation of a byte vector
       *
       * @author Christian Stueble (stueble@sirrix.com)
       */
      class ByteVector : public Streamable {
         public:
            friend bool operator==( const ByteVector &, const ByteVector & );
         public:
            ByteVector();
            /**
             * Initialize ByteVector from a @ref string. Note that the string is
             * interpreted as a byte sequence, not as a printable string.
             */
            ByteVector( const std::string &str );
            ByteVector( const std::vector<unsigned char> &v );
            ByteVector( const unsigned char *data, unsigned int length );
            /// Creates an empty byte array of length @c length.
            explicit ByteVector( unsigned int length );

            /// ByteVector object into an binary output stream.
            virtual obinstream& toBinaryStream( obinstream &ostr ) const;
            /// Create object from an binary input stream.
            virtual ibinstream& fromBinaryStream( ibinstream &istr );

            /// ByteVector object into an string-based output stream.
            virtual std::ostream& toStringStream( std::ostream &ostr ) const;
            /// Create object from an string-based input stream.
            virtual std::istream& fromStringStream( std::istream &istr );

            ByteVector& operator+=( const ByteVector & );
            ByteVector& operator+=( const unsigned int & );
            inline ByteVector& operator+=( unsigned char );
            inline ByteVector& operator+=( char );

            inline const unsigned char& operator[]( size_t index ) const;
            inline unsigned char&       operator[]( size_t index );

            /// @return const C array of internal byte sequence
            inline const unsigned char* toCArray() const;
            /// @return C array of internal byte sequence
            inline unsigned char* toCArray();

            /// @return Length of byte vector.
            inline size_t size() const;

            inline void resize( size_t size );

            inline void clear();

            /// Returns a ByteVector extracted from this ByteVector starting from @c index and @c numBytes bytes in length
            ByteVector sub( size_t index, size_t numBytes ) const;

            /// Returns a ByteVector consisting of the first @c numBytes bytes of this ByteVector
            ByteVector left( size_t numBytes ) const;

            /// Returns a ByteVector consisting of the last @c numBytes bytes of this ByteVector
            ByteVector right( size_t numBytes ) const;

         protected:
            std::vector<unsigned char>    myData;
      };

      // ----------------------- Related functions ----------------------------

      /// Plus operator
      inline ByteVector operator+( const ByteVector &l, const ByteVector &r );

      /// Comparison operator
      inline bool operator==( const ByteVector &l, const ByteVector &r );
      inline bool operator!=( const ByteVector &l, const ByteVector &r );

      /**
       * @brief   Casts an arbitrary type @c T into a ByteVector.
       * @param x    The object to be converted.
       * @return  The resulting ByteVector representation of @c x.
       * @note    Requires an binary output text stream operator ostream to convert
       *          an arbitrary type X.
       * @author  Christian Stueble (stueble@sirrix.com)
       */
      template<typename T>
      inline ByteVector ByteVector_cast( const T &x );


      // ----------------------- Inline implementations ----------------------------

      ByteVector& ByteVector::operator+=( unsigned char c ) {
            myData.push_back( c );
            return *this;
      }

      ByteVector& ByteVector::operator+=( char c ) {
            myData.push_back( c );
            return *this;
      }

      const unsigned char& ByteVector::operator[]( size_t index ) const {
            return myData.at( index );   // throw std::out_of_range exception here
      }

      unsigned char& ByteVector::operator[]( size_t index ) {
            return myData.at( index );   // throw std::out_of_range exception here
      }

      const unsigned char* ByteVector::toCArray() const {
            return &myData.front();
      }

      unsigned char* ByteVector::toCArray() {
            return &myData.front();
      }

      size_t ByteVector::size() const {
            return myData.size();
      }

      void ByteVector::resize( size_t size ) {
            myData.resize( size );
      }

      void ByteVector::clear() {
            myData.clear();
      }

      /// General implementation based on ostringstream
      template<typename T> ByteVector ByteVector_cast( const T &x ) {
            osbinstream o;
            o << x; // throw BadConversion here...
            return ByteVector( o.str() );
      }

      /// Specialized implementation for ByteVector
      template<>
      inline ByteVector ByteVector_cast<ByteVector>( const ByteVector &x ) {
            return x;
      }

      ByteVector operator+( const ByteVector &l, const ByteVector &r ) {
            ByteVector tmp = l;
            return tmp += r;
      }

      bool operator==( const ByteVector &l, const ByteVector &r ) {
            return l.myData == r.myData;
      }

      bool operator!=( const ByteVector &l, const ByteVector &r ) {
            return !(l == r);
      }
}

#endif // ByteVector_hxx_included
