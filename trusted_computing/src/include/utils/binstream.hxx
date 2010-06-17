// Copyright (C) 2007, 2008 by Sirrix AG security technologies
//
// This file is part of the microTSS framework. The microTSS 
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//

#ifndef OBINSTREAM_HXX_
#define OBINSTREAM_HXX_

#include <iostream>
#include <string>

namespace utils {
	
	#define OUTPUT_OPERATOR
	
	/**
	 * @brief  Basic interface of a binary output stream.
	 * 
	 * This is the base class for all output streams. Derived classes to
	 * be used are @ref osbinstream, isbinstream, ofbinstream, and ifbinstream.
	 * 
	 * @note   The iobinstream class throws an exception of the type @ref std::ios_base::failure
     *         if the stream is in a bad state (e.g., eof).
     *
     * @author Christian Stueble
	 */
	class obinstream : virtual public std::basic_ios<char>
	{
	public:
		obinstream();
		obinstream( std::streambuf *sbuf );
		virtual ~obinstream();
		
		inline obinstream& operator<<( const void* );
		inline obinstream& operator<<( const std::string& );
		inline obinstream& operator<<( const bool& );
		
		inline obinstream& operator<<( const char& );
		inline obinstream& operator<<( const short& );
		inline obinstream& operator<<( const int& );
		inline obinstream& operator<<( const long& );
		inline obinstream& operator<<( const long long& );
		
		inline obinstream& operator<<( const unsigned char& );
		inline obinstream& operator<<( const unsigned short& );
		inline obinstream& operator<<( const unsigned int& );
		inline obinstream& operator<<( const unsigned long& );
		inline obinstream& operator<<( const unsigned long long& );
		
		/// Stream operator for manipulators.
		inline obinstream& operator<<( obinstream & (*f)( obinstream & ) );
		
		inline obinstream& flush();
		
	protected:
		// Writes bytestring to stream
		inline void write( const char*, unsigned long );
	};
	
	// ------------------ Manipulators -----------------
	inline obinstream& flush( obinstream &ostr ) {
		return ostr.flush();	
	}
	
	/**
	 * @brief  Basic interface of a binary input stream.
	 * 
	 * This is the base class for all input streams.
	 * 
	 * @note   The iobinstream class throws an exception of the type @ref std::ios_base::failure
     *         if the stream is in a bad state (e.g., eof).
     *
     * @author Christian Stueble
	 */
	class ibinstream : virtual public std::basic_ios<char>
	{
	public:
		ibinstream();
		ibinstream( std::streambuf *sbuf );
		virtual ~ibinstream();
		
		inline ibinstream& operator>>( void* );
		inline ibinstream& operator>>( std::string& );
		inline ibinstream& operator>>( bool& );
		
		inline ibinstream& operator>>( char& );
		inline ibinstream& operator>>( short& );
		inline ibinstream& operator>>( int& );
		inline ibinstream& operator>>( long& );
		inline ibinstream& operator>>( long long& );
		
		inline ibinstream& operator>>( unsigned char& );
		inline ibinstream& operator>>( unsigned short& );
		inline ibinstream& operator>>( unsigned int& );
		inline ibinstream& operator>>( unsigned long& );
		inline ibinstream& operator>>( unsigned long long& );
		
	protected:
		// Writes bytestring to stream
		inline void read( char*, unsigned long );
	};
	
	/**
	 * @brief  Basic interface of a binary input/output stream.
	 * 
	 * This is the base class for all binary input/output streams.
     * 
     * @note   The iobinstream class throws an exception of the type @ref std::ios_base::failure
     *         if the stream is in a bad state (e.g., eof).
	 * 
	 * @author Christian Stueble
	 */
	class iobinstream : public ibinstream, public obinstream {
	public:
		/// Creates a new iobinstream.
		iobinstream(); 
		/// Creates a new iobinstream.
		iobinstream( std::streambuf *sbuf ); 
		/// Virtual destructor.
		virtual ~iobinstream();
	};
	
	// --------------- Inline Implementation ---------------
	obinstream& obinstream::operator<<( const bool &v ) {
		write( reinterpret_cast<const char*>( &v ), sizeof( bool ) );
		return *this;	
	}
	obinstream& obinstream::operator<<( const int &v ) {
		write( reinterpret_cast<const char*>( &v ), sizeof( int ) );
		return *this;	
	}
	obinstream& obinstream::operator<<( const char &c ) {
		write( reinterpret_cast<const char*>( &c ), sizeof( char ) );
		return *this;	
	}
	obinstream& obinstream::operator<<( const unsigned char &c ) {
		write( reinterpret_cast<const char*>( &c ), sizeof( unsigned char ) );
		return *this;	
	}
	obinstream& obinstream::operator<<( const unsigned int &v ) {
		write( reinterpret_cast<const char*>( &v ), sizeof( unsigned int ) );
		return *this;	
	}
	obinstream& obinstream::operator<<( const unsigned short &c ) {
		write( reinterpret_cast<const char*>( &c ), sizeof( unsigned short ) );
		return *this;	
	}
	obinstream& obinstream::operator<<( const unsigned long &c ) {
		write( reinterpret_cast<const char*>( &c ), sizeof( unsigned long ) );
		return *this;	
	}
	obinstream& obinstream::operator<<( const unsigned long long &c ) {
		write( reinterpret_cast<const char*>( &c ), sizeof( unsigned long long ) );
		return *this;	
	}
	
	void obinstream::write( const char *v, unsigned long len ) {
		rdbuf()->sputn( v, len );
	}
	obinstream& obinstream::operator<<( obinstream & (*manip)( obinstream & ) ) {
		return manip( *this );	
	}
	obinstream& obinstream::flush() {
		if ( !rdbuf() )
			return *this;
			
		if ( !rdbuf()->pubsync() )
			setstate( std::ios_base::failbit );
		
		return *this;		
	}
	
        ibinstream& ibinstream::operator>>( std::string &str ) {
                str = (char*) rdbuf()->sgetc();
                return *this;
        }
	ibinstream& ibinstream::operator>>( bool &v ) {
		read( reinterpret_cast<char*>( &v ), sizeof( bool ) );
		return *this;	
	}
	ibinstream& ibinstream::operator>>( int &v ) {
		read( reinterpret_cast<char*>( &v ), sizeof( int ) );
		return *this;	
	}
	ibinstream& ibinstream::operator>>( char &c ) {
		read( reinterpret_cast<char*>( &c ), sizeof( char ) );
		return *this;	
	}
	ibinstream& ibinstream::operator>>( unsigned char &c ) {
		read( reinterpret_cast<char*>( &c ), sizeof( unsigned char ) );
		return *this;	
	}
	ibinstream& ibinstream::operator>>( unsigned int &i ) {
		read(reinterpret_cast<char*>( &i ), sizeof( unsigned int ));
		return *this;
	}
	ibinstream& ibinstream::operator>>( unsigned short &i ) {
		read(reinterpret_cast<char*>( &i ), sizeof( unsigned short ));
		return *this;
	}
	ibinstream& ibinstream::operator>>( unsigned long &i ) {
		read(reinterpret_cast<char*>( &i ), sizeof( unsigned long ));
		return *this;
	}
	ibinstream& ibinstream::operator>>( unsigned long long &i ) {
		read(reinterpret_cast<char*>( &i ), sizeof( unsigned long long ));
		return *this;
	}
	void ibinstream::read( char *v, unsigned long len ) {
		if ( rdbuf()->sgetn( v, len ) != (signed long) len  )
            setstate( eofbit );
	}
}

#endif /*OBINSTREAM_HXX_*/
