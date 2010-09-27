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

#ifndef OPTIONPARSER_HH_
#define OPTIONPARSER_HH_

#include <Option.hxx>

namespace utils {

   
    /**
     * @brief Command line options parser.
     *
     * The parser has short (-v) and long-form (--verbose) option support, and
     * also allows options with associated values (-d 2, --debug 2).
     *
     * @maintainers Anoosheh Zaerin (zaerin@sirrix.com)
     *
     * @see CmdLineOption, CmdLineBoolOption
     */
    class OptionParser {
    public:
      /**
       * Creates a new command line parser object and initializes with empty options.
       * To parse for options, you have to add new command line options after the parser
       * is created. To add new options to parse, use the method addCmdLineOption() .
       */
      OptionParser( OptionList &optionList );
      
      /// Allow inheritance
      virtual ~OptionParser();
      
      /**
       * @return The non-parsed arguments in the form of a string vector.
       */
      virtual const std::vector<StringPair> getRemainingArgs();

      /**
       * Converts vector of string into a vector of Argument and then parses it.
       *  @param args Option-value pairs in the format args[i] = "option=value", or
       *              args[i]="option" && args[i+1]="value".
       *  @throw ParseError Thrown if an argument, i.e.,  args[i+1] is missing.
       */
      virtual void parse( const std::vector<std::string> &args );
      
      /**
       * Converts vector of string into a vector of Argument and then parses it.
       *  @param args Option-value pairs
       */
      virtual void parse( std::vector<StringPair> args );
      
      /**
       * @return a brief help that contains a set of all options in the long form.
       *   @param program Name of the program to be executed.
       */
      virtual std::string getBriefHelp( const std::string &program );
      
      /**
       * @return A complete help string containing a set of all options in short and long form 
       *         and the description of the respecitve option.
       */
      virtual std::string getLongHelp();

    private:
	  OptionList               &myOptions;  	   ///< vector of all required and optional CmdLineOptions
      std::vector<StringPair>  myRemainingArgs;    ///< vector of all arguments that could not be parsed. 
    };

    /**
     * @brief Exception that is thrown when an option is required but not given by the user.
     */
    class ParseError : public std::runtime_error {
    public:
      ParseError( const std::string &what ) :
         std::runtime_error( what ) {};
    };
    
    /**
     * @example CmdLineParserExample.cpp
     *
     * This is an example on how to use the classes CmdLineParser
     * and CmdLineOption.
     */
}

#endif // CMDLINEPARSER_HH_
