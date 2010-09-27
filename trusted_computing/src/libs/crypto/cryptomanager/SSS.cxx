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

/*!
 *  @file       SSS.cxx
 *  @brief      Shamir Secret Sharing 
 *  @author     Marian Kuehnel ( m.kuehnel@sirrix.com)
 *  @date       Last modification at $Date: 2009-03-17$
 */

//--------------------------------------------------------------------------------
#include <SSS.hxx>
#include <iostream>
#include <ConversionPrimitives.hxx>
#include <stdlib.h>
#include <KeyronaHelper.hxx>

//--------------------------------------------------------------------------------
using namespace algo;
using namespace integer;
using namespace utils;

//--------------------------------------------------------------------------------
// constructor & destructor
SSS::SSS():
myX(),
myY()
{}

SSS::~SSS()
{}

//--------------------------------------------------------------------------------
// scheme initialisation
void SSS::initialisation(ByteVector Participant, ByteVector Access, ByteVector Key, ByteVector Modul)
{
	Integer P( OS2IP(Participant) );
	Integer A( OS2IP(Access) );
	Integer key( OS2IP(Key) );
	Integer modul( OS2IP(Modul) );
	
	srand ( time(NULL) );
	std::vector<Integer> coeff;
	Integer x,y;
	Integer tmp;
	
	// new coeff
	coeff.push_back(key);	
	for(int i=1; i < A; i++)
	{
            ByteVector randomBytes = getRandomBytes(Modul.size());
            Integer randomBytesInteger( OS2IP(randomBytes) );
            coeff.push_back( randomBytesInteger % modul) ;
	}				
	
	// computes shares		
	for(Integer x=1; x < (P+1);x+=1)
	{
		y = 0;
		tmp = 1;
		for(unsigned int j=0; j < coeff.size() ; j++)
		{
			y += coeff[j]*tmp;
			y %= modul;			
			tmp *= x;
		}
		myX.push_back( I2OSP( x, x.size() ) );
		myY.push_back( I2OSP( y, y.size() ) );
	}		
}

//--------------------------------------------------------------------------------
// scheme reconstruction
ByteVector SSS::reconstruction(std::vector<ByteVector> X, std::vector<ByteVector> Y, ByteVector Modul)
{
	Integer key(0);
	
	if( X.size() != Y.size() )	
	{
		std::cerr << "error: size X and size Y is different";		
	}
	else
	{
		std::vector<Integer> x;
		std::vector<Integer> y;
		Integer modul( OS2IP(Modul) );
		
		for(UInt32 k=0;k < X.size(); k++)
		{
			x.push_back( OS2IP(X[k]) );
			y.push_back( OS2IP(Y[k]) );			
		}
		
		UInt32 degree = x.size();

		Integer divident(0);
		Integer divisor(0);		

		for (unsigned int j = 0; j < degree; j++)
	   	{
	   		divident = 1;
	   		divisor = 1;
	   		
	   		for (unsigned int i = 0; i < degree; i++)
	   		{
	        	// The i-th term has to be skipped
	        	if (j != i)
	        	{
					divident *= (0 - x[i]);
					divisor  *= (x[j] - x[i]);
	        	}
	        }
	        divident *= y[j];
	        key += divident * Inverse(divisor,modul);
	        key %=  modul;      
	   	}
	} 
   return I2OSP( key, key.size() );
}
