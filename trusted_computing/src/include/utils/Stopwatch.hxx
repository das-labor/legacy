// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

#ifndef STOPWATCH_HXX_
#define STOPWATCH_HXX_

#include <Types.hxx>
#include <iostream>
#include <string>

namespace sirrix {
   namespace utils {
      /// Predeclaration of the internally used data structs.
      struct StopwatchData;

      /**
       * @brief  Simple time measuring class.
       *
       * The little helper class helps to measure time intervals. At object creation,
       * the current system time is measured. Alternatively, you can restart the stop
       * watch using @ref start(). The method @ref stop() measures the system time the
       * second time and @ref getDuration() returns the measured time interval.
       *
       * A text stream operator exists to output the measured time in seconds.
       *
       *
       * @note Please note that this class does not measure the time used by this process,
       *       but the absolute time.
       *
       * @author Christian Stueble (stueble@sirrix.com)
       */
      class Stopwatch {
      public:
         /// Starts a new stop watch.
         Stopwatch();
         /// Deletes Stopwatch object.
         ~Stopwatch();

         /// Restarts stop watch.
         void start();

         /// Stops stop watch.
         void stop();

         /// Returns the measured amount of time in seconds.
         double getDuration() const;

         /// Returns the measured amount of time in microseconds.
         UInt32 getDurationMicroseconds() const;

      private:
         Stopwatch( const Stopwatch & );
         Stopwatch& operator=( const Stopwatch & );

         StopwatchData *myData;
      };

      /// Outputs the measured time in seconds to text stream.
      std::ostream& operator<<( std::ostream &ostr, const Stopwatch &watch );
   }
}

#endif /*STOPWATCH_HXX_*/
