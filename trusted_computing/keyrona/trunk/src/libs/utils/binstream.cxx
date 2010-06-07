// Copyright (C) 2007, 2008 by Sirrix AG security technologies
//
// This file is part of the microTSS framework. The microTSS 
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//

#include "binstream.hxx"

using namespace utils;

obinstream::obinstream() {
    // exceptions( std::ios::failbit | std::ios::eofbit | std::ios::badbit );
}

obinstream::obinstream( std::streambuf *sbuf )
{
	init( sbuf );
    // exceptions( std::ios::failbit | std::ios::eofbit | std::ios::badbit );
}

obinstream::~obinstream()
{}

ibinstream::ibinstream() {
    // exceptions( std::ios::failbit | std::ios::eofbit | std::ios::badbit );
}

ibinstream::ibinstream( std::streambuf *sbuf )
{
	init( sbuf );
    // exceptions( std::ios::failbit | std::ios::eofbit | std::ios::badbit );
}

ibinstream::~ibinstream()
{}

iobinstream::iobinstream( std::streambuf *sbuf ) :
  ibinstream( sbuf ),
  obinstream( sbuf )
{}

iobinstream::iobinstream() :
  ibinstream(),
  obinstream()
{}
 
iobinstream::~iobinstream() 
{}
