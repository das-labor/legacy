// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

/**
 * @file        Option.hxx
 * @brief       Declaration and definition of the template Option<> and the classes InvalidptionValue
 *              and MissedCmdLineOptionValue.
 * @version     $Revision: 532 $
 * @date        Last modification at $Date: 2007-05-07 20:06:34 +0200 (Mo, 07 Mai 2007)$
 *              by $Author: selhorst $
 */

#ifndef OPTION_HH_
#define OPTION_HH_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <BaseOption.hxx>
#include <typeinfo>
#include <string_cast.hxx>

namespace utils {

    /**
     * @brief Template representative of a command line option.
     * @param TYPE Type of the command line option.
     * @example CmdLineOption<Int32> longOption
     *
     * @maintainers Anoosheh Zaerin (zaerin@sirrix.com)
     * @see CmdLineParser
     */
    template <typename TYPE>
    class Option: public BaseOption {
    public:
      /*
       * Constructor of CmdLineOption.
	   *   @param longForm long form of the option, e.g. --help
	   *   @param shortform short form of the option, e.g. -h
	   *   @param defaultvalue default value of the option
	   *   @param mode mode of the option, could be optional, required or single.
       */
      Option( const std::string &longForm, const std::string &shortForm, TYPE defaultValue, OptionMode mode, const std::string &description="" );

      /*
       * Constructor of CmdLineOption.
       *   @param longForm long form of the option, e.g. --help
       *   @param shortform short form of the option, e.g. -h
       *   @param defaultvalue default value of the option
       *   @param mode mode of the option, could be optional, required or single.
       */
      Option( const std::string &longForm, const std::string &shortForm, OptionMode mode, const std::string &description="" );

      /*
       * Constructor of CmdLineOption that registers to the CmdLineParser.
       *   @param parser The command line parser to be registered to.
	   *   @param longForm long form of the option, e.g. --help
	   *   @param shortform short form of the option, e.g. -h
	   *   @param defaultvalue default value of the option
	   *   @param mode mode of the option, could be optional, required or single.
       */
      Option( OptionList &optionList, const std::string &longForm, const std::string &shortForm, TYPE defaultValue, OptionMode mode, const std::string &description="" );

      /*
       * Constructor of CmdLineOption that registers to the CmdLineParser.
       *   @param parser The command line parser to be registered to.
       *   @param longForm long form of the option, e.g. --help
       *   @param shortform short form of the option, e.g. -h
       *   @param defaultvalue default value of the option
       *   @param mode mode of the option, could be optional, required or single.
       */
      Option( OptionList &optionList, const std::string &longForm, const std::string &shortForm, OptionMode mode, const std::string &description="" );

      /// @return true if at least one option exists
      inline bool hasValue() const;

      /// @return the option value of type TYPE.
      inline TYPE getValue() const;

      /// @return the default value if set.
      inline std::string getDefaultValue() const;

      /// @return the list of option values of type TYPE.
      inline std::vector<TYPE> getValues() const;

      /// Parse the argument list for the given option
      virtual bool parseArgs( std::vector<StringPair> &arguments );

    protected:
      /**
       * Sets and converts a set of strings to the TYPE of this CmdLineOption.
       * @throw InvalidCmdLineOptionValue when value is in bad format or occures a overflow exception.
       */
      virtual void setValue( const std::string &value );

    private:
        /// Value of CmdLineOption
        std::vector<TYPE> myValue;
    };

    /**
     * Function allowing to write options into a text stream.
     */
    template<typename TYPE>
    std::ostream& operator<<( std::ostream &ostr, const Option<TYPE> &option ) {
       typename std::vector<TYPE>                 optionList = option.getValues();
       typename std::vector<TYPE>::const_iterator it = optionList.begin();

       while ( it != optionList.end() ) {
    	   ostr << option.getLongForm() << "=" << *it++ << std::endl;
       }

       return ostr;
    };

   /**
    * @brief Base class of all option specific exceptions.
    */
   class OptionError: public std::runtime_error {
   public:
      OptionError( const std::string &what ) throw() :
         std::runtime_error( what ) {
      };
   };

