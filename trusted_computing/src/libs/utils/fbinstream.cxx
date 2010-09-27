// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

#include "fbinstream.hxx"

using namespace utils;
using namespace std;


ofbinstream::ofbinstream() :
  obinstream(),
  myFileBuffer()
{
	init( &myFileBuffer );
}

ofbinstream::ofbinstream( const string &file, ios_base::openmode mode ) :
  obinstream(),
  myFileBuffer()
{
	init( &myFileBuffer );
	open( file, mode | ios_base::binary );
}

ofbinstream::~ofbinstream()
{}

void ofbinstream::open( const string &file, ios_base::openmode mode ) {
  	if (!myFileBuffer.open( file.c_str(), mode | ios_base::out | ios_base::binary ))
    	setstate( ios_base::failbit );
  	else
    	clear();
}

void ofbinstream::close() {
	if ( !myFileBuffer.close() )
		setstate( ios_base::failbit );
}

ifbinstream::~ifbinstream()
{}

fbinstream::fbinstream() :
  iobinstream(),
  myFileBuffer()
{
	init( &myFileBuffer );
}

fbinstream::fbinstream( const std::string &file, std::ios_base::openmode mode ) :
  iobinstream(),
  myFileBuffer()
{
	init( &myFileBuffer );
	open( file, mode | ios_base::binary );
}

fbinstream::~fbinstream() {
}
