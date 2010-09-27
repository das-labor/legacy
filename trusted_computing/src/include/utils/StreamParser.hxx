// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS 
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//

#ifndef StreamParser_hxx_included
#define StreamParser_hxx_included

#include <OptionParser.hxx>
#include <iostream>

   namespace utils {

      /**
       * @brief  Option parser based on input text stream.
       * @author Christian Stueble (stueble@sirrix.com) 
       * 
       * Using this class, it is possible to parse an input text stream, e.g., a file. The
       * configuration options have to be defined as follows:
       * <code>
       *    option = value
       *    option2=value
       * </code>
       * @note Both, the option field and the value field must not contain spaces.
       * @note Comments such as '#' are currently not supported.
       * @see  Option, BaseOption
       */
      class StreamParser : public OptionParser {
      public:
	      /// Defines a new config file to be parsed.
         StreamParser( OptionList &optionList );
	
         /// Allow inheritance.
         virtual ~StreamParser();
         
         /// Opens and parses the config file. 
         std::istream& parseStream( std::istream &istr );
      };
   }

#endif /* StreamParser_hxx_included */
