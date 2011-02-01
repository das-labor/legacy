#!/usr/bin/ruby
# gcdext-test.rb
=begin
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2008, 2009  Daniel Otte (daniel.otte@rub.de)

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
=end

$debug = true
$debug = false
require 'rubygems'
require 'serialport'
require 'getopt/std'
require 'ftools'
require 'date'

def gcdext(x,y)
  g=1
  i=0
  while(x&1==0 && y&1==0) do
    x>>=1
    y>>=1
    g<<=1
    i+=1
  end
  printf("DBG: initshift = %04X\n", i)
  u=x; v=y; a=1; b=0; c=0; d=1
  begin
    printf(" while u%%2==0; u = %X\n", u)
    printf("DBG: (10) a = %s\n", a.to_s(16).upcase)
    printf("DBG: (10) b = %s\n", b.to_s(16).upcase)
    printf("DBG: (10) c = %s\n", c.to_s(16).upcase)
    printf("DBG: (10) d = %s\n", d.to_s(16).upcase)
    while(u&1==0) do
      if(a%2==1 || b%2==1)
        a += y
        b -= x
      end
      u>>=1; a>>=1; b>>=1
    end
    printf(" while v%%2==0; v = %X\n", v)
    printf("DBG: (20) a = %s\n", a.to_s(16).upcase)
    printf("DBG: (20) b = %s\n", b.to_s(16).upcase)
    printf("DBG: (20) c = %s\n", c.to_s(16).upcase)
    printf("DBG: (20) d = %s\n", d.to_s(16).upcase)
    while(v&1==0) do
      if(c%2==1 || d%2==1)
        c += y
    #    printf("DBG: (qq) b = %s\n", b.to_s(16).upcase)
        d -= x
      end
      printf("DBG: (xx) d = %s\n", d.to_s(16).upcase) 
      v>>=1; c>>=1; d>>=1;
    end
    
    printf(" if u>=v ...\n")
    printf("DBG: (30) a = %s\n", a.to_s(16).upcase)
    printf("DBG: (30) b = %s\n", b.to_s(16).upcase)
    printf("DBG: (30) c = %s\n", c.to_s(16).upcase)
    printf("DBG: (30) d = %s\n", d.to_s(16).upcase)

    if(u>=v)
      u -= v; a-=c; b-=d
    else
      v -= u; c-=a; d-=b
    end
  end while(u!=0)
  return[g*v, c, d]
end


if ARGV.length==2
  a = ARGV[0].to_i
  b = ARGV[1].to_i
else
  # CD319349, 9EFD76CC
  # 1609000771, 6fac577d72
  a = 0x1609000771
  b = 0x6fac577d72
end
r = gcdext(a, b)
printf("gcdext( %s, %s) => a = %s; b = %s; gcd = %s\n",
  a.to_s(16),b.to_s(16),r[1].to_s(16),r[2].to_s(16),r[0].to_s(16))


