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

#include <OptionParser.hxx>
#include <iostream>
#include <strip.hxx>

using namespace std;
using namespace utils;

OptionParser::OptionParser( OptionList &optionList ):
  	myOptions( optionList ),
  	myRemainingArgs()
{}

OptionParser::~OptionParser()
{}

const vector<StringPair> OptionParser::getRemainingArgs() {
  	return myRemainingArgs;
}

void OptionParser::parse( const vector<string> &args ) {
   vector<StringPair>             arguments;
   vector<string>::const_iterator it = args.begin();
   string::size_type              index;

   while ( it != args.end() ) {
      index = (*it).find_first_of( "=" );
      if ( index == string::npos ) {
         if ( it+1 == args.end() ) {
            throw ParseError("Unknown or incorrect option specified.");
         }
         arguments.push_back( StringPair( strip( *it ), strip( *(it+1) ) ) );
         ++it; // skip first of two parameters.

      }
      else {
         arguments.push_back(  StringPair( strip( (*it).substr( 0, index ) ), strip( (*it).substr( index+1 ) ) ) );
      }

      ++it;
   }

   parse( arguments );
}

void OptionParser::parse( vector<StringPair> args ) {
   OptionList::iterator its = myOptions.begin();

   while ( its != myOptions.end() ) {
      if ( (*its)->isSingle() ) {
         if ( (*its)->parseArgs( args ) ) {  // if the single option exists in the list of arguments
            myRemainingArgs = args;
            return;
         }
      }
      ++its;
   }

   // If no single option is found, parse the list of optional and required options
   OptionList::iterator it = myOptions.begin();

   while ( it != myOptions.end() ) {
      (*it++)->parseArgs( args );
   }

   /**
   * at end moves non-parsed Arguments to raminingArgs.
   * then that is simple to detect non-valid CmdLineOptions
   */
   myRemainingArgs = args;
}

string OptionParser::getBriefHelp( const string &program )
{
	OptionList::iterator it = myOptions.begin();
	BaseOption* option;

   string single;
   string required = "   " + program;
   string optional;

	// First lists the required options
	while( it != myOptions.end() ) {
		option = *(it++);
      if ( option->isSingle() )
         single += "   " + program + " --" + option->getLongForm() + "\n";
		else if ( option->isRequired() )
         required += " --" + option->getLongForm();
      else
			optional += " --" + option->getLongForm();
	}

	return "Usage:\n" + single + required + " [" + optional + " ]";
}

string OptionParser::getLongHelp()
{
	string longHelp = "";
	string temp = "";
	OptionList::iterator it = myOptions.begin();
	BaseOption* opt;

	while( it != myOptions.end() ) {
		opt = *it++;
		longHelp +=  "   --" + opt->getLongForm();

		if ( (temp = opt->getShortForm()) != "")
			longHelp += " (-" + temp +" short option)";

		if ( (temp = opt->getDefaultValue()) != "" )
		   longHelp += "   [ default = " + temp + " ]";

		if ( (temp = opt->getDescription()) != "" )
			longHelp += "\n\t" + temp;
			longHelp += "\n";
	}

	return "Options:\n" + longHelp;
}
