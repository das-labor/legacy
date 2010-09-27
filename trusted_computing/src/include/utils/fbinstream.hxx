// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

#ifndef OFBINSTREAM_HXX_
#define OFBINSTREAM_HXX_

#include <binstream.hxx>
#include <fstream>

namespace utils {
	/**
	 * @brief A binary output stream based on a file.
	 *
	 * @author Christian Stueble
	 */
	class ofbinstream : public obinstream {
	public:
		ofbinstream();
		ofbinstream( const std::string &file,
		             std::ios_base::openmode mode=std::ios_base::out|std::ios_base::trunc );

		virtual ~ofbinstream();

		void open( const std::string &file,
		           std::ios_base::openmode mode=std::ios_base::out|std::ios_base::trunc );
		/// Closes the binary file stream.
		void close();

	private:
		std::filebuf myFileBuffer;
	};

	/**
	 * @brief A binary input stream based on a file.
	 *
	 * @author Christian Stueble
	 */
	class ifbinstream : public ibinstream {
	public:
		inline ifbinstream();
		inline ifbinstream( const std::string &file,
		                    std::ios_base::openmode mode=std::ios_base::in );

		virtual ~ifbinstream();

		inline void open( const std::string &file,
		                  std::ios_base::openmode mode=std::ios_base::in );
		/// Closes the binary file stream.
		inline void close();

	private:
		std::filebuf myFileBuffer;
	};

	/**
	 * @brief A binary input/output stream based on a file.
	 *
	 * @author Christian Stueble
	 */
	class fbinstream : public iobinstream {
	public:
		fbinstream();
		inline fbinstream( const std::string &file,
		                    std::ios_base::openmode mode=std::ios_base::in | std::ios_base::out );

		virtual ~fbinstream();
		/// Opens a new file to be used as iostream
		inline void open( const std::string &file,
		                  std::ios_base::openmode mode=std::ios_base::in | std::ios_base::out );
		/// Closes the binary file stream.
		inline void close();

	private:
		std::filebuf myFileBuffer;
	};

	// ----------------------- inline implementation --------------------------------
	ifbinstream::ifbinstream() :
 	  ibinstream(),
 	  myFileBuffer()
	{
		init( &myFileBuffer );
	}

	ifbinstream::ifbinstream( const std::string &file, std::ios_base::openmode mode ) :
  	  ibinstream(),
  	  myFileBuffer()
	{
		init( &myFileBuffer );
		open( file, mode | ios_base::binary );
	}

	void ifbinstream::open( const std::string &file, std::ios_base::openmode mode ) {
  		if (!myFileBuffer.open( file.c_str(), mode | std::ios_base::in | std::ios_base::binary ))
    		setstate( std::ios_base::failbit );
  		else
    		clear();
	}
	void ifbinstream::close() {
		if ( !myFileBuffer.close() )
			setstate( std::ios_base::failbit );
	}

	void fbinstream::open( const std::string &file, std::ios_base::openmode mode ) {
  		if (!myFileBuffer.open( file.c_str(), mode | std::ios_base::in | std::ios_base::out | std::ios_base::binary ))
    		setstate( std::ios_base::failbit );
  		else
    		clear();
	}
	void fbinstream::close() {
		if ( !myFileBuffer.close() )
			setstate( std::ios_base::failbit );
	}
}

#endif /*OFBINSTREAM_HXX_*/
