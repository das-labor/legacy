// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

#ifndef OBABINSTREAM_HXX_
#define OBABINSTREAM_HXX_

#include <binstream.hxx>
#include <ByteVector.hxx>
#include <sstream>

namespace utils {

	/**
	 * @brief A binary output stream based on type @ref ByteVector.
	 *
	 * This binsary stream uses a @ref ByteVector as internal buffer. It can
	 * be accessed with the method getByteArray().
    *
	 * @author Christian Stueble (stueble@sirrix.com)
	 */
	class obabinstream : public obinstream {
	public:
		inline obabinstream( std::ios_base::openmode mode=std::ios_base::out );
		inline obabinstream( const ByteVector &ba, std::ios_base::openmode mode=std::ios_base::out );
		virtual ~obabinstream();

		/// @return The internal byte string.
		inline ByteVector getByteVector();

	private:
		std::stringbuf myStringBuffer;
	};

	/**
	 * @brief A binary input stream based on byte strings.
	 *
	 * The current implementation uses a @c string as internal buffer. It can
	 * be accessed with the method str(). Later, the class @c ByteString
	 * or something similar could be used instead of @c string.
	 *
	 * @author Christian Stueble
	 */
	class ibabinstream : public ibinstream {
	public:
		inline ibabinstream( std::ios_base::openmode mode=std::ios_base::in );
		inline ibabinstream( const ByteVector &ba, std::ios_base::openmode mode=std::ios_base::in );
		virtual ~ibabinstream();

		/// @return The internal byte string.
		inline ByteVector getByteVector();

	private:
		std::stringbuf myStringBuffer;
	};

	/**
	 * @brief A binary input/output stream based on byte strings.
	 *
	 * @author Christian Stueble
	 */
	class babinstream : public iobinstream {
	public:
		inline babinstream( std::ios_base::openmode mode=std::ios_base::in | std::ios_base::out );
		inline babinstream( const ByteVector &ba, std::ios_base::openmode mode=std::ios_base::in | std::ios_base::out );
		virtual ~babinstream();

		/// @return The internal byte string.
		inline ByteVector getByteVector();

	private:
		std::stringbuf myStringBuffer;
	};

	// ----------------------- inline implementation -----------------------
	obabinstream::obabinstream( std::ios_base::openmode mode ) :
	  obinstream(),
	  myStringBuffer( mode | std::ios_base::out | ios_base::binary )
	{
		init( &myStringBuffer );
	}

	obabinstream::obabinstream( const ByteVector &ba, std::ios_base::openmode mode ) :
	  obinstream(),
	  myStringBuffer( std::string( (const char*) ba.toCArray(), ba.size() ), mode | std::ios_base::out | std::ios_base::binary )
	{
		init( &myStringBuffer );
	}

	ByteVector obabinstream::getByteVector() {
		return ByteVector( myStringBuffer.str() );
	}

	ibabinstream::ibabinstream( std::ios_base::openmode mode ) :
	  ibinstream(),
	  myStringBuffer( mode | std::ios_base::out | std::ios_base::binary )
	{
		init( &myStringBuffer );
	}

	ibabinstream::ibabinstream( const ByteVector &ba, std::ios_base::openmode mode ) :
	  ibinstream(),
	  myStringBuffer( std::string( (const char*) ba.toCArray(), ba.size() ), mode | std::ios_base::in | std::ios_base::binary )
	{
		init( &myStringBuffer );
	}

	ByteVector ibabinstream::getByteVector() {
		return ByteVector( myStringBuffer.str() );
	}

	babinstream::babinstream( std::ios_base::openmode mode ) :
	  iobinstream(),
	  myStringBuffer( mode | std::ios_base::out | std::ios_base::in | std::ios_base::binary )
	{
		init( &myStringBuffer );
	}

	babinstream::babinstream( const ByteVector &ba, std::ios_base::openmode mode ) :
	  iobinstream(),
	  myStringBuffer( std::string( (const char*) ba.toCArray(), ba.size() ), mode | std::ios_base::in | std::ios_base::out | std::ios_base::binary )
	{
		init( &myStringBuffer );
	}

	ByteVector babinstream::getByteVector() {
		return ByteVector( myStringBuffer.str() );
	}
}

#endif /*OSBINSTREAM_HXX_*/
