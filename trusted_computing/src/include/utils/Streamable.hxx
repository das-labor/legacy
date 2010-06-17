// Copyright (C) 2007, 2008 by Sirrix AG security technologies
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

#ifndef STREAMABLE_HXX_
#define STREAMABLE_HXX_

#include <iostream>
#include <string>
#include <binstream.hxx>

namespace utils {
	/**
	 * @brief Interface of classes that can be converted into a binary stream
	 *        or a string stream.
	 *
	 * @author Christian Stueble (stueble@sirrix.com)
	 */
	class Streamable {
	public:
		Streamable();
		virtual ~Streamable();

		/// Stream object into an binary output stream.
		virtual obinstream& toBinaryStream( obinstream &ost ) const;
		/// Create object from an binary input stream.
		virtual ibinstream& fromBinaryStream( ibinstream &istr );
		/// @return bytearray representation of this object.
		//virtual ByteVector toByteArray() const;

		/// Stream object into an string-based output stream.
		virtual std::ostream& toStringStream( std::ostream &ostr ) const;
		/// Create object from an string-based input stream.
		virtual std::istream& fromStringStream( std::istream &istr );
		/// @return string representation of this object.
		//virtual std::string toString() const;
	};

	/// Stream object into an binary output stream.
	inline obinstream& operator<<( obinstream &, const Streamable & );
	/// Create object from an binary input stream.
	inline ibinstream& operator>>( ibinstream &, Streamable & );

	/// Stream object into an string-based output stream.
	inline std::ostream& operator<<( std::ostream &, const Streamable & );
	/// Create object from an string-based input stream.
	inline std::istream& operator>>( std::istream &, Streamable & );


	// -------------------------- inline implementations ---------------------------
	inline obinstream& operator<<( obinstream &ostr, const Streamable &obj ) {
		return obj.toBinaryStream( ostr );
	};
	/// Create object from an binary input stream.
	inline ibinstream& operator>>( ibinstream &istr, Streamable &obj ) {
		return obj.fromBinaryStream( istr );
	};

	/// Stream object into an string-based output stream.
	inline std::ostream& operator<<( std::ostream &ostr, const Streamable &obj ) {
		return obj.toStringStream( ostr );
	};
	/// Create object from an string-based input stream.
	inline std::istream& operator>>( std::istream &istr, Streamable &obj ) {
		return obj.fromStringStream( istr );
	};
}

#endif /*STREAMABLE_HXX_*/
