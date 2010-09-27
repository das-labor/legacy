// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

#ifndef Logger_hxx_included
#define Logger_hxx_included

#include <iostream>
#include <sstream>
#include <Config.hxx>
#include <Types.hxx>

namespace utils {
   
   /// Macro returning the logging stream.
   #define log( loglevel ) ::utils::Logging::getInstance().getLogStream( loglevel )
   
	/**
	 * @brief Helper class to manage logging of output streams.
	 */
	class Logging {
	public:
	   static Logging& getInstance() {
	      static Logging logger;

	      return logger;
	   }

	public:
      /// Sets the log level with 0 as the minimal verbosity.
	   void setLogLevel( UInt16 loglevel ) {
         myLogLevel = loglevel;
      }
      
      /// @return The currently set log level.
      UInt16 getLogLevel() const {
         return myLogLevel;
      }
      
      /// Sets the text stream to be used for logging.
	   void setLogStream( std::ostream &str ) {
	      myStream = &str;
	   };
      
      /// @return The text stream to be used for logging.
      std::ostream& getLogStream( UInt16 level ) {
        static std::ostream nullstream( NULL );

         if ( level <= getLogLevel() )
            return *myStream;
         else
            return nullstream;
      };

	protected:
	   Logging() :
	      myLogLevel( 0 ),
	      myStream( &::std::cout )
	   {};

	private:
	   UInt16       myLogLevel;
	   std::ostream *myStream;
	};
}

#endif
