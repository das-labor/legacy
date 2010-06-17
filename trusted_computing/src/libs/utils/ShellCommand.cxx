// Copyright (C) 2007, 2008 by Sirrix AG security technologies
//
// This file is part of the microTSS framework. The microTSS 
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//

#include <ShellCommand.hxx>
#include <stdio.h>

using namespace sirrix;
using namespace utils;
using namespace std;

ShellCommand::ShellCommand( string cmd ) :
   myCommand( cmd )
{}


void ShellCommand::execute( ostream &ostr ) const {
   FILE *file = popen( myCommand.c_str(), "r" );
   string buffer;
   
   int c;
   
   while ((c = fgetc( file )) != EOF) {
      buffer += (char) c;  
   } 
   
   pclose( file );
   
   // Remove newline at the end
   if (buffer[buffer.size()-1] == '\n')
        buffer.resize( buffer.size()-1 );
        
   // buffer[buffer.size()-1] = '\0';
      
   ostr << buffer;
}
