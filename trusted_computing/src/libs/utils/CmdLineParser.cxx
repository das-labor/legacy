// Copyright (C) 2007, 2008 by Sirrix AG security technologies
//
// This file is part of the microTSS framework. The microTSS 
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//

/**
 * @file       CmdLineParser.cxx
 * @brief       class CmdLineParser 
 * @version     $Revision: 532 $
 * @modified   $Date: 2009-05-05 16:02:08 +0200 (Tue, 05 May 2009) $ by $Author: selhorst $
 */

#include <CmdLineParser.hxx>
#include <iostream>

using namespace std;
using namespace utils;

CmdLineParser::CmdLineParser( OptionList &optionList ) :
   OptionParser( optionList )
{}

void CmdLineParser::parseCmdLine( int argc, const char* args[] ) {
    vector<string> argv;
    int i=1; // skip first parameter, since this is the program name
   
    while ( i < argc ) {
        if ( args[i][0] == '-' ) { // if argument is command line option
            if ( args[i][1] == '-' )   // if argument is in long form
                argv.push_back( args[i]+2 ); // skip '--'
            else
                argv.push_back( args[i]+1 ); // skip '-'
            
            // if value does not include '=' && if next argument is empty || also command line option
            if ( ( argv.back().find_first_of('=') == string::npos ) && ( (i+1 == argc) || (args[i+1][0] == '-') ) )
                argv.push_back( "" ); // insert empty string
        }
        else                           // if argument is value
    	       argv.push_back( args[i] ); // append value
      
        ++i;
  	}
   
   
    parse( argv );
}