    /**
     * @brief Exception that is thrown when an illegal value is given by the user for an CmdLineOption.
     */
    class InvalidOptionValue: public OptionError {
    public:
      InvalidOptionValue( const std::string &what ) throw() :
		   OptionError( what ) {};
    };

    /**
     *  @brief Exception that is thrown when no value is given by the user for an CmdLineOption.
     */
    class MissedOptionValue: public OptionError {
    public:
      MissedOptionValue( const std::string &what ) throw() :
		   OptionError( what ) {};
    };

   /**
    * @brief Exception that can be thrown if an unknown option was used.
    */
   class InvalidOption: public OptionError {
   public:
      InvalidOption( const std::string &what ) throw() :
         OptionError( what )
      {};
   };

    // ------------------------------- inline implementations -------------------------
    template <typename TYPE>
    Option<TYPE>::Option( const std::string &longForm, const std::string &shortForm, TYPE defaultValue, OptionMode mode, const std::string &description ):
      BaseOption( longForm, shortForm, mode, description),
      myValue( 1, defaultValue )
    {}

    template <typename TYPE>
    Option<TYPE>::Option( const std::string &longForm, const std::string &shortForm, OptionMode mode, const std::string &description ):
    	BaseOption( longForm, shortForm, mode, description),
        myValue()
    {}

	template <typename TYPE>
    Option<TYPE>::Option( OptionList &optionList, const std::string &longForm, const std::string &shortForm, TYPE defaultValue, OptionMode mode, const std::string &description ):
      BaseOption( optionList, longForm, shortForm, mode, description),
      myValue( 1, defaultValue )
    {}

    template <typename TYPE>
    Option<TYPE>::Option( OptionList &optionList, const std::string &longForm, const std::string &shortForm, OptionMode mode, const std::string &description ):
    	BaseOption( optionList, longForm, shortForm, mode, description),
        myValue()
    {}

    template <typename TYPE>
    bool Option<TYPE>::hasValue() const {
    	return !myValue.empty();
    }

    template <typename TYPE>
    TYPE Option<TYPE>::getValue() const {
    	return myValue.at( 0 );
    }

    template <typename TYPE>
    std::string Option<TYPE>::getDefaultValue() const {
       if ( hasValue() )
          return string_cast( getValue() );
       else
          return "";
    }

    template <typename TYPE>
    std::vector<TYPE> Option<TYPE>::getValues() const {
    	return myValue;
    }

    template <typename TYPE>
    void Option<TYPE>::setValue( const std::string &value ) {
         if (value == "")
            throw MissedOptionValue( "The option '--" + getLongForm() + "' (-" + getShortForm() + ") needs an additional parameter.");

         try {
            // change str to IStringStream in order to use the operator >> to convert to TYPE
            std::istringstream in( value );
            TYPE value;
            // Convert String to TYPE
            in >> value;

            if ( isMulti() )
            	myValue.push_back( value );  // add value
            else {
            	myValue.resize( 1 );
            	myValue.at(0) = value;			 // overwrite value
            }
         }
         catch( std::exception &e ) {
            // if overflow occured
            throw InvalidOptionValue( "An invalid option was given for the '--" + getLongForm() + "' (-" + getShortForm() + ") option.");
         }
   }

   template <>
   inline void Option<std::string>::setValue( const std::string &value ) {
         if (value == "")
            throw MissedOptionValue( "The option '--" + getLongForm() + "' (-" + getShortForm() + ") needs an additional parameter.");

         try {
            if ( isMulti() )
               myValue.push_back( value );  // add value
            else {
               myValue.resize( 1 );
               myValue.at(0) = value;         // overwrite value
            }
         }
         catch( std::exception &e ) {
            // if overflow occured
            throw InvalidOptionValue( "An invalid option was given for the '--" + getLongForm() + "' (-" + getShortForm() + ") option.");
         }
   }

   template <typename TYPE>
   bool Option<TYPE>::parseArgs( std::vector<StringPair> &arguments ) {
        if ( BaseOption::parseArgs( arguments ) ) {
            return true;
        }
        else {
            return false;
        }
   }
}

#endif /*OPTION_HH_*/
