/* exteuclid.c */
/*
    This sourcefile is part of the crygop programm.
    Copyright (C) 2008  Daniel Otte

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdint.h>

#include "exteuclid.h"

uint32_t exteuclid(int32_t x, int32_t y, int32_t* r, int32_t* s){
	int32_t g=1,u,v,A=1,B=0,C=0,D=1;
	while(((x&1)==0) && ((y&1)==0)){
		x>>=1;
		y>>=1;
		g<<=1;
	}
	u=x; v=y; 
	do{
		while((u&1)==0){
			u>>=1;
			if(((A&1)==0) && ((B&1)==0)){
				A/=2; B/=2;
			} else {
				A = (A+y)/2;
				B = (B-x)/2;
			}
		}
		while((v&1)==0){
			v>>=1;
			if(((C&1)==0) && ((D&1)==0)){ 
				C/=2; D/=2;
			} else {
			        C = (C+y)/2;
			        D = (D-x)/2;
			}
		}
		if(u>=v){
			u-=v;
			A-=C;
			B-=D;
		} else {
			v-=u;
			C-=A;
			D-=B;
		}
	}while(u!=0);
	*r = C;
	*s = D;
	return g*v;
}



