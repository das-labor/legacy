// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

#ifndef _STRIP_HXX_
#define _STRIP_HXX_

#include <string>
#include <sstream>

namespace utils {
   /// Removes the spaces around the string @c s.
   inline std::string strip( const std::string &s ) {
      std::string        result;
      std::istringstream istr( s );

      istr >> result;

      return result;
   };
}

#endif /*STRING_CAST_HXX_*/
