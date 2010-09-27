// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

#ifndef Debugging_hxx_included
#define Debugging_hxx_included

#include <iostream>
#include <sstream>
#include <Config.hxx>

#ifndef DEBUG
//	#define debug	if ( false ) std::cerr							// should be skipped by a compiler
	#define debug	if ( true ) std::cerr							// should be skipped by a compiler
#else
	#define debug   Debugger::getInstance().getDebugStream()    // return debug stream

	/**
	 * @brief Helper class to manage debugging of output streams.
	 */
	class Debugger {
	public:
	   static Debugger& getInstance() {
	      static Debugger debugger;

	      return debugger;
	   }

	public:
	   void enableDebugging( bool enable ) {
	     myIsEnabled = enable;
	   };

	   void setDebugStream( std::ostream &str ) {
	      myStream = &str;
	   };

	   std::ostream& getDebugStream() {
		  static std::ostream nullstream( NULL );

	      if (myIsEnabled)
	         return *myStream;
	      else
	         return nullstream;
	   };

	protected:
	   Debugger() :
	      myIsEnabled( false ),
	      myStream( &::std::cout )
	   {};

	private:
	   bool myIsEnabled;
	   std::ostream *myStream;
	};
#endif

#endif
