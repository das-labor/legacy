// Copyright (C) 2007, 2008 by Sirrix AG security technologies
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

/**
 * @file        BaseOption.hxx
 * @brief       Command line base Option
 * @version     $Revision: 532 $
 * @date        Last modification at $Date: 2009-05-05 16:02:08 +0200 (Tue, 05 May 2009) $ by $Author: selhorst $
 */

#ifndef BASEOPTION_HH_
#define BASEOPTION_HH_

#include <vector>
#include <list>
#include <string>
#include <stdexcept>

namespace utils {

   class OptionParser;
   class BaseOption;

   /// Tuple of strings representing an option/value pair.
   typedef std::pair<std::string, std::string> StringPair;

   /// A list of options to be parsed
   typedef std::list<BaseOption*> OptionList;

   /**
    * @brief Base class for command line option, represnets the option without TYPE.
    *
    * @see CmdLineParser
    *
    * @author      Christian Stueble (stueble@sirrix.com)
    * @author      Anoosheh Zaerin (zaerin@sirrix.com)
    * @maintainers Anoosheh Zaerin (zaerin@sirrix.com)
    */
   class BaseOption {
   public:
      enum OptionMode {
         optional, ///< the option can be omitted
         required, ///< the option is required
         single,   ///< the option will be used in single form, for example for help option or version
         multi     ///< multiple options are possible
      };

      /**
       * Constructs a new object of this class with the given parameters.
       *
       * @param longForm long option form, e.g. --help
       * @param shortForm short option form, e.g. -h
       * @param mode mode of the option. Can be optional, required or single.
       * @param description description of the option
       */
      BaseOption( const std::string& longForm, const std::string& shortForm, const OptionMode& mode, const std::string& description = "" );

      /**
       * @brief Creates a new command line option and registers it at the given parser.
       *
       *   @param parser The parser to be registered to.
       *   @param longForm long option form, e.g. --help
       *   @param shortForm short option form, e.g. -h
       *   @param mode mode of the option. Can be optional, required or single.
       *   @param description description of the option
       */
      BaseOption( OptionList &optionList, const std::string& longForm, const std::string& shortForm, const OptionMode& mode, const std::string& description = "" );

      /// Destructor.
      virtual ~BaseOption();

      /**
       * Parse the arguments and check if there exists the CmdLineOption in shortform or longform
       * in the list of arguments, and check if the CmdLineOption is required, or may be a single option.
       * Delete each argument and its value after the CmdLineOption is found in the arguments
       * list. Call setValue to set the value with correspounded TYPE
       * @return true if the option in arguments list found
       */
      virtual bool parseArgs( std::vector<StringPair> &arguments );

      /// Returns the default value or an empty string ("").
      virtual std::string getDefaultValue() const = 0;

      /// @return a String LongForm of CmdLineOption
      inline const std::string getLongForm() const;

      /// @return a String as ShortForm of CmdLineOption
      inline const std::string getShortForm() const;

      /// @return true if the Option is in optional mode.
      inline const bool isOptional() const;

      /// @return true if the Option is in required mode.
      inline const bool isRequired() const;

      /// @return true if the Option is in single mode.
      inline const bool isSingle() const;

      /// @return true if the Option is in multi mode.
      inline const bool isMulti() const;

      /// @return a String as Description of CmdLineOption
      inline const std::string getDescription() const;

   protected:
      /**
       * Pure virtual method to set and convert the value of a CmdLineOption.
       */
      virtual void setValue( const std::string &value ) = 0;

   private:
      std::string myLongForm; ///< longform of the CmdLineOption
      std::string myShortForm; ///< shortform of the CmdLineOption
      OptionMode mode; ///< mode of the option. can be optional, required or single.
      std::string myDescription; ///< description of the option. will be shown as help text for the option.
   };

   /**
    * @brief Exception that is thrown when an option is required but not given by the user.
    */
   class MissedCmdLineOption: public std::runtime_error {
   public:
      MissedCmdLineOption( const std::string &what ) :
         std::runtime_error( what ) {
      }
      ;
   };

   // ------------------------------- inline implementations -------------------------

   const std::string BaseOption::getLongForm() const {
      return myLongForm;
   }

   const std::string BaseOption::getShortForm() const {
      return myShortForm;
   }

   const bool BaseOption::isOptional() const {
      return ( mode == BaseOption::optional );
   }

   const bool BaseOption::isRequired() const {
      return ( mode == BaseOption::required );
   }

   const bool BaseOption::isSingle() const {
      return ( mode == BaseOption::single );
   }

   const bool BaseOption::isMulti() const {
      return ( mode == BaseOption::multi );
   }

   const std::string BaseOption::getDescription() const {
      return ( myDescription );
   }
}

#endif /*CMDLINEBASEOPTION_HH_*/
