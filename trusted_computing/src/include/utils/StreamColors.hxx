// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS 
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//

///
/// @file        StreamColors.hxx
/// @brief       Definition of text stream manipulators to change text colors
/// @version     $Revision: 532 $
/// @date        Last modification at $Date: 2009-05-05 16:02:08 +0200 (Tue, 05 May 2009) $ by $Author: selhorst $
/// @reviews
///    - 2006-02-02 Last review
///

#include <iostream>  
#include <iomanip>

#ifndef StreamColors_hxx_included
#define StreamColors_hxx_included

namespace sirrix {
   namespace utils {
      /**
       * @brief  Text stream manipulator setting the default text color.
       * @author Christian Stueble (stueble@sirrix.com)
       */
      inline std::ostream& standard( std::ostream &ostr ) {
         return ostr << "\033[0;39m"; 
      }
      
      /**
       * @brief  Text stream manipulator setting a red text color.
       * @author Christian Stueble (stueble@sirrix.com)
       */
      inline std::ostream& red( std::ostream &ostr ) {
         return ostr << "\033[1;31m"; 
      }
      
      /**
       * @brief  Text stream manipulator setting a green text color.
       * @author Christian Stueble (stueble@sirrix.com)
       */
      inline std::ostream& green( std::ostream &ostr ) {
         return ostr << "\033[1;32m"; 
      }
   }
}

#endif
