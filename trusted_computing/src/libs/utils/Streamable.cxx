// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

#include "Streamable.hxx"
#include <sbinstream.hxx>
#include <sstream>

using namespace std;
using namespace utils;


Streamable::Streamable()
{}

Streamable::~Streamable()
{}

obinstream& Streamable::toBinaryStream( obinstream &ostr ) const {
	return ostr;
}

ibinstream& Streamable::fromBinaryStream( ibinstream &istr ) {
	return istr;
}

ostream& Streamable::toStringStream( ostream &ostr ) const {
	return ostr;
}

istream& Streamable::fromStringStream( istream &istr ) {
	return istr;
}
