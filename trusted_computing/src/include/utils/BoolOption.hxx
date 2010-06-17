// Copyright (C) 2007, 2008 by Sirrix AG security technologies
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

/**
 * @file        BoolOption.hxx
 * @brief       Declaration and definition of the class CmdLineOption<bool>.
 * @see         CmdLineParser, CmdLineOption
 * @version     $Revision: 532 $
 * @date        Last modification at $Date: 2009-05-05 16:02:08 +0200 (Tue, 05 May 2009) $ by $Author: selhorst $
 */

#ifndef BOOLOPTION_HH_
#define BOOLOPTION_HH_

#include <string>
#include <Option.hxx>

namespace utils {
    /**
     * @brief Explicit Specialization of Class Template CmdLineOption of TYPE bool.
     *
     * Because of the special case of bool should be realised explicitly in other form.
     *   @todo  it should be possible to use the long option in the form: --booloption=false.
     *
     * @author      Christian Stueble (stueble@sirrix.com)
     * @author      Anoosheh Zaerin (zaerin@sirrix.com)
     * @maintainers Anoosheh Zaerin (zaerin@sirrix.com)
     */
    template <> class Option<bool> : public BaseOption {
    public:
      /**
       * @brief Constructor with long-, shortform. The default value will be set to false.
       *
       * For bool option is now just defaultvalue false possible and with parse the bool option
       * must set myvalue = true.
       *
       * @todo: later would be able to set defaultvalue as true or false. Now will be always false
       */
      inline Option( const std::string &longForm, const std::string &shortForm, const OptionMode mode, const std::string &description="" );
      /**
       * @brief Constructor with long-, shortform. The default value will be set to false.
       *
       * For bool option is now just defaultvalue false possible and with parse the bool option
       * must set myvalue = true.
       *
       * @todo: later would be able to set defaultvalue as true or false. Now will be always false
       */
      inline Option( OptionList &optionList, const std::string &longForm, const std::string &shortForm, const OptionMode mode, const std::string &description="" );

      /// Returns the value of the CmdLineOption with adequated type.
      inline bool getValue() const;

      /// @return the list of option values of type TYPE.
      inline std::vector<bool> getValues() const;

      /// @return the default value if set.
      inline std::string getDefaultValue() const;

    protected:
      /**
       * @brief Sets and converts a set of std::strings to the TYPE CmdLineOption.
       * @throw Exception when value is in bad format or occures a overflow exception.
       */
      inline void setValue( const std::string &value );

    private:
      /// value of CmdLineOption
      bool myValue;
    };

    // ------------------------------- inline implementations -------------------------
    Option<bool>::Option( const std::string &longForm, const std::string &shortForm, const OptionMode mode, const std::string &description ):
      	BaseOption( longForm, shortForm, mode, description ),
      	myValue( false )
    {}

	Option<bool>::Option( OptionList &optionList, const std::string &longForm, const std::string &shortForm, const OptionMode mode, const std::string &description ):
      	BaseOption( optionList, longForm, shortForm, mode, description ),
      	myValue( false )
    {}

    bool Option<bool>::getValue() const {
      	return myValue;
    }

    std::vector<bool> Option<bool>::getValues() const {
        return std::vector<bool>( myValue );
    }

    std::string Option<bool>::getDefaultValue() const {
       return "";
    }

    void Option<bool>::setValue( const std::string &value ) {
      myValue = true;
    }
}

#endif /*BOOLOPTION_HH_*/
