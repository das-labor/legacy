// Copyright (C) 2007, 2008 by Sirrix AG security technologies
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

/**
 * @file        BaseOption.cxx
 * @brief       Declaration of the class CmdLineBaseOption.
 * @version     $Revision: 532 $
 * @modified    $Date: 2009-05-05 16:02:08 +0200 (Tue, 05 May 2009) $ by $Author: selhorst $
 */

#include <BaseOption.hxx>

#include <iostream>
#include <OptionParser.hxx>

using namespace std;
using namespace utils;

BaseOption::BaseOption( const string &longForm,
									           const string &shortForm,
									           const OptionMode &mode,
									           const string &description ) :
  myLongForm( longForm ),
  myShortForm( shortForm ),
  mode(mode),
  myDescription(description)
{}

BaseOption::BaseOption( OptionList &optionList,
                        const string &longForm,
							   const string &shortForm,
							   const OptionMode &mode,
							   const string &description ) :
  myLongForm( longForm ),
  myShortForm( shortForm ),
  mode( mode ),
  myDescription(description)
{
	optionList.push_back( this );
}

bool BaseOption::parseArgs( vector<StringPair> &arguments ) {
    vector<StringPair>::iterator it = arguments.begin();
  	bool found = false;
    bool stop = false;

    // Parse the arguments and check if this CmdLineOption in short form or long form in the
  	// list of arguments exists. If the parameter is given multiple times, the first
  	// option and its value will be parsed.
  	while ( it != arguments.end() && (!stop) ) {
    	string arg = it->first;
      // if there exist the option arg as long form or short form in arguments list,
    	// set found = true.
    	if ( arg == getLongForm() || arg == getShortForm() ) {
            found = true;

            if ( !isMulti() )
                stop = true;

      		// Call setValue to get the next argument from the list as value and set as option-value
      		// with corresponded TYPE
            setValue( it->second );
            it = arguments.erase( it );
    	}
      else
    	 ++it;
  	}

    // If option is defined as required and not found in arguments list throw MissedCmdLineOption
    if (mode == required && !found)
     	throw MissedCmdLineOption( "parseArgs() CmdLineOption [" + getShortForm()+
			      "] ["+getLongForm()+"] is required.");

    return found;
}

BaseOption::~BaseOption()
{}
