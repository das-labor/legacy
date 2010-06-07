// Copyright (C) 2007, 2008 by Sirrix AG security technologies
//
// This file is part of the microTSS framework. The microTSS 
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//

#include <StreamParser.hxx>

using namespace utils;
using namespace std;

StreamParser::StreamParser( OptionList &optionList ) :
   OptionParser( optionList )
{}

StreamParser::~StreamParser()
{}

istream& StreamParser::parseStream( istream &istr ) {
   vector<string> options;
   string         tmp;
   size_t         comment;
   
   while ( istr.good() ) {
      getline( istr, tmp );
      
      comment = tmp.find_first_of( '#' );
      if (comment != string::npos)
    	  tmp.erase( comment );
      
      if ( tmp.length() > 0 )
         options.push_back( tmp );     
   }
   
   parse( options );
   
   return istr;
}
