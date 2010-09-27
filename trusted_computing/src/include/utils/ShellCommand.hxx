// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS 
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//

#ifndef SHELLCOMMAND_HXX_
#define SHELLCOMMAND_HXX_

#include <string>
#include <iostream>

namespace sirrix {
   namespace utils {
      /**
       * @brief  Executes a shell command and returns the resulting output.
       * @author Christian Stueble (stueble@sirrix.com)
       */
      class ShellCommand {
      public:
	     ShellCommand( std::string cmd );
	     
        /// Executes command and writes output to @c ostr.
        void execute( std::ostream &ostr ) const;
        
      private:
         std::string myCommand;
      };

      /// Executes command and writes output to @c ostr.
      inline std::ostream& operator<<( std::ostream &ostr, const ShellCommand &cmd ) {
         cmd.execute( ostr );
         return ostr;
      };
   }
}

#endif /*SHELLCOMMAND_HXX_*/
