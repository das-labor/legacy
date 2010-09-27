// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

#ifndef OSBINSTREAM_HXX_
#define OSBINSTREAM_HXX_

#include <binstream.hxx>
#include <sstream>

namespace utils {

	/**
	 * @brief A binary output stream based on type @ref string.
	 *
	 * This binsary stream uses a @ref string as internal buffer. It can
	 * be accessed with the method str().
    *
	 * @author Christian Stueble
	 */
	class osbinstream : public obinstream {
	public:
		inline osbinstream( std::ios_base::openmode mode=std::ios_base::out );
		inline osbinstream( const std::string &str, std::ios_base::openmode mode=std::ios_base::out );
		virtual ~osbinstream();

		/// @return The internal byte string.
		inline std::string str();

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
	class isbinstream : public ibinstream {
	public:
		inline isbinstream( std::ios_base::openmode mode=std::ios_base::in );
		inline isbinstream( const std::string &str, std::ios_base::openmode mode=std::ios_base::in );
		virtual ~isbinstream();

		/// @return The internal byte string.
		inline std::string str();

	private:
		std::stringbuf myStringBuffer;
	};

	/**
	 * @brief A binary input/output stream based on byte strings.
	 * 	 *
	 * @author Christian Stueble
	 */
	class sbinstream : public iobinstream {
	public:
		inline sbinstream( std::ios_base::openmode mode=std::ios_base::in | std::ios_base::out );
		inline sbinstream( const std::string &str, std::ios_base::openmode mode=std::ios_base::in | std::ios_base::out );
		virtual ~sbinstream();

		/// @return The internal byte string.
		inline std::string str();

	private:
		std::stringbuf myStringBuffer;
	};

	// ----------------------- inline implementation -----------------------
	osbinstream::osbinstream( std::ios_base::openmode mode ) :
	  obinstream(),
	  myStringBuffer( mode | std::ios_base::out | std::ios_base::binary )
	{
		init( &myStringBuffer );
	}

	osbinstream::osbinstream( const std::string &str, std::ios_base::openmode mode ) :
	  obinstream(),
	  myStringBuffer( str, mode | std::ios_base::out | std::ios_base::binary )
	{
		init( &myStringBuffer );
	}

	std::string osbinstream::str() {
		return myStringBuffer.str();
	}

	isbinstream::isbinstream( std::ios_base::openmode mode ) :
	  ibinstream(),
	  myStringBuffer( mode | std::ios_base::out | std::ios_base::binary )
	{
		init( &myStringBuffer );
	}

	isbinstream::isbinstream( const std::string &str, std::ios_base::openmode mode ) :
	  ibinstream(),
	  myStringBuffer( str, mode | std::ios_base::in | std::ios_base::binary )
	{
		init( &myStringBuffer );
	}

	std::string isbinstream::str() {
		return myStringBuffer.str();
	}

	sbinstream::sbinstream( std::ios_base::openmode mode ) :
	  iobinstream(),
	  myStringBuffer( mode | std::ios_base::out | std::ios_base::in | std::ios_base::binary )
	{
		init( &myStringBuffer );
	}

	sbinstream::sbinstream( const std::string &str, std::ios_base::openmode mode ) :
	  iobinstream(),
	  myStringBuffer( str, mode | std::ios_base::in | std::ios_base::out | std::ios_base::binary )
	{
		init( &myStringBuffer );
	}

	std::string sbinstream::str() {
		return myStringBuffer.str();
	}
}

#endif /*OSBINSTREAM_HXX_*/
