// Copyright (C) 2008, 2009 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#ifndef CONVERSIONPRIMITIVES_HXX_
#define CONVERSIONPRIMITIVES_HXX_

#include <Integer.hxx>
#include <ByteVector.hxx>

	namespace utils
	{		
		/// @brief I2OSP converts a nonnegative integer to a ByteVector of a specified length( PKCS#1 v2.1)
		///
		/// @param x nonnegative integer to be converted
		/// @param xLen intended length of the resulting octet string
		/// @returns corresponding octet string of length xLen
		inline utils::ByteVector I2OSP( integer::Integer x, UInt32 xLen)
		{
			if((xLen%2)==1)
				xLen = (xLen+1)/2;
			else
				xLen /= 2;

			ByteVector X(xLen);
			
			for ( UInt32 i = X.size(); i>0; i-- )
		    {
		    	X[i-1] += (unsigned char) x.lowU32();
		    	x.operator >>=(8);        
		    }
		    
			return X;
		}
				
		/// @brief OS2IP converts an octet string to a nonnegative integer
		///
		/// @param X octet string to be converted
		/// @return corresponding nonnegative integer
		inline integer::Integer OS2IP(const utils::ByteVector X)
		{
			return integer::Integer(X);
		}		
	}

#endif /*CONVERSIONPRIMITIVES_HXX_*/
