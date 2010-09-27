// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS 
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//

#ifndef STRING_CAST_HXX_
#define STRING_CAST_HXX_

#include <string>
#include <sstream>

namespace utils {
	/**
     * @brief   Casts an arbitrary type @c T into a string.
     *   @param x    The object to be converted.
     *   @param base The basis of the conversion.
     * @return  The resulting string representation of @c x.
     * @note    Requires an output text stream operator ostream to convert
     *          an arbitrary type X.
     * @author  Christian Stueble (stueble@sirrix.com)
     */
    template<typename T>
    inline std::string string_cast( const T& x, const std::ios_base::fmtflags &base=std::ios_base::dec );
  
    // ----------------------- inline implementations ----------------------------
             
    /// general implementation based on ostringstream
    template<typename T> std::string string_cast( const T& x, const std::ios_base::fmtflags &base ) {
      std::ostringstream o;
      
      o.setf( base );
      o << x; // throw BadConversion here...

      return o.str();
    }

    /// Specialized implementation for bool.
    template<>
    inline std::string string_cast<bool>( const bool& x, const std::ios_base::fmtflags &base ) {
      return x ? "true" : "false";
    }

    /// Specialized implementation for String.
    // template<>
    // inline std::string string_cast<std::string>( const std::string& x, const std::ios_base::fmtflags &base ) {
    //   return x;
    // }
}

#endif /*STRING_CAST_HXX_*/
