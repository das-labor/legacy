// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS 
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//

/**
 * @file        CmdLineParser.hxx
 * @brief       Recieve a Command from commandline and check its validity
 * @version     $Revision: 532 $
 * @date        Last modification at $Date: 2009-05-05 16:02:08 +0200 (Tue, 05 May 2009) $ by $Author: selhorst $
 */

#ifndef CMDLINEPARSER_HH_
#define CMDLINEPARSER_HH_

#include <OptionParser.hxx>

namespace utils {

    /**
     * @brief An OptionParser that converts command line arguments accordingly.
     *
     * @maintainers Anoosheh Zaerin (zaerin@sirrix.com)
     *
     * @see CmdLineOption, CmdLineBoolOption
     */
    class CmdLineParser : public OptionParser {
    public:
      /**
       * Creates a new command line parser object and initializes with empty options.
       * To parse for options, you have to add new command line options after the parser
       * is created. To add new options to parse, use the method addCmdLineOption() .
       */
      CmdLineParser( OptionList &optionList );

      /** 
       * Extract the CmdLineOptions and non-CmdLineOption arguments from the given 
       * list of command-line arguments.
       * @param argc 				number of arguments from command line
       * @param argv 				list of arguments comming from commandline
       * @throws InvalidCmdLineOptionValue throws when the CmdLineOption doesn't have the correct value
       * @throws MissedCmdLineOption       throws when in the CmdLineOption isRequired = true and exist nicht in parsed arguments.
       */
      virtual void parseCmdLine( int argc, const char* argv[] );
    };

    /**
     * @example CmdLineParserExample.cpp
     *
     * This is an example on how to use the classes CmdLineParser
     * and CmdLineOption.
     */
}

#endif // CMDLINEPARSER_HH_
