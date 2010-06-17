// Copyright (C) 2007, 2008 by Sirrix AG security technologies
//
// This file is part of the microTSS framework. The microTSS 
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//

#include "TypeInfo.hxx"
#include <sstream>

using namespace utils;
using namespace std;

TypeInfo::TypeInfo( const std::string &typeinfo ) :
	myType( "" ),
	myNamespace( 0 )
{
	istringstream istr( typeinfo );
	int           length;
	char          c;
	
	if (istr.peek() == 'P') {
		istr.get();
	}
	
	if (istr.peek() == 'N') {
		istr.get();
	}
		
	string name;	
	
	while (myType == "") {
		name = "";
		istr >> length;
		while ( length-- ) {
			istr >> c;
			name += c;
		}
		if ((istr.peek() >= '0') && (istr.peek() <= '9')) {
			myNamespace.push_back( name );
		} 
		else {
			myType = name;	
		}
	}
}

TypeInfo::~TypeInfo() 
{}

std::string TypeInfo::getType() const {
	return myType;
}

list<string> TypeInfo::getNamespace() const {
	return myNamespace;
}

