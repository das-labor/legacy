// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS 
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//

#ifndef TYPEINFO_HXX_
#define TYPEINFO_HXX_

#include <string>
#include <list>
#include <typeinfo>
#include <iostream>

namespace utils { 
	/**
	 * @brief  Providing type information in a more flexible manner than typeid().
	 * 
	 * This implementation depends on the typeinfo format as provided by the GCC 
	 * compiler.
	 * 
	 * @see    typeInfo()
	 * @author Christian Stueble (stueble@sirrix.com)
	 */
	class TypeInfo {
	public:
		TypeInfo( const std::string &typeinfo );
		~TypeInfo();
		
		/// @return the type name of the given object.
		std::string getType() const;
		
		/// @return List of namespaces of the given object.
		std::list<std::string> getNamespace() const;
		
	private:
		std::string myType;
		std::list<std::string> myNamespace;
	};

	/**
	 * @brief  Template function to create a @ref TypeInfo from an object.
	 * 
	 * @author Christian Stueble
	 */
	template<typename T> TypeInfo typeInfo( const T &obj ) {
		return TypeInfo( typeid( obj ).name() );
	};
}

#endif /*TYPEINFO_HXX_*/
